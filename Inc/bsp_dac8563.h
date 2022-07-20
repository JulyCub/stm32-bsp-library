/**
 ******************************************************************************
 * @file		:bsp_dac8563.h
 * @brief		:DAC8563 driver.
 * @version	:0.1.3
 * @author	:��������
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


#ifndef __BSP_DAC8563_H__
#define __BSP_DAC8563_H__

#include "bsp_config.h" 


#define ADC8563_DIN   	PBout(10)
#define ADC8563_SCLK 		PBout(11)
#define ADC8563_SYNC		PEout(14)
#define ADC8563_CLR 		PEout(15)
#define ADC8563_LDAC		PEout(12)

#define SCLK_H       ADC8563_SCLK = 1
#define SYNC_H       ADC8563_SYNC = 1
#define CLR_H        ADC8563_CLR = 1
#define LDAC_H       ADC8563_LDAC = 1
#define DIN_H        ADC8563_DIN = 1

#define SCLK_L       ADC8563_SCLK = 0
#define SYNC_L       ADC8563_SYNC = 0
#define CLR_L        ADC8563_CLR = 0
#define LDAC_L       ADC8563_LDAC = 0
#define DIN_L        ADC8563_DIN = 0

#define CMD_SETA_UPDATEA      		0x18	// д��DAC-A����Ĵ���������DAC-A +16λ����
#define CMD_SETB_UPDATEB      		0x19	// д��DAC-B����Ĵ���������DAC-B +16λ����
#define CMD_UPDATE_ALL_DACS	   		0x0F	// ��������DAC ��16λֻ��ʱ�Ӽ���

// Gain
#define CMD_GAIN	    						0x02	// default with internal Vref
#define DATA_GAIN_B2_A2    				0x00	// ���棺DAC-B����= 2��DAC-A����= 2��Ĭ��Ϊ�ڲ�VREF��
#define DATA_GAIN_B2_A1       		0x01	// ���棺DAC-B����= 2��DAC-A����= 1
#define DATA_GAIN_B1_A2         	0x02	// ���棺DAC-B����= 1��DAC-A����= 2
#define DATA_GAIN_B1_A1       		0x03	// ���棺DAC-B����= 1��DAC-A����= 1���ϵ�Ĭ��ֵ��


#define CMD_PWR_UP_A_B		      		0x20	// ����DAC�ϵ�����ģʽ
#define DATA_PWR_UP_A		      	  	0x0001	// �ϵ�DAC-A 
#define DATA_PWR_UP_B		      	  	0x0002	// �ϵ� DAC-B
#define DATA_PWR_UP_A_B		      		0x0003	// �ϵ�DAC-A��DAC-B
#define DATA_PWR_DOWN1_A		      	0x0041	// �ض�DAC-A; 1k����GND
#define DATA_PWR_DOWN1_B		      	0x0042	// �ض�DAC-B; 1k����GND
#define DATA_PWR_DOWN1_A_B		      0x0043	// �ض�DAC-A��DAC-B; 1k����GND
#define DATA_PWR_DOWN2_A		      	0x0081	// �ض�DAC-A; 100k����GND
#define DATA_PWR_DOWN2_B		      	0x0082	// �ض�DAC-B; 100k����GND
#define DATA_PWR_DOWN2_A_B		      0x0083	// �ض�DAC-A��DAC-B; 100k����GND
#define DATA_PWR_DOWN3_A		      	0x00C1	// �ض�DAC-A; ����
#define DATA_PWR_DOWN3_B		      	0x00C2	// �ض�DAC-B; ����
#define DATA_PWR_DOWN3_A_B		      0x00C3	// �ض�DAC-A��DAC-B;����


#define CMD_RESET_ALL_REG	      	0x28	// Software reset
#define DATA_RESET_ALL_REG     		0x0001	// ��λ���мĴ�������������DAC���ϵ縴λ���£�
#define DATA_RESET_ADCAB     			0x0000 //��λDAC-A��DAC-B����Ĵ�������������DAC


#define CMD_LDAC_DIS	        		0x30	// ����LDAC�Ĵ���
#define DATA_LDAC_AB       				0x0000 //LDAC������Ч����DAC-B��DAC-A
#define DATA_LDAC_B       				0x0001 //LDAC������Ч����DAC-B; ��DAC-A��Ч
#define DATA_LDAC_A       				0x0002  //LDAC���Ŷ�DAC-B��Ч; DAC-A��Ч
#define DATA_LDAC_NAB       			0x0003 //LDAC���Ŷ�DAC-B��DAC-A��Ч

#define CMD_INTERNAL_REF_DIS    	0x38	// Disable internal reference and reset DACs to gain = 1
#define DATA_INTERNAL_REF_DIS    	0x0000	// �����ڲ��ο�����DAC��λΪ����= 1

#define CMD_INTERNAL_REF_EN       0x38	// Enable Internal Reference & reset DACs to gain = 2
#define DATA_INTERNAL_REF_EN    	0x0001	//�����ڲ��ο��͸�λDAC�Ի������= 2

extern uint16_t SineWave_Value[];
extern uint16_t TriangleWave_Value[];
void DAC8563_IO_Init(void);
void DAC8563_Init(void);
void DAC8563_WRITE(uint8_t cmd,uint16_t data);
void DAC8563_OutPutAB(float vlotA, float vlotB);

#endif









