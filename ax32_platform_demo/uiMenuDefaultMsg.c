#include "uiMenuDefaultRes.c"
#include "../hal/inc/hal_uart.h" 

static u8 sel_idx;

static int defaultKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	uint32 item;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			userConfigReset(0);
			configSystem();	

			#if defined(USE_BT_CONTROL_LED)
				if(configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
				{
					Bt_Control_Led_OnOff(1);
				}
				else
				{
					Bt_Control_Led_OnOff(0);
				}
			#elif defined(USE_5050IC_CONTROL_LED)
				if(configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
				{
					Control_5050Led_OnOff(1);
				}
				else
				{
					Control_5050Led_OnOff(0);
				}
			#elif defined(USE_GPIO_CONTROL_LED)
				if(configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
				{
					Control_GpioLed_OnOff(1);
				}
				else
				{
					Control_GpioLed_OnOff(0);
				}
			#elif defined(USE_FLASH_SET)
				if(configGet(CONFIG_ID_CAP_FLASH) == R_ID_STR_COM_LIGHTING_FORM)
				{
					hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
					hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_LOW);
				}
				else
				{
					hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
					hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_HIGH);
				}
			#endif
		}
		winDestroy(&handle);
	}
	return 0;
}
static int defaultKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			sel_idx=1;
		}
		else
		{
			sel_idx=0;
		}
		settingDefaultSelShow(handle,sel_idx);

	}
	return 0;
}
static int defaultKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		if(0==sel_idx)
		{
			sel_idx=1;
		}
		else
		{
			sel_idx=0;
		}
		settingDefaultSelShow(handle,sel_idx);

	}
	return 0;
}

static int defaultKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
	}
	return 0;
}

static int defaultOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("default Open Win!!!\n");
	sel_idx=0;
	//setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_DEFAULT);

	//==osd==
	settingDefaultBaterryShow(handle);
	settingDefaultSelShow(handle,sel_idx);

	return 0;

}
static int defaultCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("default Close Win!!!\n");
	return 0;
}
static int defaultWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int defaultSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingDefaultBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor defaultMsgDeal[]=
{
	{SYS_OPEN_WINDOW,defaultOpenWin},
	{SYS_CLOSE_WINDOW,defaultCloseWin},
	{SYS_CHILE_COLSE,defaultWinChildClose},
	{KEY_EVENT_OK,defaultKeyMsgOk},
	{KEY_EVENT_RIGHT,defaultKeyMsgUp},
	{KEY_EVENT_LEFT,defaultKeyMsgDown},
	{KEY_EVENT_RETURN,defaultKeyMsgMenu},
	{SYS_EVENT_1S,defaultSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(defaultWindow,defaultMsgDeal,defaultWin)


