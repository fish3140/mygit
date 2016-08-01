SECTIONS { 
  firtst  	0x00000000 : { boot.o nand.o}
  second 	0x30000000 : AT(4096) { led.o interrupt.o main.o }
} 