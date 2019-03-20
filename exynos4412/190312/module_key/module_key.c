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

static u32 reg_value = 0;
static void __iomem * led_conptr = NULL;
static void __iomem * led_datptr = NULL;
static void __iomem * key_conptr = NULL;
static void __iomem * key_datptr = NULL;

static struct file_operations g_tfops;
static struct miscdevice g_my_miscdevice;
static struct miscdevice g_smisc_key;

ssize_t misc_led_write (struct file * flip, const char __user * buf, size_t count, loff_t * offset)
{
	u32 ret;
	char inter_buf[INTERBUFSIZE] = {0};
	
	ret = copy_from_user(inter_buf, buf, INTERBUFSIZE);
	if(strncmp(inter_buf, "led_on",6) == 0)
	{
		reg_value = readl(led_datptr);
		reg_value &= ~0xf;
		writel(reg_value,led_datptr);
	}
	else if(strncmp(inter_buf, "led_off", 7) == 0)
	{
		reg_value = readl(led_datptr);
		reg_value |= 0xf;
		writel(reg_value,led_datptr);
	}
	else
	{
		printk("******** error :incorrect enter **********\n");
	}
	return min(count, sizeof(inter_buf));
}

ssize_t misc_key_read (struct file * flip, char __user * buf, size_t count, loff_t * offset)
{
	char inter_buf[INTERBUFSIZE] = {0};
	long ret;
	reg_value = readl(key_datptr);
	sprintf(inter_buf,"%d",reg_value);
	ret = copy_to_user(buf, inter_buf, min(count,strlen(inter_buf) + 1));

	return min(count,strlen(inter_buf) + 1);
}



void misc_device_init(struct file_operations * g_sfilop, struct miscdevice * g_smiscdev)
{
	g_sfilop->owner	=	THIS_MODULE;
	g_sfilop->write =	misc_led_write;
	
	g_smiscdev->minor	=	MISC_DYNAMIC_MINOR;
	g_smiscdev->name	= 	"my_led";
	g_smiscdev->fops	=	g_sfilop;
}

void misc_key_init(struct file_operations * g_sfilop, struct miscdevice * g_smisc_key)
{
	g_sfilop->owner	=	THIS_MODULE;
	g_sfilop->read	=	misc_key_read;
	
	g_smisc_key->minor	=	MISC_DYNAMIC_MINOR;
	g_smisc_key->name	= 	"my_key";
	g_smisc_key->fops	=	g_sfilop;
}



static int __init misc_led_init(void)
{
	misc_device_init(&g_tfops, &g_my_miscdevice);
	misc_register(&g_my_miscdevice);
	misc_key_init(&g_tfops, &g_smisc_key);
	misc_register(&g_smisc_key);
	
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
	misc_deregister(&g_my_miscdevice);
	misc_deregister(&g_smisc_key);
	iounmap(led_conptr);
	iounmap(led_datptr);
	iounmap(key_conptr);
	iounmap(key_datptr);
}


module_init(misc_led_init);
module_exit(misc_led_exit);

MODULE_LICENSE("GPL");



