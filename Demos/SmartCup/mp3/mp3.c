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
{"BIN"},			//BIN�ļ�
{"LRC"},			//LRC�ļ�
{"NES"},			//NES�ļ�
{"TXT","C","H"},	//�ı��ļ�
{"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLAC"},//�����ļ�
{"BMP","JPG","JPEG","GIF"},//ͼƬ�ļ�
};

//��Сд��ĸתΪ��д��ĸ,���������,�򱣳ֲ���.
u8 char_upper(u8 c)
{
	if(c<'A')return c;//����,���ֲ���.
	if(c>='a')return c-0x20;//��Ϊ��д.
	else return c;//��д,���ֲ���
}	      
//�����ļ�������
//fname:�ļ���
//����ֵ:0XFF,��ʾ�޷�ʶ����ļ����ͱ��.
//		 ����,����λ��ʾ��������,����λ��ʾ����С��.
u8 f_typetell(u8 *fname)
{
	u8 tbuf[5];
	u8 *attr='\0';//��׺��
	u8 i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//ƫ�Ƶ��������.
		fname++;
	}
	if(i==250)return 0XFF;//������ַ���.
 	for(i=0;i<5;i++)//�õ���׺��
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
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//ȫ����Ϊ��д 
	for(i=0;i<6;i++)
	{
		for(j=0;j<13;j++)
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//�����Ѿ�û�пɶԱȵĳ�Ա��.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//�ҵ���
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;//û�ҵ�		 			   
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
   
   res = f_mount(&fs,"0:/",1); 	//����SD�� 
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
   
  	//д�ļ�����
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
     //���ļ�����
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
   
     VS_Init();				//��ʼ��VS1053	
    // KEY_Init();            //������ʼ��
     
    //ɨ���Ѿ����ڵ��ļ�
	//printf("\n\rbegin scan files path......\n\r");
    //scan_files(path);
    //SD_TotalSize(path);//��ȡSD���� 
    vsset.mvol=250;						//Ĭ����������Ϊ210. 
    return 1;
}


/*******************************************************************************
  * @��������	MP3_getSdSize
  * @����˵��   �ļ��ռ�ռ����� 
  * @�������   �� 
  * @�������   ��
  * @���ز���   1: �ɹ� 
  				0: ʧ��
  * @ע������	��
  *****************************************************************************/
u8 MP3_getSdSize(SD_sizeInfo *pSdSizeInfo)
{
    FATFS *fs;
    DWORD fre_clust;        
    
    res = f_getfree(path, &fre_clust, &fs);  /* �����Ǹ�Ŀ¼��ѡ�����0 */
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


//��������,��Χ:0~30
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

//��ȡ��ǰ����,��Χ:0~30
u8 MP3_getVolume(void) 	
{
   return (vsset.mvol-100)/5;
}


//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
u16 MP3_getMp3FileNum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO tfileinfo;	//��ʱ�ļ���Ϣ		
	u8 *fn; 			 			   			     
    res=f_opendir(&tdir,(const TCHAR*)path); 	//��Ŀ¼
  	tfileinfo.lfsize=_MAX_LFN*2+1;				//���ļ�����󳤶�
	tfileinfo.lfname=malloc(tfileinfo.lfsize);//Ϊ���ļ������������ڴ�
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	        res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);	
            printf("��������: %s\r\n", fn);                   //��ʾ��������
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	} 
	free(tfileinfo.lfname);
	return rval;
}


//����һ��ָ���ĸ���	
//pname:����·��+����
//����ֵ:0,�����������
//		 1,��һ��
//       2,��һ��
//       0XFF,���ִ�����
u8 MP3_playSong(const char *pName)
{	 
 	FIL* fmp3;
	u16 br;
	u8 res,rval;	  
	u8 *databuf;	   		   
	u16 i=0; 
	u8 key;  	    
	//static u8 pause=0;		//��ͣ��־ 
    static u8 lastVol = 0;  //��һ������
    //u8 j = 0;
	rval=0;	    
	fmp3=(FIL*)malloc(sizeof(FIL));	//�����ڴ�
	databuf=(u8*)malloc(READ_MP3_SIZE);		//����4096�ֽڵ��ڴ�����
	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//�ڴ�����ʧ��.
	if(rval==0)
	{
        VS_HD_Reset();
        VS_Soft_Reset();
        
        lastVol = vsset.mvol;
        mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30  
        
	  	VS_Restart_Play();  					//�������� 
		VS_Set_All();        					//������������Ϣ 			 
		VS_Reset_DecodeTime();					//��λ����ʱ�� 	  
		/*res=f_typetell(pname);	 	 			//�õ��ļ���׺	 			  	 						 
		if(res==0x4c)//�����flac,����patch
		{	
			VS_Load_Patch((u16*)vs1053b_patch,VS1053B_PATCHLEN);
		} */ 				 		   		 						  
		res=f_open(fmp3,(const TCHAR*)pName,FA_READ);//���ļ�
        printf("@res=%d, mp3Name=%s\r\n",res, pName);
		//printf("sram :%d",mem_perused(0));
		if(res==0)//�򿪳ɹ�.
		{ 
			VS_SPI_SpeedHigh();	//����						   
			while(rval==0)
			{
				res=f_read(fmp3,databuf,READ_MP3_SIZE,(UINT*)&br);//����4096���ֽ�  
				i=0;   
               // printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\r\n");
           
                if (lastVol != vsset.mvol)  //��������Ƿ���������
                {
                    VS_Set_Vol(vsset.mvol);
                    lastVol = vsset.mvol; 
                    //���浽�����ļ�
                }      
#if 1
				do//������ѭ��
			    {  	
					if((VS_Send_MusicData(databuf+i)==0))//��VS10XX������Ƶ����
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
								rval=KEY_UP;		//����
								break;
							default:
								break;
						}
						//mp3_msg_show(fmp3->fsize);//��ʾ��Ϣ	
                       
					}	    	    
				}while(i<READ_MP3_SIZE);//ѭ������4096���ֽ� 
#endif               
				if(br!=READ_MP3_SIZE||res!=0)
				{
					rval=KEY_UP;
					break;//������.	
				} 							 
			}
			f_close(fmp3);
		}else rval=0XFF;//���ִ���	   	  
	}	
    
    if (databuf != NULL)
      free(databuf);	
    
    if (fmp3 != NULL)
      free(fmp3);
    
	return rval;	  	 		  	    
}

/*******************************************************************************
��������MP3_writeSong
����  ����SD��д��ָ���ĸ�����
����  ��pname: ����·��+����,�磺"0:/MUSIC/С���� - ����ǣ�������.mp3"
        addr : �ļ�ƫ�Ƶ�ַ��
��������pBuf : Ҫд�����ݻ������׵�ַ��
��������size : Ҫд�����ݵĳ��ȡ�512�ֽڶ��룬������0
����ֵ��0:ʧ�ܣ�value��ʵ��д�����ݴ�С
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
     fmp3=(FIL*)malloc(sizeof(FIL));	//�����ڴ�
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
��������MP3_removeSong
����  ��ɾ��ָ���ĸ�����
����  ��pName:����·��+����,�磺"0:/MUSIC/С���� - ����ǣ�������.mp3"
����ֵ��0:ʧ�ܣ������ɹ�
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




//��ʾ��Ŀ����
//index:��ǰ����
//total:���ļ���
void mp3_index_show(u16 index,u16 total)
{
	//��ʾ��ǰ��Ŀ������,������Ŀ��
    printf("��ǰ��Ŀ:%d, ����Ŀ��:%d\r\n", index, total);
    
}
//��ʾ��ǰ����
void mp3_vol_show(u8 vol)
{			      	  
	printf("����:%d\r\n",vol); 	//��ʾ����	 
}





#if 0  //////////////////////////////////////////////////////////////////////////
//��������
void mp3_play(void)
{
	u8 res;
 	DIR mp3dir;	 		//Ŀ¼
	FILINFO mp3fileinfo;//�ļ���Ϣ
	u8 *fn;   			//���ļ���
	u8 *pname;			//��·�����ļ���
	u16 totmp3num; 		//�����ļ�����
	u16 curindex;		//��ǰ����
	u8 key;					//��ֵ		  
 	u16 temp;
	u16 *mp3indextbl;	//���������� 
    
 	while(f_opendir(&mp3dir,"0:/MUSIC"))//�������ļ���
 	{	    
        printf("MUSIC�ļ��д���!\r\n");
		mico_thread_msleep(200);				  
	} 									  
	totmp3num=mp3_get_tnum("0:/MUSIC"); //�õ�����Ч�ļ���
  	while(totmp3num==NULL)//�����ļ�����Ϊ0		
 	{	    
        printf("û�������ļ�!\r\n");
		mico_thread_msleep(200);				  
	}
    
  	mp3fileinfo.lfsize = _MAX_LFN*2+1;				//���ļ�����󳤶�
	mp3fileinfo.lfname = malloc(mp3fileinfo.lfsize);//Ϊ���ļ������������ڴ�
 	pname = malloc(mp3fileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
 	mp3indextbl = malloc(2*totmp3num);				//����2*totmp3num���ֽڵ��ڴ�,���ڴ�������ļ�����
 	while(mp3fileinfo.lfname==NULL||pname==NULL||mp3indextbl==NULL)//�ڴ�������
 	{	    
        printf("�ڴ����ʧ��!\r\n");
		mico_thread_msleep(200);			  
	}  	
    

	//��¼����
    res=f_opendir(&mp3dir,"0:/MUSIC"); //��Ŀ¼
	if(res==FR_OK)
	{
		curindex=0;//��ǰ����Ϊ0
		while(1)//ȫ����ѯһ��
		{
			temp=mp3dir.index;								//��¼��ǰindex
	        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     		fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
			{
				mp3indextbl[curindex]=temp;//��¼����
				curindex++;
			}	    
		} 
	}   
    
   	curindex=0;											//��0��ʼ��ʾ
   	res=f_opendir(&mp3dir,(const TCHAR*)"0:/MUSIC"); 	//��Ŀ¼
	while(res==FR_OK)//�򿪳ɹ�
	{	
		dir_sdi(&mp3dir,mp3indextbl[curindex]);			//�ı䵱ǰĿ¼����	   
        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
     	fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
		strcpy((char*)pname,"0:/MUSIC/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)fn);  	    //���ļ������ں���
        printf("��������: %s\r\n", fn);                   //��ʾ��������
		mp3_index_show(curindex+1,totmp3num);
		key=MP3_playSong(pname); 				 		//�������MP3    
        
         if(key==KEY0_PRES)//��һ��
		{
			curindex++;		   	
			if(curindex>=totmp3num)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
 		}else break;	//�����˴��� 	 
	} 											  
	free(mp3fileinfo.lfname);	//�ͷ��ڴ�			    
	free(pname);				//�ͷ��ڴ�			    
	free(mp3indextbl);			//�ͷ��ڴ�	 
}



u16 f_kbps=0;//�����ļ�λ��	
//��ʾ����ʱ��,������ ��Ϣ 
//lenth:�����ܳ���
void mp3_msg_show(u32 lenth)
{	
	static u16 playtime=0;//����ʱ����	     
 	u16 time=0;// ʱ�����
	u16 temp=0;	  
	if(f_kbps==0xffff)//δ���¹�
	{
		playtime=0;
		f_kbps=VS_Get_HeadInfo();//��ñ�����
	}	 	 
	time=VS_Get_DecodeTime(); //�õ�����ʱ��

	if(playtime==0)playtime=time;
	else if((time!=playtime)&&(time!=0))//1sʱ�䵽,������ʾ����
	{
 		playtime=time;//����ʱ�� 	 				    
 		temp=VS_Get_HeadInfo(); //��ñ�����	   				 
		if(temp!=f_kbps)
		{
			f_kbps=temp;//����KBPS	  				     
		}			  
		//��ʾ����ʱ��			 	
        //printf("��ǰ����ʱ��: %d:%d\r\n",time/60, time%60);//����:����
		//��ʾ��ʱ��
		if(f_kbps)time=(lenth/f_kbps)/125;//�õ�������   (�ļ�����(�ֽ�)/(1000/8)/������=����������    	  
		else time=0;//�Ƿ�λ��	  
        //printf("�����ܲ���ʱ��: %d:%d\r\n",time/60, time%60);//����:����
		//��ʾλ��			   	
        //printf("����λ��: %d\r\n",f_kbps);
         
 		//LED1=!LED1;		//DS0��ת
	}   		 
}	
#endif   //////////////////////////////////////////////////////////////////////


#if 0 ////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
  * @��������	scan_files
  * @����˵��   �����ļ�Ŀ¼�������ļ� 
  * @�������   path: ��Ŀ¼ 
  * @�������   ��
  * @���ز���   FRESULT
  * @ע������	��
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

























