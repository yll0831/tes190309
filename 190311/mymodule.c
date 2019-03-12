#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>

#define GPX3CON 0x11000c60	//key1 ~ 3 -> GPX3_2 ~ 5
#define GPX3DAT 0x11000c64  
#define GPX0CON 0x114000A0	//beep GPD0_0
#define GPX0DAT 0x114000A4
#define GPM4CON 0x110002e0
#define GPM4DAT 0x110002e4

#define REGSIZE 4
/* 
* 	入口函数 -- 模块加载时自动调用
*	模块加载时要做的所有操作都必须编写在这里
*/
static  int    __init   my_module_init(void)
{
	
	void __iomem * keyConPtr;
	void __iomem * keyDatPtr;
	void __iomem * beepConPtr;
	void __iomem * beepDatPtr;
	void __iomem * ledConPtr;
	void __iomem * ledDatPtr;

	unsigned int keyValue = 0;
	unsigned int value;
	
	keyConPtr 	= ioremap(GPX3CON, REGSIZE);
	keyDatPtr   = ioremap(GPX3DAT, REGSIZE);
	
	beepConPtr	= ioremap(GPX0CON, REGSIZE);
	beepDatPtr 	= ioremap(GPX0DAT, REGSIZE);

	ledConPtr  	= ioremap(GPM4CON, REGSIZE);
	ledDatPtr 	= ioremap(GPM4DAT, REGSIZE);
	

	value = readl(keyConPtr);
	value &= ~( 0xf << (2 * 4) );	//key1
	value &= ~( 0xf << (3 * 4) );	//key2
	writel(value, keyConPtr);	//初始化key1、2对应端口
	 		
	value = readl(beepConPtr);
	value &= ~( 0xf << (0 * 4) );	//beep
	value |=  0x1 << (0 * 4) ;
	writel(value, beepConPtr);	//初始化beep对应端口
	
	value = readl(ledConPtr);
	value &= ~( 0xf << (0 * 4) );	//key1
	value &= ~( 0xf << (1 * 4) );	//key2
	value &= ~( 0xf << (2 * 4) );	//key3
	value &= ~( 0xf << (3 * 4) );	//key4

	value |=  0x1 << (0 * 4);	//key1
	value |=  0x1 << (1 * 4);	//key2
	value |=  0x1 << (2 * 4);	//key3
	value |=  0x1 << (3 * 4);	//key4
	writel(value,ledConPtr);

	
	while(1)
	{
		keyValue = readl(keyDatPtr);
		keyValue = keyValue & ( 1 << (1 * 2) );	//key1
		printk("%x\n",keyValue);
		if(!keyValue)
		{
			value = readl(beepDatPtr);
			value |= 1 << (1 * 0);
			writel(value, beepDatPtr);

			value = readl(ledDatPtr);
			value &= ~0xf;
			writel(value,ledDatPtr);
		}
		keyValue = readl(keyDatPtr);
		keyValue = keyValue & ( 1 << (1 * 3) );	//key2
		if(!keyValue)
		{
			//value = 50000000000000;
			//while(value--);
			value = readl(beepDatPtr);
			value &= ~( 1 << (1 * 0) );
			writel(value, beepDatPtr);

			value = readl(ledDatPtr);
			value |= 0xf;
			writel(value,ledDatPtr);
		}
		
	}
	


	printk("---my first moudle---\n");
	
	return  0;	// 必须返回0，模块才能加载成功
}

/*
*	出口函数 -- 模块卸载时自动调用
*	模块卸载时要做的所有操作都必须在这里完成
*/
static  void   __exit   my_module_exit(void)
{
	iounmap;
	printk("---my moudle finish---\n");
}


/* 模块入口声明，声明哪个函数是模块加载时调用的入口函数 */
module_init(my_module_init);		// 传参是入口函数的函数名
/* 模块出口声明，声明哪个函数是模块卸载时调用的出口函数 */
module_exit(my_module_exit);	//传参是出口函数的函数名

MODULE_LICENSE("GPL");



