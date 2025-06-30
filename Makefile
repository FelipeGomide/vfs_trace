SRCS = main.o vfs_probes.o
MODULE_NAME = vfs_tracer

obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-objs += $(SRCS)

PWD := $(CURDIR)

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean