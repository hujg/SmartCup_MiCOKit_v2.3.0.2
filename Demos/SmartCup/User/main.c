/******************************************************************************
* @file    main.c 
* @author  Jianguang Hu
* @version V1.0.0
* @date    5-sept-2015
* @brief   Smart cup main.
*******************************************************************************/


#include "MiCO.h" 
#include "syscom.h"
#include "MICOAppDefine.h"

extern void net_test( app_context_t * const app_context );
extern OSStatus net_init( app_context_t * const app_context );

/*����ˮ����ģʽ */
typedef enum
{
    PLAY_MP3_LED = 0,
    PLAY_ONLY_MP3,
    PLAY_ONLY_LED
}PLAY_MODE;


typedef struct 
{							  
	mico_timer_t cup_timer;   //��ˮ��ʱ��
	int          unitTimes;  //��λʱ�䣬��ʱ����  	 
    
    mico_semaphore_t  playMp3_sem; //����mp3�ź���
    mico_semaphore_t  playLed_sem; //����led�ź���
    mico_semaphore_t  stopLed_sem; //ֹͣ����led�ź���
    int               drinkTime;   //��ˮʱ�䣬��λ����
    PLAY_MODE         playMode;    //���Ѻ�ˮ��ʾģʽ
}cup_obj;

static cup_obj  cupTimeObj;

 //������ˮ��ʱ
static void TIMER_start(void) 
{
    //��ȡ����
    
    mico_reload_timer(&cupTimeObj.cup_timer);
    cupTimeObj.unitTimes = 0;
    mico_start_timer(&cupTimeObj.cup_timer);
}

//ֹͣ��ˮ��ʱ
static void TIMER_stop(void)
{
    mico_stop_timer(&cupTimeObj.cup_timer);
}

//ÿһ���ӳ�ʱ������
static void cup_timer_timeout_handler( void* inContext )
{
    cup_obj *pCpuTimeObj;
    pCpuTimeObj = (cup_obj *)inContext;

    pCpuTimeObj->unitTimes ++;
    if (pCpuTimeObj->unitTimes == pCpuTimeObj->drinkTime)
    {
        printf("go to drink++++++++++++++\r\n");
        mico_rtos_set_semaphore(&pCpuTimeObj->playMp3_sem);  //֪ͨ��������
        mico_rtos_set_semaphore(&pCpuTimeObj->playLed_sem);  //֪ͨ��LED��
        TIMER_start();
    }
}

//�ᱭ�ͷű�������
static void  KEY_irq_handler( void* arg )
{
  (void)(arg);

   if ( KEY_getValue() == KEY_UP) 
   {
        printf("key up!\r\n");
        TIMER_stop();
   } 
   else
   {
        TIMER_start();
        printf("key down!\r\n");
   }
    
}

/* �ƹ⴦���߳� */
void LED_handleThread(void *inContext)
{
    u8  ledValue = 0;
    u8  playLedTimes = 0; //�޲�������ʱ����ʾLED����
    
    printf( "This is LED_handleThread.\r\n");
    LED_openRGB(0, 0, 0); //��ʼ���ر����е�
    LED_closeRGB();
    
     while(mico_rtos_get_semaphore(&cupTimeObj.playLed_sem, MICO_WAIT_FOREVER) == kNoErr)
    {
        
       while(mico_rtos_get_semaphore(&cupTimeObj.stopLed_sem, MICO_NO_WAIT) != kNoErr \
           && cupTimeObj.playMode != PLAY_ONLY_MP3)
       {
          printf("Playing led ........\r\n");
#if 0
          for (ledValue = 0; ledValue < 100; ledValue++)
          {
              LED_openRGB(100 - ledValue, ledValue, ledValue);
              mico_thread_msleep(15);
          }
          mico_thread_msleep(100);
          
          for (ledValue = 100; ledValue > 0; ledValue--)
          {
              LED_openRGB(100 - ledValue, ledValue, ledValue);
              mico_thread_msleep(15);
          } 
          mico_thread_msleep(100);
#else
          /* ���� */
          LED_openRGB(50, 50, 50);
          mico_thread_msleep(500);
          LED_openRGB(0, 0, 0);
          mico_thread_msleep(500);
#endif
          
          /* ֻ��LEDģʽʱ��LEDֻ��ʾ���� */
          if (cupTimeObj.playMode == PLAY_ONLY_LED)
          {
              playLedTimes++;
              if (playLedTimes == 3 || KEY_getValue() == KEY_UP)
              {
                  playLedTimes = 0; 
                  break;
              }
          }
       }
       
       LED_openRGB(0, 0, 0);
       LED_closeRGB();
     
    }
}

/* MP3���ִ����߳� */
void MP3_handleThread(void *inContext)
{
    SD_sizeInfo sdSizeInfo;
    u32 ret = 0;
    u8  *pBuf = NULL;
    
    
     /* ����Ƿ����SD�� */
     if (MP3_sdCardDetect() == 0)
    {
        while(1)
       {
         printf( "sdCard not insert !!!!!\r\n");
         mico_thread_sleep( 1 );
       }
    }

     /* MP3��ʼ����*/   
    if (MP3_init() == 0)
    {
        while(1)
       {
         printf( "mp3 init failed !!!!!\r\n");
         mico_thread_sleep( 1 );
       }
    }

    /* ��ȡSD��������Ϣ */
    MP3_getSdSize(&sdSizeInfo);
    printf("%d MB total drive space.\r\n" "%d MB available.\r\n",
           sdSizeInfo.totalSize, sdSizeInfo.availableSize);
 
    /* ��ȡMP3�ļ����� */
    ret = MP3_getMp3FileNum("0:/MUSIC");
    printf("mp3FileNum=%d\r\n", ret);

#if 0
    /* д�ļ� */
    pBuf = malloc(512);
    if (pBuf != NULL)
    {
      memset(pBuf, 0x5a, 512); 
      ret = MP3_writeSong("0:/MUSIC/hujg.mp3", 0, pBuf, 512);
      printf("ret0=%d\r\n", ret);
      memset(pBuf, 0xa5, 512);
      ret = MP3_writeSong("0:/MUSIC/hujg.mp3", 512, pBuf, 512);
      printf("ret1=%d\r\n", ret);
      free(pBuf);
    }
  
     /* ɾ���ļ� */
     ret = MP3_removeSong("0:/MUSIC/hujg_bak.mp3");
    if (ret == 1)
    {
       printf("remove mp3 file success!\r\n");
    }
    else
    {
       printf("remove mp3 file failed !\r\n");
    }
#endif
    
    /* ����ָ���������������� */
    while(mico_rtos_get_semaphore(&cupTimeObj.playMp3_sem, MICO_WAIT_FOREVER) == kNoErr)
    {
       if (cupTimeObj.playMode != PLAY_ONLY_LED)  //������ǵ��������Ѻ�ˮʱ
       {
         MP3_setVolume(20);
         printf("MP3_getVolume=%d\r\n", MP3_getVolume());

         ret = MP3_playSong("0:/MUSIC/ˮ������.mp3");  //����ָ������
         printf("play over!!!!ret=%d\r\n", ret);
         mico_rtos_set_semaphore(&cupTimeObj.stopLed_sem);  //ֹ֪ͨͣ��LED��
       }
      // mp3_play();
    }
}


/* ��������߳� */
void BAT_handleThread(void *inContext)
{

    printf( "This is BAT_handleThread.\r\n");
    while(1)
    {
        mico_thread_msleep(5000); /* �͵���ʱ�������ϴ�ƽ̨ */
    }
}


/* user main function, called by AppFramework after system init done && wifi
 * station on in user_main thread.
 */
OSStatus user_main( app_context_t * const app_context )
{
  OSStatus err = kNoErr;
  int time_sencond = 50*1000;  /* 60s */
   

  require(app_context, exit);
  net_init(app_context);

  /* Create a new thread */

  err = mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "RGB_LED", LED_handleThread, 1024, NULL );
  require_noerr_string( err, exit, "ERROR: Unable to start the RGB LED thread ." );

  err = mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "MP3_PLAY", MP3_handleThread, 1024, NULL );
  require_noerr_string( err, exit, "ERROR: Unable to start the MP3 PLAY thread" ); 
  
  err = mico_rtos_create_thread(NULL, MICO_APPLICATION_PRIORITY, "BAT_DETECT", BAT_handleThread, 500, NULL );
  require_noerr_string( err, exit, "ERROR: Unable to start the BAT DETECT thread ." );
  
  KEY_Init(KEY_irq_handler); //������ʼ��
  
  mico_rtos_init_semaphore(&cupTimeObj.playMp3_sem, 1);  //�ź�����ʼ��
  mico_rtos_init_semaphore(&cupTimeObj.playLed_sem, 1);
  mico_rtos_init_semaphore(&cupTimeObj.stopLed_sem, 1);
  
  err = mico_init_timer(&cupTimeObj.cup_timer, time_sencond, cup_timer_timeout_handler, (void *)&cupTimeObj);
  cupTimeObj.drinkTime = 1;
  cupTimeObj.playMode = PLAY_MP3_LED;
  if (KEY_getValue() == KEY_DOWN)
  {
    TIMER_start(); //������ʱ��ˮ
  }
   
  while(1)
  {
   // printf("this is main thread.\r\n");
     //net_test(app_context);
      mico_thread_sleep(10);
  }
  
exit:
  if(kNoErr != err){
    printf("ERROR: user_main thread exit with err=%d", err);
  }
  mico_rtos_delete_thread(NULL);
  return kNoErr;  
}


