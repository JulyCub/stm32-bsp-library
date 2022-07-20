/**
 ******************************************************************************
 * @file		:bsp_dac8563.c
 * @brief		:DAC8563 driver.
 * @version	:0.1.3
 * @author	:康威电子
 * @date		:2022.07.05
 ******************************************************************************
 * @pinset
 *					PB10		-->	DIN
 *					PB11		-->	SCK
 *					PE14		-->	SYN
 *					PE15		-->	CLR
 *					PE12		-->	LD
 *					NONE		--> NC
 *
 ******************************************************************************
 */



#include "bsp_dac8563.h"
#if			1
	#include "bsp_delay.h"
#elif		0
	#include "bsp_dwt.h"
#else
	#include "bsp_SysTick.h"
#endif


uint16_t SineWave_Value[255]={
32967,33775,34581,35387,36190,36992,37791,38588,39380,40169,
40953,41732,42506,43274,44035,44790,45537,46277,47008,47731,
48444,49148,49842,50526,51199,51861,52511,53149,53775,54388,
54988,55575,56147,56706,57250,57779,58292,58791,59273,59740,
60190,60623,61040,61439,61821,62185,62532,62860,63170,63462,
63735,63989,64224,64440,64637,64815,64973,65112,65230,65330,
65409,65469,65509,65528,65528,65509,65469,65409,65330,65230,
65112,64973,64815,64637,64440,64224,63989,63735,63462,63170,
62860,62532,62185,61821,61439,61040,60623,60190,59740,59273,
58791,58292,57779,57250,56706,56147,55575,54988,54388,53775,
53149,52511,51861,51199,50526,49842,49148,48444,47731,47008,
46277,45537,44790,44035,43274,42506,41732,40953,40169,39380,
38588,37791,36992,36190,35387,34581,33775,32967,32160,31353,
30547,29742,28939,28139,27341,26547,25756,24969,24188,23411,
22640,21876,21118,20367,19623,18888,18161,17443,16734,16034,
15345,14667,14000,13344,12699,12067,11448,10841,10248,9668,
9103,8552,8015,7494,6988,6497,6022,5564,5122,4697,
4289,3899,3526,3170,2833,2514,2213,1930,1667,1422,
1196,990,802,635,486,358,248,159,90,40,
10,0,10,40,90,159,248,358,486,635,
802,990,1196,1422,1667,1930,2213,2514,2833,3170,
3526,3899,4289,4697,5122,5564,6022,6497,6988,7494,
8015,8552,9103,9668,10248,10841,11448,12067,12699,13344,
14000,14667,15345,16034,16734,17443,18161,18888,19623,20367,
21118,21876,22640,23411,24188,24969,25756,26547,27341,28139,
28939,29742,30547,31353,32160
};

uint16_t TriangleWave_Value[255]={
511,1023,1535,2047,2559,3071,3583,4095,4607,5119,5631,6143,
	6655,7167,7679,8191,8703,9215,9727,10239,10751,11263,11775,
	12287,12799,13311,13823,14335,14847,15359,15871,16383,16895,
	17407,17919,18431,18943,19455,19967,20479,20991,21503,22015,
	22527,23039,23551,24063,24575,25087,25599,26111,26623,27135,
	27647,28159,28671,29183,29695,30207,30719,31231,31743,32255,
	32767,33279,33791,34303,34815,35327,35839,36351,36863,37375,
	37887,38399,38911,39423,39935,40447,40959,41471,41983,42495,
	43007,43519,44031,44543,45055,45567,46079,46591,47103,47615,
	48127,48639,49151,49663,50175,50687,51199,51711,52223,52735,
	53247,53759,54271,54783,55295,55807,56319,56831,57343,57855,
	58367,58879,59391,59903,60415,60927,61439,61951,62463,62975,
	63487,63999,64511,65023,65535,65023,64511,63999,63487,62975,
	62463,61951,61439,60927,60415,59903,59391,58879,58367,57855,
	57343,56831,56319,55807,55295,54783,54271,53759,53247,52735,
	52223,51711,51199,50687,50175,49663,49151,48639,48127,47615,
	47103,46591,46079,45567,45055,44543,44031,43519,43007,42495,
	41983,41471,40959,40447,39935,39423,38911,38399,37887,37375,
	36863,36351,35839,35327,34815,34303,33791,33279,32767,32255,
	31743,31231,30719,30207,29695,29183,28671,28159,27647,27135,
	26623,26111,25599,25087,24575,24063,23551,23039,22527,22015,
	21503,20991,20479,19967,19455,18943,18431,17919,17407,16895,
	16383,15871,15359,14847,14335,13823,13311,12799,12287,11775,
	11263,10751,10239,9727,9215,8703,8191,7679,7167,6655,6143,
	5631,5119,4607,4095,3583,3071,2559,2047,1535,1023,511,
};
void DAC8563_IO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();	 
	__HAL_RCC_GPIOE_CLK_ENABLE();	 
	
	GPIO_InitStructure.Pin = GPIO_PIN_10|GPIO_PIN_11;		
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 		 			
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;		 			
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);		
	
	GPIO_InitStructure.Pin = GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15;		
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 		 			
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;		 			
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);		
	LDAC_H;
}

void DAC8563_WRITE(uint8_t cmd,uint16_t data)
{
	uint8_t s=0;
  SYNC_H;
  HAL_Delay_us(4);
  SYNC_L;
  SCLK_L;
  for( s=0;s<8;s++)
  {
    if((cmd&0x80)==0x80){DIN_H;}
    else{DIN_L;}
    HAL_Delay_us(4);
    SCLK_H;
    HAL_Delay_us(4);
    cmd<<=1;
    SCLK_L;
    HAL_Delay_us(4);
  }
  for( s=0;s<16;s++)
  {
    if((data&0x8000)==0x8000){DIN_H;}
    else{DIN_L;}
    HAL_Delay_us(4);
    SCLK_H;
    HAL_Delay_us(4);
    data<<=1;
    SCLK_L;
    HAL_Delay_us(4);
  }
}

void DAC8563_Init(void)
{
	CLR_L;
  LDAC_H;
	DAC8563_IO_Init();
	DAC8563_WRITE(CMD_RESET_ALL_REG, DATA_RESET_ALL_REG);			//复位
	DAC8563_WRITE(CMD_PWR_UP_A_B, DATA_PWR_UP_A_B);						//上电DAC-A和DAC-B
	DAC8563_WRITE(CMD_INTERNAL_REF_EN, DATA_INTERNAL_REF_EN);	// 启用内部参考和复位DAC以获得增益= 2
	DAC8563_WRITE(CMD_GAIN, DATA_GAIN_B2_A1);									// Set Gains

}


//=============================================
//
//		设置并更新DAC输出电压（更改模块跳线帽及设置DAC8563内部增益 可设置不同输出范围 ）
//
//	参数：data_a为A路输出，data_b为B路输出 
//
//=============================================
uint16_t DAC8563_NumChangeA(float num)
{
	uint16_t	result = 0;
	
	if(num >= 0 && num <= 10)
	{
		result = (uint16_t)(num / 10.0 * 65535);
	}
	
	return result;
}

uint16_t DAC8563_NumChangeB(float num)
{
	uint16_t	result = 32767;
	if(num <= 0 && num >= -10)
	{
		result = (uint16_t)((10 + num)/10.0 * 32767);
	}
	else if(num > 0 && num <= 10)
	{
		result = (uint16_t)(num / 10.0 * 32767 + 32768);
	}
	return result;
}

void DAC8563_OutPutAB(float vlotA, float vlotB)
{
	uint16_t dataA, dataB;
	
	dataA = DAC8563_NumChangeA(vlotA);
	dataB = DAC8563_NumChangeB(vlotB);
	DAC8563_WRITE(CMD_SETA_UPDATEA, dataA);	
	DAC8563_WRITE(CMD_SETB_UPDATEB, dataB);		
	LDAC_L;
  HAL_Delay_us(4);
  LDAC_H;
}



