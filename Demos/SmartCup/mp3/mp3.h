#ifndef __MP3_H
#define __MP3_H

#include "syscom.h"

//SD卡容量信息
typedef struct 
{							  
	u16 totalSize;		//总容量，单位M
	u16 availableSize;	//剩余可用容量，单位M
}SD_sizeInfo;

/*******************************************************************************
函数名：
描述  ：
参数  ：
返回值：
*******************************************************************************/

/*******************************************************************************
函数名：MP3_sdCardDetect
描述  ：检测是否插上SD卡，如果没有就不支持MP3相关功能操作。
参数  ：无
返回值：0:无，１：有
*******************************************************************************/
u8 MP3_sdCardDetect(void);

/*******************************************************************************
函数名：MP3_init
描述  ：初始化MP3所需要的资源，如文件系统挂载、VS1003初始化和音量等信息设置。
参数  ：无
返回值：0:失败，1：成功
*******************************************************************************/
u8 MP3_init(void);

/*******************************************************************************
函数名：MP3_getSdSize
描述  ：获取SD卡容量信息。
参数  ：pSdSizeInfo,结构体SD_sizeInfo指针
返回值：0:失败，１：成功
*******************************************************************************/
u8 MP3_getSdSize(SD_sizeInfo *pSdSizeInfo);

/*******************************************************************************
函数名：MP3_setVolume
描述  ：设置音量
参数  ：vol:音量范围 0~30
返回值：0:失败，１：成功
*******************************************************************************/
u8 MP3_setVolume(u8 vol); 

/*******************************************************************************
函数名：MP3_getVolume
描述  ：获取当前音量
参数  ：无
返回值：0:失败，vol:返回音量范围 0~30
*******************************************************************************/
u8 MP3_getVolume(void);

/*******************************************************************************
函数名：MP3_getMp3FileNum
描述  ：获取指定路径MP3的歌曲总数目。
参数  ：path:指定路径,如："0:/MUSIC" 表目录MUSIC下面MP3文件总数
返回值：0: 失败，value：总有效文件数
*******************************************************************************/
u16 MP3_getMp3FileNum(u8 *path);


/*******************************************************************************
函数名：MP3_getMp3FileName
描述  ：获取指定路径和索引MP3的歌曲名称。
参数  ：path:指定路径,如："0:/MUSIC", index: 指定索引值，其最大值为该目录下mp3文件总数
       ，pMp3FileName:保存mp3名称空间首地址。
返回值：0:失败，1：成功
*******************************************************************************/
u8 MP3_getMp3FileName(u8 *path, u8 index, u8 *pMp3FileName);


/*******************************************************************************
函数名：MP3_playSong
描述  ：播放一曲指定的歌曲。
参数  ：pname:歌曲路径+名字,如："0:/MUSIC/小虎队 - 让我牵着你的手.mp3"
返回值：0:正常播放完毕，１：用户提杯操作，则中断音乐
*******************************************************************************/
u8 MP3_playSong(const char *pName);

/*******************************************************************************
函数名：MP3_writeSong
描述  ：往SD卡写入指定的歌曲。
参数  ：pname: 歌曲路径+名字,如："0:/MUSIC/小虎队 - 让我牵着你的手.mp3"
        addr : 文件偏移地址。
　　　　pBuf : 要写入数据缓冲区首地址。
　　　　size : 要写入数据的长度。512字节对齐，不够补0
返回值：0:失败，value：实际写入数据大小
*******************************************************************************/
u32 MP3_writeSong(const char *pName, u32 addr, u8 *pBuf, u32 size);

/*******************************************************************************
函数名：MP3_removeSong
描述  ：删除指定的歌曲。
参数  ：pname:歌曲路径+名字,如："0:/MUSIC/小虎队 - 让我牵着你的手.mp3"
返回值：0:失败，１：成功
*******************************************************************************/
u8 MP3_removeSong(const char *pName);

#endif












