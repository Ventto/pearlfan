obj-m         += pfan.o
pfan-objs     += ../../common/convert.o module.o
ccflags-y += -I$(src)/../../common

KDIR = /lib/modules/$(shell uname -r)/build
PWD  = $(shell pwd)

.PHONY: all clean install

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules_install

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
