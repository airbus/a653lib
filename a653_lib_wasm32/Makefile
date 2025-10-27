#
# Makfile for liba653_wasm32.a
#

include ../a653_lib/Makefile

OBJS_REL = $(patsubst %, $(MY_BUILD_DIR)/%, $(OBJS))
OBJS_WASM32 = $(OBJS_REL) a653_i_common_wasm32.o \
	arinc653_part1_apex_time_wasm32.o \
	arinc653_part1_apex_process_wasm32.o \
	arinc653_part1_apex_partition_wasm32.o \
	arinc653_part1_apex_sampling_port_wasm32.o \
	arinc653_part2_apex_sampling_port_extension_wasm32.o \
	arinc653_part1_apex_queuing_port_wasm32.o \
	arinc653_part1_apex_semaphore_wasm32.o \
	arinc653_part1_apex_error_wasm32.o

TARGET_WASM32 =  $(BUILD_DIR)/liba653_wasm32.a

a653_lib_wasm32: mk_build_dir $(OBJS_WASM32)
#	cd $(MY_BUILD_DIR); $(CC) $(LDFLAGS) -shared -o $(TARGET_WASM32) $(OBJS_WASM32) -lm -ldl
	cd $(MY_BUILD_DIR); $(AR) crs $(TARGET_WASM32) $(OBJS_WASM32) # static lib
	cd $(MY_BUILD_DIR); ranlib $(TARGET_WASM32)
