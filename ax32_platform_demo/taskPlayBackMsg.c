#include "application.h"
#include "taskPlayBackRes.c"
#include "../device/printer/printer.h"

#define  VIDEO_PLAY_WAIT_KEYSOUND_END           while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}
//static u32 keep_print_wait_playbak;

extern int video_playback_start(int index);

static int playBackKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	INT32U temp_ad;
	//int ret;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		//filelist_build();
		if(managerFileCount(SysCtrl.avi_list)>=1)
		{
			
			if(FILE_TYPE_JPG == SysCtrl.file_type)	// scale up
			{
				//==set printer==
				if(palyback_print_en)
				{
					palyback_print_en = 0;
				}
				else
				{
					palyback_print_en = 1;
				}
				playbackPrinterShow(handle);
				deg_Printf("playBackKeyMsgOk:%d\n",palyback_print_en);	
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;	
				//boardIoctrl(SysCtrl.bfd_battery,IOCTRL_BAT_CHECK,(INT32U)&temp_ad);
				deg("bat=%d level[%d]  tempt[%d]\n",hal_adcGetChannel(ADC_CH_PE0),SysCtrl.battery,hal_adcGetChannel(ADC_CH_PA6));//PE0:bat ad

				/*playbackPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIP_KEEPQ);
				drawUIService(true);
				XOSTimeDly(2000);
				playbackPrinterTipsShow(handle,false,NULL,NULL);*/
			}
		}
	}
	return 0;
}

static int playBackKeyMsgPlayVideo(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		if(managerFileCount(SysCtrl.avi_list)>=1)
		{
			if(FILE_TYPE_JPG == SysCtrl.file_type)
			{
			
			}
			else
			{
				if(videoPlaybackGetStatus()== MEDIA_STAT_START)
				{
					videoPlaybackPuase();
					while(videoPlaybackGetStatus()==MEDIA_STAT_START);
					playbackStateShow(handle,8);
				}
				else if(videoPlaybackGetStatus() == MEDIA_STAT_PAUSE)
				{
					videoPlaybackResume();
					playbackStateShow(handle,2);
				}
			}
		}
	}
	return 0;
}


static int playBackKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(videoPlaybackGetStatus()== MEDIA_STAT_START)
		{
			videoPlaybackPuase();
			while(videoPlaybackGetStatus()==MEDIA_STAT_START);
			playbackStateShow(handle,8);
		}
		else
		{
		    ax32xx_dacVolumeSet((10*SysCtrl.volume_level*0x7fff)/100);
			XOSTimeDly(50);
			taskStart(TASK_MAIN_MENU,0);
			if(0==SysCtrl.cartoon_mode)
			{
				SysCtrl.cartoon_mode=2;
				SysCtrl.cartoon_show_cnt=0;
			}
			deamon_keysound_play();
		    VIDEO_PLAY_WAIT_KEYSOUND_END;
		}
	}
	return 0;
}

static int playBackKeyMsgSetting(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//deamon_keysound_play();

		taskStart(TASK_SETTING_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}

static int playBackKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		palyback_print_en=0;
		playbackPrinterShow(handle);
		
		if(managerFileCount(SysCtrl.avi_list)>=1)
		{
			if(MEDIA_STAT_PLAY==videoPlaybackGetStatus())
			{
				//videoPlaybackFastForward();
				//playbackStateShow(handle,videoPlaybackGetplayspeed());
			}
			else
			{
				/*
				if(videoPlaybackGetStatus()!= MEDIA_STAT_STOP)
				{
	                  videoPlaybackStop();
				}
				*/
				ax32xx_dacVolumeSet((10*SysCtrl.volume_level*0x7fff)/100);
				XOSTimeDly(50);
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;
				SysCtrl.file_index--;
				if(SysCtrl.file_index<1)
					SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);
				SysCtrl.jpeg_scale_flag = 0;
				video_playback_start(SysCtrl.file_index);
				playbackResolutionShow(handle);
				playbackFileNameShow(handle);
				playbackLockShow(handle);
				if(FILE_TYPE_JPG==SysCtrl.file_type)
				{
					playbackStateShow(handle,2);
				}
				else
				{
					if(videoPlaybackGetStatus() == MEDIA_STAT_PAUSE)
					{
						playbackStateShow(handle,8);
					}
				}
			}
		}
	}
	return 0;
}
static int playBackKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		palyback_print_en=0;
		playbackPrinterShow(handle);
	
		if(managerFileCount(SysCtrl.avi_list)>=1)
		{
			if(MEDIA_STAT_PLAY==videoPlaybackGetStatus())
			{
				//videoPlaybackFastBackward();
				//playbackStateShow(handle,videoPlaybackGetplayspeed());
			}
			else
			{
				/*
				if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
				{
	                  videoPlaybackStop();
				}
				*/
				ax32xx_dacVolumeSet((10*SysCtrl.volume_level*0x7fff)/100);
				XOSTimeDly(50);
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;
				SysCtrl.file_index++;
				if(SysCtrl.file_index>managerFileCount(SysCtrl.avi_list))
					SysCtrl.file_index = 1;
				SysCtrl.jpeg_scale_flag = 0;
				video_playback_start(SysCtrl.file_index);
				deg(" ---- in playback :check index[%d]----\n",SysCtrl.file_index);
				playbackResolutionShow(handle);
				playbackFileNameShow(handle);
				playbackLockShow(handle);
				if(FILE_TYPE_JPG==SysCtrl.file_type)
				{
					playbackStateShow(handle,2);
				}
				else
				{
					if(videoPlaybackGetStatus() == MEDIA_STAT_PAUSE)
					{
						playbackStateShow(handle,8);
					}
				}

			}
		}
	}
	return 0;
}
extern menu MENU(playBack);
static int playBackKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if((videoPlaybackGetStatus() != MEDIA_STAT_START)&&(managerFileCount(SysCtrl.avi_list)>0))
		{
			videoPlaybackStop();
			deamon_keysound_play();
			VIDEO_PLAY_WAIT_KEYSOUND_END;
			uiOpenWindow(&delCurWindow,0);
            //uiOpenWindow(&menuItemWindow,1,&MENU(playBack));						
			//layout_video_playback_name(manangerGetFileFullPathName(SysCtrl.avi_list,SysCtrl.file_index,NULL));
		}
	}
	return 0;
}

static int playBackKeyMsgPrinterEn(winHandle handle,uint32 parameNum,uint32* parame)
{

	uint32 keyState=KEY_STATE_INVALID;
	//int ret;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		u16 printer_delay_time;
		u32 start_time,space_time;
		if(managerFileCount(SysCtrl.avi_list)>=1)
		{
			if(FILE_TYPE_JPG == SysCtrl.file_type)	// scale up
			{
				deamon_keysound_play();
				VIDEO_PLAY_WAIT_KEYSOUND_END;
				
			#if  (1 == LCD_PLAY_JPEG_SCALE)
				if(SysCtrl.jpeg_scale_flag < 3)
				{
					SysCtrl.jpeg_scale_flag ++;
				}
				else
				{
					SysCtrl.jpeg_scale_flag = 0;
				}
				video_playback_start(SysCtrl.file_index);
			#endif

				#if defined(USE_BOARD_DY45)||defined(USE_BOARD_DY79)||defined(USE_BOARD_DY116)||defined(USE_BOARD_DY152)
					palyback_print_en = 1;
				#endif

				if(palyback_print_en)
				{
					TCHAR fname[64];
					char *name;
					int type;
					FHANDLE fd;
					if(videoPlaybackGetStatus() != MEDIA_STAT_STOP)
						videoPlaybackStop();
					if(managerFileCount(SysCtrl.avi_list)<1)
					{
						deg_Printf("no file\n");
						return 0;
					}
					//deg_Printf("video play : index = %d ",SysCtrl.file_index);
					name = manangerGetFileFullPathName(SysCtrl.avi_list,SysCtrl.file_index,&type);
					if(name == NULL)
					{
						deg_Printf("index err\n");
						return 0;
					}
					SysCtrl.file_index = SysCtrl.file_index;
					SysCtrl.file_type=type;
					deg_Printf("%s\n",name);
					Ascii2Tchar(name, fname, sizeof(fname)/sizeof(fname[0]));
					fd = (FHANDLE)open(/*name*/fname,FA_READ);
					deg_Printf("fd=%d,%d\n",fd,fs_size(fd));
					if((int)fd<0)
					{
						deg_Printf("open file err\n");
						return 0;
					}

					#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
					if(0)
					#else
					if(1) //printer dly 3s
					#endif
					{
						start_time = XOSTimeGet();
						XOSTimeDly(100);
						hal_wdtClear();
						printer_delay_time = 3;
						playbackPrintDlyShow(handle,true,printer_delay_time);
						XOSTimeDly(1000);
						while(1)
						{
							hal_wdtClear();
							space_time = XOSTimeGet()-start_time;
							if(space_time >= 1000)
							{
								printer_delay_time--;
								playbackPrintDlyShow(handle,true,printer_delay_time);
								if(0 == printer_delay_time)
								{
									playbackPrintDlyShow(handle,false,0);
									break;
								}
								start_time = XOSTimeGet();
							}
						}
					}


					playbackPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_PRINTING);
					drawUIService(true);

					#if 0
					//wait at least 6s
					keep_print_wait_playbak=XOSTimeGet();
					if((XOSTimeGet()-keep_print_wait_playbak)<6000)
					{
						u32 last_print_time_pbak= 6-((XOSTimeGet()-keep_print_wait_playbak)/1000); //keep wait (8-x) sec
						while(last_print_time_pbak)
						{
							last_print_time_pbak--;
							hal_sysDelayMS(1000);
							ax32xx_wdtClear();
						}
					}
					#endif

					u8 print_ret=printer_jpeg(fd,SysCtrl.printer_level,SysCtrl.printer_print_mode,SysCtrl.battery);
					if(8==print_ret)
					{
						playbackPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_NO_PAPER);
						drawUIService(true);
						XOSTimeDly(2000);
					}
					else if(9==print_ret)
					{
						playbackPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_OVER_HEAT);
						drawUIService(true);
						XOSTimeDly(2000);
					}else if(12==print_ret) //Do not print while charging
					{
						playbackPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_CHARG_N_PRINT);
						drawUIService(true);
						XOSTimeDly(2000);
					}else if(15 == print_ret)
					{
						playbackPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_PWR_LOW);
						drawUIService(true);
						XOSTimeDly(2000);
					}
					playbackPrinterTipsShow(handle,false,0,0);
					close(fd);

				}
				#if 0//!defined(USE_BOARD_DY45)
				palyback_print_en = 0;
				#endif
			}
			#if !defined(USE_BOARD_DY20)
			else
			{
				if(videoPlaybackGetStatus()== MEDIA_STAT_START)
				{
					videoPlaybackPuase();
					while(videoPlaybackGetStatus()==MEDIA_STAT_START);
					playbackStateShow(handle,8);
				}
				else if(videoPlaybackGetStatus() == MEDIA_STAT_PAUSE)
				{
					videoPlaybackResume();
					playbackStateShow(handle,2);
				}
			}
			#endif
		}
	}
	return 0;
}

static int playBackSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("player : sdc stat ->%d\n",SysCtrl.sdcard);
	if((SysCtrl.sdcard != SDC_STAT_NORMAL)/* && (state == MEDIA_STAT_PLAY)*/) // sdc out when recording
	{
		if(videoPlaybackGetStatus() == MEDIA_STAT_PLAY)
		    videoPlaybackStop();
		uiOpenWindow(&noFileWindow,0);
	}
	else
	{
		SysCtrl.file_index = managerFileCount(SysCtrl.avi_list);
		if(SysCtrl.file_index>0)
		{
			video_playback_start(SysCtrl.file_index);
			playbackResolutionShow(handle);
			playbackFileNameShow(handle);
			playbackLockShow(handle);
		}
		else
			uiOpenWindow(&noFileWindow,0);
	}
	return 0;
}
static int playBackSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{	
	playbackBaterryShow(handle);
	return 0;
}
static int playBackSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		playbackBaterryShow(handle);
	return 0;
}
static int playBackSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

void backBrushBlack()
{
	disp_frame_t *show_buf = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(show_buf)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		hal_dispframeVideoCfg(show_buf,0,0,lcd_w,lcd_h);

		memset(show_buf->y_addr,40,show_buf->w*show_buf->h);
		memset(show_buf->uv_addr,0x80,show_buf->w*show_buf->h/2);

		ax32xx_sysDcacheFlush((u32)show_buf->y_addr,show_buf->w*show_buf->h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)show_buf,0,0,VIDEO_ROTATE);
		XOSTimeDly(5);
	}
}

static int playBackOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("playBack Open Win!!!\n");
	//backBrushBlack();
	winSetName(handle,"play back");
	winSetResid(winItem(handle,PLAYBACK_MODE_ID),R_ID_ICON_MTPLAY);
//	#if SMALL_PANEL_SUPPORT==0
//	playbackPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
//	playbackPlayPrint_num(handle,false);
	palyback_print_en=0;

	playbackResolutionShow(handle);
	playbackFileNameShow(handle);
	playbackLockShow(handle);
	playbackMDShow(handle);
	playbackMonitorShow(handle);
	playbackIrLedShow(handle);
	playbackSDShow(handle);
	playbackMicShow(handle);
	playbackBaterryShow(handle);
	playbackPrinterShow(handle);
	playbackStateShow(handle,2);
	playbackPrinterTipsShow(handle,false,0,0);

	if(managerFileCount(SysCtrl.avi_list)<1)
	{
		uiOpenWindow(&noFileWindow,0);
	}

	if(0==SysCtrl.cartoon_mode)
	{
		SysCtrl.cartoon_mode=1;
		SysCtrl.cartoon_show_cnt=0;
	}
	
	return 0;
}
static int playBackCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	u16 bk_w,bk_h;
	deg_Printf("playBack Close Win!!!\n");
	hal_lcdGetResolution(&bk_w,&bk_h);
	u8 *buff = (u8 *)dispLayerGetFreeFrame(DISP_LAYER_OSD0);
	if(buff)
	{
		u32 i = 0;
		for(; i < bk_w*bk_h; i++)
			*(buff + i) = R_COLOR_TRANSFER;
		ax32xx_sysDcacheWback((u32)buff,bk_w*bk_h);
		dispLayerSetFrame(DISP_LAYER_OSD0,(u32)buff);
	}
	ax32xx_deUpdate();
	return 0;
}
static int playBackWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("playBack WinChild Close!!!\n");
	if(managerFileCount(SysCtrl.avi_list)<1)
	{
		uiOpenWindow(&noFileWindow,0);
		return 0;
	}
	else
	{
		video_playback_start(SysCtrl.file_index); 
	}
//	#if SMALL_PANEL_SUPPORT==0
//	playbackPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
	playbackResolutionShow(handle);
	playbackFileNameShow(handle);
	playbackLockShow(handle);
	playbackMDShow(handle);
	playbackMonitorShow(handle);
	playbackIrLedShow(handle);
	playbackSDShow(handle);
	playbackMicShow(handle);
	playbackBaterryShow(handle);	
	playbackPrinterShow(handle);
	playbackStateShow(handle,2);
	return 0;
}
static int playBackSysMsgTimeUpdate(winHandle handle,uint32 parameNum,uint32* parame)
{
	INT32 playtime=0,totaltime = 0;
	videoPlaybackGetTime((u32*)&totaltime,(u32*)&playtime);
	if(playtime%1000 >500)
		playtime=playtime/1000;
	else
		playtime=playtime/1000;
	if(totaltime%1000 >500)
		totaltime = totaltime/1000 + 1;
	else 
		totaltime = totaltime/1000;
	playbackPlayTimeShow(handle,playtime,totaltime);
	return 0;
}


static int playBackSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
//	#if SMALL_PANEL_SUPPORT==0
//	playbackPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif


		

	if(FILE_TYPE_AVI==SysCtrl.file_type)
	{
		if(videoPlaybackGetStatus()==MEDIA_STAT_PAUSE)
		{
			playbackStateShow(handle,8);
		}
	}

	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		playbackBaterryShow(handle);	
	}

	

	return 0;
}

msgDealInfor playBackMsgDeal[]=
{
	{SYS_OPEN_WINDOW,playBackOpenWin},
	{SYS_CLOSE_WINDOW,playBackCloseWin},
	{SYS_CHILE_COLSE,playBackWinChildClose},
	#if defined(USE_BOARD_DY05)
	{KEY_EVENT_OK,playBackKeyMsgPrinterEn},
	{KEY_EVENT_MODE,playBackKeyMsgMenu},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	//{KEY_EVENT_RECORD,playBackKeyMsgPhoto},
	//{KEY_EVENT_PHOTO,playBackKeyMsgPrinterEn},
	#elif defined(USE_BOARD_DY20)
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_RECORD,playBackKeyMsgPlayVideo},
	{KEY_EVENT_DELETE,playBackKeyMsgMenu},
	{KEY_EVENT_PHOTO,playBackKeyMsgPrinterEn},
	#elif defined(USE_BOARD_DY26)
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	//{KEY_EVENT_OK,playBackKeyMsgPlayVideo},
	{KEY_EVENT_RECORD,playBackKeyMsgMenu},
	{KEY_EVENT_PHOTO,playBackKeyMsgPrinterEn},
	#elif defined(USE_BOARD_DY43)
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_RECORD,playBackKeyMsgPrinterEn},
	{KEY_EVENT_OK,playBackKeyMsgMenu},
	{KEY_EVENT_MODE,playBackKeyMsgSetting},
	#elif defined(USE_BOARD_DY151)	
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_RECORD,playBackKeyMsgPrinterEn},
	{KEY_EVENT_OK,playBackKeyMsgMenu},
	#elif defined(USE_BOARD_DY152)
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_RECORD,playBackKeyMsgPrinterEn},
	{KEY_EVENT_OK,playBackKeyMsgMenu},
	{KEY_EVENT_MENU,playBackKeyMsgSetting},
	#elif defined(USE_BOARD_DY45)
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_RECORD,playBackKeyMsgOk},
	{KEY_EVENT_OK,playBackKeyMsgPrinterEn},
	{KEY_EVENT_DELETE,playBackKeyMsgMenu},
	#elif defined(USE_BOARD_DY46)
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_OK,playBackKeyMsgPrinterEn},
	{KEY_EVENT_PHOTO,playBackKeyMsgMenu},
	#elif defined(USE_BOARD_DY49)
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_OK,playBackKeyMsgPrinterEn},
	{KEY_EVENT_MENU,playBackKeyMsgMenu},	
	#elif defined(USE_BOARD_DY16)||defined(USE_BOARD_J589)||defined(USE_BOARD_DY116)
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_RECORD,playBackKeyMsgOk},
	{KEY_EVENT_OK,playBackKeyMsgPrinterEn},
	{KEY_EVENT_DELETE,playBackKeyMsgMenu},	
	#elif defined(USE_BOARD_DY79)
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	//{KEY_EVENT_RECORD,playBackKeyMsgOk},
	{KEY_EVENT_OK,playBackKeyMsgPrinterEn},
	{KEY_EVENT_DELETE,playBackKeyMsgMenu},
	#elif defined(USE_BOARD_DY128)
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_RECORD,playBackKeyMsgOk},
	{KEY_EVENT_OK,playBackKeyMsgPrinterEn},
	{KEY_EVENT_DELETE,playBackKeyMsgMenu},	
	#else
	{KEY_EVENT_PHOTO,playBackKeyMsgOk},
	{KEY_EVENT_RECORD,playBackKeyMsgOk},
	{KEY_EVENT_RETURN,playBackKeyMsgExit},
	{KEY_EVENT_LEFT,playBackKeyMsgUp},
	{KEY_EVENT_RIGHT,playBackKeyMsgDown},
	{KEY_EVENT_MENU,playBackKeyMsgMenu},
	{KEY_EVENT_MODE,playBackKeyMsgSetting},
	{KEY_EVENT_OK,playBackKeyMsgPrinterEn},
	//{KEY_EVENT_UP,playBackKeyMsgUp},
	//{KEY_EVENT_DOWN,playBackKeyMsgDown},
    //{KEY_EVENT_PLAYBACK,playBackKeyMsgPlayBack},
    //{KEY_EVENT_PHOTO,playBackKeyMsgPhoto},
    //{KEY_EVENT_RECORD,playBackKeyMsgVideo},
	#endif
	{SYS_EVENT_SDC,playBackSysMsgSD},
	{SYS_EVENT_USB,playBackSysMsgUSB},
	{SYS_EVENT_BAT,playBackSysMsgBattery},
	{SYS_EVENT_MD,playBackSysMsgMD},
	{SYS_EVENT_1S,playBackSysMsg1S},
	{SYS_EVENT_TIME_UPDATE,playBackSysMsgTimeUpdate},
	{EVENT_MAX,NULL},
};

WINDOW(playBackWindow,playBackMsgDeal,playBackWin)


