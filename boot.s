.text
.global _start
_start:
	#;关闭看门狗
	ldr r0 , =0x53000000
	mov r1 , #0
	str r1 , [r0]
	
	#sdram init BWSCON =0x48000000 
	ldr r0 , =0x48000000
	adr r1 , sdram_init
	add r3 , r0 , #(13*4)
co_my:
	ldr r2 , [r1], #4
	str r2 , [r0] ,#4
	cmp r3 , r0
	bne co_my
	#;点个灯
	ldr sp , =0x34000000
	bl led_on
MAIN_LOOP:
       	b	MAIN_LOOP
sdram_init:
	.long 0x22011110	 
	.long 0x00000700	 
	.long 0x00000700	 
	.long 0x00000700	 
	.long 0x00000700	 
	.long 0x00000700	 
	.long 0x00000700	
	.long 0x00018005	
	.long 0x00018005	 
	.long 0x008C04F4	 
	.long 0x000000B1	 
	.long 0x00000030	
	.long 0x00000030	 
