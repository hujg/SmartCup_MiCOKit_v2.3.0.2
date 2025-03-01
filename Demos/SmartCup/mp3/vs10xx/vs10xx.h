#ifndef __VS10XX_H__
#define __VS10XX_H__

#include "MICO.h"
#include "platform.h"
#include "mico_platform.h"

#define u8     uint8_t
#define u16    uint16_t
#define u32    uint32_t

//////////////////////////////////////////////////////////////
//与外部的接口

#define   VS_SO_PIN	       (MICO_GPIO_27)	//串行数据输出，单片机从VS1003中读取数据
#define   VS_SI_PIN	       (MICO_GPIO_29)	//串行数据输入，由单片机写入到VS1003
#define   VS_SCK_PIN       (MICO_GPIO_37)	//串行时钟，由单片机产生

#define   VS_DQ_PIN        (MICO_GPIO_2)                  //pin2
#define   VS_RST_PIN       (MICO_GPIO_16)                //pin16
#define   VS_XCS_PIN       (MICO_GPIO_17)                 //pin17
#define   VS_XDCS_PIN      (MICO_GPIO_30)                //pin30



#define    VS_SO             MicoGpioInputGet(VS_SO_PIN) 
#define    VS_SI_Clr()       MicoGpioOutputLow(VS_SI_PIN) 
#define    VS_SI_Set()       MicoGpioOutputHigh(VS_SI_PIN)
#define    VS_SCK_Clr()      MicoGpioOutputLow(VS_SCK_PIN) 
#define    VS_SCK_Set()      MicoGpioOutputHigh(VS_SCK_PIN)


#define    VS_DQ                  MicoGpioInputGet(VS_DQ_PIN)     

#define    VS_RST_Clr()       MicoGpioOutputLow(VS_RST_PIN) 
#define    VS_RST_Set()      MicoGpioOutputHigh(VS_RST_PIN)

#define    VS_XCS_Clr()        MicoGpioOutputLow(VS_XCS_PIN) 
#define    VS_XCS_Set()       MicoGpioOutputHigh(VS_XCS_PIN) 

#define    VS_XDCS_Clr()       MicoGpioOutputLow(VS_XDCS_PIN)  
#define    VS_XDCS_Set()      MicoGpioOutputHigh(VS_XDCS_PIN)

//////////////////////////////////////////////////////////////

__packed typedef struct 
{							  
	u8 mvol;		//主音量,范围:0~254
	u8 bflimit;		//低音频率限定,范围:2~15(单位:10Hz)
	u8 bass;		//低音,范围:0~15.0表示关闭.(单位:1dB)
	u8 tflimit;		//高音频率限定,范围:1~15(单位:Khz)
	u8 treble;		//高音,范围:0~15(单位:1.5dB)(原本范围是:-8~7,通过函数修改了);
	u8 effect;		//空间效果设置.0,关闭;1,最小;2,中等;3,最大.

	u8 saveflag; 	//保存标志,0X0A,保存过了;其他,还从未保存	   
}_vs10xx_obj;


extern _vs10xx_obj vsset;		//VS10XX设置

#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03
//VS10XX寄存器定义
#define SPI_MODE        	0x00   
#define SPI_STATUS      	0x01   
#define SPI_BASS        	0x02   
#define SPI_CLOCKF      	0x03   
#define SPI_DECODE_TIME 	0x04   
#define SPI_AUDATA      	0x05   
#define SPI_WRAM        	0x06   
#define SPI_WRAMADDR    	0x07   
#define SPI_HDAT0       	0x08   
#define SPI_HDAT1       	0x09 
  
#define SPI_AIADDR      	0x0a   
#define SPI_VOL         	0x0b   
#define SPI_AICTRL0     	0x0c   
#define SPI_AICTRL1     	0x0d   
#define SPI_AICTRL2     	0x0e   
#define SPI_AICTRL3     	0x0f   
#define SM_DIFF         	0x01   
#define SM_JUMP         	0x02   
#define SM_RESET        	0x04   
#define SM_OUTOFWAV     	0x08   
#define SM_PDOWN        	0x10   
#define SM_TESTS        	0x20   
#define SM_STREAM       	0x40   
#define SM_PLUSV        	0x80   
#define SM_DACT         	0x100   
#define SM_SDIORD       	0x200   
#define SM_SDISHARE     	0x400   
#define SM_SDINEW       	0x800   
#define SM_ADPCM        	0x1000   
#define SM_ADPCM_HP     	0x2000 		 



u16  VS_RD_Reg(u8 address);				//读寄存器
u16  VS_WRAM_Read(u16 addr);	    	//读RAM
void VS_WR_Data(u8 data);				//写数据
void VS_WR_Cmd(u8 address,u16 data);	//写命令
u8   VS_HD_Reset(void);			    	//硬复位
void VS_Soft_Reset(void);           	//软复位
u16 VS_Ram_Test(void);             		//RAM测试	    
void VS_Sine_Test(void);            	//正弦测试
													 
u8 	 VS_SPI_ReadWriteByte(u8 data);
void VS_SPI_SpeedLow(void);
void VS_SPI_SpeedHigh(void);
void VS_Init(void);						//初始化VS10XX	 
void VS_Set_Speed(u8 t);				//设置播放速度
u16  VS_Get_HeadInfo(void);     		//得到比特率
u32 VS_Get_ByteRate(void);				//得到字节速率
u16 VS_Get_EndFillByte(void);			//得到填充字节
u8 	 VS_Send_MusicData(u8* buf);		//向VS10XX发送32字节
void VS_Restart_Play(void);				//重新开始下一首歌播放	  
void VS_Reset_DecodeTime(void);			//重设解码时间
u16  VS_Get_DecodeTime(void);   		//得到解码时间

void VS_Load_Patch(u16 *patch,u16 len);	//加载用户patch
u8 	 VS_Get_Spec(u16 *p);       		//得到分析数据	   
void VS_Set_Bands(u16 *buf,u8 bands);	//设置中心频率
void VS_Set_Vol(u8 volx);				//设置主音量   
void VS_Set_Bass(u8 bfreq,u8 bass,u8 tfreq,u8 treble);//设置高低音
void VS_Set_Effect(u8 eft);				//设置音效
void VS_Set_All(void);

void vs10xx_read_para(_vs10xx_obj * vs10xxdev);
void vs10xx_save_para(_vs10xx_obj * vs10xxdev);

#endif

















