obj-m += main.o

BUILD=/lib/modules/$(shell uname -r)/build
M=$(shell pwd)

all:
	make -C $(BUILD) M=$(M) modules
	make -C $(BUILD) M=$(M) modules_install
clean:
	make -C $(BUILD) M=$(M) clean
