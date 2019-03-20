#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/module.h>
#include <asm/string.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define		GPM4CON	0x110002e0    //led
#define		GPM4DAT	0x110002e4
#define 	GPX3CON 0x11000c60	//key1 ~ 3 -> GPX3_2 ~ 5
#define 	GPX3DAT 0x11000c64  	
#define		INTERBUFSIZE 32
#define  	BASEMINOR  	110
#define  	MINORCNT	4
#define  	DEVNAME	"cdev"

static void __iomem * led_conptr = NULL;
static void __iomem * led_datptr = NULL;
static void __iomem * key_conptr = NULL;
static void __iomem * key_datptr = NULL;

/* 1.定义标准字符设备结构体指针变量 */
static  struct  cdev*   g_ptcdevkey = NULL;	//标准字符设备结构体
static  dev_t   g_devnum = 0;	//32 位设备号(包含主设备号和次设备号)

/* 2.文件操作函数结构体 */
static struct file_operations g_top;	//文件操作函数结构体

//重定义write函数
ssize_t misc_led_write (struct file * flip, const char __user * buf, size_t count, loff_t * offset)
{
	u32 ret;
	char inter_buf[INTERBUFSIZE] = {0};
	u32 led_value = 0;
	ret = copy_from_user(inter_buf, buf, INTERBUFSIZE);
	led_value = readl(led_datptr);
	if(strncmp(inter_buf, "led1",4) == 0)	//led1
	{
		if(strncmp(inter_buf + 4, "_on",3) == 0)
		{
		
	led_value &= ~(0x1 << (1 * 0));
		}
		else if(strncmp(inter_buf + 4, "_off",4) == 0)
		{
			led_value |= 0x1 << (1 * 0);
		}
		else if(strncmp(inter_buf + 4, "_xor",4) == 0)
		{
			led_value ^= 0x1 << (1 * 0);
		}
	}	
	else if(strncmp(inter_buf, "led2",4) == 0)	//led2
	{
		if(strncmp(inter_buf + 4, "_on",3) == 0)
		{
		
	led_value &= ~(0x1 << (1 * 1));
		}
		else if(strncmp(inter_buf + 4, "_off",4) == 0)
		{
			led_value |= 0x1 << (1 * 1);
		}
		else if(strncmp(inter_buf + 4, "_xor",4) == 0)
		{
			led_value ^= 0x1 << (1 * 1);
		}
	}
	else if(strncmp(inter_buf, "led3",4) == 0)	//led3
	{
		if(strncmp(inter_buf + 4, "_on",3) == 0)
		{
		
	led_value &= ~(0x1 << (1 * 2));
		}
		else if(strncmp(inter_buf + 4, "_off",4) == 0)
		{
			led_value |= 0x1 << (1 * 2);
		}
		else if(strncmp(inter_buf + 4, "_xor",4) == 0)
		{
			led_value ^= 0x1 << (1 * 2);
		}
	}
	else
	{
		printk("******** error :incorrect enter **********\n");
		return -1;
	}

	writel(led_value,led_datptr);
	printk("%s\n",inter_buf);
	return min(count, sizeof(inter_buf));
}

//重定义read函数
ssize_t misc_key_read (struct file * flip, char __user * buf, size_t count, loff_t * offset)
{
	u32 key_value;
	char inter_buf[INTERBUFSIZE] = {0};
	long ret;
	memset(inter_buf, 0, INTERBUFSIZE);
	key_value = readl(key_datptr);
	if(!(key_value & (0x1 << (1 * 2))))	//key1
	{
		strcpy(inter_buf,"key1_on");
		while(1)
		{
			key_value = readl(key_datptr);
			if((key_value & (0x1 << (1 * 2))))
			{
				break;
			}
		}
		
		printk("%s\n",inter_buf);
	}
	else if(!(key_value & (0x1 << (1 * 3))))	//key2
	{
		if(!(key_value & (0x1 << (1 * 3))))
		strcpy(inter_buf,"key2_on");
		while(1)
		{
			key_value = readl(key_datptr);
			if((key_value & (0x1 << (1 * 3))))
			{
				break;
			}
		}
		printk("%s\n",inter_buf);
	}
	else if(!(key_value & (0x1 << (1 * 4))))	//key3
	{
		strcpy(inter_buf,"key3_on");
		while(1)
		{
			key_value = readl(key_datptr);
			if((key_value & (0x1 << (1 * 4))))
			{
				break;
			}
		}
		printk("%s\n",inter_buf);
	}
	else if(!(key_value & (0x1 << (1 * 5))))	//key4
	{
		strcpy(inter_buf,"key4_on");
		while(1)
		{
			key_value = readl(key_datptr);
			if((key_value & (0x1 << (1 * 5))))
			{
				break;
			}
		}
		printk("%s\n",inter_buf);
	}
		
	ret = copy_to_user(buf, inter_buf, min(count,strlen(inter_buf) + 1));
	
	return min(count,strlen(inter_buf) + 1);
}

//对文件操作函数结构体进行赋值
void file_ops_init(struct file_operations * ops)
{
	ops->owner	=	THIS_MODULE;
	ops->read	=	misc_key_read;
	ops->write	=	misc_led_write;
}




static int __init misc_led_init(void)
{
	u32 reg_value;
	file_ops_init(&g_top);
	/* 3.注册标准字符设备 */
	alloc_chrdev_region(&g_devnum,  BASEMINOR,  MINORCNT,  DEVNAME);	//分配字符设备类
	g_ptcdevkey = cdev_alloc();	//分配标准字符设备类型结构体
	cdev_init(g_ptcdevkey,  &g_top);	//对标准字符设备结构体初始化
	cdev_add(g_ptcdevkey,  g_devnum,  MINORCNT);	//将标准字符设备加入到内核
	printk("========== create a new cdev ==========\n");
	printk("--- major: %d --- minor: %d  ---\n",  MAJOR(g_devnum), MINOR(g_devnum)); 
	
	
		
	led_conptr = ioremap(GPM4CON, 4);
	led_datptr = ioremap(GPM4DAT, 4);
	key_conptr = ioremap(GPX3CON, 4);
	key_datptr = ioremap(GPX3DAT, 4);

	/* led初始化 */
	reg_value = readl(led_conptr);
	reg_value &= ~( 0xf << (0 * 4) );	
	reg_value &= ~( 0xf << (1 * 4) );	
	//reg_value &= ~( 0xf << (2 * 4) );	
	//reg_value &= ~( 0xf << (3 * 4) );	

	reg_value |=  0x1 << (0 * 4);	//输出模式
	reg_value |=  0x1 << (1 * 4);	
	//reg_value |=  0x1 << (2 * 4);		//灯坏了
	//reg_value |=  0x1 << (3 * 4);
	writel(reg_value,led_conptr);
	
	reg_value = readl(led_datptr);	//灯灭
	reg_value |= 0xf;
	writel(reg_value,led_datptr);

	/* key初始化 */
	reg_value = readl(key_conptr);
	reg_value &= ~( 0xf << (2 * 4) );	//key1	输入模式
	reg_value &= ~( 0xf << (3 * 4) );	//key2
	reg_value &= ~( 0xf << (4 * 4) );	//key3
	reg_value &= ~( 0xf << (5 * 4) );	//key4
	writel(reg_value, key_conptr);	//初始化key1、2对应端口

	
	
	return 0;
	
}




static void __exit misc_led_exit(void)
{
	/* 4.释放标准字符设备结构体 */
	cdev_del(g_ptcdevkey);
	kfree(g_ptcdevkey);
	unregister_chrdev_region(g_devnum,  MINORCNT);
	iounmap(led_conptr);
	iounmap(led_datptr);
	iounmap(key_conptr);
	iounmap(key_datptr);
}


module_init(misc_led_init);
module_exit(misc_led_exit);

MODULE_LICENSE("GPL");









