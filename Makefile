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
CINCL    = -I$(SRC_DIR)/a653_lib -I$(MY_BUILD_DIR)/a653_inc -I/usr/include

LDFLAGS  = -g2 #-L /tools/gcc-810-ppc/lib  #-L/usr/lib/i386-linux-gnu/
LDLIBS   = -lpthread -lrt -lm -ldl






export CC

OBJS = main.o
OBJS_A = partition_a.o init.o
OBJS_B = partition_b.o init.o


MY_BUILD_DIR  =  $(BUILD_DIR)
TARGET        =  $(BIN_DIR)/a653_main
TARGET_A       =  $(BIN_DIR)/partition_a
TARGET_B       =  $(BIN_DIR)/partition_b



all: mk_build_dir alib amain part_a part_b
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


alib: $(TMP_DIR)/download/a653Blackboard.h $(TMP_DIR)/download/a653Buffer.h $(TMP_DIR)/download/a653Event.h $(TMP_DIR)/download/a653Mutex.h
	cp -r $(SRC_DIR)/a653_inc $(MY_BUILD_DIR)
	cp $^ $(MY_BUILD_DIR)/a653_inc
#	cp -r $(SRC_DIR)/a653_ada $(MY_BUILD_DIR)
	# relies on the above headers
	make -e -C $(SRC_DIR)/a653_lib


# rule to download the ARINC headerfiles
$(TMP_DIR)/download/arinc653.h.zip:
	@mkdir -p -- $(@D)
	test -f $@ || curl --user-agent 'Mozilla/5.0 (Windows NT 6.3; WOW64; rv:41.0) Gecko/20100101 Firefox/41.0' \
		--location --output-dir $(TMP_DIR)/download --remote-name-all \
		https://brx-content.fullsight.org/site/binaries/content/assets/itc/content/support-files/arinc653.h.zip

# rule to extract the ARINC headerfiles
$(TMP_DIR)/download/ARINC653.h: $(TMP_DIR)/download/arinc653.h.zip
	@mkdir -p -- $(@D)
	test -f $@ || echo $^ | xargs --max-args=1 bsdtar -x --cd $(TMP_DIR)/download --modification-time --file

# rule to generate our Wasm header file, by making every open type a 32 Bit integer
$(TMP_DIR)/download/ARINC653_patched.h: $(TMP_DIR)/download/ARINC653.h
	@mkdir -p -- $(@D)
	awk -f $(SRC_DIR)/scripts/process-arinc-header.awk $< > $@

$(TMP_DIR)/download/a653Blackboard.h $(TMP_DIR)/download/a653Buffer.h $(TMP_DIR)/download/a653Event.h $(TMP_DIR)/download/a653Mutex.h &: $(TMP_DIR)/download/ARINC653_patched.h
	cd $(TMP_DIR)/download; awk -f $(SRC_DIR)/scripts/split-arinc-header.awk $<


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



