compilation
	make 

install
	sudo insmod checkersDriver.ko

check if correctly installed
	sudo lsmod checkersDriver

uninstall
	sudo rmmod checkersDriver
