.text
.global _start
_start:
	#;关闭看门狗
	ldr r0 , =0x53000000
	mov r1 , #0
	str r1 , [r0]
	#时钟初始化
	#锁频时间
	ldr r0 , =0x4C000000
	ldr r1 , =0x00ffffff
	str r1 , [r0]
	#设置分频系数
	ldr r0 , =0x4C000014
	mov r1, #0x05 
	str r1, [r0]     
	#; 修改CPU总线模式
    mrc    p15, 0, r1, c1, c0, 0
    orr    r1, r1, #0xc0000000
    mcr    p15, 0, r1, c1, c0, 0
	#设置mpll
    ldr r0, =MPLLCON
    ldr r1, =0x5c011                                ; MPLL is 400MHz
    str r1, [r0]
    mov pc, lr

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
	#代码重定向，拷贝代码到sdram中
	bl nand_init
	
	ldr  r0 , =0x0
	ldr r1 , =0x30000000
	ldr r2 , =0x20000
	bl nand_read
	bl main
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
