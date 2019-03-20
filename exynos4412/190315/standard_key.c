/*#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/mach.h/io.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/asm.h>*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>


#define  BASEMINOR 	110
#define  MINORCNT	4
#define  DEVNAME	"cdev_key"
#define GPX3CON 0x11000c60
#define GPX3DAT 0x11000c64


//led
#define GPM4CON 0x110002e0
#define GPM4DAT 0x110002e4

static void __iomem* rtn_led_con = NULL;
static void __iomem* rtn_led_dat = NULL;
static void __iomem* rtn = NULL;
static u32 value = 0;
static struct cdev* room = NULL;
static dev_t dev = 0;

//char buf[12] = {0};

static int key_read (struct file *filp, char __user *buff, size_t count, loff_t *off)
{
	int i = 0, ret = 0;		//***
	char buf[12] = {0};
	value = readl(rtn);
	for(i = 0;i < 4; i++)
	{
		if(!(value & (0x04 << i)))
		{
				sprintf(buf, "key%d",i+1);
				ret = copy_to_user(buff,buf,min(strlen(buf),count));//
				break;
		}		
	}
	    return  min(strlen(buf),count);
}




static int key_write(struct file *filp, const char __user *buff, size_t count, loff_t * off)
{

	//printk("E\n");
	
    //char buf[12] = {0};
    char buffff[12] = {0};
	int ret = 0;
	
    printk("sdfdsfds\n");
	ret = copy_from_user(buffff,buff,min(strlen(buffff),count));
	printk("%s\n",buffff);
	if(strncmp(buffff,"1",1) == 0)
	{	
		value = readl(rtn_led_dat);
		value |= 0x0f;				
		writel(value, rtn_led_dat);
	}
	return min(strlen(buffff),count);
}



//文件操作集合
static struct file_operations fp = {
	.owner = THIS_MODULE,			//**
	.read = key_read,			//**
	.write = key_write
}; 


//------------------------------------------------------------------------
static int __init key_init(void)
{
//standard
	alloc_chrdev_region(&dev, BASEMINOR,  MINORCNT,  DEVNAME);
	room = cdev_alloc();
	cdev_init(room, &fp);
	cdev_add(room,dev,MINORCNT);
	printk("major : %d   minor: %d\n", MAJOR(dev), MINOR(dev));

//led  
	rtn_led_con = ioremap(GPM4CON, 4);		//
	value = readl(rtn_led_con);
	value  &=  0xffffffff;
	//value  |=  0x00001111;
	writel(value, rtn_led_con);

	rtn_led_dat = ioremap(GPM4DAT, 4);		//
	value = readl(rtn_led_dat);
	value |= 0x0f;					//四盏灯亮
	writel(value, rtn_led_dat);

//key
	rtn = ioremap(GPX3CON,4);		//模式配置
	value = readl(rtn);
	value &= 0xff0000ff;			//输入模式
	writel(value,rtn);
	rtn = ioremap(GPX3DAT,4);
	
	return 0;
}


static void __exit key_exit(void)
{
	//iounmap(rtn);
	iounmap(rtn);
	unregister_chrdev_region(dev,MINORCNT);
	cdev_del(room);
	kfree(room);
}




module_init(key_init);
module_exit(key_exit);
MODULE_LICENSE("GPL");































































































