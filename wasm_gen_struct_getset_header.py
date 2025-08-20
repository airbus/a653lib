import re
import sys
import json
import argparse

parser = argparse.ArgumentParser(description="Process log and types files.")
parser.add_argument('--types', required=True, help='Path to JSON file with types')
args = parser.parse_args()

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


def size_to_uint_type(size):
    if size == 1: return 'uint8_t'
    elif size == 2: return 'uint16_t'
    elif size == 4: return 'uint32_t'
    elif size == 8: return 'uint64_t'
    else: return None

def generate_nested(struct_name, fnc_name, fields):
    global structs

    header = ''
    for field in fields:
        fname = field['name']

        # in case of struct in struct
        if field['type']['kind'] == 'typedef' and field['type']['type']['kind'] == 'struct': # nested
            nested_tname = field['type']['type']['name']

            # find first matching:
            nested_field = next((struct for struct in structs if struct['name'] == nested_tname), None)

            header += generate_nested(struct_name, f'{fnc_name}__{fname}', nested_field['fields'])
        else:
            tname = field['type']['name']
            uint_type = size_to_uint_type(field['type']['size'])
            if uint_type is None:
                header += f'#define SET__{fnc_name}__{fname}__FNC_PTR_DCL( __X__ ) void (* __X__)({struct_name}*, const char*)\n'
                header += f'void SET__{fnc_name}__{fname}({struct_name}* guest_struct, const char* host_buf);\n'
                header += f'#define GET__{fnc_name}__{fname}__FNC_PTR_DCL( __X__ ) void (* __X__)(char*, {struct_name}*)\n'
                header += f'void GET__{fnc_name}__{fname}(char* host_buf, {struct_name}* guest_struct);\n\n'
            else:
                header += f'#define SET__{fnc_name}__{fname}__FNC_PTR_DCL( __X__ ) {tname} (* __X__)({struct_name}*, {uint_type} /* {tname} */)\n'
                header += f'void SET__{fnc_name}__{fname}({struct_name}* guest_struct, {uint_type} /* {tname} */ host_value);\n'
                header += f'#define GET__{fnc_name}__{fname}__FNC_PTR_DCL( __X__ ) {uint_type} /* {tname} */ (* __X__)({struct_name}*)\n'
                header += f'{uint_type} /* {tname} */ GET__{fnc_name}__{fname}({struct_name}* guest_struct);\n\n'

    return header

structs = [ t for t in types if t['kind'] == 'struct' ]
header = []
for struct in structs:
    header.append( generate_nested(struct['name'], struct['name'], struct['fields']) )

includes = (
    '#include <stdint.h>\n'
    '#include "a653Lib.h"\n\n'
)
print(includes + ''.join(sorted(header)))
