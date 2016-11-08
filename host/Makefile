CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

OBJS = filesystem.o

CFLAGS += -Wall -I../ta/include -I$(TEEC_EXPORT)/include -I/usr/include/fuse -D_FILE_OFFSET_BITS=64
LDADD += -lteec -L$(TEEC_EXPORT)/lib -lfuse

.PHONY: all
all: filesystem

filesystem: $(OBJS)
	$(CC) $(LDADD) -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJS) filesystem
