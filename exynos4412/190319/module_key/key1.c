#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/module.h>
#include <asm/string.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/delay.h>
#define   	CMD_GET_KEY1  		_IOR('k',  1,  char*)
#define   	CMD_GET_KEY2  		_IOR('k',  2,  char*)
#define  	BASEMINOR  	110
#define  	MINORCNT	4
#define  	DEVNAME	"cdev_key"

static struct file_operations g_tfops;	//文件操作函数结构体
dev_t g_devnum;	//设备号
//unsigned long g_devnum;	//设备号
static struct cdev * g_ptcdev;	//标准字符设备结构体指针
struct class *  g_ptclass = NULL;	//设备类

struct key_into
{
	char  name[12];
	int  gpio;
	int  value;
};

struct key_into g_tkey[4];
long cdev_key_ioctl (struct file * filp, unsigned int cmd, unsigned long arg)
{
	unsigned char key_statu = 1;
	char buf[32] = {0};
	unsigned long ret;

	//printk("%d\n", cmd);
	switch(cmd)
	{
		case CMD_GET_KEY1:
			key_statu = gpio_get_value(g_tkey[0].gpio);
			if(key_statu == 0)
			{
				msleep(10);
				key_statu = gpio_get_value(g_tkey[0].gpio);
				if(key_statu == 0)
				{
					while(1)
					{
						key_statu = gpio_get_value(g_tkey[0].gpio);
						if(key_statu == 1)
						{
							break;
						}
					}

				}
				strcpy(buf,g_tkey[0].name);
				ret = copy_to_user((void *)arg, buf, strlen(buf) + 1);
				//printk("==== %s has been pressed!\n",g_tkey[0].name);
			}
			break;
			
		case CMD_GET_KEY2:
			key_statu = gpio_get_value(g_tkey[1].gpio);
			if(key_statu == 0)
			{
				msleep(10);
				key_statu = gpio_get_value(g_tkey[1].gpio);
				if(key_statu == 0)
				{
					while(1)
					{
						key_statu = gpio_get_value(g_tkey[1].gpio);
						if(key_statu == 1)
						{
							break;
						}
					}

				}
				strcpy(buf,g_tkey[1].name);
				ret = copy_to_user((void *)arg, buf, strlen(buf) + 1);
				//printk("==== %s has been pressed!\n",g_tkey[1].name);
			}
			break;
			
		default:printk("*********11111 error cmd *********\n");
			break;
	}

	return 0;
}




void struct_init(struct file_operations * fops, struct  key_into * k)
{
	/* 文件操作函数结构体 */
	fops->owner= THIS_MODULE;
	fops->unlocked_ioctl = cdev_key_ioctl;

	/* 按键结构体 */
	strcpy(k[0].name, "key1");
	k[0].gpio 	= EXYNOS4_GPX3(2);
	k[0].value 	= 1;

	strcpy(k[1].name, "key2");
	k[1].gpio 	= EXYNOS4_GPX3(2);
	k[1].value 	= 1;
}






static int __init cdev_key_init(void)
{
	struct_init(&g_tfops, &g_tkey[0]);

	/* 注册标准字符设备 */
	alloc_chrdev_region(&g_devnum,  BASEMINOR,  MINORCNT,  DEVNAME);	//分配字符设备类
	g_ptcdev = cdev_alloc();	//分配标准字符设备类型结构体
	cdev_init(g_ptcdev,  &g_tfops);	//对标准字符设备结构体初始化
	cdev_add(g_ptcdev,  g_devnum,  MINORCNT);	//将标准字符设备加入到内核
	printk("========== create a new cdev ==========\n");
	printk("--- major: %d --- minor: %d  ---\n",  MAJOR(g_devnum), MINOR(g_devnum)); 
	
	/* 设备资源申请、初始化 */
	gpio_request(g_tkey[0].gpio, g_tkey[0].name);
	gpio_direction_input(g_tkey[0].gpio);

	/* 创建设备节点文件 */
	g_ptclass = class_create(THIS_MODULE,  "button");
	device_create(g_ptclass, NULL, g_devnum, NULL, "auto_key1");
	
	return 0;
}




static void __exit cdev_key_exit(void)
{
	/* 释放设备资源 */
	gpio_free(g_tkey[0].gpio);

	/* 释放标准字符设备结构体 */
	cdev_del(g_ptcdev);
	kfree(g_ptcdev);
	unregister_chrdev_region(g_devnum,  MINORCNT);

	/* 移除设备节点文件 */
	device_destroy(g_ptclass, g_devnum);
	class_destroy(g_ptclass);
}














module_init(cdev_key_init);	//模块初始化
module_exit(cdev_key_exit);	//模块退出

MODULE_LICENSE("GPL");

EXPORT_SYMBOL(g_devnum);
EXPORT_SYMBOL(g_ptclass);

