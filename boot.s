.equ        IRQ_IN,         0x5f
.equ        IRQ_EIN,         0x2ff
.text
.global _start
_start:
	b reset
	b .
	b .
	b .
	b .
	b .
	b HandleIRQ 
	#0x18
	b .
reset:
	#;关闭看门狗
	ldr r0 , =0x53000000
	mov r1 , #0
	str r1 , [r0]
	#时钟初始化
	#锁频时间 默认时间
	ldr r0 , =0x4C000000
	ldr r1 , =0x00ffffff
	str r1 , [r0]
	#设置分频系数 mpll=fclk=4hclk=8pclk
	#cpu----fclk 400
	#hclk----AHB 内部高速总线 100
	#pclk----APB 内部的外设总线 50
	ldr r0 , =0x4C000014
	mov r1, #0x05 
	str r1, [r0]     
	#; 修改CPU总线模式
	#/* 如果HDIVN非0，CPU的总线模式应该从“fast bus mode”变为“asynchronous bus mode” */
    mrc    p15, 0, r1, c1, c0, 0
    orr    r1, r1, #0xc0000000
    mcr    p15, 0, r1, c1, c0, 0
	#设置mpllcon
    ldr r0, =0x4C000004
	 # MPLL is 400MHz
    ldr r1, =0x5c011                               
    str r1, [r0]


	#sdram init BWSCON =0x48000000 
	ldr r0 , =0x48000000
	adr r1 , sdram_init
	add r3 , r0 , #(13*4)
co_my:
	ldr r2 , [r1], #4
	str r2 , [r0] ,#4
	cmp r3 , r0
	bne co_my
	
	ldr sp , =0x34000000
	msr cpsr_c, #0xd2       @ 进入中断模式
    ldr sp, =4096    @ 设置中断模式栈指针

    msr cpsr_c, #0xd3       @ 进入管理模式
    ldr sp, =0x34000000     @ 设置管理模式栈指针，
	#代码重定向，拷贝代码到sdram中
	
	bl nand_init
	
	mov  r0 , #0
	ldr r1 , =_start
	ldr r2 , =0x20000
	bl copy_to_sdram
	#在指定运行地址为0x30000000（内存中），
	#前端代码任然可以在0x0地址处运行原因：以上为位置无关代码
	#实现代码从片内内存跳转到sdram中
	ldr pc,=go_sdram 
go_sdram:
	#;点个灯
	bl led_on
	#;中断初始化 GPGcon
	ldr r0 , =0x56000060
	mov r1 , #0x2
	str r1 , [r0]
	#EINTMASK 
	ldr r0 , =0x560000A4
	ldr r1 , =IRQ_EIN
	str r1 , [r0]
	#INTMSK
	ldr r0 , =0X4A000008
	ldr r1 , =IRQ_IN
	str r1 , [r0]
	#管理模式，允许中断
	msr cpsr_c, #0x53
	ldr lr, =MAIN_LOOP
	
	
	bl main
MAIN_LOOP:
       	b	MAIN_LOOP

HandleIRQ:
    sub lr, lr, #4                  @ 计算返回地址
    stmdb   sp!,    { r0-r12,lr }   @ 保存使用到的寄存器
                                    @ 注意，此时的sp是中断模式的sp
                                    @ 初始值是上面设置的3072
    
    ldr lr, =int_return             @ 设置调用ISR即EINT_Handle函数后的返回地址  
    ldr pc, =EINT_Handle            @ 调用中断服务函数，在interrupt.c中
int_return:
    ldmia   sp!,    { r0-r12,pc }^  @ 中断返回, ^表示将spsr的值复制到cpsr
	
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
