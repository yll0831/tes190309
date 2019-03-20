#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/module.h>
#include <asm/string.h>

#define		GPM4CON	0x110002e0    //led
#define		GPM4DAT	0x110002e4
#define 	GPX3CON 0x11000c60	//key1 ~ 3 -> GPX3_2 ~ 5
#define 	GPX3DAT 0x11000c64  	
#define		INTERBUFSIZE 32

static void __iomem * led_conptr = NULL;
static void __iomem * led_datptr = NULL;
static void __iomem * key_conptr = NULL;
static void __iomem * key_datptr = NULL;

static u32 reg_value = 0;	//暂存寄存器的值
static struct file_operations g_top;	//文件操作函数结构体
static int major = 0;	//早期字符主设备号

ssize_t misc_led_write (struct file * flip, const char __user * buf, size_t count, loff_t * offset)
{
	u32 ret;
	char inter_buf[INTERBUFSIZE] = {0};
	
	ret = copy_from_user(inter_buf, buf, INTERBUFSIZE);
	reg_value = readl(led_datptr);
	if(strncmp(inter_buf, "led1",4) == 0)	//led1
	{
		if(strncmp(inter_buf + 4, "_on",3) == 0)
		{
			reg_value &= ~(0x1 << (1 * 0));
		}
		else if(strncmp(inter_buf + 4, "_off",4) == 0)
		{
			reg_value |= 0x1 << (1 * 0);
		}
		else if(strncmp(inter_buf + 4, "_xor",4) == 0)
		{
			reg_value ^= 0x1 << (1 * 0);
		}
	}	
	else if(strncmp(inter_buf, "led2",4) == 0)	//led2
	{
		if(strncmp(inter_buf + 4, "_on",3) == 0)
		{
			reg_value &= ~(0x1 << (1 * 1));
		}
		else if(strncmp(inter_buf + 4, "_off",4) == 0)
		{
			reg_value |= 0x1 << (1 * 1);
		}
		else if(strncmp(inter_buf + 4, "_xor",4) == 0)
		{
			reg_value ^= 0x1 << (1 * 1);
		}
	}
	else if(strncmp(inter_buf, "led3",4) == 0)	//led3
	{
		if(strncmp(inter_buf + 4, "_on",3) == 0)
		{
			reg_value &= ~(0x1 << (1 * 2));
		}
		else if(strncmp(inter_buf + 4, "_off",4) == 0)
		{
			reg_value |= 0x1 << (1 * 2);
		}
		else if(strncmp(inter_buf + 4, "_xor",4) == 0)
		{
			reg_value ^= 0x1 << (1 * 2);
		}
	}
	else if(strncmp(inter_buf, "led4",4) == 0)	//led4
	{
		if(strncmp(inter_buf + 4, "_on",3) == 0)
		{
			reg_value &= ~(0x1 << (1 * 3));
		}
		else if(strncmp(inter_buf + 4, "_off",4) == 0)
		{
			reg_value |= 0x1 << (1 * 3);
		}
		else if(strncmp(inter_buf + 4, "_xor",4) == 0)
		{
			reg_value ^= 0x1 << (1 * 3);
		}
	}
	else
	{
		printk("******** error :incorrect enter **********\n");
		return -1;
	}
	
	writel(reg_value,led_datptr);
	return min(count, sizeof(inter_buf));
}

ssize_t misc_key_read (struct file * flip, char __user * buf, size_t count, loff_t * offset)
{
	char inter_buf[INTERBUFSIZE] = {0};
	long ret;
	memset(inter_buf, 0, INTERBUFSIZE);
	reg_value = readl(key_datptr);
	if(!(reg_value & (0x1 << (1 * 0))))	//key1
	{
		strcpy(inter_buf,"key1_on");
	}
	else if(!(reg_value & (0x1 << (1 * 1))))	//key2
	{
		strcpy(inter_buf,"key2_on");
	}
	else if(!(reg_value & (0x1 << (1 * 2))))	//key3
	{
		strcpy(inter_buf,"key3_on");
	}
	else if(!(reg_value & (0x1 << (1 * 3))))	//key4
	{
		strcpy(inter_buf,"key4_on");
	}
	ret = copy_to_user(buf, inter_buf, min(count,strlen(inter_buf) + 1));
	
	return min(count,strlen(inter_buf) + 1);
}


void file_ops_init(struct file_operations * ops)
{
	ops->owner	=	THIS_MODULE;
	ops->read	=	misc_key_read;
	ops->write	=	misc_led_write;
}




static int __init misc_led_init(void)
{
	file_ops_init(&g_top);
	major = register_chrdev(0, "chrdevs_key_led", &g_top);
	printk("========== create a new chrdevs ==========\n");
	printk("name:chrdevs_key_led | major:%d\n",major);
	
		
	led_conptr = ioremap(GPM4CON, 4);
	led_datptr = ioremap(GPM4DAT, 4);
	key_conptr = ioremap(GPX3CON, 4);
	key_datptr = ioremap(GPX3DAT, 4);

	/* led初始化 */
	reg_value = readl(led_conptr);
	reg_value &= ~( 0xf << (0 * 4) );	
	reg_value &= ~( 0xf << (1 * 4) );	
	reg_value &= ~( 0xf << (2 * 4) );	
	reg_value &= ~( 0xf << (3 * 4) );	

	reg_value |=  0x1 << (0 * 4);	
	reg_value |=  0x1 << (1 * 4);	
	reg_value |=  0x1 << (2 * 4);	
	reg_value |=  0x1 << (3 * 4);	
	writel(reg_value,led_conptr);

	/* key初始化 */
	reg_value = readl(key_datptr);
	reg_value &= ~( 0xf << (2 * 4) );	//key1
	reg_value &= ~( 0xf << (3 * 4) );	//key2
	writel(reg_value, key_datptr);	//初始化key1、2对应端口
	return 0;
	
}




static void __exit misc_led_exit(void)
{
	unregister_chrdev(major,  "chrdevs_key_led");
	iounmap(led_conptr);
	iounmap(led_datptr);
	iounmap(key_conptr);
	iounmap(key_datptr);
}


module_init(misc_led_init);
module_exit(misc_led_exit);

MODULE_LICENSE("GPL");








