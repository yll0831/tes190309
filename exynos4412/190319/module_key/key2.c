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


#define   	CMD_GET_KEY2  		_IOR('k',  2,  char*)

//module_param(g_devnum, unsigned long,  0644);
//extern dev_t g_devnum;	//设备号
extern unsigned long g_devnum;
static struct file_operations g_tfops;	//文件操作函数结构体
//static  struct class *  g_ptclass = NULL;	//设备类
extern struct class *  g_ptclass;
struct key_into
{
	char  name[12];
	int  gpio;
	int  value;
};

struct key_into g_tkey2;
long cdev_key2_ioctl (struct file * filp, unsigned int cmd, unsigned long arg)
{
	unsigned char key_statu = 1;
	char buf[32] = {0};
	unsigned long ret;
	
	switch(cmd)
	{
		case CMD_GET_KEY2:
			key_statu = gpio_get_value(g_tkey2.gpio);
			if(key_statu == 0)
			{
				msleep(10);
				key_statu = gpio_get_value(g_tkey2.gpio);
				if(key_statu == 0)
				{
					while(1)
					{
						key_statu = gpio_get_value(g_tkey2.gpio);
						if(key_statu == 1)
						{
							break;
						}
					}

				}
				strcpy(buf,g_tkey2.name);
				ret = copy_to_user((void *)arg, buf, strlen(buf) + 1);
				//printk("==== %s has been pressed!\n",g_tkey2.name);
			}
			break;
			
		default:printk("*********222222 error cmd *********\n");
			break;
	}

	return 0;
}




void struct_init(struct file_operations * fops, struct  key_into * kinfo)
{
	/* 文件操作函数结构体 */
	fops->owner= THIS_MODULE;
	fops->unlocked_ioctl = cdev_key2_ioctl;

	/* 按键结构体 */
	strcpy(kinfo->name, "key2");
	kinfo->gpio 	= EXYNOS4_GPX3(3);
	kinfo->value 	= 1;
}






static int __init cdev_key2_init(void)
{
	struct_init(&g_tfops, &g_tkey2);

	
	/* 设备资源申请、初始化 */
	gpio_request(g_tkey2.gpio, g_tkey2.name);
	gpio_direction_input(g_tkey2.gpio);

	/* 创建设备节点文件 */
	device_create(g_ptclass, NULL, g_devnum + 1, NULL, "auto_key2");
	
	return 0;
}




static void __exit cdev_key2_exit(void)
{
	/* 释放设备资源 */
	gpio_free(g_tkey2.gpio);


	/* 移除设备节点文件 */
	device_destroy(g_ptclass, g_devnum + 1);
}














module_init(cdev_key2_init);	//模块初始化
module_exit(cdev_key2_exit);	//模块退出

MODULE_LICENSE("GPL");





