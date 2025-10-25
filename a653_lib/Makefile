#
# Makfile for liba653.a
#

#CC = $(CC_PATH)/tools/gcc-810-ppc/bin/powerpc-linux-gnu-gcc
CC = gcc
AR = ar

CFLAGS   = -Wall -Wno-unused-function -g2 -O0 -fPIC -c -I../a653_inc -I$(BUILD_DIR) $(COMMON_SWITCH)
LDFLAGS  = -g2 #-L /tools/gcc-810-ppc/lib

OBJS = a653_i_semaphore.o a653_i_partition.o a653_i_process.o a653_i_sampling.o a653_i_queuing.o a653_i_time.o a653Init.o a653_i_shm_if.o a653_i_sync.o a653_i_time_lib.o a653_i_error.o

MY_BUILD_DIR  =  $(BUILD_DIR)/a653_lib
TARGET        =  $(BUILD_DIR)/liba653.a

all: clean mk_build_dir a653_lib
	echo build all $(MY_BUILD_DIR)

a653_lib: mk_build_dir $(OBJS)
#	cd $(MY_BUILD_DIR); $(CC) $(LDFLAGS) -shared -o $(TARGET) $(OBJS) -lm -ldl
	cd $(MY_BUILD_DIR); $(AR) crs $(TARGET) $(OBJS) # static lib
	cd $(MY_BUILD_DIR); ranlib $(TARGET)

copy: a653_lib
	echo all


mk_build_dir:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(MY_BUILD_DIR)


clean :
	$(RM) -f *~
	$(RM) -f $(MY_BUILD_DIR)/*.o
	$(RM) -f $(PANELS_EXE)

# c - code 
%.o: %.c
	$(CC) -c $(CFLAGS) $(CINCL) $< -o $(MY_BUILD_DIR)/$@
