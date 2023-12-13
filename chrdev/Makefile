obj-m += cdev_sample.o 

cdev_sample-y += cdev_sample_rust.o cdev_sample_c.o
# mix_rust_c-objs += c_helper.o

RUSTFLAGS_cdev_sample.o +=

KERNEL_VERSION = $(shell uname -r)
.PHONY += clean help expand_mix_rust_c

all:
	make LLVM=1 -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) modules

clean:
	make LLVM=1 -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) clean

help:
	make LLVM=1 -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) help

expand_cdev_sample_rust:
	make LLVM=1 -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) cdev_sample_rust.rsi

