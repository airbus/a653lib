#
# Makfile arinc 653 linux scheduler
#

#export COMMON_SWITCH   = -D__LITTLE_ENDIAN -m32 -rdynamic	
export COMMON_SWITCH   = -D__LITTLE_ENDIAN -rdynamic	

CC                    ?= gcc
AR                    ?= ar
EXTRA_LDFLAGS         ?= #-L /tools/gcc-810-ppc/lib  #-L/usr/lib/i386-linux-gnu/


export TMP_DIR         = $(HOME)/tmp
export SRC_DIR         = $(shell pwd)
export BUILD_DIR       = $(TMP_DIR)/a653_build
#export BIN_DIR         = $(HOME)/mut_bin_$(BUILD_TARGET)
export BIN_DIR         = $(HOME)/bin


CFLAGS                 = -Wall -g2 -fPIC  $(COMMON_SWITCH)
CINCL                  = -I$(SRC_DIR)/a653_lib -I$(MY_BUILD_DIR)/a653_inc -I/usr/include

LDFLAGS                = -g2 $(EXTRA_LDFLAGS)
LDLIBS                 = -lpthread -lrt -lm -ldl






export CC

OBJS                   = main.o
OBJS_A                 = partition_a.o init.o
OBJS_B                 = partition_b.o init.o


MY_BUILD_DIR           =  $(BUILD_DIR)
TARGET                 =  $(BIN_DIR)/a653_main
TARGET_A               =  $(BIN_DIR)/partition_a
TARGET_B               =  $(BIN_DIR)/partition_b



all: clean mk_build_dir alib amain part_a part_b
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


alib:
	make -e -C $(SRC_DIR)/a653_lib
	cp -r $(SRC_DIR)/a653_inc $(MY_BUILD_DIR)
#	cp -r $(SRC_DIR)/a653_ada $(MY_BUILD_DIR)




gcc_version: 
	$(CC) -v

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



