KDIR= /lib/modules/$(shell uname -r)/build
PWD= $(shell pwd)

obj-m := ventilo.o

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

install:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules_install

clean:
	rm -rf *~ Module.markers Modules.symvers $(PROG)
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
