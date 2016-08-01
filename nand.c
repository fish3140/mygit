#define NFCONF (*(volatile unsigned long *)0x4E000000)
#define NFCONT (*(volatile unsigned long *)0x4E000004)
#define NFCMMD (*(volatile unsigned long *)0x4E000008)
#define NFADDR (*(volatile unsigned long *)0x4E00000C)
#define NFDATA (*(volatile unsigned long *)0x4E000010)
#define NFSTAT (*(volatile unsigned long *)0x4E000020)

/* GPIO */
#define GPHCON              (*(volatile unsigned long *)0x56000070)
#define GPHUP               (*(volatile unsigned long *)0x56000078)

/* UART registers*/
#define ULCON0              (*(volatile unsigned long *)0x50000000)
#define UCON0               (*(volatile unsigned long *)0x50000004)
#define UFCON0              (*(volatile unsigned long *)0x50000008)
#define UMCON0              (*(volatile unsigned long *)0x5000000c)
#define UTRSTAT0            (*(volatile unsigned long *)0x50000010)
#define UTXH0               (*(volatile unsigned char *)0x50000020)
#define URXH0               (*(volatile unsigned char *)0x50000024)
#define UBRDIV0             (*(volatile unsigned long *)0x50000028)

#define TXD0READY   (1<<2)
void nand_init()
{
	NFCONT=(1<<4)|(1<<8)|(1<<12);
	NFCONT=(1<<0)|(1<<1)|(1<<4);
}

int nand_read(unsigned long *addr,unsigned long *buf, unsigned char len)
{
	int i=0;
	NFCONT &= ~(1<<1);//????
	NFCMMD = 0x00; //??00h
	
	NFADDR = *addr & 0xff;//????5*8
	for(;i<10;i++);
	NFADDR = (*addr>>8)& 0xff;
	for(;i<10;i++);
	NFADDR = (*addr>>16)& 0xff;
	for(;i<10;i++);
	NFADDR = (*addr>>24)& 0xff;
	for(;i<10;i++);
	NFADDR = (*addr>>32)& 0xff;
	for(;i<10;i++);
	NFCMMD = 0x30; ////??30h
	while(!(NFSTAT&1));//????nand????
	for (i=0;i<=len;i++)
		buf[i]=NFDATA;
	
	NFCONT |= 1<<1;//????
	
}
int norflash()
{
	unsigned long *p=(*(volatile unsigned long *)0x0);
	unsigned long aa;
	unsigned long value;
	value=*p;
	*p=12;
	aa=*p;
	if(aa=12)
	{
		*p=value;
		return 0;
	}
	return 1;
}
int copy_to_sdram(unsigned char *source , unsigned char *des, unsigned char len)
{
	int i=0;
	if(norflash())
	{
		for(;i<=len;i++)
			des[i]=source[i];
	}
	else{
		nand_read(source,des,len);
	}
}
/*
 * ???UART0
 * 115200,8N1,???
 */
void uart0_init(void)
{
    GPHCON  |= 0xa0;    // GPH2,GPH3??TXD0,RXD0
    GPHUP   = 0x0c;     // GPH2,GPH3????

    ULCON0  = 0x03;     // 8N1(8????,???,1????)
    UCON0   = 0x05;     // ????,UART????PCLK
    UFCON0  = 0x00;     // ???FIFO
    UMCON0  = 0x00;     // ?????
    UBRDIV0 = 115200; // ????115200
}

/*
 * ??????
 */
void putc(unsigned char c)
{
    /* ??,??????????????????? */
    while (!(UTRSTAT0 & TXD0READY));
    
    /* ?UTXH0????????,UART????????? */
    UTXH0 = c;
}

void puts(char *str)
{
	int i = 0;
	while (str[i])
	{
		putc(str[i]);
		i++;
	}
}
