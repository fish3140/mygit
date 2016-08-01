boot.bin:boot.s led.c
	arm-linux-gcc -g -c -o boot.o boot.s
	arm-linux-gcc -g -c -o led.o led.c
	arm-linux-gcc -g -c -o nand.o nand.c
	arm-linux-gcc -g -c -o interrupt.o interrupt.c
	arm-linux-gcc -g -c -o main.o main.c
	arm-linux-ld -Tboot.lds  -o boot_elf
	arm-linux-objcopy  -O binary -S boot_elf boot.bin
	arm-linux-objdump -D boot_elf > boot_dis
clean:
	rm -f boot.bin boot_elf  boot_dis *.o
