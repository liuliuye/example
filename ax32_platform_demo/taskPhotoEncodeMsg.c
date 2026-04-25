#include "application.h"
#include "taskPhotoEncodeRes.c"
#include "../multimedia/interface/userInterface.h"
#include "../device/printer/printer.h"
#include "../multimedia/watermark/image_watermark.h"

extern int image_take_photo(void);
extern int image_take_photo_to_sdram(u32 *jpg_sdram_addr,u32 *jpg_size);
extern void photo_animation_effect(UserInterface name,uint8 flag);
static int PhotoCloseFlash(void);

#ifdef USE_HALL_LCD_FLIP
extern u8 lcd_flip;
#endif
u32 photo_focus_show_count = 0;
static u8 *focus_sound = 0;
static s32 focus_sound_size = 0;
//static u32 keep_print_wait_encode;  //when keep print  ,need to wait

//=========for watermark==========
//keep
 u8* small_pic_id_buf[20]; 

 u32  small_pic_id[20]={RES_NUM_0,RES_NUM_1,RES_NUM_2,RES_NUM_3,RES_NUM_4,
								RES_NUM_5,RES_NUM_6,RES_NUM_7,RES_NUM_8,RES_NUM_9,
								RES_NUM_SLASH,RES_NUM_COLON,RES_NUM_BLANK,
							   };
 //=========for watermark==========
 #ifdef USE_BOARD_DY42
int deal_photoApp_fstMsg = 0;
extern int flicker_on ;
extern int flicker_slow ;
#endif

void photoFocusIconCancel(winHandle handle)
{
	if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_RED){
		if(XOSTimeGet() - photo_focus_show_count > 800){
			SysCtrl.photo_focus=PHOTO_FOCUS_ICON_NONE;
			photoFocusShow(handle);
			photoZoomRatioShow(handle);
		}
	}
}
//#ifdef DOUBLE_CAM
int photoLongKey = 0;
extern INT8U senser_select;
//#endif
static int photoKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	int ret;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		u16 take_photo_num=0,take_photo_delay_time=0,printer_delay_time;
		u32 start_time,space_time;
		//filelist_build();
		deg_Printf("photo_mode_switch=%d,photo_mode_idx=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);
		//flicker_on = 1;
		//flicker_slow = 0;
		//return 1;						

		#if defined(USE_BOARD_DY20)||defined(USE_BOARD_DY05)
		if(SysCtrl.key_down_long == 1)
		{
			SysCtrl.key_down_long = 0;
			return 0;
		}
		#else
		if(photoLongKey == 1)
		{
			photoLongKey = 0;
			deg_Printf("in double cam change\n");
			return 0;
		}
		#endif

		#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY16)||defined(USE_BOARD_DY128)
			if(0==SysCtrl.photo_mode_switch)	// photo mode
			{
				if((0==SysCtrl.photo_mode_idx)||(7==SysCtrl.photo_mode_idx)||(8==SysCtrl.photo_mode_idx))
				{
					take_photo_num=1;
					take_photo_delay_time=0;
				}
				else if(1==SysCtrl.photo_mode_idx)
				{
					take_photo_num=3;
					take_photo_delay_time=0;
				}
				else if(2==SysCtrl.photo_mode_idx)
				{
					take_photo_num=5;
					take_photo_delay_time=0;
				}
				else if(3==SysCtrl.photo_mode_idx)
				{
					take_photo_num=1;
					take_photo_delay_time=3;
				}
				else if(4==SysCtrl.photo_mode_idx)
				{
					take_photo_num=1;
					take_photo_delay_time=5;
				}
				else if(5==SysCtrl.photo_mode_idx)
				{
					take_photo_num=1;
					take_photo_delay_time=10;
				}
				else if(6==SysCtrl.photo_mode_idx)
				{
					take_photo_num=3;
					take_photo_delay_time=5;
				}

			}
			else
			{
				take_photo_num=1;
				take_photo_delay_time=0;
			}
		#elif defined(USB_TIMEMOREPHOTO_INSETTING)
			take_photo_num=configValue2Int(CONFIG_ID_MOREPHOTO);
			take_photo_delay_time = configValue2Int(CONFIG_ID_TIMEPHOTO);
			if(take_photo_num == 0)
				take_photo_num = 1;
		#else
			take_photo_num=1;
			take_photo_delay_time = configValue2Int(CONFIG_ID_TIMEPHOTO);
		#endif
		
		deg_Printf("take_photo_num:%d,take_photo_delay_time=%d\n",take_photo_num,take_photo_delay_time);

		//while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);
		if(0 != take_photo_delay_time)	// delay take photo show num
		{
			start_time = XOSTimeGet();
			XOSTimeDly(100);
			hal_wdtClear();
			photo_time_num_show(handle,true,take_photo_delay_time);
			while(1)
			{
				hal_wdtClear();
				space_time = XOSTimeGet()-start_time;
				if(space_time >= 1000)
				{
					take_photo_delay_time--;
					photo_time_num_show(handle,true,take_photo_delay_time);
					if(0 == take_photo_delay_time)
					{
						photo_time_num_show(handle,false,0);
						break;
					}
					start_time = XOSTimeGet();
				}
				taskPhotoEncodeService(/*NULL*/0);
				deg_Printf("taskPhotoEncodeService(NULL);");
			}
		}

		while(take_photo_num--)
		{

			taskPhotoEncodeService(/*NULL*/0);
			XOSTimeDly(300); //keep //XOSTimeDly(300);

			if(SDC_STAT_NORMAL==SysCtrl.sdcard || SDC_STAT_FULL==SysCtrl.sdcard)// take photo to sdcard 
			{
				SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
				photoFocusShow(handle);
				drawUIService(true);

				boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);
				//==show pencil effect==
				if(0==SysCtrl.photo_mode_switch)
				{
					if((7==SysCtrl.photo_mode_idx)||(8==SysCtrl.photo_mode_idx))
					{
						disp_frame_t * handle_frame=yuv420_software_get_handle_frame();
						if(NULL!=handle_frame)
						{
							u8 color;
							if(7==SysCtrl.photo_mode_idx)
							{
								color=0;
							}
							else
							{
								color=1;
							}
							yuv420_pencil_sketch(handle_frame->y_addr,handle_frame->uv_addr,handle_frame->w,handle_frame->h,color);
							hal_lcdDealBuffer(handle_frame);
						}
					}
				}



				if(SysCtrl.sd_freesize<=REDUCE_2_5M)
				{
					ret =-1 ;
					SysCtrl.sdcard=SDC_STAT_FULL;
					XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
				
				}else{

						#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
						if(senser_select == 0)
						{
							if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_ON)
							{
								#if defined(USB_PWM_LCDBK)
								ax32xx_VDDGSENEnable(1);
								#endif
							}
						}
						#elif defined(USE_FLASH_SET)
						if(senser_select == 0)
						{
							if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
							{						
								#ifdef USE_BOARD_DY116
									hal_gpioInit(GPIO_PE,GPIO_PIN0,GPIO_OUTPUT,GPIO_PULL_FLOATING);
									hal_gpioWrite(GPIO_PE,GPIO_PIN0,GPIO_HIGH);
								#else
									hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
									hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_LOW);									
								#endif
							}
						}
						#elif defined(USE_BOARD_DY152)
						if(senser_select == 0)
						{
							if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
							{						
								ax32xx_WKI0OutputEnable(1,1);
							}
						}
						#endif
						#if 1
						ret = image_take_photo();
						#else
						u32 jpg_addr;
						u32 jpg_size;
						ret=image_take_photo_to_sdram(&jpg_addr,&jpg_size);

						char *name;
						int fHandle;
						fHandle = task_image_createfile(VIDEO_CH_A,&name);
						
						write(fHandle,(void *)jpg_addr,jpg_size);
						deg_Printf("imageL save thumb :%x Size:%x\n",jpg_addr,jpg_size);
						deamon_fsSizeModify(-1,fs_size(fHandle));
						close(fHandle);
						hal_sysMemFree(jpg_addr);
						watermark_buf_bmp2yuv_free();
						#endif

						SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
						photoFocusShow(handle);
						drawUIService(true);
						
						//--add photo effect
						if(ret==0)
						{	
							UserInterface captureAnimation;
							ANIMATION(captureAnimation, LEFTBOTTOM2RIGHTUPPER)
							photo_animation_effect(captureAnimation,1);
							photoRemainNumShow(handle);
							deg_Printf("out photo effect\n");
						

							hal_csiEnable(0);						// set lcd image stop
							dispLayerSetPIPMode(DISP_PIP_DISABLE);

							/*SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
							photoPrinterTipsShow(handle,false,0,0);
							photoFocusShow(handle);
							drawUIService(true);*/

						#ifdef USB_TIMEMOREPHOTO_INSETTING
							if((SysCtrl.printer_en==1)&&(take_photo_num == 0))
						#else
							if(SysCtrl.printer_en)
						#endif
							{
								
								/*SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
								photoPrinterTipsShow(handle,false,0,0);
								photoFocusShow(handle);
								drawUIService(true);*/
								
								#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)||defined(USE_BOARD_DY42)
								if(0)
								#else
								if(1) //printer dly 3s
								#endif
								{
									start_time = XOSTimeGet();
									XOSTimeDly(100);
									hal_wdtClear();
									printer_delay_time = 3;
									photoPrintDlyShow(handle,true,printer_delay_time);
									XOSTimeDly(1000);
									while(1)
									{
										hal_wdtClear();
										space_time = XOSTimeGet()-start_time;
										if(space_time >= 1000)
										{
											printer_delay_time--;
											photoPrintDlyShow(handle,true,printer_delay_time);
											if(0 == printer_delay_time)
											{
												photoPrintDlyShow(handle,false,0);
												break;
											}
											start_time = XOSTimeGet();
										}
									}
								}
							}

						}

				}	
				
				//==end show pencil effect=== 
				boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);

				
				/*photoPrinterTipsShow(handle,false,0,0);
				SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
				photoFocusShow(handle);
				drawUIService(true);*/

				#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
				if(senser_select == 0){
					if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_ON){
						#if defined(USB_PWM_LCDBK)
						ax32xx_VDDGSENEnable(0);
						#endif
					}
				}
				#elif defined(USE_FLASH_SET)
				if(senser_select == 0)
				{
					if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
					{						
						#ifdef USE_BOARD_DY116
							hal_gpioInit(GPIO_PE,GPIO_PIN0,GPIO_OUTPUT,GPIO_PULL_FLOATING);
							hal_gpioWrite(GPIO_PE,GPIO_PIN0,GPIO_LOW);
						#else
							hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
							hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_HIGH);
						#endif
					}
				}
				#elif defined(USE_BOARD_DY152)
				if(senser_select == 0)
				{
					if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
					{						
						ax32xx_WKI0OutputEnable(1,0);
					}
				}
				#endif
					
				if(0==ret)
				{
					#ifdef USB_TIMEMOREPHOTO_INSETTING
					if((SysCtrl.printer_en==1)&&(take_photo_num == 0))
					#else
					if(SysCtrl.printer_en)
					#endif
					{	
						TCHAR fname[64];
						char *name;
						int type;
						int fd;
						if(managerFileCount(SysCtrl.avi_list)<1)
						{
							deg_Printf("no file\n");
							goto PHOTO_TAKE_PIC_TFCARD_EXIT;
						}
						name = manangerGetFileFullPathName(SysCtrl.avi_list,managerFileCount(SysCtrl.avi_list),&type);
						if(name == NULL)
						{
							deg_Printf("index err\n");
							goto PHOTO_TAKE_PIC_TFCARD_EXIT;
						}

						deg_Printf("%s\n",name);
						Ascii2Tchar(name, fname, sizeof(fname)/sizeof(fname[0]));
						fd = (int)open(/*name*/fname,FA_READ);
						if((int)fd<0)
						{
							deg_Printf("open file err\n");
							goto PHOTO_TAKE_PIC_TFCARD_EXIT;
						}
						close(fd);

						SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
						photoFocusShow(handle);
						//drawUIService(true);

						photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_PRINTING);
					    drawUIService(true);
						#if 0
							//wait at least 6s
							keep_print_wait_encode=XOSTimeGet();
							if((XOSTimeGet()-keep_print_wait_encode)<3000)
							{
								u32 last_print_time= 3-((XOSTimeGet()-keep_print_wait_encode)/1000); //keep wait (6-x) sec
								while(last_print_time)
								{
									last_print_time--;
									hal_sysDelayMS(1000);
									ax32xx_wdtClear();
								}
							}
						#endif
						fd = (int)open(/*name*/fname,FA_READ);
						deg_Printf("fd=%d,%d\n",fd,fs_size(fd));
						if((int)fd<0)
						{
							deg_Printf("open file err\n");
							goto PHOTO_TAKE_PIC_TFCARD_EXIT;
						}
						
						u8 print_ret=printer_jpeg(fd,SysCtrl.printer_level,SysCtrl.printer_print_mode,SysCtrl.battery);
						if(8==print_ret)
						{
							#ifdef USE_BOARD_DY42
							flicker_on = 1;
							flicker_slow = 0;
							#endif
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_NO_PAPER);
							drawUIService(true);
							XOSTimeDly(2000);
							//#ifdef USE_BOARD_DY42
							//flicker_on = 0;
							//#endif
						}
						else if(9==print_ret)
						{
							#ifdef USE_BOARD_DY42
							flicker_on = 1;
							flicker_slow = 1;
							#endif
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_OVER_HEAT);
							drawUIService(true);
							XOSTimeDly(2000);
							//#ifdef USE_BOARD_DY42
							//flicker_on = 0;
							//#endif
						}
						
						else if(15 == print_ret)
						{
							#ifdef USE_BOARD_DY42
							flicker_on = 1;
							flicker_slow = 0;
							#endif
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_PWR_LOW);//R_ID_STR_TIP_KEEPQ
							drawUIService(true);
							XOSTimeDly(2000);
						}
						#ifndef USE_BOARD_DY42
						else if(12 == print_ret)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_CHARG_N_PRINT);
							drawUIService(true);
							XOSTimeDly(2000);
						}
						#endif
						close(fd);
						photoPrinterTipsShow(handle,false,0,0);
						drawUIService(true);


					}
					else
					{
					}

PHOTO_TAKE_PIC_TFCARD_EXIT:
					
					if(SysCtrl.printer_en)
					{
						hal_wdtClear();
						#if (2 == SENSOR_NUM)
						sensorInit_table();
						XOSTimeDly(400);
						#endif
						hal_csiEnable(1);
						dispLayerSetPIPMode(SysCtrl.pip_mode);
						hal_lcd_get_next_frame();
						drawUIService(true);	
					}else{
					hal_wdtClear();
					hal_csiEnable(1);
					dispLayerSetPIPMode(SysCtrl.pip_mode);
					}

					if(SysCtrl.spec_color_index)
					{
						cmos_spec_color(SysCtrl.spec_color_index);
					}

				}
				else
				{
					SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
					photoFocusShow(handle);
					photoPrinterTipsShow(handle,false,0,0);
					drawUIService(true);
				}
			}
			else // no TF or TF full , print out
			{
				//XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
#if 1			
				u32 jpg_addr;
				u32 jpg_size;
				#ifdef USE_BOARD_DY42
				if(1)
				#else
					#ifdef USB_TIMEMOREPHOTO_INSETTING
					if((SysCtrl.printer_en==1)&&(take_photo_num == 0))
					#else
					if(SysCtrl.printer_en)
					#endif
				#endif
		        {
					int ret;
					#ifndef USE_BOARD_DY42
					boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);
					SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
					photoFocusShow(handle);
					drawUIService(true);
					#endif
					
					#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
					if(senser_select == 0){
						if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_ON){
							#if defined(USB_PWM_LCDBK)
							ax32xx_VDDGSENEnable(1);
							#endif
						}
					}
					#elif defined(USE_FLASH_SET)
					if(senser_select == 0)
					{
						if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
						{
							#ifdef USE_BOARD_DY116
								hal_gpioInit(GPIO_PE,GPIO_PIN0,GPIO_OUTPUT,GPIO_PULL_FLOATING);
								hal_gpioWrite(GPIO_PE,GPIO_PIN0,GPIO_HIGH);
							#else
								hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
								hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_LOW);									
							#endif
						}
					}
					#elif defined(USE_BOARD_DY152)
					if(senser_select == 0)
					{
						if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
						{						
							ax32xx_WKI0OutputEnable(1,1);
						}
					}
					#endif
					
					deg_Printf("before taske photo remain:0x%x\n",hal_sysMemRemain());
					ret=image_take_photo_to_sdram(&jpg_addr,&jpg_size);
					deg_Printf("after taske photo remain:0x%x\n",hal_sysMemRemain());

					SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
					photoFocusShow(handle);
					drawUIService(true);
						
					//--add photo effect
					if(ret==0)
					{					
						UserInterface captureAnimation;
						ANIMATION(captureAnimation, LEFTBOTTOM2RIGHTUPPER)
						photo_animation_effect(captureAnimation,1);
						photoRemainNumShow(handle);
						deg_Printf("out photo effect\n");
						
						hal_csiEnable(0);						// set lcd image stop
						dispLayerSetPIPMode(DISP_PIP_DISABLE);

						//SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
						//photoFocusShow(handle);
						//drawUIService(true);
						
						#ifdef USE_BOARD_DY42
						if(1)
						#else
						if(SysCtrl.printer_en)
						#endif
						{
						    #ifndef USE_BOARD_DY42
							//SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
							//photoPrinterTipsShow(handle,false,0,0);
							//photoFocusShow(handle);
							//drawUIService(true);
							#endif
							//hal_sysMemPrint();
							#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)||defined(USE_BOARD_DY42) 
							if(0)
							#else
							if(1) //printer dly 3s
							#endif
							{
								start_time = XOSTimeGet();
								XOSTimeDly(100);
								hal_wdtClear();
								printer_delay_time = 3;
								photoPrintDlyShow(handle,TRUE,printer_delay_time);
								XOSTimeDly(1000);
								while(1)
								{
									hal_wdtClear();
									space_time = XOSTimeGet()-start_time;
									if(space_time >= 1000)
									{
										printer_delay_time--;
										photoPrintDlyShow(handle,TRUE,printer_delay_time);
										if(0 == printer_delay_time)
										{
											photoPrintDlyShow(handle,false,0);
											break;
										}
										start_time = XOSTimeGet();
									}
								}
							}
							//hal_sysMemPrint();
						}
					}
					else
					{					
						SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
						photoPrinterTipsShow(handle,false,0,0);
						photoFocusShow(handle);
						drawUIService(true);
					}
					#ifndef USE_BOARD_DY42
					boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
					
					//photoPrinterTipsShow(handle,false,0,0);
					//SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
					//photoFocusShow(handle);
					//drawUIService(true);
					#endif
					#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
					if(senser_select == 0){
						if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_ON){
							#if defined(USB_PWM_LCDBK)
							ax32xx_VDDGSENEnable(0);
							#endif
						}
					}
					#elif defined(USE_FLASH_SET)
					if(senser_select == 0)
					{
						if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
						{
							#ifdef USE_BOARD_DY116
								hal_gpioInit(GPIO_PE,GPIO_PIN0,GPIO_OUTPUT,GPIO_PULL_FLOATING);
								hal_gpioWrite(GPIO_PE,GPIO_PIN0,GPIO_LOW);
							#else
								hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
								hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_HIGH);
							#endif
						}
					}
					#elif defined(USE_BOARD_DY152)
					if(senser_select == 0)
					{
						if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
						{						
							ax32xx_WKI0OutputEnable(1,0);
						}
					}
					#endif

					if(0==ret)
					{

						hal_wdtClear();
						//hal_csiEnable(0);						// set lcd image stop
						//dispLayerSetPIPMode(DISP_PIP_DISABLE);	
						#ifndef USE_BOARD_DY42
						photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_PRINTING);
						drawUIService(true);
						XOSTimeDly(100);
						#endif

						deg_Printf("before print remain:0x%x\n",hal_sysMemRemain());
						u8 print_ret=printer_jpeg_buf((u8*)jpg_addr,SysCtrl.printer_level,SysCtrl.printer_print_mode,SysCtrl.battery,jpg_size);
						deg_Printf("after print remain:0x%x\n",hal_sysMemRemain());
						deg_Printf("print_ret:%d \n",print_ret);
						if(8==print_ret)
						{
							#ifdef USE_BOARD_DY42
							flicker_on = 1;
							flicker_slow = 0;
							#endif
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_NO_PAPER);
							drawUIService(true);
							XOSTimeDly(2000);
							//#ifdef USE_BOARD_DY42
							//flicker_on = 0;
							//#endif
						}
						else if(9==print_ret)
						{
							#ifdef USE_BOARD_DY42
							flicker_on = 1;
							flicker_slow = 1;
							#endif
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_OVER_HEAT);
							drawUIService(true);
							XOSTimeDly(2000);
							//#ifdef USE_BOARD_DY42
							//flicker_on = 0;
							//#endif
						}
						else if(15 == print_ret)
						{
							#ifdef USE_BOARD_DY42
							flicker_on = 1;
							flicker_slow = 0;
							#endif
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_PWR_LOW);//R_ID_STR_TIP_KEEPQ
							drawUIService(true);
							XOSTimeDly(2000);
						}
						#ifndef USE_BOARD_DY42
						else if(12 == print_ret)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_CHARG_N_PRINT);
							drawUIService(true);
							XOSTimeDly(2000);
						}

						
						photoPrinterTipsShow(handle,false,0,0);
						drawUIService(true);
						#endif
						
						hal_csiEnable(0);						// set lcd image stop
						dispLayerSetPIPMode(DISP_PIP_DISABLE);
						#if (2 == SENSOR_NUM)
						sensorInit_table();
						XOSTimeDly(400);
						#endif
						hal_csiEnable(1);
						dispLayerSetPIPMode(SysCtrl.pip_mode);
						hal_lcd_get_next_frame();
						//SysCtrl.lcd_frame_stop = 0;
						//hal_lcd_get_next_frame();
						
						if(SysCtrl.spec_color_index)
						{
							cmos_spec_color(SysCtrl.spec_color_index);
						}

					}
						deg_Printf("before free remain:0x%x addr:%x\n",hal_sysMemRemain(),jpg_addr);
						hal_sysMemFree((u8*)jpg_addr);
						watermark_buf_bmp2yuv_free();
						deg_Printf("after free remain:0x%x\n addr:%x",hal_sysMemRemain(),jpg_addr);
						//hal_sysMemPrint();
				}
				else
				{
					if(SysCtrl.sdcard!=SDC_STAT_NORMAL)	// no tf , not print ,tips 
					{
						#ifdef USB_TIMEMOREPHOTO_INSETTING
							if(take_photo_num == 0)
							{
								XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
							}
						#else
							XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
						#endif
					}
				}
  #endif	              
			}

		}
		//return 0;

		photoSurplusQuantityShow(handle);

	}		
	else if(keyState==KEY_CONTINUE)
	{
		deg_Printf("key continue-photo\n");
		#if defined(USE_BOARD_DY20)||defined(USE_BOARD_DY05)
			if(SysCtrl.key_down_long == 0)
			{
				SysCtrl.key_down_long = 1;
				taskStart(TASK_VIDEO_RECORD,0);
				if(0==SysCtrl.cartoon_mode)
				{
					SysCtrl.cartoon_mode=2;
					SysCtrl.cartoon_show_cnt=0;
				}
			}
		#else
			if(photoLongKey == 0)
			{
				photoLongKey = 1;
				//sensor_change();	
				#ifdef DOUBLE_CAM	
					SysCtrl.sensor_change_flag =1;	
					#if 1			
						uint8 flag=0;
						uint16 lcd_w,lcd_h;
						{
							deg_Printf("ready photo interface!\n");
							disp_frame_t * handle_frame=NULL;
							UserInterface recordeAnimation;
							ANIMATION(recordeAnimation, SQUARE_INSIDE2OUTSIDE)
				
							//SysCtrl.photo_task=1;
							bool change_finir = false;
							//deg_Printf("..z handle:%d\n",SysCtrl.photo_software_handle);
							while(1)
							{
								//deg_Printf("..a handle:%d\n",SysCtrl.photo_software_handle);
								if(0==SysCtrl.photo_software_handle)
								{
									//deg_Printf("..b\n");
									handle_frame=yuv420_software_get_handle_frame();
									if(handle_frame)
									SysCtrl.photo_software_handle=1;
								}
								else if(1==SysCtrl.photo_software_handle)
								{
									//deg_Printf("..c\n");
									SysCtrl.photo_software_handle=2;
									if(!flag)
									{
										flag=1;
										hal_lcdGetResolution(&lcd_w,&lcd_h);
										lcd_w=(lcd_w + 0x1f) & (~0x1f); // add 32bit align
										memcpy(recordeAnimation.preFrameBuf,handle_frame->y_addr,lcd_w*lcd_h*3/2);
										ax32xx_sysDcacheFlush((u32)recordeAnimation.preFrameBuf,lcd_w*lcd_h*3/2);
									}
									change_finir = recordeAnimation.run(&recordeAnimation, handle_frame->y_addr);
									hal_lcdDealBuffer(handle_frame);
									hal_lcd_get_next_frame();
									if(change_finir == true)
									break;
								}
							}
							
						//SysCtrl.photo_task=0;
						deg_Printf("ready photo interface! finish\n");

						}
					#endif
				
					if(SysCtrl.spec_color_index) //follow the orig setting
					{
						cmos_spec_color(SysCtrl.spec_color_index);
					}
					SysCtrl.sensor_change_flag =0;
					
					#ifdef USE_FLASH_SET
						if(senser_select == 1)	
						{
							PhotoFLASHShow(handle,false);
							if(configGet(CONFIG_ID_CAP_FLASH)!=R_ID_STR_COM_OFF)						
							{
								configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_OFF);
								hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
								hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_HIGH);
								configSys(CONFIG_ID_CAP_FLASH);
								userConfigSave();
							}
						}
						else
						{
							PhotoFLASHShow(handle,true);
						}
					#endif
				#endif
			}
		#endif
	}
	
	return 0;
}

#if 1
static int photoKeyMsgSensorChange(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint8 flag=0;
	uint16 lcd_w,lcd_h;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deg_Printf("photoKeyMsgSensorChange\n");
		sensor_change();
		SysCtrl.sensor_change_flag =1;	
	#if 0
		{
			deg_Printf("ready photo interface!\n");
			disp_frame_t * handle_frame;
			UserInterface recordeAnimation;
			ANIMATION(recordeAnimation, SQUARE_INSIDE2OUTSIDE)

			//SysCtrl.photo_task=1;
			bool change_finir = false;
			deg_Printf("..z handle:%d\n",SysCtrl.photo_software_handle);
			while(1)
			{
				deg_Printf("..a handle:%d\n",SysCtrl.photo_software_handle);
				if(0==SysCtrl.photo_software_handle)
				{
					deg_Printf("..b\n");
					handle_frame=yuv420_software_get_handle_frame();
					if(handle_frame)
					SysCtrl.photo_software_handle=1;
				}
				else if(1==SysCtrl.photo_software_handle)
				{
					deg_Printf("..c\n");
					SysCtrl.photo_software_handle=2;
					if(!flag)
					{
						flag=1;
						hal_lcdGetResolution(&lcd_w,&lcd_h);
						lcd_w=(lcd_w + 0x1f) & (~0x1f); // add 32bit align
						memcpy(recordeAnimation.preFrameBuf,handle_frame->y_addr,lcd_w*lcd_h*3/2);
						ax32xx_sysDcacheFlush((u32)recordeAnimation.preFrameBuf,lcd_w*lcd_h*3/2);
					}
					change_finir = recordeAnimation.run(&recordeAnimation, handle_frame->y_addr);
					hal_lcdDealBuffer(handle_frame);
					hal_lcd_get_next_frame();
					if(change_finir == true)
					break;
				}
			}
		//SysCtrl.photo_task=0;
		deg_Printf("ready photo interface! finish\n");

		}
		if(SysCtrl.spec_color_index) //follow the orig setting
		{
			cmos_spec_color(SysCtrl.spec_color_index);
		}
	#endif
		SysCtrl.sensor_change_flag =0;
	#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)
	if(senser_select == 1)	
	{
		if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_ON)
		{
			configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_OFF);
		#if defined(USB_PWM_LCDBK)
			ax32xx_VDDGSENEnable(0);
		#endif
			configSys(CONFIG_ID_CAP_FLASH);
			userConfigSave();
			PhotoFLASHShow(handle);
		}
	}
	#endif

	}
	return 0;
}

#endif

#if 0
static int photoKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
#endif

#if 0
static int photoKeyMsgIr(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		u32 val=0;
		boardIoctrl(SysCtrl.bfd_ir,IOGET_IR_GET,(INT32U)&val);
		deg_Printf("ir:%d\n",val);
		if(val)
		{
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
		}
		else
		{
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,1);
		}
		photoIRShow(handle);
	}
	return 0;
}
#endif

u32 sensor_rotate_flag = 0;
#if 0
static int photoKeyMsgSensorRotate(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deg_Printf("SensorRotate:%d\n",sensor_rotate_flag);
		if(sensor_rotate_flag == 1)
		{
			sensor_rotate_flag=0;
		}
		else if(sensor_rotate_flag == 0)
		{
			sensor_rotate_flag=1;
		}
		hal_csiEnable(0);
		_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
		csiSensor->p_fun_adapt.fp_rotate(sensor_rotate_flag);
		hal_csiEnable(1);
	}
	return 0;
}
#endif

#ifdef USE_FLASH_SET
static int PhotoKeyOkMsgFlash(winHandle handle,u32 parameNum,u32* parame)
{
	u32 keyState = KEY_STATE_INVALID;
	if(parameNum == 1)
		keyState = parame[0];
	if( keyState == KEY_PRESSED)
	{
		deg_Printf("PhotoKeyOkMsgFlash------>\n");

		if(senser_select == 0)
		{
			if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_OFF)
			{//切换自动
				configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_ISP_AUTO);				
				//hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
				//hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_LOW);
			}
			/*else if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
			{//切换常亮
				configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_LIGHTING_FORM);
				hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
				hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_LOW);
			}*/
			else if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
			{//切换关
				hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
				hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_HIGH);
				configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_OFF);
			}		
			configSys(CONFIG_ID_CAP_FLASH);
			userConfigSave();
			PhotoFLASHShow(handle,true);
		}
	}
	return 0;
}
#endif
static int PhotoCloseFlash(void)
{
	if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_ON)
	{
		configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_OFF);
		#if defined(USB_PWM_LCDBK)
		ax32xx_VDDGSENEnable(0);
		#endif
		configSys(CONFIG_ID_CAP_FLASH);
		userConfigSave();
	}
}

#ifdef USE_BOARD_DY42
static int PhotoKeyMsgFlash(winHandle handle,u32 parameNum,u32* parame)
{
	u32 keyState = KEY_STATE_INVALID;
	if(parameNum == 1)
		keyState = parame[0];
	if( keyState == KEY_PRESSED)
	{
		deg_Printf("PhotoKeyMsgFlash------>\n");
		deamon_keysound_play();
		//while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}
		if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_ON)
		{
			configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_OFF);
			hal_gpioInit(LCD_BK_CH,LCD_BK_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		 	hal_gpioWrite(LCD_BK_CH,LCD_BK_PIN,GPIO_LOW);
		}
		else
		{
			configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_ON);
			hal_gpioInit(LCD_BK_CH,LCD_BK_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		 	hal_gpioWrite(LCD_BK_CH,LCD_BK_PIN,GPIO_HIGH);
		}
		configSys(CONFIG_ID_CAP_FLASH);
		userConfigSave();
	}
	return 0;
}
#else
static int PhotoKeyMsgFlash(winHandle handle,u32 parameNum,u32* parame)
{
	u32 keyState = KEY_STATE_INVALID;
	if(parameNum == 1)
		keyState = parame[0];
	if( keyState == KEY_PRESSED)
	{
		deg_Printf("recordPhotoKeyMsgFlash------>\n");

		if(senser_select == 1)
		{
			return 0;
		}
		
		if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_ISP_AUTO)
		{
			configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_OFF);

		}
		else if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_OFF)
		{
			configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_ISP_AUTO);
		}
		configSys(CONFIG_ID_CAP_FLASH);
		userConfigSave();
		PhotoFLASHShow(handle,true);
	}
	return 0;
}
#endif
#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
static int photoKeyMsgPrintMode(winHandle handle,u32 parameNum,u32* parame)
{
	u32 keyState = KEY_STATE_INVALID;
	if(parameNum == 1)
		keyState = parame[0];
	
	if(keyState == KEY_RELEASE)
	{
		deg_Printf("photoKeyMsgPrintMode------>\n");

		if(photoLongKey == 1)
		{
			photoLongKey = 0;
			return 0;
		}

		if(SysCtrl.printer_en == 0)
		{
			return 0;
		}
		
		if(configGet(CONFIG_ID_PRINTER_MODE)==R_ID_STR_SET_PRINT_DOT)
		{
			configSet(CONFIG_ID_PRINTER_MODE,R_ID_STR_SET_PRINT_GRAY);
		}
		else
		{
			configSet(CONFIG_ID_PRINTER_MODE,R_ID_STR_SET_PRINT_DOT);
		}
		configSys(CONFIG_ID_PRINTER_MODE);
		userConfigSave();
		PhotoPrinterModeShow(handle,true);
	}
	else if(keyState==KEY_CONTINUE)
	{
		if(photoLongKey == 0)
		{
			photoLongKey = 1;
			if(SysCtrl.printer_en)
			{
				SysCtrl.printer_en = 0;
			}
			else
			{
				SysCtrl.printer_en = 1;
				if(0==SysCtrl.photo_mode_switch)	// photo mode
				{
					//if(SysCtrl.photo_mode_idx>0)
					//{
					//	SysCtrl.photo_mode_idx=7;
					//}
					SysCtrl.photo_mode_idx=0;
				}
			}

			if(SysCtrl.printer_en == 1)
				PhotoPrinterModeShow(handle,true);
			else
				PhotoPrinterModeShow(handle,false);
			//photoPrinterShow(handle);
			photo_mode_show(handle);
		}
	}

	return 0;
}
#endif


static int photoKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskStart(TASK_MAIN_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}


static int photoKeyMsgPlayBack(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskStart(TASK_PLAY_BACK,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}

static int photoKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskStart(TASK_SETTING_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}

#ifdef USE_BOARD_DY42
static int photoKeyMsgSwitchDensity(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(configGet(CONFIG_ID_PRINTER_DENSITY)==R_ID_STR_COM_LEVEL_1)
		{
			configSet(CONFIG_ID_PRINTER_DENSITY,R_ID_STR_COM_LEVEL_4);
			SysCtrl.volume_level = 7;
		}
		else if(configGet(CONFIG_ID_PRINTER_DENSITY)==R_ID_STR_COM_LEVEL_4)
		{
			configSet(CONFIG_ID_PRINTER_DENSITY,R_ID_STR_COM_LEVEL_7);
			SysCtrl.volume_level = 10;
		}
		else if(configGet(CONFIG_ID_PRINTER_DENSITY)==R_ID_STR_COM_LEVEL_7)
		{
			configSet(CONFIG_ID_PRINTER_DENSITY,R_ID_STR_COM_LEVEL_1);
			SysCtrl.volume_level = 4;
		}
		deamon_keysound_play();
		//while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}
		configSys(CONFIG_ID_PRINTER_DENSITY);
		userConfigSave();
		SysCtrl.volume_level = 10;
	}
	return 0;
}
#endif


extern INT8U photoPipMode;
static int photoLongKeyFlag = 0;
static int photoKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}
		
		deg_Printf("SysCtrl.photo_mode_switch:=%d=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);

		if(0==SysCtrl.photo_mode_switch)	// photo mode 
		{
			if(SysCtrl.photo_mode_idx>0)	// pre photo mode 
			{
				if(SysCtrl.printer_en)
					SysCtrl.photo_mode_idx=0;
				else
					SysCtrl.photo_mode_idx--;
			}
			else							//  swith frame mode
			{
				SysCtrl.photo_mode_switch=3;

				//reset crop
				/*
				SysCtrl.crop_level =0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				photoZoomRatioShow(handle);
				*/

				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);

				SysCtrl.photo_frame_idx = (ICON_FRAME_NUM -1);
				deg_Printf("photo_frame_idx:%d\n",SysCtrl.photo_frame_idx);

				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
		}
		else if(1==SysCtrl.photo_mode_switch)	// spec col mode
		{
			if(SysCtrl.spec_color_index>0)
			{
				SysCtrl.spec_color_index--;
				cmos_spec_color(SysCtrl.spec_color_index);
				if(0==SysCtrl.spec_color_index)// switch photo mode
				{
					//==color normal
					cmos_spec_color(SysCtrl.spec_color_index);

					SysCtrl.photo_mode_switch=0;
					if(SysCtrl.printer_en)
						SysCtrl.photo_mode_idx=0;//7
					else
						SysCtrl.photo_mode_idx=6;//8
				}
			}
		}
		else if(2==SysCtrl.photo_mode_switch)	// effect mode
		{
			if(SysCtrl.photo_effect_idx>0)
			{
				//SysCtrl.photo_effect_idx--;
				if(SysCtrl.photo_effect_idx == 7)
					SysCtrl.photo_effect_idx = 6;
				else if(SysCtrl.photo_effect_idx == 6)
					//SysCtrl.photo_effect_idx = 5;
					SysCtrl.photo_effect_idx = 4;
				else if(SysCtrl.photo_effect_idx == 5)
					SysCtrl.photo_effect_idx = 4;
				else if(SysCtrl.photo_effect_idx == 4)
					SysCtrl.photo_effect_idx = 1;
				else if(SysCtrl.photo_effect_idx == 1)
					SysCtrl.photo_effect_idx = 0;
			}
			else
			{
				//==close effect==
				//==switch spec color mode==
				SysCtrl.photo_mode_switch=1;
				SysCtrl.spec_color_index=5;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
		}
		else if(3==SysCtrl.photo_mode_switch)	// frame mode
		{
			if(SysCtrl.photo_frame_idx>0)
			{
				SysCtrl.photo_frame_idx--;
				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
			else	
			{
				//==close frame==
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				hal_custom_frame_unit();
				//SysCtrl.photo_frame_idx = ICON_FRAME_NUM -1;

				// switch effect mode 
				SysCtrl.photo_mode_switch=2;
				SysCtrl.photo_effect_idx=CAM_EFFECT_MAX-2;
				
				//reset crop
				/*SysCtrl.crop_level =0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				photoZoomRatioShow(handle);
				*/
			}
		}
		if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8)){
			if(SysCtrl.crop_level != 0){
			    SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
				hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				photoZoomRatioShow(handle);
			}
		}

		photo_mode_show(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		deg_Printf("S2222ysCtrl.photo_mode_switch:=%d=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);

	/*	if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		SysCtrl.crop_level+=1;
	    if(SysCtrl.crop_level > 15)
			SysCtrl.crop_level=15;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);  */
		if((SysCtrl.photo_mode_switch==2 )|| (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		deg_Printf("before crop:%d\t",SysCtrl.crop_level);
		SysCtrl.crop_level+=1;
	#ifdef PHOTO_ZOOM_RATIO_16		
		if(SysCtrl.crop_level > 15){
			SysCtrl.crop_level=15;
		}
	#else
		if(SysCtrl.crop_level > 5){
			SysCtrl.crop_level=5;
		}	
	#endif
		deg_Printf("aftercrop:%d\t",SysCtrl.crop_level);
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}
	
	return 0;
}
static int photoKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW)
		{
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}

		
		if(0==SysCtrl.photo_mode_switch)	// photo mode 
		{
			if(SysCtrl.photo_mode_idx<6)	// 8next photo mode 
			{
				if(SysCtrl.printer_en)
				{
					//if(SysCtrl.photo_mode_idx == 7)						//	swith spec color mode
					//{
						//==close photo mode show==
						SysCtrl.photo_mode_idx=0;
						
						SysCtrl.photo_mode_switch=1;
						SysCtrl.spec_color_index=1;
						cmos_spec_color(SysCtrl.spec_color_index);
					//}
					//else
					//	SysCtrl.photo_mode_idx = 7;
				}
				else
				{
					SysCtrl.photo_mode_idx++;
				}
			}
			else							//	swith spec color mode
			{
				//==close photo mode show==
				SysCtrl.photo_mode_idx=0;
				
				SysCtrl.photo_mode_switch=1;
				SysCtrl.spec_color_index=1;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
		}
		else if(1==SysCtrl.photo_mode_switch)	// spec col mode
		{
			if(SysCtrl.spec_color_index<5)
			{
				SysCtrl.spec_color_index++;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
			else	// switch effect mode
			{
				//==normal color==
				SysCtrl.spec_color_index=0;
				cmos_spec_color(SysCtrl.spec_color_index);
				//==
				SysCtrl.photo_mode_switch=2;
				SysCtrl.photo_effect_idx=0;

				deg_Printf("photo_mode_switch=%d,idx=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_effect_idx);
			}
		}
		else if(2==SysCtrl.photo_mode_switch)	// effect mode
		{
			if(SysCtrl.photo_effect_idx<CAM_EFFECT_MAX-2)
			{
				
				{
					//SysCtrl.photo_effect_idx++;
					if(SysCtrl.photo_effect_idx == 0)
						SysCtrl.photo_effect_idx = 1;
					else if(SysCtrl.photo_effect_idx == 1)
						SysCtrl.photo_effect_idx = 4;
					else if(SysCtrl.photo_effect_idx == 4)
						//SysCtrl.photo_effect_idx = 5;
						SysCtrl.photo_effect_idx = 6;
					else if(SysCtrl.photo_effect_idx == 5)
						SysCtrl.photo_effect_idx = 6;
					//else if(SysCtrl.photo_effect_idx == 6)
					//	SysCtrl.photo_effect_idx = 7;

				}

			}
			else
			{
				//==close effect==
	
				//==switch frame mode==
				SysCtrl.photo_mode_switch=3;

				//reset crop level , don't set this when frame mode and effect mode 
				/*SysCtrl.crop_level =0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				photoZoomRatioShow(handle);
				*/
				
				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);



				SysCtrl.photo_frame_idx = 0;
				deg_Printf("frame_index:%d\n",SysCtrl.photo_frame_idx);

				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
		}
		else if(3==SysCtrl.photo_mode_switch)	// frame mode
		{
			if(SysCtrl.photo_frame_idx<ICON_FRAME_NUM-1)
			{
				SysCtrl.photo_frame_idx++;
				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
			else	
			{
				//==close frame==
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				hal_custom_frame_unit();
				// switch photo mode 
				SysCtrl.photo_mode_switch=0;
				SysCtrl.photo_mode_idx=0;
			}
		}
		
		if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8)){
			if(SysCtrl.crop_level != 0){
			    SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
				hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				photoZoomRatioShow(handle);
			}
		}

		photo_mode_show(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		/*if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);*/
		if(((SysCtrl.photo_mode_switch==2 )) || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		deg_Printf("before crop:%d\t",SysCtrl.crop_level);
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		deg_Printf("after crop:%d\n",SysCtrl.crop_level);
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}

	return 0;
}

static int photoKeyMsgUp2(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		/*
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}
		*/
		SysCtrl.crop_level+=1;
	    if(SysCtrl.crop_level > 5)
			SysCtrl.crop_level=5;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			//photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			//return 0;
		}
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		deg_Printf("S2222ysCtrl.photo_mode_switch:=%d=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);

		if((SysCtrl.photo_mode_switch==2 ||SysCtrl.photo_mode_switch==3)|| (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		SysCtrl.crop_level+=1;
	    if(SysCtrl.crop_level > 5)
			SysCtrl.crop_level=5;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}

	return 0;
}
static int photoKeyMsgDown2(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		/*
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}
		*/
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			//photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			//return 0;
		}
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
		
	}
	else if(keyState==KEY_CONTINUE)
	{
		if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}

	return 0;
}


#ifdef USE_DIE_MAUS_UI
static int photoKeyMsgUp3(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}

		if(photoLongKeyFlag == 1)
		{
			photoLongKeyFlag = 0;
			return 0;
		}
		
		deg_Printf("SysCtrl.photo_mode_switch:=%d=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);

		if(0==SysCtrl.photo_mode_switch)	// photo mode 
		{
			if(SysCtrl.photo_mode_idx>0)	// pre photo mode 
			{
				if(1)//SysCtrl.printer_en
					SysCtrl.photo_mode_idx=0;
				else
					SysCtrl.photo_mode_idx--;
			}
			else							//  swith frame mode
			{
				SysCtrl.photo_mode_switch=3;

				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);

				SysCtrl.photo_frame_idx = (ICON_FRAME_NUM -1);
				deg_Printf("photo_frame_idx:%d\n",SysCtrl.photo_frame_idx);

				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
		}
		else if(2==SysCtrl.photo_mode_switch)	// effect mode
		{
			if(SysCtrl.photo_effect_idx>0)
			{
				//SysCtrl.photo_effect_idx--;
				if(SysCtrl.photo_effect_idx == 7)
					SysCtrl.photo_effect_idx = 6;
				else if(SysCtrl.photo_effect_idx == 6)
					//SysCtrl.photo_effect_idx = 5;
					SysCtrl.photo_effect_idx = 4;
				else if(SysCtrl.photo_effect_idx == 5)
					SysCtrl.photo_effect_idx = 4;
				else if(SysCtrl.photo_effect_idx == 4)
					SysCtrl.photo_effect_idx = 1;
				else if(SysCtrl.photo_effect_idx == 1)
					SysCtrl.photo_effect_idx = 0;
			}
			else
			{
				//==close effect==
				
				//==show photo mode==
				SysCtrl.photo_mode_switch=0;
				SysCtrl.photo_mode_idx=0;
			}
		}
		else if(3==SysCtrl.photo_mode_switch)	// frame mode
		{
			if(SysCtrl.photo_frame_idx>0)
			{
				SysCtrl.photo_frame_idx--;
				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
			else	
			{
				//==close frame==
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				hal_custom_frame_unit();
				//SysCtrl.photo_frame_idx = ICON_FRAME_NUM -1;

				// switch effect mode 
				SysCtrl.photo_mode_switch=2;
				SysCtrl.photo_effect_idx=CAM_EFFECT_MAX-2;
				
			}
		}
		
		if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8)){
			if(SysCtrl.crop_level != 0){
			    SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
				hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				photoZoomRatioShow(handle);
			}
		}

		photo_mode_show(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		deg_Printf("S2222ysCtrl.photo_mode_switch:=%d=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);
		if(photoLongKeyFlag == 0)
		{
			photoLongKeyFlag = 1;
		}

		if((SysCtrl.photo_mode_switch==2 )|| (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		deg_Printf("before crop:%d\t",SysCtrl.crop_level);
		SysCtrl.crop_level+=1;
	#ifdef PHOTO_ZOOM_RATIO_16		
		if(SysCtrl.crop_level > 15){
			SysCtrl.crop_level=15;
		}
	#else
		if(SysCtrl.crop_level > 5){
			SysCtrl.crop_level=5;
		}	
	#endif
		deg_Printf("aftercrop:%d\t",SysCtrl.crop_level);
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}
	
	return 0;
}

static int photoKeyMsgDown3(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW)
		{
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}
		
		if(photoLongKeyFlag == 1)
		{
			photoLongKeyFlag = 0;
			return 0;
		}

		if(0==SysCtrl.photo_mode_switch)	// photo mode 
		{
			if(SysCtrl.photo_mode_idx<6)	// 8next photo mode 
			{
				if(1)//SysCtrl.printer_en
				{
					//if(SysCtrl.photo_mode_idx == 7)						//	swith spec color mode
					//{
						//==close photo mode show==
						SysCtrl.photo_mode_idx=0;
						
						SysCtrl.photo_mode_switch=2;
						SysCtrl.photo_effect_idx=0;
					//}
					//else
					//	SysCtrl.photo_mode_idx = 7;
				}
				else
				{
					SysCtrl.photo_mode_idx++;
				}
			}
			else							//	swith spec color mode
			{
				//==close photo mode==
				SysCtrl.photo_mode_idx=0;
				
				//==show effect mode==
				SysCtrl.photo_mode_switch=2;
				SysCtrl.photo_effect_idx=0;
			}
		}
		else if(2==SysCtrl.photo_mode_switch)	// effect mode
		{
			if(SysCtrl.photo_effect_idx<CAM_EFFECT_MAX-2)
			{				
				{
					//SysCtrl.photo_effect_idx++;
					if(SysCtrl.photo_effect_idx == 0)
						SysCtrl.photo_effect_idx = 1;
					else if(SysCtrl.photo_effect_idx == 1)
						SysCtrl.photo_effect_idx = 4;
					else if(SysCtrl.photo_effect_idx == 4)
						//SysCtrl.photo_effect_idx = 5;
						SysCtrl.photo_effect_idx = 6;
					else if(SysCtrl.photo_effect_idx == 5)
						SysCtrl.photo_effect_idx = 6;
					//else if(SysCtrl.photo_effect_idx == 6)
					//	SysCtrl.photo_effect_idx = 7;

				}
			}
			else
			{
				//==close effect==
	
				//==show frame mode==
				SysCtrl.photo_mode_switch=3;
				
				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);



				SysCtrl.photo_frame_idx = 0;
				deg_Printf("frame_index:%d\n",SysCtrl.photo_frame_idx);

				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
		}
		else if(3==SysCtrl.photo_mode_switch)	// frame mode
		{
			if(SysCtrl.photo_frame_idx<ICON_FRAME_NUM-1)
			{
				SysCtrl.photo_frame_idx++;
				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
			else	
			{
				//==close frame==
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				hal_custom_frame_unit();
				// switch photo mode 
				SysCtrl.photo_mode_switch=0;
				SysCtrl.photo_mode_idx=0;
			}
		}
		
		if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8)){
			if(SysCtrl.crop_level != 0){
			    SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				photoZoomRatioShow(handle);
			}
		}
		photo_mode_show(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		if(photoLongKeyFlag == 0)
		{
			photoLongKeyFlag = 1;
		}

		if(((SysCtrl.photo_mode_switch==2 )) || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		deg_Printf("before crop:%d\t",SysCtrl.crop_level);
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		deg_Printf("after crop:%d\n",SysCtrl.crop_level);
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}

	return 0;
}

#else
static int photoKeyMsgUp3(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}

		if(photoLongKeyFlag == 1)
		{
			photoLongKeyFlag = 0;
			return 0;
		}
		
		deg_Printf("SysCtrl.photo_mode_switch:=%d=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);

		if(0==SysCtrl.photo_mode_switch)	// photo mode 
		{
			if(SysCtrl.photo_mode_idx>0)	// pre photo mode 
			{
				if(1)//SysCtrl.printer_en
					SysCtrl.photo_mode_idx=0;
				else
					SysCtrl.photo_mode_idx--;
			}
			else							//  swith frame mode
			{
				SysCtrl.photo_mode_switch=3;

				//reset crop
				/*
				SysCtrl.crop_level =0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				photoZoomRatioShow(handle);
				*/

				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);

				SysCtrl.photo_frame_idx = (ICON_FRAME_NUM -1);
				deg_Printf("photo_frame_idx:%d\n",SysCtrl.photo_frame_idx);

				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
		}
		else if(1==SysCtrl.photo_mode_switch)	// spec col mode
		{
			if(SysCtrl.spec_color_index>0)
			{
				SysCtrl.spec_color_index--;
				cmos_spec_color(SysCtrl.spec_color_index);
				if(0==SysCtrl.spec_color_index)// switch photo mode
				{
					//==color normal
					cmos_spec_color(SysCtrl.spec_color_index);

					SysCtrl.photo_mode_switch=0;
					if(1)//SysCtrl.printer_en
						SysCtrl.photo_mode_idx=0;//7
					else
						SysCtrl.photo_mode_idx=6;//8
				}
			}
		}
		else if(2==SysCtrl.photo_mode_switch)	// effect mode
		{
			if(SysCtrl.photo_effect_idx>0)
			{
				//SysCtrl.photo_effect_idx--;
				if(SysCtrl.photo_effect_idx == 7)
					SysCtrl.photo_effect_idx = 6;
				else if(SysCtrl.photo_effect_idx == 6)
					//SysCtrl.photo_effect_idx = 5;
					SysCtrl.photo_effect_idx = 4;
				else if(SysCtrl.photo_effect_idx == 5)
					SysCtrl.photo_effect_idx = 4;
				else if(SysCtrl.photo_effect_idx == 4)
					SysCtrl.photo_effect_idx = 1;
				else if(SysCtrl.photo_effect_idx == 1)
					SysCtrl.photo_effect_idx = 0;
			}
			else
			{
				//==close effect==
				//==switch spec color mode==
				SysCtrl.photo_mode_switch=1;
				SysCtrl.spec_color_index=5;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
		}
		else if(3==SysCtrl.photo_mode_switch)	// frame mode
		{
			if(SysCtrl.photo_frame_idx>0)
			{
				SysCtrl.photo_frame_idx--;
				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
			else	
			{
				//==close frame==
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				hal_custom_frame_unit();
				//SysCtrl.photo_frame_idx = ICON_FRAME_NUM -1;

				// switch effect mode 
				SysCtrl.photo_mode_switch=2;
				SysCtrl.photo_effect_idx=CAM_EFFECT_MAX-2;
				
				//reset crop
				/*SysCtrl.crop_level =0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				photoZoomRatioShow(handle);
				*/
			}
		}
		if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8)){
			if(SysCtrl.crop_level != 0){
			    SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
				hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				photoZoomRatioShow(handle);
			}
		}

		photo_mode_show(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		deg_Printf("S2222ysCtrl.photo_mode_switch:=%d=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);
		if(photoLongKeyFlag == 0)
		{
			photoLongKeyFlag = 1;
		}

	/*	if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		SysCtrl.crop_level+=1;
	    if(SysCtrl.crop_level > 15)
			SysCtrl.crop_level=15;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);  */
		if((SysCtrl.photo_mode_switch==2 )|| (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		deg_Printf("before crop:%d\t",SysCtrl.crop_level);
		SysCtrl.crop_level+=1;
	#ifdef PHOTO_ZOOM_RATIO_16		
		if(SysCtrl.crop_level > 15){
			SysCtrl.crop_level=15;
		}
	#else
		if(SysCtrl.crop_level > 5){
			SysCtrl.crop_level=5;
		}	
	#endif
		deg_Printf("aftercrop:%d\t",SysCtrl.crop_level);
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}
	
	return 0;
}

static int photoKeyMsgDown3(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW)
		{
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}
		
		if(photoLongKeyFlag == 1)
		{
			photoLongKeyFlag = 0;
			return 0;
		}

		if(0==SysCtrl.photo_mode_switch)	// photo mode 
		{
			if(SysCtrl.photo_mode_idx<6)	// 8next photo mode 
			{
				if(1)//SysCtrl.printer_en
				{
					//if(SysCtrl.photo_mode_idx == 7)						//	swith spec color mode
					//{
						//==close photo mode show==
						SysCtrl.photo_mode_idx=0;
						
						SysCtrl.photo_mode_switch=1;
						SysCtrl.spec_color_index=1;
						cmos_spec_color(SysCtrl.spec_color_index);
					//}
					//else
					//	SysCtrl.photo_mode_idx = 7;
				}
				else
				{
					SysCtrl.photo_mode_idx++;
				}
			}
			else							//	swith spec color mode
			{
				//==close photo mode show==
				SysCtrl.photo_mode_idx=0;
				
				SysCtrl.photo_mode_switch=1;
				SysCtrl.spec_color_index=1;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
		}
		else if(1==SysCtrl.photo_mode_switch)	// spec col mode
		{
			if(SysCtrl.spec_color_index<5)
			{
				SysCtrl.spec_color_index++;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
			else	// switch effect mode
			{
				//==normal color==
				SysCtrl.spec_color_index=0;
				cmos_spec_color(SysCtrl.spec_color_index);
				//==
				SysCtrl.photo_mode_switch=2;
				SysCtrl.photo_effect_idx=0;

				deg_Printf("photo_mode_switch=%d,idx=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_effect_idx);
			}
		}
		else if(2==SysCtrl.photo_mode_switch)	// effect mode
		{
			if(SysCtrl.photo_effect_idx<CAM_EFFECT_MAX-2)
			{
				
				{
					//SysCtrl.photo_effect_idx++;
					if(SysCtrl.photo_effect_idx == 0)
						SysCtrl.photo_effect_idx = 1;
					else if(SysCtrl.photo_effect_idx == 1)
						SysCtrl.photo_effect_idx = 4;
					else if(SysCtrl.photo_effect_idx == 4)
						//SysCtrl.photo_effect_idx = 5;
						SysCtrl.photo_effect_idx = 6;
					else if(SysCtrl.photo_effect_idx == 5)
						SysCtrl.photo_effect_idx = 6;
					//else if(SysCtrl.photo_effect_idx == 6)
					//	SysCtrl.photo_effect_idx = 7;

				}

			}
			else
			{
				//==close effect==
	
				//==switch frame mode==
				SysCtrl.photo_mode_switch=3;

				//reset crop level , don't set this when frame mode and effect mode 
				/*SysCtrl.crop_level =0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				photoZoomRatioShow(handle);
				*/
				
				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);



				SysCtrl.photo_frame_idx = 0;
				deg_Printf("frame_index:%d\n",SysCtrl.photo_frame_idx);

				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
		}
		else if(3==SysCtrl.photo_mode_switch)	// frame mode
		{
			if(SysCtrl.photo_frame_idx<ICON_FRAME_NUM-1)
			{
				SysCtrl.photo_frame_idx++;
				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
			else	
			{
				//==close frame==
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				hal_custom_frame_unit();
				// switch photo mode 
				SysCtrl.photo_mode_switch=0;
				SysCtrl.photo_mode_idx=0;
			}
		}
		
		if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8)){
			if(SysCtrl.crop_level != 0){
			    SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
				hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				photoZoomRatioShow(handle);
			}
		}

		photo_mode_show(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		if(photoLongKeyFlag == 0)
		{
			photoLongKeyFlag = 1;
		}
		/*if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);*/
		if(((SysCtrl.photo_mode_switch==2 )) || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		deg_Printf("before crop:%d\t",SysCtrl.crop_level);
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		deg_Printf("after crop:%d\n",SysCtrl.crop_level);
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}

	return 0;
}
#endif


extern menu MENU(movie);
#if 0
static int photoKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uiOpenWindow(&menuItemWindow,1,&MENU(movie));

	}
	return 0;
}
#endif


static int photoKeyMsgPrinterEn(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	deg_Printf("msg ok photo_mode_switch:%d;photo_mode_idx:%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);
	
	/*{
		u32 start_time1,printer_delay_time1,space_time1;
		start_time1 = XOSTimeGet();
		XOSTimeDly(100);
		hal_wdtClear();
		printer_delay_time1 = 3;
		photoPrintDlyShow(handle,FALSE,printer_delay_time1);
		XOSTimeDly(1000);
		while(1)
		{
			hal_wdtClear();
			space_time1 = XOSTimeGet()-start_time1;
			if(space_time1 >= 1000)
			{
				printer_delay_time1--;
				photoPrintDlyShow(handle,FALSE,printer_delay_time1);
				if(0 == printer_delay_time1)
				{
					photoPrintDlyShow(handle,false,0);
					break;
				}
				start_time1 = XOSTimeGet();
			}
		}
	}*/
#if 1
		if(SysCtrl.printer_en)
		{
			SysCtrl.printer_en = 0;
			configSet(CONFIG_ID_PRINTER_EN,R_ID_STR_COM_OFF);
		}
		else
		{
			SysCtrl.printer_en = 1;
			configSet(CONFIG_ID_PRINTER_EN,R_ID_STR_COM_ON);
			if(0==SysCtrl.photo_mode_switch)	// photo mode
			{
				if(SysCtrl.photo_mode_idx>0)
				{
					SysCtrl.photo_mode_idx=0;
				}
			}
		}
					
		
		userConfigSave();
		
		photoPrinterShow(handle);
		photo_mode_show(handle);
		
#endif
		
	}
	return 0;
}

static int photoSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoSDShow(handle);
	photoRemainNumShow(handle);
	photoSurplusQuantityShow(handle);
	if(SysCtrl.sdcard == SDC_STAT_NULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_NOT_INSERT,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_FULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_FULL,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_ERROR)
		uiOpenWindow(&tips1Window,2,TIPS_SD_ERROR,SDC_NULL_TIMEOUT);
	deg_Printf("SD state:%d\n",SysCtrl.sdcard);
	return 0;
}
static int photoSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoBaterryShow(handle);
	return 0;
}
static int photoSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		photoBaterryShow(handle);
	return 0;
}
static int photoSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

extern bool key_focus_model;
static int photoOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo Open Win!!!\n");
		SysCtrl.photo_focus=PHOTO_FOCUS_ICON_NONE;
	photoFocusShow(handle);
	layout_version_get();
//	#if SMALL_PANEL_SUPPORT==0
//	photoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif

	photoPrinterTipsShow(handle,false,0,0);

	photoResolutionShow(handle);
	photoMDShow(handle);
	photoMonitorShow(handle);
	photoIrLedShow(handle);
	photoLockShow(handle,false);
	photoSDShow(handle);
	photoMicShow(handle);
	photoBaterryShow(handle);
	photoRemainNumShow(handle);
#ifdef USB_TIMEMOREPHOTO_INSETTING
	photo_timephoto(handle,true);
	photo_morephoto(handle,true);
#endif
	photo_mode_show(handle);

	photo_time_num_show(handle,false,0);
	
	photoPrinterShow(handle);

	photoSurplusQuantityShow(handle);

	//photoPrintDlyShow(handle,false,0);

	key_focus_model = true;
	//SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
	//photoFocusShow(handle);
	SysCtrl.photo_focus=PHOTO_FOCUS_ICON_NONE;
	photoFocusShow(handle);
	focus_sound = layout_sound_load(RES_MUSIC_PHOTO_FOCUS, &focus_sound_size);

	photoIRShow(handle);

	#if defined(USE_FLASH_SET)||defined(USE_BOARD_DY152) 
	if(senser_select == 0)
		PhotoFLASHShow(handle,true);
	else
		PhotoFLASHShow(handle,false);
	#endif

	#ifdef ADD_PRINT_PAPER_TYPE
		PhotoPinrtPaperShow(handle,true);
	#endif
	
	#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
	if(SysCtrl.printer_en == 1)
		PhotoPrinterModeShow(handle,true);
	else
		PhotoPrinterModeShow(handle,false);
	#endif

/*
	if(0==configGet(CONFIG_ID_LANGUAGE))
	{
		SysCtrl.first_menu_lan=1;
		uiOpenWindow(&menuItemWindow,2,&MENU(movie),8);// language idx
	}
*/
	if(0==SysCtrl.cartoon_mode)
	{
		SysCtrl.cartoon_mode=1;
		SysCtrl.cartoon_show_cnt=0;
	}

	return 0;
}
static int photoCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo Close Win!!!\n");
	#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
	PhotoCloseFlash();
	#endif
	boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
	key_focus_model = false;
	if(focus_sound){
		hal_sysMemFree(focus_sound);
		focus_sound = 0;
	}
	hal_custom_frame_unit();
	return 0;
}
static int photoWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo WinChild Close!!!\n");
	photoRemainNumShow(handle);
	photoResolutionShow(handle);
	photoMDShow(handle);
	photoMonitorShow(handle);
	photoIrLedShow(handle);
	photoLockShow(handle,false);
	photoSDShow(handle);
	photoMicShow(handle);
	photoBaterryShow(handle);
#ifdef USB_TIMEMOREPHOTO_INSETTING
	photo_timephoto(handle,true);
	photo_morephoto(handle,true);
#endif
	photo_time_num_show(handle,false,0);
	photoPrinterTipsShow(handle,false,0,0);
	photoPrinterShow(handle);
//	#if SMALL_PANEL_SUPPORT==0
//	photoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif

	return 0;
}

#if 0
static void VotageShow(winHandle handle)
{

	static char picSumStr[2];
		u8 temp = hal_gpioRead(GPIO_PA, GPIO_PIN7);
		picSumStr[0] = ((temp)%10)+'0';
		picSumStr[1] = 0;
		//intToString(&picSumStr,5,/*MANAGER_LIST_MAX-*/picSum);
		winSetResid(winItem(handle,TEST_VOTAGE_ID),strToResId(picSumStr));

}
#endif

static void PhotoSysMsg20A6CamSwitch(void)//3A03 switch cam
{    
    int i;
	unsigned char buf[2];
	
	//app_lcdCsiVideoShowStop();
	dispLayerSetPIPMode(DISP_PIP_DISABLE);	
	sensor_iic_enable();
	//buf[0] = 0x12;//0xfe
	//buf[1] = 0x00;
	//sensor_iic_write(buf);

	buf[0] = 0x4a;//0x17;
	if(senser_select==0)
	{
		//senser_select = 1;
		buf[1] = 0x0C;///0x08;
	}
	else
	{
		//senser_select = 0;
		buf[1] = 0x04;//0x40;//0x0C;///0x40;
	}

	sensor_iic_write(buf);
	sensor_iic_disable();
	XOSTimeDly(350);
	dispLayerSetPIPMode(SysCtrl.pip_mode);
	//app_lcdCsiVideoShowStart(LCDSHOW_WIN_CSI_BOT);
}

static int photoSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
//	static uint32 flag=0;
	uint32 ret1 = 0;

	u8 timestramp;
	
	timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);
	if(timestramp == 1 )
	{
		winSetResid(winItem(handle,PHOTO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
		winSetVisible(winItem(handle,PHOTO_SYSTIME_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_SYSTIME_ID),false);
	}	
	
	//photoIrLedShow(handle);
//  VotageShow(handle);
//	photo_timephoto(handle,true);
//	photo_morephoto(handle,true);
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		photoBaterryShow(handle);	
	}
	/*static u8 printer_en_flag = 0;
	printer_en_flag++;
	if(printer_en_flag == 10){
		printer_en_flag = 0;
		if(SysCtrl.printer_en)
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_PHOTO,0));
	}*/
	#ifdef USE_HALL_LCD_FLIP
		if(senser_select==0){
			if(lcd_flip==1){				
				senser_select = 1;
				PhotoSysMsg20A6CamSwitch();
			}
		}else if(senser_select==1){
			if(lcd_flip==0){				
				senser_select = 0;
				PhotoSysMsg20A6CamSwitch();
			}
		}
	#endif
		
	#if defined(USE_BOARD_DY128)
		hal_gpioInit(GPIO_PE,GPIO_PIN0,GPIO_INPUT,GPIO_PULL_UP);//GPIO_PULL_DOWN
		ret1 = hal_gpioRead(GPIO_PE,GPIO_PIN0);
	
		//para[0] = KEY_PRESSED;
		//para[1] = KEY_PRESSED;
		uart_Printf("%d ,%d \n",senser_select,ret1);
		if(senser_select==0){
			if(ret1==0){
				//recordPhotoKeyMsgMode(NULL,1,para);
				senser_select = 1;
				//recordPhotoSysMsg0328CamSwitch();
				//#if defined(J599_USB_SENSOR_3A03)||defined(DY27_USB_SENSOR_3A03)
				//	PhotoSysMsg3A03CamSwitch();
				//#elif defined(J599_USB_SENSOR_20A6)
					PhotoSysMsg20A6CamSwitch();
				///#endif
			}
		}else if(senser_select==1){
			if(ret1==1){
				//recordPhotoKeyMsgMode(NULL,1,para);
				senser_select = 0;
				//recordPhotoSysMsg0328CamSwitch();
				//PhotoSysMsg3A03CamSwitch();
				//#if defined(J599_USB_SENSOR_3A03)||defined(DY27_USB_SENSOR_3A03)
				//	PhotoSysMsg3A03CamSwitch();
				//#elif defined(J599_USB_SENSOR_20A6)
					PhotoSysMsg20A6CamSwitch();
				//#endif
			}
		}
	#endif
	drawUIService(true);

	return 0;
}

#ifdef USE_BOARD_DY42
static int photoKeyMsgRotate1(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uart_Printf("photoKeyMsgRotate1 \n");
		if(deal_photoApp_fstMsg == 0) 
		{
			deal_photoApp_fstMsg = 1;
			ax32xx_VDDGSENEnable(1);
		}
		else
			deamon_keysound_play();
		hal_custom_frame_add_enable(1);
		videoRecordCmdSet(CMD_COM_FRAME,1);
		if(hal_custom_frame_create(R_FRAME[0])<0)
		{
			deg_Printf(">>frame err!!<<\n");
		}
	}
	return 0;
}

static int photoKeyMsgRotate2(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uart_Printf("photoKeyMsgRotate2 \n");
		if(deal_photoApp_fstMsg == 0) 
		{
			deal_photoApp_fstMsg = 1;
			ax32xx_VDDGSENEnable(1);
		}
		else
			deamon_keysound_play();
		hal_custom_frame_add_enable(1);
		videoRecordCmdSet(CMD_COM_FRAME,1);
		if(hal_custom_frame_create(R_FRAME[1])<0)
		{
			deg_Printf(">>frame err!!<<\n");
		}
	}
	return 0;
}


static int photoKeyMsgRotate3(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uart_Printf("photoKeyMsgRotate3 \n");
		if(deal_photoApp_fstMsg == 0) 
		{
			deal_photoApp_fstMsg = 1;
			ax32xx_VDDGSENEnable(1);
		}
		else
			deamon_keysound_play();
		hal_custom_frame_add_enable(1);
		videoRecordCmdSet(CMD_COM_FRAME,1);
		if(hal_custom_frame_create(R_FRAME[2])<0)
		{
			deg_Printf(">>frame err!!<<\n");
		}
	}
	return 0;
}

static int photoKeyMsgRotate4(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uart_Printf("photoKeyMsgRotate4 \n");
		if(deal_photoApp_fstMsg == 0) 
		{
			deal_photoApp_fstMsg = 1;
			ax32xx_VDDGSENEnable(1);
		}
		else
			deamon_keysound_play();
		hal_custom_frame_add_enable(1);
		videoRecordCmdSet(CMD_COM_FRAME,1);
		if(hal_custom_frame_create(R_FRAME[3])<0)
		{
			deg_Printf(">>frame err!!<<\n");
		}
	}
	return 0;
}

static int photoKeyMsgRotate5(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uart_Printf("photoKeyMsgRotate5 \n");
		if(deal_photoApp_fstMsg == 0) 
		{
			deal_photoApp_fstMsg = 1;
			ax32xx_VDDGSENEnable(1);
		}
		else
			deamon_keysound_play();
		hal_custom_frame_add_enable(1);
		videoRecordCmdSet(CMD_COM_FRAME,1);
		if(hal_custom_frame_create(R_FRAME[4])<0)
		{
			deg_Printf(">>frame err!!<<\n");
		}
	}
	return 0;
}

static int photoKeyMsgRotate6(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uart_Printf("photoKeyMsgRotate6 \n");
		if(deal_photoApp_fstMsg == 0) 
		{
			deal_photoApp_fstMsg = 1;
			ax32xx_VDDGSENEnable(1);
		}
		else
			deamon_keysound_play();
		hal_custom_frame_add_enable(1);
		videoRecordCmdSet(CMD_COM_FRAME,1);
		if(hal_custom_frame_create(R_FRAME[5])<0)
		{
			deg_Printf(">>frame err!!<<\n");
		}
	}
	return 0;
}

static int photoKeyMsgRotate7(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uart_Printf("photoKeyMsgRotate7 \n");
		if(deal_photoApp_fstMsg == 0) 
		{
			deal_photoApp_fstMsg = 1;
			ax32xx_VDDGSENEnable(1);
		}
		else
			deamon_keysound_play();
		hal_custom_frame_add_enable(1);
		videoRecordCmdSet(CMD_COM_FRAME,1);
		if(hal_custom_frame_create(R_FRAME[6])<0)
		{
			deg_Printf(">>frame err!!<<\n");
		}
	}
	return 0;
}

static int photoKeyMsgRotate8(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uart_Printf("photoKeyMsgRotate8 \n");
		if(deal_photoApp_fstMsg == 0) 
		{
			deal_photoApp_fstMsg = 1;
			ax32xx_VDDGSENEnable(1);
		}
		else
			deamon_keysound_play();
		hal_custom_frame_add_enable(1);
		videoRecordCmdSet(CMD_COM_FRAME,1);
		if(hal_custom_frame_create(R_FRAME[7])<0)
		{
			deg_Printf(">>frame err!!<<\n");
		}
	}
	return 0;
}

static int photoKeyMsgRotate9(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uart_Printf("photoKeyMsgRotate9 \n");
		if(deal_photoApp_fstMsg == 0) 
		{
			deal_photoApp_fstMsg = 1;
			ax32xx_VDDGSENEnable(1);
		}
		else
			deamon_keysound_play();
		//==close frame==
		videoRecordCmdSet(CMD_COM_FRAME,0);
		hal_custom_frame_add_enable(0);
		hal_custom_frame_unit();
	}
	return 0;
}
#endif

msgDealInfor photoEncodeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,photoOpenWin},
	{SYS_CLOSE_WINDOW,photoCloseWin},
	{SYS_CHILE_COLSE,photoWinChildClose},

	#if defined(USE_BOARD_DY05)
	{KEY_EVENT_LEFT,photoKeyMsgUp},
	{KEY_EVENT_RIGHT,photoKeyMsgDown},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_MODE,photoKeyMsgSensorChange},
	{KEY_EVENT_PHOTO,photoKeyMsgPrintMode},
	{KEY_EVENT_POWER, PhotoKeyMsgFlash},
	#elif defined(USE_BOARD_DY20)
	{KEY_EVENT_LEFT,photoKeyMsgUp},
	{KEY_EVENT_RIGHT,photoKeyMsgDown},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_PHOTO,photoKeyMsgPrintMode},
	{KEY_EVENT_POWER, PhotoKeyMsgFlash},
	#ifdef USE_DY20_DOUBLE_SENSOR
	{KEY_EVENT_DELETE,photoKeyMsgSensorChange},
	#endif
	#elif defined(USE_BOARD_DY26)
	{KEY_EVENT_LEFT,photoKeyMsgUp},
	{KEY_EVENT_RIGHT,photoKeyMsgDown},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_PHOTO,photoKeyMsgPrintMode},
	{KEY_EVENT_OK, PhotoKeyMsgFlash},
	#elif defined(USE_BOARD_DY42)
	{KEY_EVENT_OK, photoKeyMsgPhoto},
	{KEY_EVENT_MENU,PhotoKeyMsgFlash},
	{KEY_EVENT_POWER,photoKeyMsgSwitchDensity},
	{KEY_EVENT_ROTATE1,photoKeyMsgRotate1},
	{KEY_EVENT_ROTATE2,photoKeyMsgRotate2},
	{KEY_EVENT_ROTATE3,photoKeyMsgRotate8},
	{KEY_EVENT_ROTATE4,photoKeyMsgRotate4},
	{KEY_EVENT_ROTATE5,photoKeyMsgRotate5},
	{KEY_EVENT_ROTATE6,photoKeyMsgRotate6},
	{KEY_EVENT_ROTATE7,photoKeyMsgRotate7},
	{KEY_EVENT_ROTATE8,photoKeyMsgRotate9},//kong
	{KEY_EVENT_ROTATE9,photoKeyMsgRotate3},
	#elif defined(USE_BOARD_DY43)
	{KEY_EVENT_LEFT,photoKeyMsgUp3},
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_MODE,photoKeyMsgMenu},
	{KEY_EVENT_OK,photoKeyMsgPlayBack},
	//{KEY_EVENT_MODE,photoKeyMsgPlayBack},
	//{KEY_EVENT_DELETE,photoKeyMsgSensorChange},
	#elif defined(USE_BOARD_DY151)	
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_LEFT,photoKeyMsgUp3},
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	#elif defined(USE_BOARD_DY152)	
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_LEFT,photoKeyMsgUp3},
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_MENU,photoKeyMsgMenu},
	{KEY_EVENT_PLAYBACK,photoKeyMsgPrinterEn},
	{KEY_EVENT_POWER,PhotoKeyMsgFlash},
	{KEY_EVENT_SENSOR_CHANGE,photoKeyMsgSensorChange},
	#elif defined(USE_BOARD_DY45)
	{KEY_EVENT_LEFT,photoKeyMsgUp3},
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	#elif defined(USE_BOARD_DY46)
	{KEY_EVENT_LEFT,photoKeyMsgUp3},
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_MODE,photoKeyMsgPlayBack},
	{KEY_EVENT_PHOTO,photoKeyMsgPrinterEn},
	#elif defined(USE_BOARD_DY49)
	{KEY_EVENT_LEFT,photoKeyMsgUp3},
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_OK,photoKeyMsgPhoto},
	#elif defined(USE_BOARD_DY16)||defined(USE_BOARD_J589)
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_LEFT,photoKeyMsgUp3},
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},
	#elif defined(USE_BOARD_DY116)
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_LEFT,photoKeyMsgUp3},
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},
	{KEY_EVENT_OK,PhotoKeyOkMsgFlash},
	#elif defined(USE_BOARD_DY79)
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_LEFT,photoKeyMsgUp3},//photoKeyMsgUp3
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},//photoKeyMsgDown3	
	#ifdef USE_FLASH_SET
	{KEY_EVENT_OK,PhotoKeyOkMsgFlash},
	#endif
	#elif defined(USE_BOARD_DY128)
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_LEFT,photoKeyMsgUp3},
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},
	{KEY_EVENT_OK,photoKeyMsgPrinterEn},
	#else
	{KEY_EVENT_PHOTO,photoKeyMsgPhoto/*photoKeyMsgPrinterEn*/},//设置打印按键
	{KEY_EVENT_SENSOR_CHANGE,photoKeyMsgSensorChange},
	{KEY_EVENT_UP,photoKeyMsgUp2},
	{KEY_EVENT_DOWN,photoKeyMsgDown2},
	{KEY_EVENT_MENU,photoKeyMsgMenu},
	{KEY_EVENT_MODE,photoKeyMsgPlayBack},
	{KEY_EVENT_LEFT,photoKeyMsgUp3},
	{KEY_EVENT_RIGHT,photoKeyMsgDown3},
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	{KEY_EVENT_OK,photoKeyMsgPhoto},//photoKeyMsgPrinterEn photoKeyMsgIr
	{KEY_EVENT_PLAYBACK,photoKeyMsgPlayBack},
	//{KEY_EVENT_MENU,photoKeyMsgMenu},
	//{KEY_EVENT_ROTATE,photoKeyMsgSensorRotate},
	//{KEY_EVENT_IR,photoKeyMsgIr},
	#endif

	{SYS_EVENT_SDC,photoSysMsgSD},
	{SYS_EVENT_USB,photoSysMsgUSB},
	{SYS_EVENT_BAT,photoSysMsgBattery},
	{SYS_EVENT_MD,photoSysMsgMD},
	{SYS_EVENT_1S,photoSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(photoEncodeWindow,photoEncodeMsgDeal,photoEncodeWin)



