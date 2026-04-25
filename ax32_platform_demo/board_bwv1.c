/****************************************************************************
**
 **                              BOARD
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  BOARD BWV1
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : board_bwv1.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is for user board
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h"
//#include "../device/gsensor/gsensor.h"
//#include "../device/tft_lcd/tft_lcd.h"
//#include "../device/cmos_sensor/cmos_sensor.h"



#if SYSTEM_BOARD == BOARD_TYPE_BWV1


#if IR_MENU_EN	
	#define  IR_LED_CH 			   GPIO_PF
	#define  IR_LED_PIN			   GPIO_PIN0
#endif 
	//LCD 背光 改为 Gavdd 使用 ax32xx_VDDGSENEnable() 控制 
	#ifndef USE_BOARD_DY42
	#define  LCD_BK_CH       GPIO_PA
	#define  LCD_BK_PIN      GPIO_PIN9
	#endif

	#define  LCD_RESET_CH		 GPIO_PD
	#define  LCD_RESET_PIN 	 GPIO_PIN4

	#define  LED_N0_CH		 GPIO_PA
	#define  LED_N0_PIN 	 GPIO_PIN7
	//PWR KEY 换为 ADKEY
	#define  PWR_KEY_CH      GPIO_PD
	#define  PWR_KEY_PIN     GPIO_PIN1

	//#define  UP_KEY_CH      GPIO_PE
	//#define  UP_KEY_PIN     GPIO_PIN1

//镜头翻转不需要
	#define  LF_KEY_CH      GPIO_PE
	#define  LF_KEY_PIN     GPIO_PIN0

	#define  SET_KEY_CH      GPIO_PE
	#define  SET_KEY_PIN     GPIO_PIN1
	
	// #define  ROTATE_KEY_CH      GPIO_PF
	// #define  ROTATE_KEY_PIN     GPIO_PIN11

#if	BAT_CHECK_TPYE
	#define  ADC_BAT_CH      ADC_CH_MVOUT
#else	
	#define  ADC_BAT_CH 	 ADC_CH_PE0
#endif	
	#define  ADC_BGP_CH      ADC_CH_BGOP
	#define  ADC_KEY_CH      ADC_CH_PB1 
	#define  BAT_CH 	 	 GPIO_PE
	#define  BAT_PIN	 	 GPIO_PIN0


static u8 adCh;

static int board_led_init(void)
{
//==io output control==
//	hal_gpioInit(LED_N0_CH,LED_N0_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
//	hal_gpioWrite(LED_N0_CH,LED_N0_PIN,GPIO_LOW);

//==300R input pull up ,pull donw control==
	// hal_gpioInit(LED_N0_CH,LED_N0_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING); 
	// hal_gpioWrite(LED_N0_CH,LED_N0_PIN,GPIO_LOW);

	//hal_gpioLedInit(GPIO_LED5_PB5,GPIO_PULL_UP,0); 
	// hal_gpioLedInit(GPIO_LED5_PB5,GPIO_PULL_DOWN,0); 
	// hal_gpioLedPull(LED_N0_CH,LED_N0_PIN,GPIO_PULLE_DOWN); 

	return 0;
}

static int board_led_ioctrl(INT32U prit,INT32U op,INT32U para)
{

	static u8 led0=0;
	
    if(op == IOCTRL_LED_CHECK)
	{
		para = led0;
	}
    else if(op == IOCTRL_LED_NO0)
    {
		led0 = para;
		if(para)
		{
			//hal_gpioWrite(LED_N0_CH,LED_N0_PIN,GPIO_HIGH);
			//hal_gpioLedPull(LED_N0_CH,LED_N0_PIN,GPIO_PULLE_UP); 
		}
		else

		{
			//hal_gpioWrite(LED_N0_CH,LED_N0_PIN,GPIO_LOW);
			//hal_gpioLedPull(LED_N0_CH,LED_N0_PIN,GPIO_PULLE_DOWN); 
		}
    }
    
	return 0;
}

static int board_lcd_init(void)
{
//=====lcd bk light close===
	// hal_gpioWrite(LCD_BK_CH,LCD_BK_PIN,GPIO_LOW);
	// hal_gpioInit(LCD_BK_CH,LCD_BK_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);

//--------------LCD RESET------------------------------------

	hal_dacHPSet(1,HP_VDD_2_8V);
	hal_sysDelayMS(50);
	
	hal_dacHPSet(0,0);
	hal_sysDelayMS(50);
	
	hal_dacHPSet(1,HP_VDD_2_8V);
	hal_sysDelayMS(50);

/*
	hal_gpioInit(LCD_RESET_CH,LCD_RESET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(LCD_RESET_CH,LCD_RESET_PIN,GPIO_HIGH);
	XOSTimeDly(10);
	hal_gpioWrite(LCD_RESET_CH,LCD_RESET_PIN,GPIO_LOW);
	XOSTimeDly(10);
	hal_gpioWrite(LCD_RESET_CH,LCD_RESET_PIN,GPIO_HIGH);
	XOSTimeDly(50);
	hal_gpioWrite(LCD_RESET_CH,LCD_RESET_PIN,GPIO_LOW);
	XOSTimeDly(300);
	hal_gpioWrite(LCD_RESET_CH,LCD_RESET_PIN,GPIO_HIGH);
	XOSTimeDly(100);
*/
	lcdInit();

	return 0;
}
static int board_cmos_init(void)
{
	sensorInit();
	hal_mjpegEncodeInit();
	ax32xx_csi_only_FrameSet();
	hal_csiEnable(1);
	
	return 0;
}

#if 0
void photo_cmos_init()
{
	board_cmos_init();
}
#endif

static int board_cmos_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_CMOS_INIT)
	{
		board_cmos_init();
	}
	return -1;
}

#if defined(USB_PWM_LCDBK)
int bright_level = 3;
void set_bright_level(u8 level)
{
	if((level>=1)&&(level<=4))
	{
		bright_level = level-1;
	}
	//uart_Printf("bright_level=%d\n",bright_level);
}
#endif
static int board_lcd_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	#if defined(USB_PWM_LCDBK)
	static u8 lcdBkBriLevel[4]={5,36,67,98};
	#endif
    static INT32U  lcdState=1;
    if(op == IOCTRL_LCD_BKLIGHT)
    {
		if(para)
		{
			deg("lcd bl on");
			
			#ifdef USE_HALL_LCD_FLIP//lcd_flip
            hal_gpioInit(GPIO_PE,GPIO_PIN0,GPIO_INPUT,GPIO_PULL_FLOATING);
			#endif
			ax32xx_VDDGSENEnable(1);
			SysCtrl.redraw_lcd_flag=1;
			lcdState=1;
		}
		else
		{
			deg("lcd bl off");
			ax32xx_VDDGSENEnable(0);
			setKeyEn(false);
			lcdState=0;
		}
    }
	else if(op == IOGET_LCD_BKLIGHT)
	{
   		*(INT32U*)para=lcdState;
	}
    else if(op == IOCTRL_LCD_LCMOFF)
    {
//        hal_lcdLCMPowerOff();
		#ifdef USE_HALL_LCD_FLIP//lcd_flip
			*(INT32U*)para=hal_gpioRead(GPIO_PE,GPIO_PIN0);
		#endif
		hal_gpioWrite(LCD_RESET_CH,LCD_RESET_PIN,GPIO_LOW);
    }
	return 0;
}

static int board_sdc_init(void)
{
//#if USENSOR_PWRCTRL_EN
    hal_sysLDOSet(SYS_LDO_SDC,0,0);  // disable SD_VDD
//#else
//    hal_sysLDOSet(SYS_LDO_SDC,0,1);  // enable SD_VDD
//#endif
    return 0;
}

static int board_sdc_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_SDC_CHECK)
	{
		if(hal_sdCheck())
			*(INT32U *)para = 1;
		else
			*(INT32U *)para = 0;

		return 0;
	}

	return -1;
}
static int board_usb_init(void)
{
    hal_wki1InputEnable(1);

	//==PB5 PB6 handle clean USB setting==
		USB11CON0=0;
		USB11CON1=0;
		USB11CON0 |= (1<<6);//control by soft
		USB11CON1 &= ~(BIT(4)|BIT(6));	//disable dp,dm 120K pullup
		USB11CON1 &= ~(BIT(7)|BIT(5));	//disable dp,dm 15k pulldown
	//==end PB5 PB6 handle clean USB setting==

    return 0;
}

static int board_usb_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	if(op == IOCTRL_USB_CHECK)
	{ 
       *(INT32U *)para = hal_wki1Read();
		return 0;
	}

	return -1;
}
static int board_usb2_init(void)
{
#if (AX32_PLATFORM == AX3292)
	
	//hal_gpioInit_io1d1(USB2_PWR_CH,USB2_PWR_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);  //配置为高压IO
	// hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_INPUT,GPIO_PULL_DOWN);  //B_5V enable
#else
/*
	#if USENSOR_PWRCTRL_EN
		//detect-pin config as output for ir_led
		hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		hal_gpioWrite(USB2_DET_CH,USB2_DET_PIN,GPIO_HIGH);
		
		hal_gpioInit(USB2_PWR_CH,USB2_PWR_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		hal_gpioWrite(USB2_PWR_CH,USB2_PWR_PIN,GPIO_LOW);
	#else
		hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
		hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_INPUT,GPIO_PULL_DOWN);
	#endif
*/
#endif
	return 0;
}
static int board_usb2_ioctrl(INT32U prit,INT32U op,INT32U para)
{
/*
	static INT32U lastUSBPower=0xffffffff;
	
#if (AX32_PLATFORM != AX3292)
    UNUSED u8 output_stat = hal_gpioRead(USB2_DET_CH,USB2_DET_PIN);// store detect-pin output status
#endif

	if(op == IOCTRL_USB_CHECK)
	{
#if (AX32_PLATFORM == AX3292)
	
	*(INT32U *)para = 1;  // use soft detect
#else
	#if USENSOR_PWRCTRL_EN
        if(hal_gpioRead(USB2_PWR_CH,USB2_PWR_PIN))
        {
            //debg("set usb2 check : pull-down\n");
            hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_INPUT,GPIO_PULL_DOWN);
            *(INT32U *)para = !hal_gpioRead(USB2_DET_CH,USB2_DET_PIN);
        }
        else
        {
            //debg("set usb2 check : pull-up\n");
            hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_INPUT,GPIO_PULL_UP);
            *(INT32U *)para = hal_gpioRead(USB2_DET_CH,USB2_DET_PIN);
        }
        
        hal_gpioInit(USB2_DET_CH,USB2_DET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
        hal_gpioWrite(USB2_DET_CH,USB2_DET_PIN,output_stat);
	#else
        *(INT32U *)para = !hal_gpioRead(USB2_DET_CH,USB2_DET_PIN);
	#endif
#endif
		return 0;
	}
    else if(op == IOCTRL_USB_POWER)
    {
		if(para==lastUSBPower)
			return -1;
		  // debg("[usb2]set power : %x\n",para);
#if (AX32_PLATFORM == AX3292)
	
     
	 if(para)
	 {
		 hal_gpioInit(USB2_PWR_CH,USB2_PWR_PIN,GPIO_OUTPUT,GPIO_PULL_DOWN);
		 hal_gpioWrite(USB2_PWR_CH,USB2_PWR_PIN,GPIO_LOW);
		 delay_5ms(10);
	 }
	 else
	 {
		hal_gpioInit_io1d1(USB2_PWR_CH,USB2_PWR_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);  //配置为高压IO
		delay_5ms(10);
	 }
		lastUSBPower=para;
#else

	#if USENSOR_PWRCTRL_EN
        if(para)
        {
            hal_gpioWrite(USB2_PWR_CH,USB2_PWR_PIN,GPIO_HIGH);
			delay_5ms(1000);
        }
        else
        {
            hal_gpioWrite(USB2_PWR_CH,USB2_PWR_PIN,GPIO_LOW);
			delay_5ms(1000);
        }
		lastUSBPower=para;
	#endif
#endif
        return 0;
    }

	return -1;
*/


	if(op == IOCTRL_USB_CHECK)
	{
        *(INT32U *)para = 0;	// no usb2
		return 0;
	}
    else if(op == IOCTRL_USB_POWER)
    {
        return 0;
    }

	return -1;

}

//------------battery --------
#define  BATTERY_MAX     6
#define  BATTERY_AVG     100
#define  BATTERY_INV     0//0x5
#define  BATTERY_OFS     (30) //电池到IC引脚之间的压差,unit:mV
//const static u16 batteryValueTable[BATTERY_MAX] = {3500,3550,3650,3750,3850,3900,4100};


/*
7.0V  = 0x2D4
7.1V  = 0x2DD
7.2V  = 0x2e9
7.3V  = 0x2f2
7.4V  = 0x300
7.5V  = 0x307
7.6V  = 0x312
7.8V  = 0x327
7.9V  = 0x331
8.0V  = 0x33c
8.1V  = 0x346
8.2V  = 0x353
*/
#if defined(USE_BOARD_DY116) 
const static u16 batteryValueTable[BATTERY_MAX] = {3542,3748,3854,3955,3826,4000};
#elif defined(USE_BOARD_DY16) //0Ω 电阻              3.5v 3.7v 3.8v 3.9v
const static u16 batteryValueTable[BATTERY_MAX] = {358, 378, 388, 398, 433, 441};
#elif defined(USE_BOARD_J589) //0Ω 电阻           
const static u16 batteryValueTable[BATTERY_MAX] = {3500,3700,3800,3900,4000,4500};
#elif defined(USE_BOARD_DY79) //0Ω 电阻           
const static u16 batteryValueTable[BATTERY_MAX] = {3523,3720,3823,3922,3826,4000};
#elif defined(USE_BOARD_DY128)
const static u16 batteryValueTable[BATTERY_MAX] = {3484,3690,3787,3890,4000,5000};
#elif defined(USE_BOARD_DY43)
const static u16 batteryValueTable[BATTERY_MAX] = {3523,3720,3781/*3823*/,3922,4200,5000};//
#elif defined(USE_BOARD_DY151)
const static u16 batteryValueTable[BATTERY_MAX] = {3471,3671,3768,3871,4200,5000};//
#elif defined(USE_BOARD_DY152)
const static u16 batteryValueTable[BATTERY_MAX] = {3523,3723,3821,3922,4200,5000};//
#else//DY19 DY05 DY06
const static u16 batteryValueTable[BATTERY_MAX] = {3260,3446,3529,3620,3920,4000};
#endif	

int board_bat_ofs(void)
{
	return BATTERY_OFS;
}

u16 board_bat_lowest(void)
{
	return batteryValueTable[0];
}
static int board_battery_init(void)
{
#if BAT_CHECK_TPYE
//==use RTC check bat==
	u16 batValue=0,bgpValue=0;
	int value;

	/*if(!HAL_CFG_EN_DBG)
	{
 		hal_gpioInit(CHECK_FULL_CH,CHECK_FULL_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	}*/

	hal_batDetectEnable(1);

    int i = 3;
    while(i--)
    {
        batValue += hal_adcGetChannel(ADC_BAT_CH);
        bgpValue += hal_adcGetChannel(ADC_BGP_CH);
    }

	if(bgpValue==0)
		value = 0;
	else
		value = (batValue/3)*100*66/1024+BATTERY_OFS;//hal_adcVDDRTCCalculate(batValue,bgpValue) + BATTERY_OFS;


	if(value<batteryValueTable[0])
	{
		board_usb_ioctrl(0,IOCTRL_USB_CHECK,(INT32U)&value);
		if(value==0)
		{
			deg_Printf("board : battary low power.try to power off.\n");
			//hal_vddWKOEnable(0);
			//ax32xx_wkiCleanPending();
			//XOSTimeDly(200);
		    return -1;
		}
	}
	
	return 0;
#else

//===use adc check===
//===init io==

	s32 value;
	s32 batValue=0;
	u8 i = 3;
	hal_gpioInit(BAT_CH,BAT_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	while(i--)
	{
		batValue += hal_adcGetChannel(ADC_BAT_CH);
	}
	batValue=batValue/3;

	//deg("batValue:%d\n",batValue);
	
	if(batValue<batteryValueTable[0])
	{
		board_usb_ioctrl(0,IOCTRL_USB_CHECK,(INT32U)&value);
		if(value==0)
		{
			deg_Printf("board : battary low power.try to power off.\n");
			return -1;
		}
	}

	return 0;
#endif
}
static int curBatteryADC=0;
int getCurBatteryADC(void)
{
	return curBatteryADC;
}
int cnt = 0;
int printer_tm_n=0;
int	printer_tm_flag=0;
#ifdef FIT_FLASH_ON_RTCBAT_ERR
static u8 batf_now,batf_old = 2;
extern u8 bat_fresh;
static int val_old;
#endif
static int board_battery_ioctrl(INT32U prit,INT32U op,INT32U para)
{
#if BAT_CHECK_TPYE
	static u8 step = 0,oldBat=0xff;
	static u32 batValue=0,bgpValue=0;
	int i,value;
	if(op == IOCTRL_BAT_CHECK)
	{	
		//deg_Printf("xxxx open to bat check\n");
		
	//	if((step&1) == 0)
		{
			batValue += hal_adcGetChannel(ADC_BAT_CH);
			bgpValue += hal_adcGetChannel(ADC_BGP_CH);
			step++;
		}
		/*else if(step&1)
		{            
			step++;
		}*/

	#ifdef FIT_FLASH_ON_RTCBAT_ERR
		batf_now = configGet(CONFIG_ID_CAP_FLASH);
		if(bat_fresh)
		{
			bat_fresh = 0;
			step = 0;
			bgpValue=0;
			batValue=0;
			*(INT32U *)para = oldBat;
			//uart_Printf("22222return,Rtc value = %d srcValue = %d,batf_old: %d,batf_now: %d,oldBat: %d\n",value,batValue,batf_old,batf_now,oldBat);
			return 0;	
		}
	#endif
		
		if(step>=BATTERY_AVG)
		{
			if(bgpValue==0)
				value = 0;
            else
                value = (batValue/step)*100*66/1024+BATTERY_OFS;//hal_adcVDDRTCCalculate(batValue,bgpValue) + BATTERY_OFS;
                
			//deg_Printf("xxxx value:%d\n",value);

			
			#ifdef FIT_FLASH_ON_RTCBAT_ERR
				if(taskCurrent()==TASK_VIDEO_RECORD)//||(taskCurrent()==TASK_MAIN_MENU)
				{
					/*if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_OFF)
					{
						val_old = value;
					}*/
					if(batf_now != batf_old)
					{
						batf_old = batf_now;   
						step = 0;
						bgpValue=0;
						batValue=0;
						*(INT32U *)para = oldBat;
						//uart_Printf("1111111return,Rtc value = %d srcValue = %d,batf_old: %d,batf_now: %d,oldBat: %d\n",value,batValue,batf_old,batf_now,oldBat);
						return 0;
					}
					else
					{
						if(configGet(CONFIG_ID_CAP_FLASH)==R_ID_STR_COM_ON)
						{
							//if((val_old-value)>200)//开关闪光灯RTC电量误差达200以上为旧板子，否则为修正了硬件电量检测的新板子，此函数做兼容新旧板子做处理
							//{
							#if defined(USE_BOARD_DY05)
								value+=280;
							#else
								value+=300;
							#endif
							//}
						}
					}
				}
			#endif
			
			for(i=0;i<BATTERY_MAX;i++)
			{
				if(value<=batteryValueTable[i])
					break;
			}
			if(oldBat!=i)
			{
				if(oldBat==0xff)
					oldBat = i;
				else
				{
					if(oldBat>i)
					{
						if(value<batteryValueTable[i]-BATTERY_INV)
							oldBat=i;
					}
					else
					{
						if(value>batteryValueTable[oldBat]+BATTERY_INV)
							oldBat=i;
					}
				}
			}
			curBatteryADC=value;
			uart_Printf("battery : ad value = %d,i = %d,oldBat=%d\n",value,i,oldBat);
			step = 0;
			bgpValue=0;
			batValue=0;
			*(INT32U *)para = oldBat;
			/*#define LIMIT_BATVAL	3480 //must < batteryValueTable[0]	
			if((LIMIT_BATVAL < batteryValueTable[1])&&(value < LIMIT_BATVAL)){
				*(INT32U *)para	= -1;
			}*/
			return 0;
		}
	}

	return -1;
#else

	static u8 step = 0,oldBat=0xff;
	static u32 batValue=0/*,bgpValue=0*/;
	int i/*,value*/;
	if(op == IOCTRL_BAT_CHECK)
	{
		batValue += hal_adcGetChannel(ADC_BAT_CH);
		step++;
		if(step>=BATTERY_AVG)
		{
			batValue = batValue/BATTERY_AVG;
			//deg_Printf("batv:%d\n",batValue);
			for(i=0;i<BATTERY_MAX;i++)
			{
				if(batValue<=batteryValueTable[i])
					break;
			}
			if(oldBat!=i)
			{
				if(oldBat==0xff)
					oldBat = i;
				else
				{
					if(oldBat>i)
					{
						if(batValue<batteryValueTable[i]-BATTERY_INV)
							oldBat=i;
					}
					else
					{
						if(batValue>batteryValueTable[oldBat]+BATTERY_INV)
							oldBat=i;
					}
				}
			}
			curBatteryADC=batValue;

			//deg("xxxbod bat[%d]\n",batValue);
			if(batValue<batteryValueTable[0])	// no power
			{
				oldBat=0;
			}

			if(printer_tm_flag){
				printer_tm_n++;
				if(printer_tm_n>20){
					printer_tm_n=0;
					printer_tm_flag=0;
				}
			}
				
			//deg("battery : ad value = %d,i = %d,oldBat=%d \n",batValue,i,oldBat);
			*(INT32U *)para = oldBat;
			
			step=0;
			batValue=0;
			return 0;
		}
	}
	else if(op == IOCTRL_ADC_CHECK)
	{
		batValue += hal_adcGetChannel(ADC_BAT_CH);
		step++;
		if(step>=BATTERY_AVG)
		{
			batValue = batValue/BATTERY_AVG;
			curBatteryADC=batValue;

			step=0;
			batValue=0;
			return 0;
		}
	}
	
	return -1;
	
	
//	deg("bat=%d\n",hal_adcGetChannel(ADC_BAT_CH));

//	*(INT32U *)para = 3;

#endif

	return 0;

}

#if 0
//----------------gsensor---------------------
static int board_gsensor_init(void)
{
/*
	hal_vddGSENEnable(1);
	return gSensorInit();
*/
	return 0;
}

static int board_gsensor_ioctrl(INT32U prit,INT32U op,INT32U para)
{
/*
	INT32S value=0;
	switch(op)
	{
		case IOCTRL_GSE_SETLOCK : 
			   return gSensorActiveSet(para);
			   break;
		case IOCTRL_GSE_SETPARK:		   	
			   return gSensorWakeupSet(para);
			   break;
		case IOCTRL_GSE_LOCK:
			   value = gSensorActiveGet();
			   *(INT32U *)para = value;
			   break;
		case IOCTRL_GSE_PARK:
			   value = gSensorWakeUpGet();
			    *(INT32U *)para = value;
			   break;
		case IOCTRL_GSE_STABLE:
			   gSensorMircoMotionWait(para);
			   break;
	    case IOCTRL_GSE_GETNAME:
			   strcpy((char *)para,gSensorGetName());
			   break;
	}

	return value;
*/
	return 0;
}
#endif

//----------------ad key-----------------------
#define  _POWR_OFF_LGC_		1


/*
if you want to implement the long press function of the key,
please refer to the getKey() function, when the key long pressed,
change the value of key
*/
typedef struct _keyInfor
{
	u8   keyType;      // key event
	u16  keyADCvalue;  // adc value of key
	u16  keyADCmin;    // no need initialzation,in board_adkey_init() function,this value calculated according to (keyADCvalue-ADC_KEY_DEVIATION)
	u16  keyADCmax;    // no need initialzation,in board_adkey_init() function,this value calculated according to (keyADCvalue+ADC_KEY_DEVIATION)
}keyInfor;
#define ADC_KEY_DEVIATION     35  // if ADC value of key has a large error,ADC_KEY_DEVIATION shud be large 
#define ADD_KEY(key,keyADCvalue)  {key,keyADCvalue,0,0}
#if (AX32_PLATFORM == AX3291A)
keyInfor adckeyInfor[]=
{
		#if defined(USE_BOARD_DY05) 
		ADD_KEY(KEY_EVENT_POWER,5), 
		ADD_KEY(KEY_EVENT_PHOTO, 615),
		ADD_KEY(KEY_EVENT_RECORD,  701),
		ADD_KEY(KEY_EVENT_MODE,  413),
		ADD_KEY(KEY_EVENT_RETURN,  252),
		ADD_KEY(KEY_EVENT_LEFT,130),
		ADD_KEY(KEY_EVENT_RIGHT,324),
		ADD_KEY(KEY_EVENT_OK,507),	
        #elif defined(USE_BOARD_DY116)
		ADD_KEY(KEY_EVENT_POWER,15),	
		ADD_KEY(KEY_EVENT_OK, 411),
	 	ADD_KEY(KEY_EVENT_RIGHT,  252),//93//535
		ADD_KEY(KEY_EVENT_LEFT,  129),
		ADD_KEY(KEY_EVENT_RECORD,504),//打印
		ADD_KEY(KEY_EVENT_RETURN,322),
        #elif defined(USE_BOARD_DY49)
	  	ADD_KEY(KEY_EVENT_POWER,5),    
        ADD_KEY(KEY_EVENT_OK, 325),
        ADD_KEY(KEY_EVENT_MENU,  130),
        ADD_KEY(KEY_EVENT_MODE,  251),
        ADD_KEY(KEY_EVENT_RETURN,  413),
        ADD_KEY(KEY_EVENT_LEFT,615),
        ADD_KEY(KEY_EVENT_RIGHT,506),
        #elif defined(USE_BOARD_DY16)		
		ADD_KEY(KEY_EVENT_POWER,15), 
		ADD_KEY(KEY_EVENT_RECORD, 410),
		ADD_KEY(KEY_EVENT_OK,  250),//93//535
		ADD_KEY(KEY_EVENT_RETURN,  132),
		ADD_KEY(KEY_EVENT_LEFT,520),//打印
		ADD_KEY(KEY_EVENT_RIGHT,325),
		#elif defined(USE_BOARD_DY79)		
		ADD_KEY(KEY_EVENT_POWER,15), 
		ADD_KEY(KEY_EVENT_RECORD, 513),
		ADD_KEY(KEY_EVENT_OK,  248),
		ADD_KEY(KEY_EVENT_RETURN,  411),
		ADD_KEY(KEY_EVENT_LEFT,128),//打印
		ADD_KEY(KEY_EVENT_RIGHT,323),	
		#elif defined(USE_BOARD_J589) 
		ADD_KEY(KEY_EVENT_POWER,15),	
		ADD_KEY(KEY_EVENT_OK, 410),
	 	ADD_KEY(KEY_EVENT_RIGHT,  250),//93//535
		ADD_KEY(KEY_EVENT_LEFT,  132),
		ADD_KEY(KEY_EVENT_RECORD,520),//打印
		ADD_KEY(KEY_EVENT_RETURN,325),
		#else 
		ADD_KEY(KEY_EVENT_POWER,0),	
		ADD_KEY(KEY_EVENT_RECORD, 129),
	 	ADD_KEY(KEY_EVENT_OK,  250),
		ADD_KEY(KEY_EVENT_RETURN,410),
		ADD_KEY(KEY_EVENT_LEFT,520),
		ADD_KEY(KEY_EVENT_RIGHT,325),	
		#endif
};
#ifdef USE_BOARD_DY42
keyInfor adcrkeyInfor[]=
{
	ADD_KEY(KEY_EVENT_ROTATE1,131),	
	ADD_KEY(KEY_EVENT_ROTATE2,249),
 	ADD_KEY(KEY_EVENT_ROTATE3,324),
 	ADD_KEY(KEY_EVENT_ROTATE4,412),
	ADD_KEY(KEY_EVENT_ROTATE5,505),
	ADD_KEY(KEY_EVENT_ROTATE6,615),
	ADD_KEY(KEY_EVENT_ROTATE7,705),
	ADD_KEY(KEY_EVENT_ROTATE8,785),
	ADD_KEY(KEY_EVENT_ROTATE9,0),//913
};
#endif

#elif (AX32_PLATFORM == JT5292)||(AX32_PLATFORM == JT5295)
keyInfor adckeyInfor[]=
{
	#if defined(USE_BOARD_DY16) 	
		ADD_KEY(KEY_EVENT_POWER,15), 
		ADD_KEY(KEY_EVENT_RECORD, 410),
		ADD_KEY(KEY_EVENT_OK,  250),//93//535
		ADD_KEY(KEY_EVENT_RETURN,  132),
		ADD_KEY(KEY_EVENT_LEFT,520),//打印
		ADD_KEY(KEY_EVENT_RIGHT,325),
	#elif defined(USE_BOARD_DY79)		
		ADD_KEY(KEY_EVENT_POWER,15), 
		ADD_KEY(KEY_EVENT_RECORD, 513),
		ADD_KEY(KEY_EVENT_OK,  248),
		ADD_KEY(KEY_EVENT_RETURN,  411),
		ADD_KEY(KEY_EVENT_LEFT,128),//打印
		ADD_KEY(KEY_EVENT_RIGHT,323),	
	#elif defined(USE_BOARD_J589) 
		ADD_KEY(KEY_EVENT_POWER,15),	
		ADD_KEY(KEY_EVENT_OK, 410),
		ADD_KEY(KEY_EVENT_RIGHT,  250),//93//535
		ADD_KEY(KEY_EVENT_LEFT,  132),
		ADD_KEY(KEY_EVENT_RECORD,520),//打印
		ADD_KEY(KEY_EVENT_RETURN,325),
	#elif defined(USE_BOARD_DY116)
		ADD_KEY(KEY_EVENT_POWER,15),	
		ADD_KEY(KEY_EVENT_OK, 411),
		ADD_KEY(KEY_EVENT_RIGHT,  252),//93//535
		ADD_KEY(KEY_EVENT_LEFT,  129),
		ADD_KEY(KEY_EVENT_RECORD,504),//打印
		ADD_KEY(KEY_EVENT_RETURN,322),
	#elif defined(USE_BOARD_DY128)		
		ADD_KEY(KEY_EVENT_POWER,15),	
		ADD_KEY(KEY_EVENT_LEFT,  132),
		ADD_KEY(KEY_EVENT_RIGHT,  255),//93//535
		ADD_KEY(KEY_EVENT_RETURN,327),
		ADD_KEY(KEY_EVENT_OK, 417),
		ADD_KEY(KEY_EVENT_RECORD,512),//打印
	#elif defined(USE_BOARD_DY43)
		ADD_KEY(KEY_EVENT_POWER,15),	
		ADD_KEY(KEY_EVENT_RECORD, 619),
		ADD_KEY(KEY_EVENT_OK,  419),
		ADD_KEY(KEY_EVENT_MODE,  331),
		ADD_KEY(KEY_EVENT_RETURN,  132),
		ADD_KEY(KEY_EVENT_LEFT,514),
		ADD_KEY(KEY_EVENT_RIGHT,256),
	#elif defined(USE_BOARD_DY151)
		ADD_KEY(KEY_EVENT_POWER,15),	
		ADD_KEY(KEY_EVENT_RECORD, 619),
		ADD_KEY(KEY_EVENT_LEFT,133),
		ADD_KEY(KEY_EVENT_RIGHT,255),
		ADD_KEY(KEY_EVENT_OK,  328),
		ADD_KEY(KEY_EVENT_PLAYBACK,  415),
		ADD_KEY(KEY_EVENT_PRINTER_EN,  509),
	#elif defined(USE_BOARD_DY152)
		ADD_KEY(KEY_EVENT_POWER,15),	
		ADD_KEY(KEY_EVENT_RECORD, 619),
		ADD_KEY(KEY_EVENT_OK,  133),
		ADD_KEY(KEY_EVENT_RETURN,  256),
		ADD_KEY(KEY_EVENT_LEFT,332),
		ADD_KEY(KEY_EVENT_RIGHT,703),
		ADD_KEY(KEY_EVENT_PLAYBACK,  66),
		ADD_KEY(KEY_EVENT_MENU,  511),
	#endif
};
#endif

extern u32 sensor_rotate_flag;
static int board_adkey_init(void)
{
	u32 i;
	for(i=0;i<sizeof(adckeyInfor)/sizeof(adckeyInfor[0]);i++)
	{
		if(adckeyInfor[i].keyADCvalue<=ADC_KEY_DEVIATION)
			adckeyInfor[i].keyADCmin=0;
		else
			adckeyInfor[i].keyADCmin=adckeyInfor[i].keyADCvalue-ADC_KEY_DEVIATION;
		adckeyInfor[i].keyADCmax=adckeyInfor[i].keyADCvalue+ADC_KEY_DEVIATION;
		if(adckeyInfor[i].keyADCmax>1024)
			adckeyInfor[i].keyADCmax=1024;
	}
#ifdef USE_BOARD_DY42
	for(i=0;i<sizeof(adcrkeyInfor)/sizeof(adcrkeyInfor[0]);i++)
	{
		if(adcrkeyInfor[i].keyADCvalue<=ADC_KEY_DEVIATION)
			adcrkeyInfor[i].keyADCmin=0;
		else
			adcrkeyInfor[i].keyADCmin=adcrkeyInfor[i].keyADCvalue-20;
		adcrkeyInfor[i].keyADCmax=adcrkeyInfor[i].keyADCvalue+20;
		if(adcrkeyInfor[i].keyADCmax>1024)
			adcrkeyInfor[i].keyADCmax=1024;
	}
#endif
	
	adCh = ADC_KEY_CH;
   // hal_gpioInit(PWR_KEY_CH,PWR_KEY_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);

	//hal_gpioInit(UP_KEY_CH,UP_KEY_PIN,GPIO_INPUT,GPIO_PULL_UP);
	//hal_gpioInit(LF_KEY_CH,LF_KEY_PIN,GPIO_INPUT,GPIO_PULL_UP);

	//hal_gpioInit(SET_KEY_CH,SET_KEY_PIN,GPIO_INPUT,GPIO_PULL_UP);

	//hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_INPUT,GPIO_PULL_FLOATING); 
	// while (1)
	// {
	// 	ax32xx_wdtClear();
	// 	deg_Printf(" ---%d\n", hal_gpioRead(ROTATE_KEY_CH,ROTATE_KEY_PIN));
	// 	/* code */
	// }

		// if(hal_gpioRead(ROTATE_KEY_CH,ROTATE_KEY_PIN))
		// 	sensor_rotate_flag = 0;
		// else
		// 	sensor_rotate_flag = 1;


	return 0;
}
extern u32 csi_en_flag;
extern u8 senid;

uint32 keyADCvalue=0;
uint32 getKeyADCvalue(void)
{
	return keyADCvalue;
}

#define KEY_SCAN_UNFINISHED             0xffffffff
uint32 getKeyVal(void)
{
	static uint32 lastKey=KEY_EVENT_END;
	static uint32 keyScanTimes=0;
	uint32 value, /*value_lf, value_rd, value_set,*/ value_re=-1,i,key=0;

	//value = hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
	//value_lf = hal_gpioRead(LF_KEY_CH,LF_KEY_PIN);
	//value_up = hal_gpioRead(UP_KEY_CH,UP_KEY_PIN);
	//value_set = hal_gpioRead(SET_KEY_CH,SET_KEY_PIN);

	//value_re = hal_gpioRead(ROTATE_KEY_CH,ROTATE_KEY_PIN);
	
	//deg_Printf("re %d \n", value_re);
	//deg_Printf("value_lf = %d, value_dw = %d...........\n",value_lf, value_dw);
	//deg_Printf("ISPMODE:0x%x\n",ISPMODE);
	//if(ISPMODE!=0xffff)
/*
	if(senid!=0xff)	
	{
		if(value_re == 0 && sensor_rotate_flag == 1){
			sensor_rotate_flag = 0;
			hal_csiEnable(0);
			_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
			csiSensor->p_fun_adapt.fp_rotate(sensor_rotate_flag);
			if(csi_en_flag)
				hal_csiEnable(1);
		}
		else if(value_re == 1 && sensor_rotate_flag == 0){
			sensor_rotate_flag = 1;
			hal_csiEnable(0);
			_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
			csiSensor->p_fun_adapt.fp_rotate(sensor_rotate_flag);
			if(csi_en_flag)
				hal_csiEnable(1);
		}
	}
*/
/*	if(sensor_rotate_flag==1)
	{
		sensor_rotate_flag = 0;
		hal_csiEnable(0);
		_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
		csiSensor->p_fun_adapt.fp_rotate(0);
		//if(csi_en_flag)
		hal_csiEnable(1);
	}
*/

	/*
	 if(value)
	 	key = KEY_EVENT_POWER;   // power key ,key value
	 else 
	    if(value_lf == 0)
	    	   key = KEY_EVENT_LEFT;  
	 else 
	   if(value_set == 0)
	   	   key = KEY_EVENT_OK;//KEY_EVENT_RECORD; 
	   else if(value_re == 0)
	   	   key = KEY_EVENT_ROTATE;  
	 else  */
	   {
		  value = hal_adcGetChannel(ADC_KEY_CH);
		  //uart_Printf("adcKeyValue:%d\n",value);
		  if(value<=1000)
		  {
		  	keyADCvalue=value;
		    //uart_Printf("adcKeyValue:%d\n",value);
		  }

		   for(i=0;i<sizeof(adckeyInfor)/sizeof(adckeyInfor[0]);i++)
		   {
		   	   if(value>=adckeyInfor[i].keyADCmin&&value<=adckeyInfor[i].keyADCmax)
			   	  key = adckeyInfor[i].keyType;
		   }
	   }
	   if(lastKey!=key)
   	   {
	   	  	lastKey=key;
			keyScanTimes=1;
   	   }
	   else
	   		keyScanTimes++;
	   if(keyScanTimes>=3)
   	   {
   	  		keyScanTimes=0;
			return key;
   	   }
	   return KEY_SCAN_UNFINISHED;
}

bool key_focus_model =  false;
uint8 key_flag=0;
uint8 key_num=0;
uint32 getKey(void)
{

	uint32 keyVal;

	if(key_flag)
	keyVal=getKeyVal();
	else{
		key_num++;
		if(key_num>30){key_flag=1;}
		return KEY_SCAN_UNFINISHED;
	}
	if(keyVal==KEY_SCAN_UNFINISHED)
		return KEY_SCAN_UNFINISHED;
	//deg_Printf("keyVal = %d, \n",keyVal);


/*
static uint32 keyPowerPressCnt=0;
if(keyVal==KEY_EVENT_POWER)
{
	keyPowerPressCnt++;
	if(keyPowerPressCnt>=30)
		keyVal=KEY_EVENT_POWEROFF;
	else
	{
		keyVal=0;	
	}
}
else
{
	if(keyPowerPressCnt>0&&keyPowerPressCnt<30)
		keyVal = KEY_EVENT_POWER;
	keyPowerPressCnt=0;
}

#if (2 == SENSOR_NUM)
	static uint32 keyOkPressCnt=0;
	if(keyVal==KEY_EVENT_OK)
	{
		keyOkPressCnt++;
		if(keyOkPressCnt>=15)
			keyVal=KEY_EVENT_SENSOR_CHANGE;
		else
			keyVal=0;	
	}
	else
	{
		if((keyOkPressCnt>0)&&(keyOkPressCnt<15))
			keyVal = KEY_EVENT_OK;
		keyOkPressCnt=0;
	}
#endif

#if KEY_NUM == 5
	static uint32 keyModePressCnt=0;
	if(keyVal==KEY_EVENT_MODE)
	{
		keyModePressCnt++;
		if(keyModePressCnt>=10)
			keyVal=KEY_EVENT_MENU;
		else
			keyVal=0;	
	}
	else
	{
		if(keyModePressCnt>0&&keyModePressCnt<10)
			keyVal = KEY_EVENT_MODE;
		keyModePressCnt=0;
	}
#endif
*/


	static uint32 keyPowerPressCnt=0;
	if(keyVal==KEY_EVENT_POWER)
	{
		keyPowerPressCnt++;
		if(keyPowerPressCnt>=30)
			keyVal=KEY_EVENT_POWEROFF;
		else
		{
			keyVal=0;	
		}
	}
	else
	{
		if(keyPowerPressCnt>0&&keyPowerPressCnt<30)
			#if defined(USE_BOARD_DY26)||defined(USE_BOARD_DY151)
			keyVal = KEY_EVENT_RETURN;
			#else
			keyVal = KEY_EVENT_POWER;
			#endif
		keyPowerPressCnt=0;
	}

	/*static uint32 keyRotatePressCnt=0;
	if(keyVal==KEY_EVENT_ROTATE)
	{
		if(!keyRotatePressCnt){
			hal_csiEnable(0);
			_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
			csiSensor->p_fun_adapt.fp_rotate(sensor_rotate_flag);
			hal_csiEnable(1);
		}
		keyRotatePressCnt++;
		keyVal=0;
	}
	else
	{
		if(keyRotatePressCnt>0){
			hal_csiEnable(0);
			_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
			csiSensor->p_fun_adapt.fp_rotate(sensor_rotate_flag);
			hal_csiEnable(1);
		}
		keyRotatePressCnt=0;
		keyVal=0;
	}*/
//--
/*
	static uint32 keyRecordPressCnt=0;
	if(keyVal==KEY_EVENT_RECORD)
	{
		keyRecordPressCnt++;
		//if(keyRecordPressCnt>=30)
			//keyVal=KEY_EVENT_POWEROFF;
		//else
			keyVal=0;
	}
	else
	{
		if(keyRecordPressCnt>0&&keyRecordPressCnt<30)
			keyVal = KEY_EVENT_RECORD;
		keyRecordPressCnt=0;
	}
*/

	static uint32 keyReturnPressCnt=0;
#if defined(USE_BOARD_DY20)
	if(keyVal==KEY_EVENT_RETURN)
	{
		keyReturnPressCnt++;
		if(keyReturnPressCnt>=15)
			keyVal=KEY_EVENT_DELETE;
		else
			keyVal=0;	
	}
	else
	{
		if((keyReturnPressCnt>0)&&(keyReturnPressCnt<15))
			keyVal = KEY_EVENT_RETURN;
		keyReturnPressCnt=0;
	}
#else
	if(keyVal==KEY_EVENT_RETURN)//多功能按键
	{
		keyReturnPressCnt++;

		keyVal=0;
	}
	else
	{
		if(keyReturnPressCnt>0&&keyReturnPressCnt<30)
			keyVal = KEY_EVENT_RETURN;
		keyReturnPressCnt=0;
	}
#endif

#if 0	
	static uint32 keyReturnPressCnt=0;
	if(keyVal==KEY_EVENT_RETURN)
	{
		keyReturnPressCnt++;
		//if(keyReturnPressCnt>=30)
			//keyVal=KEY_EVENT_POWEROFF;
		//else
			keyVal=0;
	}
	else
	{
		if(keyReturnPressCnt>0&&keyReturnPressCnt<30)
			keyVal = KEY_EVENT_POWER;
		keyReturnPressCnt=0;
	}
#endif

	/*static uint32 keyphotoPressCnt=0;
	if(keyVal==KEY_EVENT_PHOTO)
	{
		keyphotoPressCnt++;
		
		//if(keyphotoPressCnt>=20)
			//keyVal=KEY_EVENT_IR;
		//else
			keyVal=0;	
			
	}
	else
	{
		if(keyphotoPressCnt>0&&keyphotoPressCnt<20)
			keyVal =KEY_EVENT_PHOTO;
		keyphotoPressCnt=0;
	}*/

#if defined(USE_BOARD_DY128)||defined(USE_BOARD_DY116)||defined(USE_BOARD_DY16)||defined(USE_BOARD_J589) 
	static uint32 keyOkPressCnt=0;
	if(keyVal==KEY_EVENT_OK)
	{
		keyOkPressCnt++;
		if(keyOkPressCnt>=15)
			keyVal=KEY_EVENT_DELETE;
		else
			keyVal=0;	
	}
	else
	{
		if((keyOkPressCnt>0)&&(keyOkPressCnt<15))
			keyVal = KEY_EVENT_OK;
		keyOkPressCnt=0;
	}
#endif

	static uint32 keyUpPressCnt=0;
	if(keyVal==KEY_EVENT_UP)
	{
		if(key_focus_model == false){
			keyUpPressCnt++;
			
			//if(keyUpPressCnt>=20)
				//keyVal=KEY_EVENT_IR;
			//else
				keyVal=0;
		}
	}
	else
	{
		if((keyUpPressCnt>0)&&(keyUpPressCnt<20))
			keyVal = KEY_EVENT_UP;
		keyUpPressCnt=0;
	}

	static uint32 keyPlaybackPressCnt=0;
	if(keyVal==KEY_EVENT_PLAYBACK)
	{
		keyPlaybackPressCnt++;
			
		//if(keyPlaybackPressCnt>=20)
			//keyVal=KEY_EVENT_IR;
		//else
			keyVal=0;
	}
	else
	{
		if((keyPlaybackPressCnt>0)&&(keyPlaybackPressCnt<20))
			keyVal = KEY_EVENT_PLAYBACK;
		keyPlaybackPressCnt=0;
	}

/*
	static uint32 keyDeletePressCnt=0;
	if(keyVal==KEY_EVENT_DELETE)
	{
		keyDeletePressCnt++;
			
		//if(keyDeletePressCnt>=20)
			//keyVal=KEY_EVENT_IR;
		//else
			keyVal=0;
	}
	else
	{
		if((keyDeletePressCnt>0)&&(keyDeletePressCnt<20))
			keyVal = KEY_EVENT_DELETE;
		keyDeletePressCnt=0;
	}
*/


	static uint32 keyLeftPressCnt=0;
	if(keyVal==KEY_EVENT_LEFT)
	{
		if(key_focus_model == false){
			keyLeftPressCnt++;
			
			if(keyLeftPressCnt>=15)
				keyVal=KEY_EVENT_LEFT;
			else
				keyVal=0;
		}
	}
	else
	{
		if((keyLeftPressCnt>0)&&(keyLeftPressCnt<20))
			keyVal = KEY_EVENT_LEFT;
		keyLeftPressCnt=0;
	}


	return keyVal;
}
#define SCAN_KEY_CONTINUE_INTERVAL  (10)
#define SCAN_KEY_CONTINUE_INTERVAL_FOCUS  (5)
uint32 keyScanKey(void)
{
	static uint32 LastKeyVal=0;
	static int32 KeyContinueCnt=0;
	uint32 CurKeyVal;
	uint32 KeySta=0;
	CurKeyVal=getKey();
	if(CurKeyVal==KEY_SCAN_UNFINISHED)
		return 0;
	if(LastKeyVal!=CurKeyVal)
	{
		if(CurKeyVal)
			KeySta=makeEvent(CurKeyVal,KEY_PRESSED);
		else
			KeySta=makeEvent(LastKeyVal,KEY_RELEASE);
		LastKeyVal=CurKeyVal;
		KeyContinueCnt=0;
	}
	else
	{
		if(CurKeyVal)
		{
			KeyContinueCnt++;
			#if defined(USE_BOARD_DY20)||defined(USE_BOARD_DY05)
			if(CurKeyVal == KEY_EVENT_RECORD)
			{
				if(KeyContinueCnt>=71) //SysCtrl.key_down_long
				{
					KeyContinueCnt=0;
					KeySta=makeEvent(CurKeyVal,KEY_CONTINUE);
				}
			}
			else if(KeyContinueCnt>=(key_focus_model ? SCAN_KEY_CONTINUE_INTERVAL_FOCUS : SCAN_KEY_CONTINUE_INTERVAL))
			{
				KeyContinueCnt=0;
				KeySta=makeEvent(CurKeyVal,KEY_CONTINUE);
			}
			else
				KeySta=0;
			#else
			if(KeyContinueCnt>=(key_focus_model ? SCAN_KEY_CONTINUE_INTERVAL_FOCUS : SCAN_KEY_CONTINUE_INTERVAL))
			{
				KeyContinueCnt=0;
				KeySta=makeEvent(CurKeyVal,KEY_CONTINUE);
			}
			else
				KeySta=0;
			#endif
		}
		else
			KeySta=0;
	}
	return KeySta;
}
#ifdef USE_BOARD_DY42
uint32 getRKeyVal(void)
{
	static uint32 lastKey=KEY_EVENT_END;
	static uint32 keyScanTimes=0;
	uint32 value, value_re=-1,i,key=0;

   {
   	  value = hal_adcGetChannel(ADC_CH_PE0);
	  //uart_Printf("adcKeyValuePE0=%d\n",value);
	  if(value<=1000)
	  {
	  	//keyADCvalue=value;
	    //deg_Printf("adcKeyValue:%d\n",value);
	  }

	   for(i=0;i<sizeof(adcrkeyInfor)/sizeof(adcrkeyInfor[0]);i++)
	   {
	   	   if(value>=adcrkeyInfor[i].keyADCmin&&value<=adcrkeyInfor[i].keyADCmax)
		   	  key = adcrkeyInfor[i].keyType;
	   }
    }

	  //uart_Printf("getRKeyVal key=%d\n",key);
	   if(lastKey!=key)
   	   {
	   	  	lastKey=key;
			keyScanTimes=1;
   	   }
	   else
	   		keyScanTimes++;
	   if(keyScanTimes>=3)
   	   {
   	  		keyScanTimes=0;
			return key;
   	   } 
	   return key;
}

extern int deal_photoApp_fstMsg;
uint32 RkeyScanKey(void)
{
	static uint32 RLastKeyVal=0;
	static int32 RKeyContinueCnt=0;
	uint32 RCurKeyVal;
	uint32 RKeySta=0;
	RCurKeyVal=getRKeyVal();
	//uart_Printf("LastKeyVal=%d,CurKeyVal=%d \n",LastKeyVal,CurKeyVal);
	if(deal_photoApp_fstMsg == 0)
	{
		if(RCurKeyVal)
			RKeySta=makeEvent(RCurKeyVal,KEY_PRESSED);
	}
	else
	{
		if(RCurKeyVal==0)
			return 0;
		if(RLastKeyVal!=RCurKeyVal)
		{
			if(RCurKeyVal)
				RKeySta=makeEvent(RCurKeyVal,KEY_PRESSED);
			//else
			//	KeySta=makeEvent(LastKeyVal,KEY_RELEASE);
			RLastKeyVal=RCurKeyVal;
			//KeyContinueCnt=0;
		}
	}
	
	return RKeySta;
}
static int R_scankey_cnt = 50;
#endif
static int board_adkey_ioctrl(INT32U prit,INT32U op,INT32U para)
{
	int value;
	if(op == IOCTRL_KEY_READ)
	{   
		   if((*(INT32U *)para = keyScanKey())==0)
		   	   return -1;			   	   
	}
	#ifdef USE_BOARD_DY42
	else if(op == IOCTRL_RKEY_READ)
	{
		 R_scankey_cnt--;
		 if(R_scankey_cnt==0)
		 {
		 	R_scankey_cnt = 50;
		 	if((*(INT32U *)para = RkeyScanKey())==0)
		   	   return -1;	
		 }
		 else
		 {
			 return -1;	
		 }
	}
	#endif
    else if(op == IOCTRL_KEY_POWER)
    {
		#if (_POWR_OFF_LGC_ == 1)
		value = hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
		#else
		value = !hal_gpioRead(PWR_KEY_CH,PWR_KEY_PIN);
		#endif
		*(int *)para = value; 
    }
	else if(op == IOCTRL_KEY_POWEROFF)
	{
		hal_gpioInit(PWR_KEY_CH,PWR_KEY_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		hal_gpioWrite(PWR_KEY_CH,PWR_KEY_PIN,GPIO_LOW);
	}
	return 0;
}


static int board_ir_init(void)
{
#if (1 == IR_MENU_EN)
	hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(IR_LED_CH,IR_LED_PIN,GPIO_LOW);
#endif
	return 0;
}

static int board_ir_ioctrl(INT32U prit,INT32U op,INT32U para)
{
#if (1 == IR_MENU_EN)
	static uint32 irState=0;
	if(op == IOCTRL_IR_SET)
	{
	   if(para)	// on
	   {
		   hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		   hal_gpioWrite(IR_LED_CH,IR_LED_PIN,GPIO_HIGH);
		   irState=1;
	   }
	   else		//off
	   {
		   hal_gpioInit(IR_LED_CH,IR_LED_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		   hal_gpioWrite(IR_LED_CH,IR_LED_PIN,GPIO_LOW);
		   irState=0;
	   }
	}
	else if(op == IOGET_IR_GET)
		*(INT32U*)para=irState;
#endif
	return 0;
}


Board_Node_T  board_bwv1[]=
{
    {//------usb-----------
		.name = DEV_NAME_USB,
		.init = board_usb_init,
		.ioctrl = board_usb_ioctrl, 
		.prit = 0
	},

	{//------bat-----------
		.name = DEV_NAME_BATTERY,
		.init = board_battery_init,
		.ioctrl = board_battery_ioctrl, 
		.prit = 0
	},
	{
		.name = DEV_NAME_USENSOR,
		.init = board_usb2_init,
		.ioctrl = board_usb2_ioctrl,
		.prit = 0
	},
	{//------sdc-----------
		.name = DEV_NAME_SDCARD,
		.init = board_sdc_init,
		.ioctrl = board_sdc_ioctrl, 
		.prit = 0
	},
    {//------lcd-----------
		.name = DEV_NAME_LCD,
		.init = board_lcd_init,
		.ioctrl = board_lcd_ioctrl,	
		.prit = 0
    }, 	
 /*   {//------gsensor-----------
		.name = DEV_NAME_GSENSOR,
		.init = board_gsensor_init,
		.ioctrl = board_gsensor_ioctrl, 
		.prit = 0
	},*/

	{//------led-----------
		.name = DEV_NAME_LED,
		.init = board_led_init,
		.ioctrl = board_led_ioctrl,	
		.prit = 0
    },
     
	{//------cmos -sensor
		.name = "cmos-sensor",
		.init = NULL,//booard_cmos_init,
		.ioctrl = board_cmos_ioctrl,
		.prit=0
	},
    {//------ad key-----------
		.name = DEV_NAME_ADKEY,
		.init = board_adkey_init,
		.ioctrl = board_adkey_ioctrl, 
		.prit = 0
	},
#if (1 == IR_MENU_EN)
	{//------ir led-----------
		.name = DEV_NAME_IR,
		.init = board_ir_init,
		.ioctrl = board_ir_ioctrl, 
		.prit = 0
	},
#endif
    {
		.name[0] = 0,
		.init = NULL,
		.ioctrl = NULL,
    }
};



#endif

