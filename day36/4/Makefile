obj-m := hello.o

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/odroid/Odroid/kernel/odroid-5.4.y/ M=$(PWD) modules
clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C /home/odroid/Odroid/kernel/odroid-5.4.y/ M=$(PWD) clean


