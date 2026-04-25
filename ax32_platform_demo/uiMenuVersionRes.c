#include"uiWin.h"

enum
{
	
	VERSION_BATTERY_ID=0,
	VERSION_TIPS_ID,
	VERSION_TIPS_ID1,
	VERSION_TIPS_ID2,
	VERSION_TIPS_ID3,
	VERSION_ICON_KC
};

static widgetCreateInfor versionWin[] =
{
	//createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createFrameWin(Rx(50),Ry(50), Rw(220),Rh(142),R_COLOR_GRAY2/*R_COLOR_TRANSFER*/,WIN_ABS_POS),
	//createImageIcon(VERSION_BATTERY_ID,Rx(320-32), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	//createStringRim(VERSION_TIPS_ID,Rx(0),Ry(70), Rw(220),Rh(30),SOFTWARE_VERSION,ALIGNMENT_CENTER, R_COLOR_BLUE,0,R_COLOR_BLACK),
#ifdef USE_KOERA_UI
	createImageIcon(VERSION_ICON_KC,Rx(86), Ry(0), Rw(48), Rh(48), R_ID_ICON_KC,ALIGNMENT_CENTER),
	createStringIcon(VERSION_TIPS_ID1,Rx(0),Ry(50), Rw(220),Rh(30),strToResId(SOFTWARE_VERSION1),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(VERSION_TIPS_ID2,Rx(0),Ry(70), Rw(220),Rh(30),R_ID_STR_VER_SELLER,ALIGNMENT_CENTER, R_COLOR_WHITE,0),//R_ID_STR_VER_SELLER
	createStringIcon(VERSION_TIPS_ID3,Rx(0),Ry(90), Rw(220),Rh(30),R_ID_STR_VER_PRODUCT_NAME,ALIGNMENT_CENTER, R_COLOR_WHITE,0),//R_ID_STR_VER_PRODUCT_NAME
	createStringIcon(VERSION_TIPS_ID,Rx(0),Ry(110), Rw(220),Rh(30),strToResId(SOFTWARE_VERSION),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
#else
	createStringIcon(VERSION_TIPS_ID,Rx(0),Ry(70), Rw(220),Rh(30),strToResId(SOFTWARE_VERSION),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
#endif

	widgetEnd(),
};


static void settingVersionBaterryShow(winHandle handle)
{
	return ;
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,VERSION_BATTERY_ID),true);
}




