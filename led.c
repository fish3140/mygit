#define GPBCON  (*(volatile unsigned long *)0x56000010)
#define GPBDAT  (*(volatile unsigned long *)0x56000014)
void led_on()
{
	int i=0;
	GPBCON = (1<<10)|(1<<12)|(1<<14);
	for(;i<10;i++);
	GPBDAT = (1<<5)|(1<<6);
	for(;i<10;i++);
	GPBDAT = 0;
	for(;i<10;i++);
}
