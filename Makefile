#
# Makfile arinc 653 linux scheduler
#

#export COMMON_SWITCH   = -D__LITTLE_ENDIAN -m32 -rdynamic	
export COMMON_SWITCH   = -D__LITTLE_ENDIAN -rdynamic	


#  CC_PATH=/home/tools/gnat/bin/
CC_PATH=


CC = $(CC_PATH)gcc
#CC = $(CC_PATH)/tools/gcc-810-ppc/bin/powerpc-linux-gnu-gcc
AR = $(CC_PATH)ar


export TMP_DIR       = $(HOME)/tmp
export SRC_DIR       = $(shell pwd)
export BUILD_DIR     = $(TMP_DIR)/a653_build
#export BIN_DIR       = $(HOME)/mut_bin_$(BUILD_TARGET)
export BIN_DIR       = $(HOME)/bin


CFLAGS   = -Wall -g2 -fPIC  $(COMMON_SWITCH)
CINCL    = -I$(SRC_DIR)/apex_lib -I$(MY_BUILD_DIR)/apex_inc -I/usr/include

LDFLAGS  = -g2 #-L /tools/gcc-810-ppc/lib  #-L/usr/lib/i386-linux-gnu/
LDLIBS   = -lpthread -lrt -lm -ldl






export CC

OBJS = main.o init.o
OBJS_A = partition_a.o init.o
OBJS_B = partition_b.o init.o


MY_BUILD_DIR  =  $(BUILD_DIR)
TARGET        =  $(BIN_DIR)/a653_main
TARGET_A       =  $(BIN_DIR)/partition_a
TARGET_B       =  $(BIN_DIR)/partition_b



all: clean mk_build_dir alib amain part_a part_b
	@echo done!


amain: $(OBJS)
	@echo build dir $(MY_BUILD_DIR)
	cd $(MY_BUILD_DIR); $(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) ./libApex.a $(LDLIBS)

part_a: $(OBJS_A)
	@echo build dir $(MY_BUILD_DIR)
	cd $(MY_BUILD_DIR); $(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET_A) $(OBJS_A) ./libApex.a $(LDLIBS)

part_b: $(OBJS_B)
	@echo build dir $(MY_BUILD_DIR)
	cd $(MY_BUILD_DIR); $(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET_B) $(OBJS_B) ./libApex.a $(LDLIBS)


alib:
	make -e -C $(SRC_DIR)/apex_lib
	cp -r $(SRC_DIR)/apex_inc $(MY_BUILD_DIR)
#	cp -r $(SRC_DIR)/apex_ada $(MY_BUILD_DIR)




gcc_version: 
	gcc -v

mk_build_dir:
	mkdir -p $(TMP_DIR)	
	mkdir -p $(MY_BUILD_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(SRC_DIR)/../lib
	mkdir -p $(SRC_DIR)/../lib/apex_nr

clean:
	make -C $(SRC_DIR)/apex_lib clean
	rm -fr $(BUILD_DIR)
	rm -f *.o
	rm -f *~
	rm -f *.so

# c - code 
%.o: %.c
	$(CC) -c $(CFLAGS) $(CINCL) $< -o $(MY_BUILD_DIR)/$@



