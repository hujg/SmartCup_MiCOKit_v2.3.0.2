#include "mp3.h"
#include "syscom.h"	 

#define READ_MP3_SIZE  1*1024


void mp3_play(void);
u16 mp3_get_tnum(u8 *path);
void mp3_index_show(u16 index,u16 total);
void mp3_msg_show(u32 lenth);
void mp3_vol_show(u8 vol);


char path[256]="0:";
FRESULT res;
FATFS fs; 
FIL fd;      /* file objects */

u8 const textFileBuffer[] = "I love you";


const u8 *FILE_TYPE_TBL[6][13]=
{
{"BIN"},			//BIN文件
{"LRC"},			//LRC文件
{"NES"},			//NES文件
{"TXT","C","H"},	//文本文件
{"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLAC"},//音乐文件
{"BMP","JPG","JPEG","GIF"},//图片文件
};

//将小写字母转为大写字母,如果是数字,则保持不变.
u8 char_upper(u8 c)
{
	if(c<'A')return c;//数字,保持不变.
	if(c>='a')return c-0x20;//变为大写.
	else return c;//大写,保持不变
}	      
//报告文件的类型
//fname:文件名
//返回值:0XFF,表示无法识别的文件类型编号.
//		 其他,高四位表示所属大类,低四位表示所属小类.
u8 f_typetell(u8 *fname)
{
	u8 tbuf[5];
	u8 *attr='\0';//后缀名
	u8 i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//偏移到了最后了.
		fname++;
	}
	if(i==250)return 0XFF;//错误的字符串.
 	for(i=0;i<5;i++)//得到后缀名
	{
		fname--;
		if(*fname=='.')
		{
			fname++;
			attr=fname;
			break;
		}
  	}
	strcpy((char *)tbuf,(const char*)attr);//copy
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//全部变为大写 
	for(i=0;i<6;i++)
	{
		for(j=0;j<13;j++)
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//此组已经没有可对比的成员了.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//找到了
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;//没找到		 			   
}	 


/*********************************************************************************************************/
u8 MP3_sdCardDetect(void)
{
    
    return 1;
}

u8 MP3_init(void)
{
    //u32 bw, br;
   
  // FRESULT res;
   
   res = f_mount(&fs,"0:/",1); 	//挂载SD卡 
   if (res != FR_OK)
   {
        printf("mount failed!!!\r\n");
        return 0;
   }
   else
   {
        printf("mount success!!!\r\n");
   }
   
#if 0
   
  	//写文件测试
	printf("write file test......\n\r");
    res = f_open(&fd, "0:/longfil.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(res != FR_OK){
		printf("open file error : %d\n\r",res);
	}else{
	    res = f_write(&fd, textFileBuffer, sizeof(textFileBuffer), &bw);               /* Write it to the dst file */
		if(res == FR_OK){
			printf("write data ok! %d\n\r",bw);
		}else{
			printf("write data error : %d\n\r",res);
		}
		f_close(&fd);
	}
     //读文件测试
	printf("read file test......\n\r");
    res = f_open(&fd, "0:/test.txt", FA_OPEN_EXISTING | FA_READ);
    if(res != FR_OK){
		printf("open file error : %d\n\r",res);
	}else{
	    res = f_read(&fd, buffer, sizeof(buffer), &br);     /* Read a chunk of src file */
		if(res==FR_OK){
			printf("read data num : %d\n\r",br);
			printf("%s\n\r",buffer);
		}else{
			printf("read file error : %d\n\r",res);
		}
		/*close file */
		f_close(&fd);
	}
#endif
   
     VS_Init();				//初始化VS1053	
    // KEY_Init();            //按键初始化
     
    //扫描已经存在的文件
	//printf("\n\rbegin scan files path......\n\r");
    //scan_files(path);
    //SD_TotalSize(path);//获取SD容量 
    vsset.mvol=250;						//默认设置音量为210. 
    return 1;
}


/*******************************************************************************
  * @函数名称	MP3_getSdSize
  * @函数说明   文件空间占用情况 
  * @输入参数   无 
  * @输出参数   无
  * @返回参数   1: 成功 
  				0: 失败
  * @注意事项	无
  *****************************************************************************/
u8 MP3_getSdSize(SD_sizeInfo *pSdSizeInfo)
{
    FATFS *fs;
    DWORD fre_clust;        
    
    res = f_getfree(path, &fre_clust, &fs);  /* 必须是根目录，选择磁盘0 */
    if ( res==FR_OK ) 
    {
	  //printf("\n\rget %s drive space.\n\r",path);
	  /* Print free space in unit of MB (assuming 512 bytes/sector) */
      //printf("%d MB total drive space.\r\n"
      //     "%d MB available.\r\n",
      //     ( (fs->n_fatent - 2) * fs->csize ) / 2 /1024 , (fre_clust * fs->csize) / 2 /1024 );
        
	  pSdSizeInfo->totalSize = ((fs->n_fatent - 2) * fs->csize ) / 2 /1024;
      pSdSizeInfo->availableSize = (fre_clust * fs->csize) / 2 /1024;
      
	  return 1;
	}
	else
	{ 
	  printf("\n\rGet total drive space faild!\n\r");
	  return 0;   
	}
}


//设置音量,范围:0~30
u8 MP3_setVolume(u8 vol) 	
{
    if (vol > 30)
   {
      return 0;
   }
   vsset.mvol= (vol * 5) + 100;	
   //printf("vsset.mvol=%d\r\n", vsset.mvol);
   //VS_Soft_Reset();
   //VS_Set_Vol(vsset.mvol);
   
   return 1;
}

//获取当前音量,范围:0~30
u8 MP3_getVolume(void) 	
{
   return (vsset.mvol-100)/5;
}


//得到path路径下,目标文件的总个数
//path:路径		    
//返回值:总有效文件数
u16 MP3_getMp3FileNum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO tfileinfo;	//临时文件信息		
	u8 *fn; 			 			   			     
    res=f_opendir(&tdir,(const TCHAR*)path); 	//打开目录
  	tfileinfo.lfsize=_MAX_LFN*2+1;				//长文件名最大长度
	tfileinfo.lfname=malloc(tfileinfo.lfsize);//为长文件缓存区分配内存
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//查询总的有效文件数
		{
	        res=f_readdir(&tdir,&tfileinfo);       		//读取目录下的一个文件
	        if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出		  
     		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);	
            printf("歌曲名字: %s\r\n", fn);                   //显示歌曲名字
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//取高四位,看看是不是音乐文件	
			{
				rval++;//有效文件数增加1
			}	    
		}  
	} 
	free(tfileinfo.lfname);
	return rval;
}


//播放一曲指定的歌曲	
//pname:歌曲路径+名字
//返回值:0,正常播放完成
//		 1,下一曲
//       2,上一曲
//       0XFF,出现错误了
u8 MP3_playSong(const char *pName)
{	 
 	FIL* fmp3;
	u16 br;
	u8 res,rval;	  
	u8 *databuf;	   		   
	u16 i=0; 
	u8 key;  	    
	//static u8 pause=0;		//暂停标志 
    static u8 lastVol = 0;  //上一次音量
    //u8 j = 0;
	rval=0;	    
	fmp3=(FIL*)malloc(sizeof(FIL));	//申请内存
	databuf=(u8*)malloc(READ_MP3_SIZE);		//开辟4096字节的内存区域
	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//内存申请失败.
	if(rval==0)
	{
        VS_HD_Reset();
        VS_Soft_Reset();
        
        lastVol = vsset.mvol;
        mp3_vol_show((vsset.mvol-100)/5);	//音量限制在:100~250,显示的时候,按照公式(vol-100)/5,显示,也就是0~30  
        
	  	VS_Restart_Play();  					//重启播放 
		VS_Set_All();        					//设置音量等信息 			 
		VS_Reset_DecodeTime();					//复位解码时间 	  
		/*res=f_typetell(pname);	 	 			//得到文件后缀	 			  	 						 
		if(res==0x4c)//如果是flac,加载patch
		{	
			VS_Load_Patch((u16*)vs1053b_patch,VS1053B_PATCHLEN);
		} */ 				 		   		 						  
		res=f_open(fmp3,(const TCHAR*)pName,FA_READ);//打开文件
        printf("@res=%d, mp3Name=%s\r\n",res, pName);
		//printf("sram :%d",mem_perused(0));
		if(res==0)//打开成功.
		{ 
			VS_SPI_SpeedHigh();	//高速						   
			while(rval==0)
			{
				res=f_read(fmp3,databuf,READ_MP3_SIZE,(UINT*)&br);//读出4096个字节  
				i=0;   
               // printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n");
           
                if (lastVol != vsset.mvol)  //检测音量是否重新配置
                {
                    VS_Set_Vol(vsset.mvol);
                    lastVol = vsset.mvol; 
                    //保存到配置文件
                }      
#if 1
				do//主播放循环
			    {  	
					if((VS_Send_MusicData(databuf+i)==0))//给VS10XX发送音频数据
					{
						i+=32;
					}
                    else   
					{
						key=KEY_getValue(); 
						switch(key)
						{
							case KEY_UP:
                                printf("+++++++++++++++++++++++++++key up\r\n");
								rval=KEY_UP;		//跳出
								break;
							default:
								break;
						}
						//mp3_msg_show(fmp3->fsize);//显示信息	
                       
					}	    	    
				}while(i<READ_MP3_SIZE);//循环发送4096个字节 
#endif               
				if(br!=READ_MP3_SIZE||res!=0)
				{
					rval=KEY_UP;
					break;//读完了.	
				} 							 
			}
			f_close(fmp3);
		}else rval=0XFF;//出现错误	   	  
	}	
    
    if (databuf != NULL)
      free(databuf);	
    
    if (fmp3 != NULL)
      free(fmp3);
    
	return rval;	  	 		  	    
}

/*******************************************************************************
函数名：MP3_writeSong
描述  ：往SD卡写入指定的歌曲。
参数  ：pname: 歌曲路径+名字,如："0:/MUSIC/小虎队 - 让我牵着你的手.mp3"
        addr : 文件偏移地址。
　　　　pBuf : 要写入数据缓冲区首地址。
　　　　size : 要写入数据的长度。512字节对齐，不够补0
返回值：0:失败，value：实际写入数据大小
*******************************************************************************/
u32 MP3_writeSong(const char *pName, u32 addr, u8 *pBuf, u32 size)
{
     u32 bw;
     FRESULT res;
     FIL* fmp3 = NULL;
     
     if (pName == NULL || pBuf == NULL || (size % 512) != 0)
     {
         printf("param error!\n\r");
        return 0;
     }
     fmp3=(FIL*)malloc(sizeof(FIL));	//申请内存
      if (fmp3 == NULL)
      {
          printf("malloc error!\n\r");
          return 0;
      }
     res = f_open(fmp3, pName, FA_CREATE_ALWAYS | FA_WRITE);
	if(res != FR_OK)
    {
		printf("open file error : %d\n\r",res);
        if (fmp3 != NULL)
        {
          free(fmp3);
        }
        return 0;
	}
    else
    {
        res = f_lseek(fmp3, addr);
        if(res != FR_OK)
        {
			printf("f_lseek error : %d\n\r",res);
            goto failed;
		}
        
	    res = f_write(fmp3, pBuf, size, &bw);               /* Write it to the dst file */
		if(res != FR_OK)
        {
			printf("write data error : %d\n\r",res);
            goto failed;
		}
        printf("write data ok! %d\n\r",bw);
        
        f_sync(fmp3);
        f_close(fmp3);
        if (fmp3 != NULL)
        {
          free(fmp3);
        }
		return bw;
	}

failed:
      f_close(fmp3);
      if (fmp3 != NULL)
      {
        free(fmp3);
      }
      return 0;
}

/*******************************************************************************
函数名：MP3_removeSong
描述  ：删除指定的歌曲。
参数  ：pName:歌曲路径+名字,如："0:/MUSIC/小虎队 - 让我牵着你的手.mp3"
返回值：0:失败，１：成功
*******************************************************************************/
u8 MP3_removeSong(const char *pName)
{
    FRESULT ret;
    
    if (pName == NULL)
    {
         printf("param error!\n\r");
        return 0;
    }
    
    ret =  f_unlink (pName);	
    if(ret != FR_OK)
    {
      printf("remove file failed !, %d\n\r",res);
      return 0;
    }
    return 1;
}
/*********************************************************************************************************/




//显示曲目索引
//index:当前索引
//total:总文件数
void mp3_index_show(u16 index,u16 total)
{
	//显示当前曲目的索引,及总曲目数
    printf("当前曲目:%d, 总曲目数:%d\r\n", index, total);
    
}
//显示当前音量
void mp3_vol_show(u8 vol)
{			      	  
	printf("音量:%d\r\n",vol); 	//显示音量	 
}





#if 0  //////////////////////////////////////////////////////////////////////////
//播放音乐
void mp3_play(void)
{
	u8 res;
 	DIR mp3dir;	 		//目录
	FILINFO mp3fileinfo;//文件信息
	u8 *fn;   			//长文件名
	u8 *pname;			//带路径的文件名
	u16 totmp3num; 		//音乐文件总数
	u16 curindex;		//当前索引
	u8 key;					//键值		  
 	u16 temp;
	u16 *mp3indextbl;	//音乐索引表 
    
 	while(f_opendir(&mp3dir,"0:/MUSIC"))//打开音乐文件夹
 	{	    
        printf("MUSIC文件夹错误!\r\n");
		mico_thread_msleep(200);				  
	} 									  
	totmp3num=mp3_get_tnum("0:/MUSIC"); //得到总有效文件数
  	while(totmp3num==NULL)//音乐文件总数为0		
 	{	    
        printf("没有音乐文件!\r\n");
		mico_thread_msleep(200);				  
	}
    
  	mp3fileinfo.lfsize = _MAX_LFN*2+1;				//长文件名最大长度
	mp3fileinfo.lfname = malloc(mp3fileinfo.lfsize);//为长文件缓存区分配内存
 	pname = malloc(mp3fileinfo.lfsize);				//为带路径的文件名分配内存
 	mp3indextbl = malloc(2*totmp3num);				//申请2*totmp3num个字节的内存,用于存放音乐文件索引
 	while(mp3fileinfo.lfname==NULL||pname==NULL||mp3indextbl==NULL)//内存分配出错
 	{	    
        printf("内存分配失败!\r\n");
		mico_thread_msleep(200);			  
	}  	
    

	//记录索引
    res=f_opendir(&mp3dir,"0:/MUSIC"); //打开目录
	if(res==FR_OK)
	{
		curindex=0;//当前索引为0
		while(1)//全部查询一遍
		{
			temp=mp3dir.index;								//记录当前index
	        res=f_readdir(&mp3dir,&mp3fileinfo);       		//读取目录下的一个文件
	        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//错误了/到末尾了,退出		  
     		fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//取高四位,看看是不是音乐文件	
			{
				mp3indextbl[curindex]=temp;//记录索引
				curindex++;
			}	    
		} 
	}   
    
   	curindex=0;											//从0开始显示
   	res=f_opendir(&mp3dir,(const TCHAR*)"0:/MUSIC"); 	//打开目录
	while(res==FR_OK)//打开成功
	{	
		dir_sdi(&mp3dir,mp3indextbl[curindex]);			//改变当前目录索引	   
        res=f_readdir(&mp3dir,&mp3fileinfo);       		//读取目录下的一个文件
        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//错误了/到末尾了,退出
     	fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
		strcpy((char*)pname,"0:/MUSIC/");				//复制路径(目录)
		strcat((char*)pname,(const char*)fn);  	    //将文件名接在后面
        printf("歌曲名字: %s\r\n", fn);                   //显示歌曲名字
		mp3_index_show(curindex+1,totmp3num);
		key=MP3_playSong(pname); 				 		//播放这个MP3    
        
         if(key==KEY0_PRES)//下一曲
		{
			curindex++;		   	
			if(curindex>=totmp3num)curindex=0;//到末尾的时候,自动从头开始
 		}else break;	//产生了错误 	 
	} 											  
	free(mp3fileinfo.lfname);	//释放内存			    
	free(pname);				//释放内存			    
	free(mp3indextbl);			//释放内存	 
}



u16 f_kbps=0;//歌曲文件位率	
//显示播放时间,比特率 信息 
//lenth:歌曲总长度
void mp3_msg_show(u32 lenth)
{	
	static u16 playtime=0;//播放时间标记	     
 	u16 time=0;// 时间变量
	u16 temp=0;	  
	if(f_kbps==0xffff)//未更新过
	{
		playtime=0;
		f_kbps=VS_Get_HeadInfo();//获得比特率
	}	 	 
	time=VS_Get_DecodeTime(); //得到解码时间

	if(playtime==0)playtime=time;
	else if((time!=playtime)&&(time!=0))//1s时间到,更新显示数据
	{
 		playtime=time;//更新时间 	 				    
 		temp=VS_Get_HeadInfo(); //获得比特率	   				 
		if(temp!=f_kbps)
		{
			f_kbps=temp;//更新KBPS	  				     
		}			  
		//显示播放时间			 	
        //printf("当前播放时间: %d:%d\r\n",time/60, time%60);//分钟:秒钟
		//显示总时间
		if(f_kbps)time=(lenth/f_kbps)/125;//得到秒钟数   (文件长度(字节)/(1000/8)/比特率=持续秒钟数    	  
		else time=0;//非法位率	  
        //printf("歌曲总播放时间: %d:%d\r\n",time/60, time%60);//分钟:秒钟
		//显示位率			   	
        //printf("歌曲位率: %d\r\n",f_kbps);
         
 		//LED1=!LED1;		//DS0翻转
	}   		 
}	
#endif   //////////////////////////////////////////////////////////////////////


#if 0 ////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
  * @函数名称	scan_files
  * @函数说明   搜索文件目录下所有文件 
  * @输入参数   path: 根目录 
  * @输出参数   无
  * @返回参数   FRESULT
  * @注意事项	无
  *****************************************************************************/
FRESULT scan_files (char* path )    /* Start node to be scanned (also used as work area) */
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                sprintf(&path[i], "/%s", fn);
				printf("scan file - %s\n\r",path);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("scan file - %s/%s\n\r", path, fn);
            }
        }
    }else{
		printf("scan files error : %d\n\r",res);
	}

    return res;
}
#endif /////////////////////////////////////////////////////////////////////////

























