export V?=0

.PHONY: all
all: host ta

.PHONY: fuse
fuse:
	mkdir -p fuse_out/
	CC=$(HOST_CROSS_COMPILE)gcc
	CPP=$(HOST_CROSS_COMPILE)cpp
#	LD=$(HOST_CROSS_COMPILE)ld
	AR=$(HOST_CROSS_COMPILE)ar
	NM=$(HOST_CROSS_COMPILE)nm
	OBJCOPY=$(HOST_CROSS_COMPILE)objcopy
	OBJDUMP=$(HOST_CROSS_COMPILE)objdump
	READELF=$(HOST_CROSS_COMPILE)readelf
	cd libfuse; ./makeconf.sh; ./configure --prefix=$(PWD)/fuse_out/ --host=$(LIBFUSE_HOST)
	make -C libfuse CROSS_COMPILE=$(HOST_CROSS_COMPILE)
	make -C libfuse CROSS_COMPILE=$(HOST_CROSS_COMPILE) install

.PHONY: host
host: fuse
	make -C host CROSS_COMPILE=$(HOST_CROSS_COMPILE)

.PHONY: ta
ta:
	make -C ta CROSS_COMPILE=$(TA_CROSS_COMPILE)

.PHONY: clean
clean:
	make -C host clean
	make -C ta clean
	make -C libfuse clean
	rm -rf fuse_out
	rm -rf out
