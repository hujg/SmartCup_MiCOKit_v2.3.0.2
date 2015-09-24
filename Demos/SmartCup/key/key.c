#include "key.h"

//按键初始化函数 
void KEY_Init( mico_gpio_irq_handler_t handler)
{
     MicoGpioInitialize((mico_gpio_t)KEY_PIN,  INPUT_HIGH_IMPEDANCE);
     if (handler  != NULL)
     {
            MicoGpioEnableIRQ( (mico_gpio_t)KEY_PIN, IRQ_TRIGGER_BOTH_EDGES, handler, NULL );
     }

}  

static void delay_us(unsigned int i)
 {     
    while(i!=0)
       {	
          i--;
        }
 }	
//毫秒级延时函数	 
static void delay_ms(unsigned int time)
{
  while(time!=0)
  {	
    delay_us(2000);
    time--;
  }
}

u8  KEY_getValue(void)
{

   if ( MicoGpioInputGet( (mico_gpio_t)KEY_PIN ) == KEY_UP) 
   {
       
       delay_ms(10);
       if ( MicoGpioInputGet( (mico_gpio_t)KEY_PIN ) == KEY_UP) 
       {
         return KEY_UP;
       }
   } 
   else
   {
       delay_ms(10);
       if ( MicoGpioInputGet( (mico_gpio_t)KEY_PIN ) == KEY_DOWN) 
       {
            return KEY_DOWN;
       }
   }
   return KEY_DOWN;
}


//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	
    if(mode)
    {
        key_up=1;  //支持连按	
    }
    
	if(key_up&&KEY ==0)
	{
		mico_thread_msleep(10);//去抖动 
		key_up=0;
		if(KEY==0)
        {
            return KEY_UP;
        }
		
	}
    else
    { 
        if(KEY ==1)
        {
            key_up=1;
        }
    }
	return 0;// 无按键按下
}

