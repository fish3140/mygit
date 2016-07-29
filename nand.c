#define NFCONF (*(volatile unsigned long *)0x4E000000)
#define NFCONT (*(volatile unsigned long *)0x4E000004)
#define NFCMMD (*(volatile unsigned long *)0x4E000008)
#define NFADDR (*(volatile unsigned long *)0x4E00000C)
#define NFDATA (*(volatile unsigned long *)0x4E000010)
#define NFSTAT (*(volatile unsigned long *)0x4E000020)
void nand_init()
{
	NFCONT=(1<<4)|(1<<8)|(1<<12);
	NFCONT=(1<<0)|(1<<1)|(1<<4);
}

int nand_read(unsigned long *buf,unsigned long *addr, unsigned int len)
{
	int i=0;
	NFCONT &= ~(1<<1);//��Ƭѡ
	NFCMMD = 00h; //����00h
	
	NFADDR = addr & 0xff;//���͵�ַ5*8
	for(;i<10;i++);
	NFADDR = (addr>>8)& 0xff;
	for(;i<10;i++);
	NFADDR = (addr>>16)& 0xff;
	for(;i<10;i++);
	NFADDR = (addr>>24)& 0xff;
	for(;i<10;i++);
	NFADDR = (addr>>32)& 0xff;
	for(;i<10;i++);
	NFCMMD = 30h; ////����30h
	while(!(NFSTAT&1));//�жϵ�ǰnand�Ƿ����
	for (i=0;i<=len;i++)
		buf[i]=NFDATA;
	
	NFCONT |= 1<<1;//ȡ��Ƭѡ
	
}
void main()
{

	
}