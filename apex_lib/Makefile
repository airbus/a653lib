#
# Makfile for libApex.a
#

#CC = $(CC_PATH)/tools/gcc-810-ppc/bin/powerpc-linux-gnu-gcc
CC = gcc
AR = ar

CFLAGS   = -Wall -g2 -fPIC -c -I../apex_inc $(COMMON_SWITCH)
LDFLAGS  = -g2 #-L /tools/gcc-810-ppc/lib

OBJS = apex_i_partition.o apex_i_process.o apex_i_sampling.o apex_i_queuing.o apex_i_time.o apexInit.o apex_i_shm_if.o apex_i_sync.o apex_i_time_lib.o

MY_BUILD_DIR  =  $(BUILD_DIR)/apex_lib
TARGET        =  $(BUILD_DIR)/libApex.a

all: clean mk_build_dir apex_lib
	echo build all $(MY_BUILD_DIR)

apex_lib: mk_build_dir $(OBJS) 
#	cd $(MY_BUILD_DIR); $(CC) $(LDFLAGS) -shared -o $(TARGET) $(OBJS) -lm -ldl
	cd $(MY_BUILD_DIR); $(AR) crs $(TARGET) $(OBJS) # static lib
	cd $(MY_BUILD_DIR); ranlib $(TARGET)

copy: apex_lib
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
