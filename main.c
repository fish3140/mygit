#include "boot.h"

extern void uart0_init(void);
extern void nand_read(unsigned int addr, unsigned char *buf, unsigned int len);
extern void puts(char *str);

static struct tag *params;

 void setup_start_tag ()
{
	params = (struct tag *)0x30000100;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}
 void setup_memory_tags ()
{
	int i;
		params->hdr.tag = ATAG_MEM;
		params->hdr.size = tag_size (tag_mem32);

		params->u.mem.start = 0x30000000;
		params->u.mem.size = 0x4000000;

		params = tag_next (params);
	
}
int strlen(char *str)
{
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return i;
}

void strcpy(char *dest, char *src)
{
	while ((*dest++ = *src++) != '\0');
}

 void setup_commandline_tag (char *commandline)
{
	char *p;

	if (!commandline)
		return;

	/* eat leading white space */
	for (p = commandline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy (params->u.cmdline.cmdline, p);

	params = tag_next (params);
}
 void setup_end_tag ()
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}
//uboot启动内核
void main()
{
	void (*theKernel)(int zero, int arch, uint params);
	
	uart0_init();
	puts("hello world\n");
	
	//1.拷贝内核及文件系统到sdram ---nand read 代码需要修改
	nand_read(0x60000+64, (unsigned char *)0x30008000, 0x200000);
	//2.设置启动参数
	setup_start_tag ();
	setup_memory_tags ();
	setup_commandline_tag ("noinitrd root=/dev/mtdblock3 init=/linuxrc console=ttySAC0");
	setup_end_tag ();
	//3.执行跳转
	theKernel = (void (*)(int, int, unsigned int))0x30008000;
	theKernel (0, 362, 0x30000100);
	
	
}