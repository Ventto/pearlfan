MODULE		= ventilo
DEVNAME		= $(MODULE)0
KERNELOBJ	= $(MODULE).ko
obj-m		:= $(MODULE).o

KDIR		= /lib/modules/$(shell uname -r)/build
PWD			= $(shell pwd)

.PHONY: all install clean distclean

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules_install
	insmod $(KERNELOBJ)

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean

distclean:
	@lsmod | grep $(MODULE) && rmmod -f $(MODULE) \
		|| echo "$(MODULE): module not found."
