#ifndef __LED_H
#define __LED_H	 

#include "MICO.h"
#include "platform.h"
#include "mico_platform.h"

#define u8     uint8_t
#define u16    uint16_t
#define u32    uint32_t

#define H2R_MAX_RGB_val 255.0

#define   R_PIN       (MICO_GPIO_18)               //pin18
#define   G_PIN       (MICO_GPIO_31)               //pin31
#define   B_PIN       (MICO_GPIO_34)               //pin33

/*******************************************************************************
函数名：LED_InitRGB
描述  ：LED初始化
参数  ：无
返回值：无
*******************************************************************************/
void LED_InitRGB(void);

/*******************************************************************************
函数名：LED_openRGB
描述  ：开启LED灯
参数  ：R_value: 红色分量,范围(0-100)
        G_value: 绿色分量,范围(0-100)
        B_value: 蓝色分量,范围(0-100)
返回值：返回值：0:失败，1：成功
*******************************************************************************/
u8 LED_openRGB(u8 R_value, u8 G_value, u8 B_value);

/*******************************************************************************
函数名：LED_closeRGB
描述  ：关闭所有LED灯
参数  ：无
返回值：无
*******************************************************************************/
void LED_closeRGB(void);
		 				    
#endif
