#include "key.h"

//������ʼ������ 
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
//���뼶��ʱ����	 
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


//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//����ֵ��
//0��û���κΰ�������
//KEY0_PRES��KEY0����
//KEY1_PRES��KEY1����
//WKUP_PRES��WK_UP���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	
    if(mode)
    {
        key_up=1;  //֧������	
    }
    
	if(key_up&&KEY ==0)
	{
		mico_thread_msleep(10);//ȥ���� 
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
	return 0;// �ް�������
}

