obj-m += keyboard_alarm.o

BUILD=/lib/modules/$(shell uname -r)/build
M=$(shell pwd)

all:
	make -C $(BUILD) M=$(M) modules
	make -C $(BUILD) M=$(M) modules_install
	cp -f 70-alarm-keyboard.rules /etc/udev/rules.d/
	modprobe -r keyboard_alarm
clean:
	make -C $(BUILD) M=$(M) clean
