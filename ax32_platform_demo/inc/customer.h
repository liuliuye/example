/****************************************************************************
**
 **                              CUSTOMER
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  CUSTOMER HEADER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : customer.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is customer define 
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  CUSTOMER_H
    #define  CUSTOMER_H

//#define USE_BOARD_DY05
//#define USE_BOARD_DY06
//#define USE_BOARD_DY19
//#define USE_BOARD_DY20
//#define USE_BOARD_DY26
//#define USE_BOARD_DY42 //sensor = 20A6
//#define USE_BOARD_DY43
//#define USE_BOARD_DY45
//#define USE_BOARD_DY46
//#define USE_BOARD_DY49 //rtc battery no debug

//#define USE_BOARD_DY16//Q5/dy127-new
//#define USE_BOARD_DY79
//#define USE_BOARD_J589//Q6
//#define USE_BOARD_DY116
//#define USE_BOARD_DY128
//#define USE_BOARD_DY151
#define USE_BOARD_DY152

#ifdef USE_BOARD_DY05
#define USB_PWM_LCDBK
#define USE_PRINTER_DZ_MUSIC
//#define DY05_OPOS_PRINTHEAD_3_7V//OPOS=沃博思打印头
#define DY05_OPOS_PRINTHEAD_7_4V
#endif

#if defined(USE_BOARD_DY06)
//#define DOUBLE_CAM
//#define PRINTHEAD_3_7V
#endif

#ifdef USE_BOARD_DY19
//#define DOUBLE_CAM
#define DY19_PRINTHEAD_7_4V
//#define USE_DY19_OLDUI
//#define USE_MMENU_JPG_MODE
#endif

#ifdef USE_BOARD_DY42
#define  LCD_BK_CH		 GPIO_PA
#define  LCD_BK_PIN 	 GPIO_PIN9 //board_bwv1.c 里也有这个定义，保持一致
#define  DY42_PRINTHEAD_7_4V
#endif

#ifdef USE_BOARD_DY49
#define DY19_PRINTHEAD_7_4V
//#define DOUBLE_CAM
#endif

#ifdef USE_BOARD_DY20
#define USB_PWM_LCDBK
#define DY20_OPOS_PRINTHEAD_7_4V //OPOS=沃博思打印头
//#define USE_DY20_DOUBLE_SENSOR
#define USE_PRINTER_DZ_MUSIC
#endif

#ifdef USE_BOARD_DY26
//#define DOUBLE_CAM
#define USB_PWM_LCDBK
#define USE_PRINTER_DZ_MUSIC
#define DY26_OPOS_PRINTHEAD_7_4V //OPOS=沃博思打印头
#endif


#if defined(USE_BOARD_DY43)
#define USE_VGA_STYLE_MUSIC_UI
#define DY43_PRINTHEAD_3_7V
#define USE_CHARGE_NO_POWERON	//充电时禁止使用
#endif

#if defined(USE_BOARD_DY151)
#define USE_VGA_STYLE_MUSIC_UI
#define DOUBLE_CAM
#define DY43_PRINTHEAD_3_7V
#define USE_CHARGE_NO_POWERON	//充电时禁止使用
#endif

#if defined(USE_BOARD_DY152)
#define USE_VGA_STYLE_MUSIC_UI
#define DY152_PRINTHEAD_7_4V
#define USE_CHARGE_NO_POWERON	//充电时禁止使用
#define USE_HALL_LCD_FLIP		//翻转屏2026-03-07 10:10:44
#endif

#ifdef USE_BOARD_DY45
#define USE_VGA_STYLE_MUSIC_UI
#define USB_REDBATTARY_PIC //[1格电量改为红色-充电是绿色]
#define USE_STR_TIPS_LOWPWR_PRINT1_PRINT2 //为保证打印质量,请充电 提示
#define USE_DY45_NEWUI
#define USB_LCD_ST7789P3BOE2_0IPS //ST7789P3+BOE2.0IPS(B3+GV020QVQ-N81-DQP0)-2.2Gamma-20230515

#define USE_APP_PRINTER
#ifdef USE_APP_PRINTER
#define USE_BT_CONTROL_LED//蓝牙控制流水灯
#else
#define USE_5050IC_CONTROL_LED//不带蓝牙
#endif
#define USE_PRINTER_7_4_SPEED//优化高压头出纸慢
#define DY45_PRINTHEAD_7_4V
#endif

#ifdef USE_BOARD_DY16
#define USE_VGA_STYLE_MUSIC_UI
#define USB_REDBATTARY_PIC 		//[1格电量改为红色-充电是绿色]
#define USE_STR_TIPS_LOWPWR_PRINT1_PRINT2 //为保证打印质量,请充电 提示
#define PHOTO_ZOOM_RATIO_16
//#define DOUBLE_CAM			//open it is double camera /特效切

//#define USE_CHARGE_NO_POWERON	//充电时禁止使用
//#define USE_CZECH_LANGUAGE	//客户定制-默认捷克语(英、捷克、德、波、斯洛伐克、匈牙利),修改捷克语翻译,2025-05-17 16:21:26
//#define USE_STICKER_PRTPAPER  //增加兼容不干胶的打印浓度，不干胶打印要浓一点
//#define ADD_BK_POWERON_DELAY	//开机背光增加500ms延时2025-06-24 15:44:59
//#define USE_THREE_LANUGAE		//中性希伯来语定制,只保留英语、希伯来语、阿拉伯语2025-09-19 10:10:25

//#define USE_LCD_9341C			//点新屏，挂在7789下面
//#define TESE_SMALLE_LCD

#define USE_TEST_FCC 0			//FCC辐射过认证
#define USE_CSI_DETECT_DEATH	//针对拍照和录像静电处理，卡死后重启并记录卡死前app状态	

//#define USE_DIE_MAUS_UI			//客定UI(主界面去掉游戏，换开关机logo和音乐、大头贴,del滤镜、五国语言)，2025-11-04 16:33:33

//#define USE_NEW_3A03_SENSOR
//#define USE_INDIA_UI
//#define USE_KOERA_UI
#ifdef USE_KOERA_UI
#define SOFTWARE_VERSION1	"R-R-al7-ATG100"
#endif

#define USE_PB6_CONTROL_PRINT_PWRD

//#define USE_APP_PRINTER
#ifdef USE_APP_PRINTER
#define USE_BT_CONTROL_LED//蓝牙控制流水灯
#else
#define USE_5050IC_CONTROL_LED//不带蓝牙
#endif

//#define USE_PRINTER_7_4_SPEED//优化高压头出纸慢
#define DY16_LY_PRINTHEAD_7_4V
//#define DY16_LY_PRINTHEAD_3_7V
#endif

#ifdef USE_BOARD_DY128
#define USE_VGA_STYLE_MUSIC_UI
#define USB_REDBATTARY_PIC 		//[1格电量改为红色-充电是绿色]
#define USE_STR_TIPS_LOWPWR_PRINT1_PRINT2 //为保证打印质量,请充电 提示
#define PHOTO_ZOOM_RATIO_16
//#define USE_CHARGE_NO_POWERON	//充电时禁止使用

#define USE_TEST_FCC 0			//FCC辐射过认证
//#define USE_CSI_DETECT_DEATH	//针对拍照和录像静电处理，卡死后重启并记录卡死前app状态	
//#define USE_APP_PRINTER
#define DY128_LY_PRINTHEAD_7_4V
#endif


#ifdef USE_BOARD_J589
#define USE_VGA_STYLE_MUSIC_UI
#define USB_REDBATTARY_PIC //[1格电量改为红色-充电是绿色]
#define USE_STR_TIPS_LOWPWR_PRINT1_PRINT2 //为保证打印质量,请充电 提示
//#define DOUBLE_CAM//open it is double camera /特效切
//#define USE_CHARGE_NO_POWERON	//充电时禁止使用
//#define USE_DISNEY_UI

#define USE_TEST_FCC 0//FCC辐射过认证
//#define USE_COLOR_PAPER//彩色打印纸效果

//#define USE_APP_PRINTER
#ifdef USE_APP_PRINTER
#define USE_BT_CONTROL_LED//蓝牙控制流水灯
#else
#define USE_GPIO_CONTROL_LED//不带蓝牙
#endif

//#define USE_PRINTER_7_4_SPEED//优化高压头出纸慢
#define J589_LY_PRINTHEAD_7_4V
#endif

#ifdef USE_BOARD_DY79
#define USE_VGA_STYLE_MUSIC_UI
#define USB_REDBATTARY_PIC //[1格电量改为红色-充电是绿色]
#define USE_STR_TIPS_LOWPWR_PRINT1_PRINT2 //为保证打印质量,请充电 提示
//#define DOUBLE_CAM//open it is double camera /特效切
#define PHOTO_ZOOM_RATIO_16

#define USB_TIMEMOREPHOTO_INSETTING//连拍和定时拍在菜单设置(2025-06-10 15:30:31)
#define USB_SHOW_SUBMENU_OPT //菜单设置界面右边每行加子菜单选项显示
//#define ADD_PRINT_PAPER_TYPE //增加打印纸选项2025-06-16 14:24:28
//#define	USE_DEFAULT_PAEPR 1	 //默认纸；0-黑白，1-彩色

//#define USE_KOERA_NEW_UI
//#define USE_DISNEY_UI	//艾莎公主/草莓熊-LOGO 大头贴
#ifdef USE_DISNEY_UI
#define USE_CHARGE_NO_POWERON//充电时禁止使用
#endif

//#define USE_APP_PRINTER
#define USE_FLASH_SET//闪光灯低使能有效
#define USE_PRINTER_7_4_SPEED//优化高压头出纸慢
#define DY79_LY_PRINTHEAD_7_4V
#endif

#ifdef USE_BOARD_DY116
#define USE_VGA_STYLE_MUSIC_UI
#define USB_REDBATTARY_PIC //[1格电量改为红色-充电是绿色]
#define USE_STR_TIPS_LOWPWR_PRINT1_PRINT2 //为保证打印质量,请充电 提示
#define PHOTO_ZOOM_RATIO_16
#define USB_SHOW_SUBMENU_OPT //菜单设置界面右边每行加子菜单选项显示
#define USB_TIMEMOREPHOTO_INSETTING//连拍和定时拍在菜单设置(2025-06-10 15:30:31)
//#define USE_CHARGE_NO_POWERON	//充电时禁止使用
//#define USE_DISNEY_FRAME_UI		//迪士尼大头贴

//#define USE_LCD_9341C	//屏蔽则是7789
//#define USE_COLOR_PAPER//彩色打印纸效果
#define USE_FLASH_SET//闪光灯低使能有效
//#define DEFAULT_KEYSOUND_OFF//按键音默认关

//#define USE_APP_PRINTER
#ifdef USE_APP_PRINTER
#define BT_CONTROL_PRINTSTB
#endif

//#define USE_PRINTER_7_4_SPEED//优化高压头出纸慢
#define DY116_LY_PRINTHEAD_7_4V
#endif

#if defined(USE_BOARD_DY46)
//#define DOUBLE_CAM
#define PRINTHEAD_3_7V
#define USB_PWM_LCDBK
#endif

#define PRINTTEMP_TEST
//#define FIT_FLASH_ON_RTCBAT_ERR//针对录像模式闪光灯常亮导致电池显示掉电做优化(1格电时会导致显示2格电)
#define SENSORBF3A03_ZLKC
#define SOLVE_PRINTHEAD_POWERON_HEAT
#define POWER_ON_DELAY_SHOWBAT //开机进入主界面延时显示电量图标，为了显示真实电量2025-12-10
//#define USE_MMENU_JPG_MODE

#define MXWAPP_PRINTER_SUPPORT 1//HAL_MXWAPP_PRINTER_SUPPORT also open
#define UART_TX_DEBUG 0

#ifdef USE_BOARD_DY16
#define BAT_CHECK_TPYE  0		//0:PE0	1:MVOUT
#define IO_PA7_NOT_INPUT 1		
#else
#define BAT_CHECK_TPYE  1		//0:PE0	1:MVOUT
#define IO_PA7_NOT_INPUT 1		//PA7不复用打开2025-12-09 19:33:30
#endif

#define USE_DEFAULT_ENGLISH
//#define USE_DEFAULT_RUSSIAN
//#define USE_DEFAULT_PORTUGUESE
//#define USE_DEFAULT_SPANISH
//#define USE_DEFAULT_FRECH
//#define USE_DEFAULT_POLISH
//#define USE_DEFAULT_HEBREW
//#define USE_DEFAULT_TAI
//#define USE_DEFAULT_TURKEY
//#define USE_DEFAULT_ARABIC
//#define USE_DEFAULT_KOERA
//#define USE_DEFAULT_DUTCH
//#define USE_DEFAULT_CZECH
//#define USE_DEFAULT_GERMAN

#if defined(USE_BOARD_DY116) 
#define  SOFTWARE_VERSION 	"5295_DY116_20260120_V1.7"
#elif defined(USE_BOARD_DY16)
#ifdef USE_DIE_MAUS_UI
#define  SOFTWARE_VERSION 	"DIE MAUS"
#else
#define  SOFTWARE_VERSION 	"5295_DY127_20260119_V1.0"
#endif
#elif defined(USE_BOARD_J589)
#define  SOFTWARE_VERSION 	"5295_Q6_20260121_V1.8"
#elif defined(USE_BOARD_DY79)
#define  SOFTWARE_VERSION 	"5295_DY79_20251018_V1.0"
#elif defined(USE_BOARD_DY128)
#define  SOFTWARE_VERSION 	"5295_DY128_20260120_V1.0"
#elif defined(USE_BOARD_DY43)
#define  SOFTWARE_VERSION 	"5295_DY43_20260128_V1.0"
#elif defined(USE_BOARD_DY151)
#define  SOFTWARE_VERSION 	"5295_DY151_20260226_V1.0"
#elif defined(USE_BOARD_DY152)
#define  SOFTWARE_VERSION 	"5295_DY152_20260416_V1.0"
#endif
#define  SYSTEM_VERSION     "3295B_DY06_20240419_V1.0" 

#define  BOARD_TYPE_BWV1          0
#define  BOARD_TYPE_FPGA          1

#define RECORD_MODE_DEFAULT       0  //  default mode,720P+VGA doule sensor.
#define RECORD_MODE_SIGNAL        1  //  only 720P
#define RECORD_MODE_AUTO          2  //  720P first.if VGA in ,creating B folder

//=====Printer Attribute================
#if defined(USE_BOARD_DY05)||defined(USE_BOARD_DY20)||defined(USE_BOARD_DY26)
#define     RESOLUTION		1   //choose sensor RESOLUTION . 1: 720P  , 0: VGA
#define 	PIC_16_9		1   //choose use VGA Sensor take photo 16:9.  	1:enable	, 0:disable
#else
#define     RESOLUTION		0
#define 	PIC_16_9		0
#endif
#define 	BAT_UDATE_RTIME	1	//choose bat level update real time . 1:enable update	,0:disable . {Description: when disable , -->
								//--> bat level only reduce not increase.bat detect only update when get a lower value.}
									
#define 	VOLTAGE			1 	//choose printer voltage . 1: 3.7V  , 0: 7.4V	
#define		WRITE_CARD_TEST	0	//only for test

#if (1== RESOLUTION)
#define RESOLUTION_720P
#else
#define RESOLUTION_VGA
#endif

/*#if (1==VOLTAGE)
#define  PRINTER_3_7V
#else
#define PRINTER_7_4V    
#endif*/

#if(1 == BAT_UDATE_RTIME)
#define BAT_UPDATE_RTIME_EN
#endif

#define  RECORD_MODE       RECORD_MODE_SIGNAL 


#define  SYSTEM_BOARD      BOARD_TYPE_BWV1//BOARD_TYPE_FPGA// BOARD_TYPE_BWV1     //----user configure


#define  VIDEO_RATIO       VIDEO_RATIO_DEFAULT     //----user configure


#define  VIDEO_ROTATE      VIDEO_ROTATE_DEFAULT   //----user configure


#define  BAT_SAVE_POWER     0 // second.when battery powered and no key event.0,disable power save


#define  SDC_NULL_TIMEOUT   2 // second .sdc null dialog showing time. 0,disable sdc null showing


//#define  POWER_ON_RECORD     1 // power on record if power_key power on.0:disable


#define  TAKE_PHOTO_MODE     0  // take photo from A and B once if B channel is valided .0:only A OR B


#define  MOTION_DEC_TIME    20 // second.when motion dection no active recording this time will stop recording


#define  KEYSOUND_VOLUME    100 // key & take photo sound volume [0-100]


#define  AVI_TYPE_ODML       1  // AVI FILE TYPE .1-open dml,0-std


#define  FILE_SYNC_WRITE    1  // write sync for avi file


#define  FILE_SYNC_DELETE   0  // delete sync 

#define  FILE_SIZE_PRE_MALLOC	0 // 0: file is not pre malloc , 1 : file is pre malloc(FILE MAX SIZE before REC)

#define  VERSION_LOAD_AUTO     0   // auto load version from resource


#define  DATETIME_LOAD_AUTO    1  // auto load default time from resource


#define  KEYSOUND_LOAD_AUTO    1  // auto load key sound from resource

#define  USENSOR_PWRCTRL_EN    1  // if enable,control usensor power by PE1;if disable,usensor power always power-on
								 // AX3282S define 1, AX3291A define 0
#define  USENSOR_AUDIO_ON      0 // back record audio volume on, use front record audio
#define  KEY_NUM			   5

#define  LCD_CROP_SCALE      1 //lcd crop scale show image ,  0 : is disable , 1 : is enable 
#define  LCD_PLAY_SCALE      1 //LCD show updown black for 4:3 LCD ,  0 : is disable , 1 : is enable 
#define  LCD_PLAY_JPEG_SCALE   0 //suport play jpeg file ,scale up or down ,  0 : is disable , 1 : is enable 


#define  USENSOR_RESOLUTION      0 //usensor recording resolution. 1: 640*480; 2: 1280:720; 3:1280:960

#define  SMALL_PANEL_SUPPORT		0//support lcd panel smaller than 320X240,remove sub menu on the right side && remove current time show on top bar and so on
									  //enable it when in need
#define  IR_MENU_EN            0  // 0: not IR control , 1 IR control

#if defined(USE_BOARD_DY43)||defined(USE_BOARD_DY152)
#define  ICON_FRAME_NUM			20//27
#elif defined(USE_BOARD_DY16)
#if defined(USE_KOERA_UI)
#define  ICON_FRAME_NUM			11
#elif defined(USE_INDIA_UI)
#define  ICON_FRAME_NUM			20
#elif defined(USE_DIE_MAUS_UI)
#define  ICON_FRAME_NUM			17
#else
#define  ICON_FRAME_NUM			22
#endif

#elif defined(USE_BOARD_DY79)
#ifdef USE_DISNEY_UI
#define  ICON_FRAME_NUM			10
#elif defined(USE_KOERA_NEW_UI)
#define  ICON_FRAME_NUM			15
#else
#define  ICON_FRAME_NUM			22
#endif

#elif defined(USE_BOARD_DY116)
#ifdef USE_DISNEY_FRAME_UI
#define  ICON_FRAME_NUM			10
#else
#define  ICON_FRAME_NUM			22
#endif

#elif defined(USE_BOARD_DY128)
#define  ICON_FRAME_NUM			22

#elif defined(USE_BOARD_J589)
#ifdef USE_DISNEY_UI
#define  ICON_FRAME_NUM			10
#else
#define  ICON_FRAME_NUM			22
#endif

#else
#define  ICON_FRAME_NUM			22//22//22//20  //resource frame num
#endif
#define MP3_SHOW_MOVE       1//1:use mp3 move , 0:disable

//����ͷSENSOR_NUMҲҪΪ2 ����DOUBLE_CAM�����Ƶ�˫��ͷ
#define  SENSOR_NUM    2 // 1: is one sensor , 2: is two sensor

#define  mjpeg_lcd_cut    0   //   0:   16:9   1: 4:3 

#define  REDUCE_2_5M  2500 // set for sd remain space
#define	REDUCE_60M	6000

#define  USE_EXTERNAL_DAC_PA	0// 0 : use ic dclass , 1: use external pa

#define NOT_COVER_FILE 0 //do not cover old file 

#define SUPPORT_SELF_TEST HAL_CFG_SELF_TEST

#if (1==USE_EXTERNAL_DAC_PA)
//==need to set ax32xx_dac.c  ax32xx_dacInit(void)
//==R_DAC_CTRL1 |= (1<<9);//Class AB
#define   DAC_PA_MUTE_CH  GPIO_PA
#define   DAC_PA_MUTE_PIN GPIO_PIN9
#define   DAC_PA_MUTE_INIT() {hal_gpioInit(DAC_PA_MUTE_CH,DAC_PA_MUTE_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);}
#define   DAC_PA_MUTE_OFF()  {hal_gpioWrite(DAC_PA_MUTE_CH,DAC_PA_MUTE_PIN,GPIO_HIGH);}	// have sound
#define   DAC_PA_MUTE_ON()  {hal_gpioWrite(DAC_PA_MUTE_CH,DAC_PA_MUTE_PIN,GPIO_LOW);}	// no sound
#else
//==need to set ax32xx_dac.c  ax32xx_dacInit(void)
//==R_DAC_CTRL1 &=~(1<<9);  //Class D==
#define   DAC_PA_MUTE_INIT() 
#define   DAC_PA_MUTE_OFF() 
#define   DAC_PA_MUTE_ON() 
#endif


#define  SEN1_RESET_CH	 GPIO_PB
#define  SEN1_RESET_PIN  GPIO_PIN6
#define  SEN1_PWDN_CH	 GPIO_PD
#define  SEN1_PWDN_PIN	 GPIO_PIN14

#if (2==SENSOR_NUM)
//#define SEN1_RESET_CH  
//#define SEN1_RESET_PIN 
#define  SEN2_RESET_CH	 GPIO_PD
#define  SEN2_RESET_PIN  GPIO_PIN4
#define  SEN2_PWDN_CH	 GPIO_PF //vdd-hp
#define  SEN2_PWDN_PIN	 GPIO_PIN0
#endif




//======watermark===
#define WATER_CHAR_GAP 0  //watermark character gap
#define ST_PIXEL_W    16	//res pic's w
#define ST_PIXEL_H	  32	//res pic's h

#if defined(RESOLUTION_VGA)
#define WATERMAKE_SET_X_POS 10	//watermark X pos
#define WATERMAKE_SET_Y_POS	448	//watermark Y pos
#elif defined(RESOLUTION_720P)
#define WATERMAKE_SET_X_POS 10	//watermark X pos
#define WATERMAKE_SET_Y_POS	360-32	//watermark Y pos

#endif

//=========


//---------file dir-----------------------
#define  FILEDIR_VIDEO  "VIDEO/"              //----user configure
#define  FILEDIR_PHOTO  "PHOTO/"              //----user configure
#define  FILEDIR_AUDIO  "AUDIO/"              //----user configure
#if RECORD_MODE == RECORD_MODE_SIGNAL
#define  FILEDIR_VIDEOA "VIDEO/"
#define  FILEDIR_VIDEOB "VIDEO/"
#else
#define  FILEDIR_VIDEOA "VIDEOA/"
#define  FILEDIR_VIDEOB "VIDEOB/"
#endif

#define  FILEDIR_MP3	 "MP3/"

#endif




