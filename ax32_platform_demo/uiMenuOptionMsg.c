#include "uiMenuOptionRes.c"

static menuItem* currentItem=NULL;
static uint32 getOptionResInfor(uint32 item,uint32* image,uint32* str)
{
	if(image)
		*image=currentItem->pOption[item].image;
	if(str)
		*str=currentItem->pOption[item].str;
	return 0;
}

#if defined(USE_BT_CONTROL_LED)
extern u8 Bt_Control_Led_OnOff(u8 onoff);
#elif defined(USE_5050IC_CONTROL_LED)
extern u8 Control_5050Led_OnOff(u8 onoff);
#elif defined(USE_GPIO_CONTROL_LED)
u8 Control_GpioLed_OnOff(u8 onoff);
#endif

static int menuOptionKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(currentItem)
		{
			configSet(currentItem->configId,currentItem->pOption[itemManageGetCurrentItem(winItem(handle,OPTION_SELECT_ID))].str);
			configSys(currentItem->configId);
			userConfigSave();
			
			#if 0//def ADD_PRINT_PAPER_TYPE
			if(currentItem->configId==CONFIG_ID_PRINTER_TYPE2)
			{
				if(configGet(CONFIG_ID_PRINTER_TYPE2)==R_ID_STR_PRINT_COL_PAPER){
					configSet(CONFIG_ID_PRINTER_DENSITY, R_ID_STR_COM_LEVEL_7);
					configSys(CONFIG_ID_PRINTER_DENSITY);
					userConfigSave();
				}else if(configGet(CONFIG_ID_PRINTER_TYPE2)==R_ID_STR_PRINT_BW_PAPER){
					configSet(CONFIG_ID_PRINTER_DENSITY, R_ID_STR_COM_LEVEL_4);				
					configSys(CONFIG_ID_PRINTER_DENSITY);
					userConfigSave();
				}
			}	
			#endif
			
			#if defined(USE_FLASH_SET)
				if(currentItem->configId==CONFIG_ID_CAP_FLASH)
				{
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
				}
			#else
				if(currentItem->configId==CONFIG_ID_IR_LED)
				{
					#if defined(USE_BT_CONTROL_LED)
						if(currentItem->configId==CONFIG_ID_IR_LED)
						{
							if(configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
							{
								Bt_Control_Led_OnOff(1);
							}
							else
							{
								Bt_Control_Led_OnOff(0);
							}
						}
					#elif defined(USE_5050IC_CONTROL_LED)
						if(currentItem->configId==CONFIG_ID_IR_LED)
						{
							if(configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
							{
								Control_5050Led_OnOff(1);
							}
							else
							{
								Control_5050Led_OnOff(0);
							}
						}
					#elif defined(USE_GPIO_CONTROL_LED)
						if(currentItem->configId==CONFIG_ID_IR_LED)
						{
							if(configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
							{
								Control_GpioLed_OnOff(1);
							}
							else
							{
								Control_GpioLed_OnOff(0);
							}
						}
					#endif				
				}
			#endif	
		}
		winDestroy(&handle);
	}
	return 0;
}
static int menuOptionKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManagePreItem(winItem(handle,OPTION_SELECT_ID));
	}
	return 0;
}
static int menuOptionKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManageNextItem(winItem(handle,OPTION_SELECT_ID));
	}
	return 0;
}
#if 0
static int menuOptionKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==configGet(CONFIG_ID_LANGUAGE))
		{
			
		}
		else
		{
			winDestroy(&handle);
		}
	}
	return 0;
}
static int menuOptionKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==configGet(CONFIG_ID_LANGUAGE))
		{
			
		}
		else
		{
		}
	}
	return 0;
}
#endif
static int menuOptionSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int menuOptionOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 itemNum=0,config/*,i*/;
	if(parameNum<1)
	{
		winDestroy(&handle);
		return 0;
	}
	currentItem=(menuItem*)parame[0];
	deg_Printf("menuOption Open Win!!!\n");
	itemNum=itemManageSetHeightAvgGap(winItem(handle,OPTION_SELECT_ID),R1h(32));
	//for(i=0;i<itemNum;i++)
	//	itemManageSetColumnMargin(winItem(handle,OPTION_SELECT_ID),i,1,5);
	itemManageCreateItem(winItem(handle,OPTION_SELECT_ID),itemCreatemenuOption,getOptionResInfor,currentItem->optionSum);
	itemManageSetCharInfor(winItem(handle,OPTION_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,OPTION_SELECT_ID),R_COLOR_YELLOW/*R_COLOR_BLUE1*/);
	itemManageSetUnselectColor(winItem(handle,OPTION_SELECT_ID),R_COLOR_GRAY/*R_COLOR_BLACK*/);
	config=configGet(currentItem->configId);
	itemNum=0;
	while(itemNum<currentItem->optionSum)
	{
		if(currentItem->pOption[itemNum].str==config)
			break;
		itemNum++;
	}
	if(itemNum>=currentItem->optionSum)
		itemNum=0;
	itemManageSetCurItem(winItem(handle,OPTION_SELECT_ID),itemNum);
	winSetResid(winItem(handle,OPTION_TITLE_ID),currentItem->str);
	return 0;
}
static int menuOptionCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("menuOption Close Win!!!\n");
	currentItem=NULL;
	return 0;
}
static int menuOptionWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("menuOption WinChild Close!!!\n");
	return 0;
}
#if 0
static int menuOptionSysMsg500Ms(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
#endif

static int menuOptionKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		/*taskStart(TASK_MAIN_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}*/
		winDestroy(&handle);
	}
	return 0;
}

msgDealInfor menuOptionMsgDeal[]=
{
	{SYS_OPEN_WINDOW,menuOptionOpenWin},
	{SYS_CLOSE_WINDOW,menuOptionCloseWin},
	{SYS_CHILE_COLSE,menuOptionWinChildClose},
	{KEY_EVENT_OK,menuOptionKeyMsgOk},
	{KEY_EVENT_LEFT,menuOptionKeyMsgUp},
	{KEY_EVENT_RIGHT,menuOptionKeyMsgDown},
	{KEY_EVENT_RETURN,menuOptionKeyMsgExit},
//	{KEY_EVENT_PLAYBACK,menuOptionKeyMsgMenu},
//	{KEY_EVENT_MODE,menuOptionKeyMsgMode},
	{SYS_EVENT_SDC,menuOptionSysMsgSD},
	{EVENT_MAX,NULL},
};

WINDOW(menuOptionWindow,menuOptionMsgDeal,menuOptionWin)


