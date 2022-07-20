/*
 *****************************************************************************
 * @file		:bsp_delay.c
 * @brief		:使用SysTick的普通计数模式对延迟进行管理.
 * @version	:1.0
 * @author	:merchant
 * @date		:2022.07.05
 *****************************************************************************
 * @ File name -> delay.c
 * @ Version   -> V1.0.1
 * @ Date      -> 02-26-2014
 * @ Brief     -> 系统延时相关的函数
 *
 * @ 详细说明请参考《Cortex-M3权威指南(中文)》第133 ~ 134页 第8章 SysTick定时器介绍
 *
 * @ Revise    -> 增加另外一种延时计算方法
 ******************************************************************************
 */

/*
* @ SysTick定时器 相关控制寄存器说明

@ 1、SysTick控制及状态寄存器（地址：0xE000_E010）复位值为0

	bit16 COUNTFLAG(R）  -> 如果在上次读取本寄存器后，SysTick已经数到了0，则该位为1。如果读取该位，该位将自动清零
	bit2  CLKSOURCE(R/W) -> 0=外部时钟源(STCLK)。1=内核时钟(FCLK) 
	bit1  ENABLE(R/W)    -> SysTick定时器的使能位

@ 2、SysTick重装载数值寄存器（地址：0xE000_E014）复位值为0

	[23:0] RELOAD(R/W) -> 当倒数至零时，将被重装载的值

@ 3、SysTick当前数值寄存器（地址：0xE000_E018） 复位值为0

	[23:0] CURRENT(R/Wc) -> 读取时返回当前倒计数的值，写它则使之清零，同时还会清除在SysTick 控制及状态寄存器中的COUNTFLAG标志

@ 4、SysTick校准数值寄存器（地址：0xE000_E01C）复位值: bit31未知。bit30未知。[23:0]为0

	bit32 NOREF(R)    -> 1=没有外部参考时钟（STCLK不可用）。0=外部参考时钟可用
	bit30 SKEW(R)     -> 1=校准值不是准确的10ms。0=校准值是准确的10ms
	[23:0] TENMS(R/W) -> 10ms的时间内倒计数的格数。芯片设计者应该通过Cortex‐M3的输入信号提供该数值。若该值读回零，则表示无法使用校准功能
	
*/ 

#include "bsp_delay.h"


void delay_us(uint32_t nus)
{
 uint32_t temp;
 SysTick->LOAD = 18*nus;
 SysTick->VAL=0X00;
 SysTick->CTRL=0X01;
 do
 {
  temp=SysTick->CTRL;
 }while((temp&0x01)&&(!(temp&(1<<16))));
     SysTick->CTRL=0x00; 
    SysTick->VAL =0X00; 
}

void delay_ms(uint16_t nms)
{
 uint32_t temp;
 SysTick->LOAD = 500*nms;
 SysTick->VAL=0X00;
 SysTick->CTRL=0X01;
 do
 {
  temp=SysTick->CTRL;
 }while((temp&0x01)&&(!(temp&(1<<16))));
    SysTick->CTRL=0x00; 
    SysTick->VAL =0X00; 
}

void delay_100ns(void)
{
	/* -- */
}

void HAL_Delay_us(uint32_t us)
{       
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000000);
    HAL_Delay(us-1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
}

#if 0
//=========================================================
#if _USER_SysTick	//定义了则使用SysTick定时器做延时函数计数
//=========================================================

/**********************************************************
                     定义计算变量
**********************************************************/

static uint8_t  fac_us=0;	//us延时倍乘数
static uint16_t fac_ms=0;	//ms延时倍乘数

/**********************************************************
* 函数功能 ---> 初始化延时函数
* 入口参数 ---> SYSCLK：系统工作最高的频率。单位M
* 返回数值 ---> none
* 功能说明 ---> 主要是初始化SysTick寄存器 
**********************************************************/
void delay_init(uint8_t SYSCLK)
{
	SysTick->CTRL &= 0xfffffffb;	//bit2清空，选择外部时钟HCLK/8，停止计数
	fac_us = SYSCLK/8;	//系统时钟的1/8		    
	fac_ms = (uint16_t)fac_us*1000;	//ms需要的SysTick时钟数
}
/**********************************************************
* 函数功能 ---> 延时n个us
* 入口参数 ---> nus：要延时的us数
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/		    								   
void delay_us(uint32_t nus)
{		
	uint32_t temp;
		    	 
	SysTick->LOAD = nus*fac_us; //时间加载	  		 
	SysTick->VAL = 0x00;        //清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;      //开始倒数 	 
	do
	{
		temp = SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));	//等待时间到达   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL = 0x00;	//清空计数器	 
}
/**********************************************************
* 函数功能 ---> 延时n个ms
* 入口参数 ---> nus：要延时的us数
* 返回数值 ---> none
* 功能说明 ---> SysTick->LOAD为24位寄存器,所以,最大延时为:
*               nms <= 0xffffff*8*1000/SYSCLK
*               SYSCLK单位为Hz,nms单位为ms
*               注意nms的范围 0 ~ 1864(72M情况下)
**********************************************************/ 
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t temp;
			   
	SysTick->LOAD = (uint32_t)nms*fac_ms;	//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL = 0x00;           //清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;      //开始倒数  
	do
	{
		temp = SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));	//等待时间到达   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL = 0x00;	//清空计数器	  	    
}

void delay_100ns(void)
{
	;
}

//=========================================================
#else	//使用另外资源进行延时计数
//=========================================================

/**********************************************************
                     定义计算变量
**********************************************************/

static uint32_t CPU_CLK;	//用于保存cpu运行频率

/**********************************************************
* 函数功能 ---> 初始化延时函数
* 入口参数 ---> SYSCLK：系统工作最高的频率。单位MHz
* 返回数值 ---> none
* 功能说明 ---> 主要Cortex-M3内核对系统时钟计数单元
*               详细见《Cortex-M3权威指南(中文)》第216页 a)  时钟周期（CYCCNT） 的内容
*               周立功《CM3计数参考手册》第28、29页、第110、125页
**********************************************************/
void delay_init(uint8_t SYSCLK)
{
	CPU_CLK = (uint32_t)SYSCLK*1000*1000;	//得到时钟频率值，单位Hz

	DEM_CTRL |= 1<<24;	//该位必须为1，使能跟踪和调试模块的使用。详细见：周立功《CM3计数参考手册》第115页介绍
						//在没有使用跟踪时，该位使能对功率使用的控制。它能够由应用程序或调试器使能，供ITM使用
						//在DWT能够使用之前，调试异常和监控控制寄存器的TRCENA(bit24)位必须置位

	DWT_CTRL |= 1<<0;	//使能DWT_CYCCNT计数器。
						//如果不使能，则计数器不执行计数操作，因此不会产生PC采样或CYCCNTENA事件。
						//在正常使用时，CYCCNT计数器应由调试器初始化为0。
}
/**********************************************************
* 函数功能 ---> 延时n个us
* 入口参数 ---> nus：要延时的us数
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/	
void delay_us(uint32_t nus)
{
	uint32_t savecount,endcnt,CPU_cnt;

	savecount = DWT_CYCCNT;	//保存计数器当前数值
	CPU_cnt = nus*(CPU_CLK/(1000*1000));	//计算达到所需延时值的CPU时钟数。即多少个系统时钟计数
											//得到更精确延时时间，减去前面代码运行的时间即可

	endcnt = savecount + CPU_cnt;	//计算所需延时时间DWT_CYCCNT的计数值，在溢出时返回到0
	
	if(endcnt > savecount)	//所需延时值大于当前计数值
	{
		while(DWT_CYCCNT < endcnt);	//循环等待所需要的延时时间的CPU时钟计数值
	}
	else	//小于当前计数值
	{
		while(DWT_CYCCNT > endcnt);	//等待计数器溢出翻转
		while(DWT_CYCCNT < endcnt);	//等待所需延时时间到达
	}
}

//=========================================================
#endif
//=========================================================
#endif