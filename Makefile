#
# Makfile arinc 653 linux scheduler
#

#export COMMON_SWITCH   = -D__LITTLE_ENDIAN -m32 -rdynamic	
export COMMON_SWITCH   = -D__LITTLE_ENDIAN -rdynamic	

HOME=$(shell pwd)

#  CC_PATH=/home/tools/gnat/bin/
CC_PATH=


CC = $(CC_PATH)gcc
#CC = $(CC_PATH)/tools/gcc-810-ppc/bin/powerpc-linux-gnu-gcc
AR = $(CC_PATH)ar
PYTHON = python3


export TMP_DIR       = $(HOME)/tmp
export SRC_DIR       = $(shell pwd)
export BUILD_DIR     = $(TMP_DIR)/a653_build
#export BIN_DIR       = $(HOME)/mut_bin_$(BUILD_TARGET)
export BIN_DIR       = $(HOME)/bin
export VENV_DIR      = $(TMP_DIR)/venv


CFLAGS   = -Wall -g2 -fPIC  $(COMMON_SWITCH)
CINCL    = -I$(SRC_DIR)/a653_lib -I$(MY_BUILD_DIR)/a653_inc -I/usr/include

LDFLAGS  = -g2 #-L /tools/gcc-810-ppc/lib  #-L/usr/lib/i386-linux-gnu/
LDLIBS   = -lpthread -lrt -lm -ldl






export CC

OBJS = main.o
OBJS_A = partition_a.o init.o
OBJS_B = partition_b.o init.o
OBJS_WASM_BOOTSTRAP = wasm32_loader.o init.o


MY_BUILD_DIR  =  $(BUILD_DIR)
TARGET        =  $(BIN_DIR)/a653_main
TARGET_A       =  $(BIN_DIR)/partition_a
TARGET_A_WASM = $(BIN_DIR)/partition_a.wasm
TARGET_B       =  $(BIN_DIR)/partition_b
TARGET_B_WASM = $(BIN_DIR)/partition_b.wasm
TARGET_WASM_BOOTSTRAP   = $(BIN_DIR)/wasm32_rt



all: clean mk_build_dir alib amain part_a part_b $(MY_BUILD_DIR)/camw32_getset.h   part_bootstrap_wasm part_wasms
	@echo done!


amain: $(OBJS)
	@echo build dir $(MY_BUILD_DIR)
	cd $(MY_BUILD_DIR); $(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) ./liba653.a $(LDLIBS)

part_a: $(OBJS_A)
	@echo build dir $(MY_BUILD_DIR)
	cd $(MY_BUILD_DIR); $(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET_A) $(OBJS_A) ./liba653.a $(LDLIBS)

part_b: $(OBJS_B)
	@echo build dir $(MY_BUILD_DIR)
	cd $(MY_BUILD_DIR); $(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET_B) $(OBJS_B) ./liba653.a $(LDLIBS)

part_bootstrap_wasm: $(OBJS_WASM_BOOTSTRAP) alib_wasm32
	@echo build dir $(MY_BUILD_DIR)
	cd $(MY_BUILD_DIR); $(CC) $(CFLAGS) $(LDFLAGS) -fsanitize=address -lwasmtime -o $(TARGET_WASM_BOOTSTRAP) $(OBJS_WASM_BOOTSTRAP) ./liba653_wasm32.a $(LDLIBS)


# for host:
# $ yay -S libwasmtime # wit-bindgen wasm-tools wabt

part_wasms: $(TARGET_A_WASM) $(TARGET_B_WASM)

$(MY_BUILD_DIR)/camw32_getset.h: mk_build_dir
	test -d $(MY_BUILD_DIR)/arinc653-wasm/pkgs/c-abi-lens || { cd $(MY_BUILD_DIR) && git clone https://github.com/psiegl/arinc653-wasm.git --branch psiegl-old; }
	test -d $(MY_BUILD_DIR)/arinc653-wasm/pkgs/c-abi-lens/target/debug/c-abi-lens || { cd $(MY_BUILD_DIR)/arinc653-wasm/pkgs/c-abi-lens && rustup default stable && cargo build; }
	# not ideal, but currently without --sysroot=/usr/share/wasi-sysroot (should be the same as during wasm compilation)
	$(MY_BUILD_DIR)/arinc653-wasm/pkgs/c-abi-lens/target/debug/c-abi-lens $(SRC_DIR)/a653_inc/a653Lib.h -- --target=wasm32-wasi > $@
	sed -i 's|camw|camw32|g' $@

# for guest:
# $ yay -S clang lld wasi-libc wasi-compiler-rt
%.wasm: alib
	@echo build dir $(MY_BUILD_DIR)
	# 1. we use the wasm32-wasi to include the stdlib (thus having __start() and main() support).
	# however, long term for avionics it would make sense to drop and go to wasm32-unknown with likely -Wl,-export=_start or similar
	# 2. --allow-undefined is required for symbols (such as WIT functions) that are not yet defined.
	cd $(MY_BUILD_DIR); clang -I$(MY_BUILD_DIR)/a653_inc --target=wasm32-wasi -Wl,--export-table -Wl,--allow-undefined --sysroot=/usr/share/wasi-sysroot -o $@ ../../$(basename $(notdir $@)).c 1> $(basename $(notdir $@)).wasm32_struct_layout.txt # ../../wasm_guest_trampoline.c

# for testing purpose
wamr:
	$(CC) -D__WAMR__ -c a653_lib_wasm32/arinc653_part1_apex_time_wasm32.c -o $(TMP_DIR)/arinc653_part1_apex_time_wasm32.o -I$(BUILD_DIR)
	$(CC) -D__WAMR__ -c a653_lib_wasm32/arinc653_part1_apex_partition_wasm32.c -o $(TMP_DIR)/arinc653_part1_apex_partition_wasm32.o -I$(BUILD_DIR)

alib_wasm32: alib
	make -e -C $(SRC_DIR)/a653_lib_wasm32 a653_lib_wasm32

alib:
	make -e -C $(SRC_DIR)/a653_lib
	cp -r $(SRC_DIR)/a653_inc $(MY_BUILD_DIR)
#	cp -r $(SRC_DIR)/a653_ada $(MY_BUILD_DIR)



gcc_version: 
	gcc -v

mk_build_dir:
	mkdir -p $(TMP_DIR)	
	mkdir -p $(MY_BUILD_DIR)
	mkdir -p $(BIN_DIR)

clean:
	make -C $(SRC_DIR)/a653_lib clean
	rm -fr $(BUILD_DIR)
	rm -f *.o
	rm -f *~
	rm -f *.so

# c - code 
%.o: %.c
	$(CC) -c $(CFLAGS) $(CINCL) $< -o $(MY_BUILD_DIR)/$@



