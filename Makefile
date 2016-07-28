boot.bin:boot.s led.c
	arm-linux-gcc -g -c -o boot.o boot.s
	arm-linux-gcc -g -c -o led.o led.c
	arm-linux-ld -Ttext 0x00000000 -g boot.o led.o -o boot_elf
	arm-linux-objcopy  -O binary -S boot_elf boot.bin
	arm-linux-objdump -D boot_elf > boot_dis
clean:
	rm -f boot.bin boot_elf *.o
