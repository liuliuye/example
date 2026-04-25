#include "application.h"
#include "taskMainMenuRes.c"

#define MAINMENU_PIXEL_W 		96
#define MAINMENU_PIXEL_H 		96

#ifdef USE_DIE_MAUS_UI
#define MAINMENU_TASK_NUM 		5
#else
#define MAINMENU_TASK_NUM 		6
#endif

static u32 mainmenu_show_time;
static u8 show_bk;
#if defined(USE_BOARD_DY19)&&!defined(USE_DY19_OLDUI)
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU_PHOTO,RES_MAINMENU_VIDEO,RES_MAINMENU_PLAYBACK,
								 RES_MAINMENU_AUDIO,RES_MAINMENU_GAME,RES_MAINMENU_SETTING,
								};
#elif defined(USE_BOARD_DY49)||defined(USE_BOARD_DY43)||defined(USE_BOARD_DY151)||defined(USE_BOARD_DY152)
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU_PHOTO,RES_MAINMENU_VIDEO,RES_MAINMENU_PLAYBACK,
								 RES_MAINMENU_AUDIO,RES_MAINMENU_GAME,RES_MAINMENU_SETTING,
								};
#elif defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)||defined(USE_DY45_OLDUI)
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU_PHOTO,RES_MAINMENU_VIDEO,RES_MAINMENU_PLAYBACK,
								 RES_MAINMENU_SETTING,RES_MAINMENU_GAME,RES_MAINMENU_AUDIO,
								};
#elif defined(USE_BOARD_DY16)||defined(USE_BOARD_DY79)||defined(USE_BOARD_J589)||defined(USE_BOARD_DY116)||defined(USE_BOARD_DY128)
#ifdef USE_APP_PRINTER
#ifdef USE_KOERA_UI
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU_PHOTO,RES_MAINMENU_VIDEO,RES_MAINMENU_PLAYBACK,
								 RES_MAINMENU_APP,RES_MAINMENU_AUDIO,RES_MAINMENU_SETTING,
								};
#else
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU_PHOTO,RES_MAINMENU_VIDEO,RES_MAINMENU_PLAYBACK,
								 RES_MAINMENU_APP,RES_MAINMENU_GAME,RES_MAINMENU_SETTING,
								};
#endif

#elif defined(USE_DIE_MAUS_UI)
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU_PHOTO,RES_MAINMENU_VIDEO,RES_MAINMENU_PLAYBACK,
								 RES_MAINMENU_AUDIO,RES_MAINMENU_SETTING,
								};

#else
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU_PHOTO,RES_MAINMENU_VIDEO,RES_MAINMENU_PLAYBACK,
								 RES_MAINMENU_AUDIO,RES_MAINMENU_GAME,RES_MAINMENU_SETTING,
								};
#endif



#else
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU_PHOTO,RES_MAINMENU_VIDEO,RES_MAINMENU_AUDIO,
								 RES_MAINMENU_PLAYBACK,RES_MAINMENU_GAME,RES_MAINMENU_SETTING,
								};
#endif

static u8* mainmenu_bk_buf;
static u16 bk_w,bk_h;

static u8* mainmenu_id_buf[MAINMENU_TASK_NUM];
static u16 id_w,id_h;


typedef	struct
{
	u16 x;
	u16 y; 
}MainMenu_Pos;

#if 1
static MainMenu_Pos mainMenu_showpos[MAINMENU_TASK_NUM]=
{
//==two pixel align==
#if defined(USE_BOARD_DY16)||defined(USE_BOARD_DY79)||defined(USE_BOARD_J589)||defined(USE_BOARD_DY116) 
	#ifdef USE_DIE_MAUS_UI	
		{12,18},	
		{12+100,18},
		{12+100+100,18},
		{12,18+98},
		{12+100,18+98},
	#else
		{12,18},	
		{12+100,18},
		{12+100+100,18},
		{12,18+98},
		{12+100,18+98},
		{12+100+100,18+98},
	#endif
#else
	{12,18},	
	{12+100,18},
	{12+100+100,18},
	{12,18+108},
	{12+100,18+108},
	{12+100+100,18+108},
#endif
};
#endif

#if defined(USE_MMENU_JPG_MODE)
static void mainMenu_showbuf(void)	 
{
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

		memcpy(p_lcd_buffer->y_addr,mainmenu_bk_buf,bk_w*bk_h*3/2);
		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,bk_w*bk_h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
}
#else

static void mainMenu_showbuf(u8 bk)	// bk  0: show idx , 1: show bk 
{
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

		if(0==bk)
		{
			//#if defined(USE_MMENU_JPG_MODE)
			//memcpy(p_lcd_buffer->y_addr,mainmenu_id_buf[SysCtrl.mainmenu_taskidx],id_w*id_h*3/2);
			//#else
			memcpy(p_lcd_buffer->y_addr,mainmenu_bk_buf,bk_w*bk_h*3/2);
			yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,mainMenu_showpos[SysCtrl.mainmenu_taskidx].x,mainMenu_showpos[SysCtrl.mainmenu_taskidx].y,id_w,id_h,mainmenu_id_buf[SysCtrl.mainmenu_taskidx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			//#endif
			ax32xx_sysDcacheWback((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
		}
		else
		{
			memcpy(p_lcd_buffer->y_addr,mainmenu_bk_buf,bk_w*bk_h*3/2);
			ax32xx_sysDcacheWback((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
		}
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
}
#endif

static int mainMenuKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		mainMenuStrShow(handle,false);
		drawUIService(true);		// clean str
		
		taskStart(TASK_PHOTO_ENCODE,0);

		//deg_Printf("id_idx:%d\n",SysCtrl.mainmenu_taskidx);
	}
	return 0;
}


static int mainMenuKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)//KEY_PRESSED
	{
		mainMenuStrShow(handle,false);
		drawUIService(true);		// clean str
		
		if(0==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_PHOTO_ENCODE,0);
		}
		else if(1==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_VIDEO_RECORD,0);
		}
		else if(2==SysCtrl.mainmenu_taskidx)
		{//
			#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY43)||defined(USE_BOARD_DY45)||defined(USE_BOARD_DY20)|| \
				defined(USE_BOARD_DY26)||defined(USE_BOARD_DY49)||defined(USE_BOARD_DY16)||defined(USE_BOARD_DY79)|| \
				defined(USE_BOARD_J589)||defined(USE_BOARD_DY116)||defined(USE_BOARD_DY128)||defined(USE_BOARD_DY151)|| \
				defined(USE_BOARD_DY152)
			taskStart(TASK_PLAY_BACK,0);
			#elif defined(USE_BOARD_DY19)&&!defined(USE_DY19_OLDUI)
			taskStart(TASK_PLAY_BACK,0);
			#else
			taskStart(TASK_AUDIO_PLAYER,0);
			if(0==SysCtrl.cartoon_mode)
			{
				SysCtrl.cartoon_mode=1;
				SysCtrl.cartoon_show_cnt=0;
			}
			#endif

		}
		else if(3==SysCtrl.mainmenu_taskidx)
		{
			#if defined(USE_BOARD_DY19)&&!defined(USE_DY19_OLDUI)
				taskStart(TASK_AUDIO_PLAYER,0);
				if(0==SysCtrl.cartoon_mode)
				{
					SysCtrl.cartoon_mode=1;
					SysCtrl.cartoon_show_cnt=0;
				}
			#elif defined(USE_BOARD_DY16)||defined(USE_BOARD_DY79)||defined(USE_BOARD_J589)||defined(USE_BOARD_DY116)||defined(USE_BOARD_DY128)|| \
				defined(USE_BOARD_DY43)||defined(USE_BOARD_DY151)||defined(USE_BOARD_DY152)
				#ifdef USE_APP_PRINTER
					//taskStart(TASK_AUDIO_PLAYER,0);
					taskStart(TASK_APP,0);				
					if(0==SysCtrl.cartoon_mode)
					{
						SysCtrl.cartoon_mode=1;
						SysCtrl.cartoon_show_cnt=0;
					}
				#else
					taskStart(TASK_AUDIO_PLAYER,0);
					if(0==SysCtrl.cartoon_mode)
					{
						SysCtrl.cartoon_mode=1;
						SysCtrl.cartoon_show_cnt=0;
					}
				#endif	
			#elif defined(USE_BOARD_DY05)||defined(USE_DY45_OLDUI)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
				taskStart(TASK_SETTING_MENU,0);
			#else
			//filelist_build();
				taskStart(TASK_PLAY_BACK,0);
			#endif
		}
		else if(4==SysCtrl.mainmenu_taskidx)
		{
			#ifdef USE_KOERA_UI
			taskStart(TASK_AUDIO_PLAYER,0);
			if(0==SysCtrl.cartoon_mode)
			{
				SysCtrl.cartoon_mode=1;
				SysCtrl.cartoon_show_cnt=0;
			}
			#elif defined(USE_DIE_MAUS_UI)
			taskStart(TASK_SETTING_MENU,0);
			#else
			taskStart(TASK_GAME_MENU,0);
			#endif
		}
		else if(5==SysCtrl.mainmenu_taskidx)
		{
			#if defined(USE_BOARD_DY05)||defined(USE_DY45_OLDUI)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
				taskStart(TASK_AUDIO_PLAYER,0);
				if(0==SysCtrl.cartoon_mode)
				{
					SysCtrl.cartoon_mode=1;
					SysCtrl.cartoon_show_cnt=0;
				}
			#else
				taskStart(TASK_SETTING_MENU,0);
			#endif
		}

		deg_Printf("id_idx:%d\n",SysCtrl.mainmenu_taskidx);
	}
	return 0;
}


static int mainMenuKeyMsgPlayback(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		mainMenuStrShow(handle,false);
		drawUIService(true);		// clean str
		
		taskStart(TASK_PLAY_BACK,0);

		deg_Printf("id_idx:%d\n",SysCtrl.mainmenu_taskidx);
	}
	return 0;
}

static int mainMenuKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		mainMenuStrShow(handle,false);
		drawUIService(true);		// clean str
		
		taskStart(TASK_SETTING_MENU,0);

		//deg_Printf("id_idx:%d\n",SysCtrl.mainmenu_taskidx);
	}
	return 0;
}

static int mainMenuKeyMsgLeft(winHandle handle,uint32 parameNum,uint32* parame)
{	
	//hal_gpioInit(GPIO_PF,GPIO_PIN10,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	//hal_gpioWrite(GPIO_PF,GPIO_PIN10,GPIO_HIGH);
	//ax32xx_WKI0OutputEnable(1,1);
	//return 0;
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

		#if 0
		LDOCON &= ~(1<<12);
		return 0;
		#endif
		
		if(SysCtrl.mainmenu_taskidx>0)
		{
			SysCtrl.mainmenu_taskidx--;
		}
		else
		{
			SysCtrl.mainmenu_taskidx=(MAINMENU_TASK_NUM-1);
		}
		#if defined(USE_MMENU_JPG_MODE)
			if(NULL!=mainmenu_bk_buf)
			{
				jpg_decode_buf(mainmenu_jap_id[SysCtrl.mainmenu_taskidx],mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
			}
			XOSTimeDly(30);
			mainMenu_showbuf();
			mainmenu_show_time=XOSTimeGet();
			show_bk=1;
		#else
			show_bk=0;
			mainMenu_showbuf(show_bk);
			mainmenu_show_time=XOSTimeGet();
		#endif

	}
	return 0;
}

static int mainMenuKeyMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{

	//hal_gpioInit(GPIO_PF,GPIO_PIN10,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	//hal_gpioWrite(GPIO_PF,GPIO_PIN10,GPIO_LOW);
	//ax32xx_WKI0OutputEnable(1,0);
	//return 0;

	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		#if 0
		LDOCON |= (1<<12);
		return 0;
		#endif
		if(SysCtrl.mainmenu_taskidx<(MAINMENU_TASK_NUM-1))
		{
			SysCtrl.mainmenu_taskidx++;
		}
		else
		{
			SysCtrl.mainmenu_taskidx=0;
		}
		#if defined(USE_MMENU_JPG_MODE)
			if(NULL!=mainmenu_bk_buf)
			{
				jpg_decode_buf(mainmenu_jap_id[SysCtrl.mainmenu_taskidx],mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
			}
			XOSTimeDly(30);
			mainMenu_showbuf();
			mainmenu_show_time=XOSTimeGet();
			show_bk=1;
		#else
			show_bk=0;
			mainMenu_showbuf(show_bk);
			mainmenu_show_time=XOSTimeGet();
		#endif
	}
	return 0;
}


static int mainMenuKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{


	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.mainmenu_taskidx>2)
		{
			SysCtrl.mainmenu_taskidx-=3;
		}
		else
		{
			SysCtrl.mainmenu_taskidx+=3;
		}
#if defined(USE_MMENU_JPG_MODE)
			if(NULL!=mainmenu_bk_buf)
			{
				jpg_decode_buf(mainmenu_jap_id[SysCtrl.mainmenu_taskidx],mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
			}
			XOSTimeDly(30);
			mainMenu_showbuf();
			mainmenu_show_time=XOSTimeGet();
			show_bk=1;
#else
			show_bk=0;
			mainMenu_showbuf(show_bk);
			mainmenu_show_time=XOSTimeGet();
#endif

	}
	return 0;
}

static int mainMenuKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{


	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.mainmenu_taskidx>2)
		{
			SysCtrl.mainmenu_taskidx-=3;
		}
		else
		{
			SysCtrl.mainmenu_taskidx+=3;
		}
#if defined(USE_MMENU_JPG_MODE)
			if(NULL!=mainmenu_bk_buf)
			{
				jpg_decode_buf(mainmenu_jap_id[SysCtrl.mainmenu_taskidx],mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
			}
			XOSTimeDly(30);
			mainMenu_showbuf();
			mainmenu_show_time=XOSTimeGet();
			show_bk=1;
#else
			show_bk=0;
			mainMenu_showbuf(show_bk);
			mainmenu_show_time=XOSTimeGet();
#endif
	}
	return 0;
}


#if defined(USE_MMENU_JPG_MODE)
static int mainMenuOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("main menu Open Win:%d\n",XOSTimeGet());
	show_bk=0;
	if(SysCtrl.mainmenu_taskidx>=MAINMENU_TASK_NUM)
	{
		SysCtrl.mainmenu_taskidx=0;
	}
	deg_Printf("SysCtrl.mainmenu_taskidx=%d\n",SysCtrl.mainmenu_taskidx);
	//==init==
	hal_lcdGetResolution(&bk_w,&bk_h);
	bk_w=(bk_w + 0x1f) & (~0x1f);  // add 32bit alignment

	if(mainmenu_bk_buf == NULL)
	{
		mainmenu_bk_buf=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
	}

	if(NULL!=mainmenu_bk_buf)
	{
		jpg_decode_buf(RES_MAINMENU_BK,mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		mainmenu_show_time=XOSTimeGet();
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}
	//mainMenuledPA7Show(handle);
	mainMenuStrShow(handle,false);
	mainMenuBaterryShow(handle);

	//SysCtrl.volume_level = configValue2Int(CONFIG_ID_VOLUME);
	//SysCtrl.volume_level = configGet(CONFIG_ID_VOLUME);
	//hal_dacSetVolume(SysCtrl.volume_level*10);

	return 0;
}
#else
static int mainMenuOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("main menu Open Win:%d\n",XOSTimeGet());
	//hal_wdtClear();
#if 1
	u8 i;
	u8 *rgb24_buf;
#endif
	show_bk=0;
	if(SysCtrl.mainmenu_taskidx>=MAINMENU_TASK_NUM)
	{
		SysCtrl.mainmenu_taskidx=0;
	}
	deg_Printf("SysCtrl.mainmenu_taskidx=%d\n",SysCtrl.mainmenu_taskidx);
	//==init==
	hal_lcdGetResolution(&bk_w,&bk_h);
	mainmenu_bk_buf=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
	if(NULL!=mainmenu_bk_buf)
	{
		jpg_decode_buf(RES_MAINMENU_BK,mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		jpg_decode_buf(RES_MAINMENU_BK,mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}

	#ifdef USE_MMENU_JPG_MODE
	hal_lcdGetResolution(&id_w,&id_h);
	#else
	id_w=(MAINMENU_PIXEL_W+0x1)&(~0x1);	// bmp must 2pixel align
	id_h=(MAINMENU_PIXEL_H+0x1)&(~0x1);
	#endif
#if 1
	deg_Printf("bk_w=%d,bk_h=%d,id_w=%d,id_h=%d\n",bk_w,bk_h,id_w,id_h);
	rgb24_buf=hal_sysMemMalloc(id_w*id_h*3,32);
	if(NULL==rgb24_buf)
	{
		deg_Printf("mem err!\n");
		return 0;
	}
#endif

#if 1
	for(i=0;i<MAINMENU_TASK_NUM;i++)
	{
		mainmenu_id_buf[i]=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(NULL!=mainmenu_id_buf[i])
		{
			#ifdef USE_MMENU_JPG_MODE
			jpg_decode_buf(mainmenu_bmp_id[i],mainmenu_id_buf[i],mainmenu_id_buf[i]+id_w*id_h,id_w,id_h);
			#else
			bmp24_to_yuv420_buf(mainmenu_bmp_id[i],rgb24_buf,mainmenu_id_buf[i],mainmenu_id_buf[i]+id_w*id_h,id_w,id_h);
			deg_Printf("id=%d,first pixel:y=0x%x,u=0x%x,y=0x%x,v=0x%x\n",i,*mainmenu_id_buf[i],*(mainmenu_id_buf[i]+id_w*id_h),*(mainmenu_id_buf[i]+1),*(mainmenu_id_buf[i]+id_w*id_h+1));
			#endif
		}
		else
		{
			uart_Printf("mem err!\n");
			return 0;
		}
	}
#else
	mainmenu_id_buf=hal_sysMemMalloc(id_w*id_h*3/2,32);
	if(NULL!=mainmenu_id_buf)
	{
		jpg_decode_buf(mainmenu_bmp_id[SysCtrl.mainmenu_taskidx],mainmenu_id_buf,mainmenu_id_buf+id_w*id_h,id_w,id_h);
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}
#endif

#if 1
	if(NULL!=rgb24_buf)
	{
		hal_sysMemFree(rgb24_buf);
		rgb24_buf=NULL;
	}
#endif

	//==show==
	mainMenu_showbuf(show_bk);
	mainmenu_show_time=XOSTimeGet();
	deg_Printf("show time:%d\n",XOSTimeGet());

	mainMenuStrShow(handle,false);
	mainMenuBaterryShow(handle);

	#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)
	SysCtrl.key_down_long = 0;
	#endif
	
	//hal_gpioInit(GPIO_PE,GPIO_PIN0,GPIO_INPUT,GPIO_PULL_FLOATING);
	
	return 0;
}
#endif
static int mainMenuCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("main menu Close Win!!!\n");
	if(NULL!=mainmenu_bk_buf)
	{
		hal_sysMemFree(mainmenu_bk_buf);
		mainmenu_bk_buf=NULL;
	}
#if defined(USE_MMENU_JPG_MODE)
#else
		u8 i;
		for(i=0;i<MAINMENU_TASK_NUM;i++)
		{
			if(NULL!=mainmenu_id_buf[i])
			{
				hal_sysMemFree(mainmenu_id_buf[i]);
				mainmenu_id_buf[i]=NULL;
			}
		}
#endif

	return 0;
}

static int mainMenuSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
#if defined(USE_MMENU_JPG_MODE)
	if(XOSTimeGet()<mainmenu_show_time+300)	// too short not show it 500
	{
		return 0;
	}

	if(show_bk)
	{
		if(NULL!=mainmenu_bk_buf)
		{
			jpg_decode_buf(RES_MAINMENU_BK,mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		}
		show_bk=0;
	}
	else
	{
		if(NULL!=mainmenu_bk_buf)
		{
			jpg_decode_buf(mainmenu_jap_id[SysCtrl.mainmenu_taskidx],mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		}
		show_bk=1;
		
	}
	mainMenu_showbuf();
	mainmenu_show_time=XOSTimeGet();
#else
	if(XOSTimeGet()<mainmenu_show_time+300)	// too short not show it
	{
		return 0;
	}

	if(show_bk)
	{
		show_bk=0;
	}
	else
	{
		show_bk=1;
	}
	mainMenu_showbuf(show_bk);
	mainmenu_show_time=XOSTimeGet();
#endif

	return 0;
}
volatile INT32U *temp_a;

static int mainMenuSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{

	//boardIoctrl(SysCtrl.bfd_battery,IOCTRL_BAT_CHECK,temp_a);
	//deg("temp_a[%d] ",temp_a);
	//deg("bat=%d level[%d]  tempt[%d]\n",hal_adcGetChannel(ADC_CH_PE0),SysCtrl.battery,hal_adcGetChannel(ADC_CH_PA6));//PE0:bat ad
	if(SysCtrl.usb == USB_STAT_DCIN)
	{
		mainMenuBaterryShow(handle);	
	}
	
	//hal_adcGetChannel(ADC_CH_PE0);
	//deg_Printf("ADC_CH_PE0: %d\n",hal_adcGetChannel(ADC_CH_PE0));
	
	return 0;
}

static int mainMenuSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	mainMenuBaterryShow(handle);
	return 0;
}
static int mainMenuSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		mainMenuBaterryShow(handle);
	return 0;
}

msgDealInfor mainMenuMsgDeal[]=
{
	{SYS_OPEN_WINDOW,mainMenuOpenWin},
	{SYS_CLOSE_WINDOW,mainMenuCloseWin},
	{KEY_EVENT_UP,mainMenuKeyMsgUp},
	{KEY_EVENT_DOWN,mainMenuKeyMsgDown},
	{KEY_EVENT_LEFT,mainMenuKeyMsgLeft},
	{KEY_EVENT_RIGHT,mainMenuKeyMsgRight},
	{KEY_EVENT_OK,mainMenuKeyMsgOk},
	{KEY_EVENT_RECORD,mainMenuKeyMsgPhoto},

	#if defined(USE_BOARD_DY43)
	{KEY_EVENT_MODE,mainMenuKeyMsgMenu},
	#elif defined(USE_BOARD_DY152)	
	{KEY_EVENT_PLAYBACK,mainMenuKeyMsgPlayback},
	{KEY_EVENT_MENU,mainMenuKeyMsgMenu},
	#else
	{KEY_EVENT_MENU,mainMenuKeyMsgMenu},
	{KEY_EVENT_MODE,mainMenuKeyMsgPlayback},
	#endif
	
	{SYS_EVENT_500MS,mainMenuSysMsg500MS},
	{SYS_EVENT_1S,mainMenuSysMsg1S},
	{SYS_EVENT_USB,mainMenuSysMsgUSB},
	{SYS_EVENT_BAT,mainMenuSysMsgBattery},
	{EVENT_MAX,NULL},
};

WINDOW(mainMenuWindow,mainMenuMsgDeal,mainMenuWin)



