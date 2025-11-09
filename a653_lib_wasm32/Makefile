#
# Makfile for liba653_wasm32.a
#

include ../a653_lib/Makefile

CFLAGS += -I$(SRC_DIR)/a653_lib -I$(MY_BUILD_DIR)

OBJS_REL = ../init.o \
	generic_helper.o

# these *.c files require -D__WAMR__ or -D__WASMTIME__
OBJS_WASM32_RT_SPECIFIC = \
	apex_host_fncs_wasm32.o \
	arinc653_part1_apex_time_wasm32.o \
	arinc653_part1_apex_process_wasm32.o \
	arinc653_part1_apex_partition_wasm32.o \
	arinc653_part1_apex_sampling_port_wasm32.o \
	arinc653_part2_apex_sampling_port_extension_wasm32.o \
	arinc653_part1_apex_queuing_port_wasm32.o \
	arinc653_part1_apex_semaphore_wasm32.o \
	arinc653_part1_apex_error_wasm32.o \
	arinc653_part1_apex_buffer_wasm32.o \
	arinc653_part1_apex_event_wasm32.o \
	arinc653_part1_apex_blackboard_wasm32.o \
	arinc653_part1_apex_mutex_wasm32.o \
	wasm32_main.o

$(TMP_DIR)/arinc653-wasm/pkgs/c-abi-lens:
	test -d $@ || { cd $(TMP_DIR) && git clone https://github.com/psiegl/arinc653-wasm.git --branch psiegl-old; }

$(TMP_DIR)/arinc653-wasm/pkgs/c-abi-lens/target/debug/c-abi-lens: $(TMP_DIR)/arinc653-wasm/pkgs/c-abi-lens
	test -f $@ || { cd $(TMP_DIR)/arinc653-wasm/pkgs/c-abi-lens && rustup default stable && cargo build; }

$(MY_BUILD_DIR)/camw32_getset.h: $(TMP_DIR)/arinc653-wasm/pkgs/c-abi-lens/target/debug/c-abi-lens
	# not ideal, but currently without --sysroot=/usr/share/wasi-sysroot (should be the same as during wasm compilation)
	$(TMP_DIR)/arinc653-wasm/pkgs/c-abi-lens/target/debug/c-abi-lens $(BUILD_DIR)/a653_inc/a653Lib.h -- --target=wasm32-wasi > $@
	sed -i 's|camw|camw32|g' $@


WAMR_OBJS_WASM32 = $(patsubst %, $(MY_BUILD_DIR)/wamr/%, $(notdir $(OBJS_WASM32_RT_SPECIFIC)) a653_wamr.o)
$(WAMR_OBJS_WASM32): | $(MY_BUILD_DIR)/wamr/ $(MY_BUILD_DIR)/camw32_getset.h

$(MY_BUILD_DIR)/wamr/:
	mkdir -p $@

$(MY_BUILD_DIR)/wamr/%.o: %.c
	$(CC) -c $(CFLAGS) -D__WAMR__ $(CINCL) $< -o $@

# Do not use ASAN (-fsanitize=address) ... https://github.com/bytecodealliance/wasm-micro-runtime/issues/4638
%/p_wamr: $(OBJS_REL) $(WAMR_OBJS_WASM32)
	cd $(MY_BUILD_DIR); $(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS) -liwasm -lm /usr/lib/libiwasm.a $(TMP_DIR)/a653_build/liba653.a


WASMTIME_OBJS_WASM32 = $(patsubst %, $(MY_BUILD_DIR)/wasmtime/%, $(notdir $(OBJS_WASM32_RT_SPECIFIC)) a653_wasmtime.o)
$(WASMTIME_OBJS_WASM32): | $(MY_BUILD_DIR)/wasmtime/ $(MY_BUILD_DIR)/camw32_getset.h

$(MY_BUILD_DIR)/wasmtime/:
	mkdir -p $@

$(MY_BUILD_DIR)/wasmtime/%.o: %.c
	$(CC) -c $(CFLAGS) -D__WASMTIME__ $(CINCL) $< -o $@

%/p_wasmtime: $(OBJS_REL) $(WASMTIME_OBJS_WASM32)
	cd $(MY_BUILD_DIR); $(CC) $(LDFLAGS) -fsanitize=address -lwasmtime -o $@ $^ $(LDLIBS) $(TMP_DIR)/a653_build/liba653.a
