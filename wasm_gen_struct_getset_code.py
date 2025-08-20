import re
import sys
import json
import argparse

parser = argparse.ArgumentParser(description="Process log and types files.")
parser.add_argument('--log', required=True, help='Path to log file')
parser.add_argument('--types', required=True, help='Path to JSON file with types')
args = parser.parse_args()

# Read log file
try:
    with open(args.log, 'r', encoding='utf-8') as f:
        log = f.read()
except FileNotFoundError:
    print(f"ERR: Log file '{args.log}' not found.")
    sys.exit(-1)

# Read types JSON file
try:
    with open(args.types, 'r', encoding='utf-8') as f:
        types = json.load(f)
except FileNotFoundError:
    print(f"ERR: Types file '{args.types}' not found.")
    sys.exit(-1)
except json.JSONDecodeError as e:
    print(f"ERR: Failed to parse types JSON file: {e}")
    sys.exit(-1)

type_to_size_map = { t['name'] : t['size'] for t in types if t['kind'] == 'typedef' }



def parse_layout(log_text):
    structs = {}
    sizes = {}
    current_struct = None
    
    depth_no = None
    current_depth = None

    in_valid_block = False

    lines = log_text.strip().split('\n')
    struct_name_pattern = re.compile(r'\|\s+([^\s]+)$')
    field_pattern = re.compile(r'\s*(\d+)\s+\|(\s+)([^\s]+)\s+([^\s]+)$')
    size_pattern = re.compile(r'\[sizeof=(\d+),')

    for line in lines:
        line = line.rstrip()

        # Enter valid block on "AST Record Layout"
        if line.startswith('*** Dumping AST Record Layout'):
            current_struct = None
            depth_no = None
            in_valid_block = True
            continue
        # Exit valid block on IRgen or "Layout"
        elif line.startswith('*** Dumping IRgen Record Layout') or line.startswith('Layout:') or '__wasi' in line:
            current_struct = None
            depth_no = None
            in_valid_block = False
            continue
        elif not in_valid_block:
            continue

        # Safe check for empty lines
        m_struct = struct_name_pattern.search(line)
        if m_struct:
            current_struct = m_struct.group(1)
            structs[m_struct.group(1)] = []
            current_depth = structs[m_struct.group(1)]
            continue

        if current_struct:
            m_field = field_pattern.match(line)
            if m_field:
                offset, ftype, fname = int(m_field.group(1)), m_field.group(3), m_field.group(4)
                depth = len(m_field.group(2))
                if depth_no == None or depth_no == depth:
                    current_depth.append((offset, ftype, fname, []))
                else:
                    current_depth = structs[current_struct][-1][-1]
                    current_depth.append((offset, ftype, fname, []))

                depth_no = len(m_field.group(2))

            else:
                m_size = size_pattern.search(line)
                if m_size:
                    sizes[current_struct] = int(m_size.group(1))

    return structs, sizes

def size_to_uint_type(size):
    if size == 1: return 'uint8_t'
    elif size == 2: return 'uint16_t'
    elif size == 4: return 'uint32_t'
    elif size == 8: return 'uint64_t'
    else: return None

need_memcpy = False
def generate_nested(struct_name, fnc_name, fields, struct_size):
    global need_memcpy
    global type_to_size_map
    code = ""
    for i, (offset, ftype, fname, deeper_fields) in enumerate(fields):
        if deeper_fields:
            code += generate_nested(struct_name, f'{fnc_name}__{fname}', deeper_fields, struct_size)
        else:
            next_offset = fields[i+1][0] if i+1 < len(fields) else struct_size
            size_local_with_pad = next_offset - offset
            size_local = type_to_size_map[ftype]
            uint_type = size_to_uint_type(size_local)
            struct_access = f'{fnc_name}__{fname}'.replace('__', '.')
            if size_local != size_local_with_pad:
                pad = f", size with pad: {size_local_with_pad}"
            else:
                pad = ""
            code += f'// {struct_access}\n// offset: {offset}, size: {size_local}{pad}\n'
            if uint_type is None:
                need_memcpy = True
                code += (
                    f'void SET__{fnc_name}__{fname}({struct_name}* guest_struct, const char* host_buf) {{\n'
                    f'    memcpy(((uint8_t*)guest_struct) + {offset}, host_buf, {size_local});\n}}\n\n'
                    f'void GET__{fnc_name}__{fname}({struct_name}* guest_struct, char* host_buf) {{\n'
                    f'    memcpy(host_buf, ((uint8_t*)guest_struct) + {offset}, {size_local});\n}}\n\n'
                )
            else:
                code += (
                    f'void SET__{fnc_name}__{fname}({struct_name}* guest_struct, {uint_type} /* {ftype} */ host_value) {{\n'
                    f'    *({uint_type}*)(((uint8_t*)guest_struct) + {offset}) = /* ({uint_type}) */ host_value;\n}}\n\n'
                    f'{uint_type} /*{ftype}*/ GET__{fnc_name}__{fname}({struct_name}* guest_struct) {{\n'
                    f'    return /*({ftype})*/ *({uint_type}*)(((uint8_t*)guest_struct) + {offset});\n}}\n\n'
                )

            
    return code

def generate_getters_setters(structs, sizes):
    global need_memcpy
    code = []
    #need_memcpy = False
    for struct_name, fields in structs.items():
        struct_size = sizes.get(struct_name, 0)
        if struct_size == 0:
            print(f"WARNING: Struct {struct_name} has no valid size, skipping")
            continue
        c = generate_nested(struct_name, struct_name, fields, struct_size)
        code.append( f'// Struct {struct_name} size: {struct_size} bytes\n' + c )

    # let's sort, then we can compare resulting

    includes = '#include "a653Lib.h"\n'
    includes += '#include <stdint.h>\n'
    includes += '#include <stdio.h>\n'
    if need_memcpy:
        includes += '#include <string.h>\n'
    includes += '\n'
    return includes + ''.join(sorted(code))

structs, sizes = parse_layout(log)
code = generate_getters_setters(structs, sizes)
print(code)
