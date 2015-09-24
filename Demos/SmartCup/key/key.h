#ifndef __KEY_H
#define __KEY_H	 

#include "MICO.h"
#include "platform.h"
#include "mico_platform.h"

#define u8     uint8_t
#define u16    uint16_t
#define u32    uint32_t


#define  KEY_PIN	       (MICO_GPIO_19)	
#define  KEY              MicoGpioInputGet(KEY_PIN) 

#define KEY_PRES	1		//KEY0   
#define KEY_UP  	1
#define KEY_DOWN  	0

void KEY_Init( mico_gpio_irq_handler_t handler);//IO初始化
u8  KEY_getValue(void);
u8 KEY_Scan(u8 mode); //按键扫描函数	

#endif
