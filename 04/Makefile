obj-m += keyboard_alarm.o

BUILD=/lib/modules/$(shell uname -r)/build
M=$(shell pwd)

all:
	make -C $(BUILD) M=$(M) modules
	cp -f 70-alarm-keyboard.rules /etc/udev/rules.d/
clean:
	make -C $(BUILD) M=$(M) clean
