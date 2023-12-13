obj-m += mix_rust_c.o 

mix_rust_c-y += rust_code.o c_helper.o
# mix_rust_c-objs += c_helper.o

RUSTFLAGS_mix_rust_c.o +=

KERNEL_VERSION = $(shell uname -r)
.PHONY += clean help expand_mix_rust_c

all:
	make LLVM=1 -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) modules

clean:
	make LLVM=1 -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) clean

help:
	make LLVM=1 -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) help

expand_rust_code:
	make LLVM=1 -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) rust_code.rsi

