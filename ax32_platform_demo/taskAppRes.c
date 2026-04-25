#include"uiWin.h"

enum
{
	APP_STRING1_ID=0,
	APP_STRING2_ID,
	APP_STRING3_ID,
	APP_STRING4_ID,
	APP_QRCODE_ID,
	APP_PRINTER_TIPS_BK_ID,
	APP_PRINTER_TIPS_HEAD_ID,
	APP_PRINTER_TIPS_STR_ID,
	APP_PRINTER_TIPS_CHARGING_NPRINT_ID,
	APP_PRINTER_TIPS_PLZ_CHARG,
	APP_PRINTER_TIPS_PIC_ERR,
	APP_STRING5_ID,
	APP_STRING6_ID,
	APP_STRING_BTNAME3,
	APP_MAX_ID
};

static widgetCreateInfor AppWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

	//createImageIcon(APP_QRCODE_ID,Rx(95), Ry(110), Rw(129),Rh(129), R_ICON_MAX,ALIGNMENT_CENTER),
	
	createStringIcon(APP_STRING1_ID, Rx(0), Ry(5), Rw(320),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(APP_STRING2_ID, Rx(3), Ry(32), Rw(317),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(APP_STRING3_ID, Rx(0), Ry(59), Rw(320),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(APP_STRING4_ID, Rx(3), Ry(86), Rw(317),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(APP_STRING5_ID, Rx(210), Ry(126), Rw(317),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(APP_STRING6_ID, Rx(210), Ry(156), Rw(317),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(APP_STRING_BTNAME3,Rx(10),Ry(222), Rw(310),Rh(25),strToResId(""),ALIGNMENT_CENTER, R_COLOR_BLACK,0),
	//===tips===
	createRect(APP_PRINTER_TIPS_BK_ID,Rx(70),Ry(50),Rw(180),Rh(140),R_COLOR_TBLACK),
	createStringIcon(APP_PRINTER_TIPS_HEAD_ID,Rx(70),Ry(50), Rw(180),Rh(45),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(APP_PRINTER_TIPS_STR_ID,Rx(70),Ry(95), Rw(180),Rh(95),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(APP_PRINTER_TIPS_CHARGING_NPRINT_ID,Rx(70),Ry(95), Rw(180),Rh(95),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createStringIcon(APP_PRINTER_TIPS_PIC_ERR,Rx(70),Ry(50), Rw(180),Rh(45),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(APP_PRINTER_TIPS_PLZ_CHARG,Rx(70),Ry(142), Rw(180),Rh(32),R_ID_STR_TIPS_PRINTING,ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	widgetEnd(),
};


#if 0
static void AppQRShow(winHandle handle)
{

/*	if(R_ID_STR_LAN_SCHINESE == configGet(CONFIG_ID_LANGUAGE))
	{
		winSetVisible(winItem(handle,APP_QRCODE_EN_ID),false);
		winSetVisible(winItem(handle,APP_QRCODE_SC_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,APP_QRCODE_SC_ID),false);
		winSetVisible(winItem(handle,APP_QRCODE_EN_ID),true);
	}

	winSetResid(winItem(handle,APP_QRCODE_ID), R_ID_ICON_MTQRIN);
	winSetVisible(winItem(handle,APP_QRCODE_ID),true);
	drawUIService(true);
*/
}
#endif

static void AppStrShow(winHandle handle)
{
	winSetResid(winItem(handle,APP_STRING1_ID), R_ID_STR_APP_TIPS1);
	winSetResid(winItem(handle,APP_STRING2_ID), R_ID_STR_APP_TIPS2);
	winSetResid(winItem(handle,APP_STRING3_ID), R_ID_STR_APP_TIPS3);
	winSetResid(winItem(handle,APP_STRING4_ID), R_ID_STR_APP_TIPS4);

	winSetVisible(winItem(handle,APP_STRING1_ID),true);
	winSetVisible(winItem(handle,APP_STRING2_ID),true);
	winSetVisible(winItem(handle,APP_STRING3_ID),true);
	winSetVisible(winItem(handle,APP_STRING4_ID),true);
}

static void AppStrPrintModeShow(winHandle handle)
{
#if 0
	if(SysCtrl.printer_print_mode == 0)
	{
		winSetResid(winItem(handle,APP_STRING5_ID), strToResId("PM:GREY MODE"));
	}
	else if(SysCtrl.printer_print_mode == 1)
	{
		winSetResid(winItem(handle,APP_STRING5_ID), strToResId("PM:DOT MODE"));
	}
		
	winSetVisible(winItem(handle,APP_STRING5_ID),true);
#endif	
}

static void AppStrPrintLevelShow(winHandle handle)
{
#if 0
	if(SysCtrl.printer_level == 0)
		winSetResid(winItem(handle,APP_STRING6_ID), strToResId("PL:0"));
	else if(SysCtrl.printer_level == 1)
		winSetResid(winItem(handle,APP_STRING6_ID), strToResId("PL:1"));
	else if(SysCtrl.printer_level == 2)
		winSetResid(winItem(handle,APP_STRING6_ID), strToResId("PL:2"));
	else if(SysCtrl.printer_level == 3)
		winSetResid(winItem(handle,APP_STRING6_ID), strToResId("PL:3"));
	else if(SysCtrl.printer_level == 4)
		winSetResid(winItem(handle,APP_STRING6_ID), strToResId("PL:4"));
	else if(SysCtrl.printer_level == 5)
		winSetResid(winItem(handle,APP_STRING6_ID), strToResId("PL:5"));
	else if(SysCtrl.printer_level == 6)
		winSetResid(winItem(handle,APP_STRING6_ID), strToResId("PL:6"));
	else if(SysCtrl.printer_level == 7)
		winSetResid(winItem(handle,APP_STRING6_ID), strToResId("PL:7"));
	else
		winSetResid(winItem(handle,APP_STRING6_ID), strToResId("PL:other"));

	winSetVisible(winItem(handle,APP_STRING6_ID),true);
#endif	
}


static void AppPrinterTipsShow(winHandle handle,bool show,u32 head_str_id,u32 tips_str_id)
{

	if(true==show)
	{
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_BK_ID),true);
		
		winSetResid(winItem(handle,APP_PRINTER_TIPS_HEAD_ID),head_str_id);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_HEAD_ID),true);
		
		winSetResid(winItem(handle,APP_PRINTER_TIPS_STR_ID),tips_str_id);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_STR_ID),true);
			
		winSetResid(winItem(handle,APP_PRINTER_TIPS_CHARGING_NPRINT_ID),tips_str_id);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_CHARGING_NPRINT_ID),true);

		#if defined(USE_STR_TIPS_LOWPWR_PRINT1_PRINT2)
		if(tips_str_id == R_ID_STR_PWR_LOW)
		{
			tips_str_id = R_ID_STR_TIPS_LOW_POWER_PRINT1;
		}
		#endif
	}
	else
	{
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_BK_ID),false);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_HEAD_ID),false);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_STR_ID),false);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_CHARGING_NPRINT_ID),false);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_PLZ_CHARG),false);
		//winSetVisible(winItem(handle,APP_PRINTER_TIPS_PIC_ERR),false);
		#if defined(USE_STR_TIPS_LOWPWR_PRINT1_PRINT2)
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_CHARGING_NPRINT_ID),false);
		#endif
	}
}


extern u8 BtNameBuf[25];
static void AppStrBtNameShow(winHandle handle)
{
	int i;

	winSetResid(winItem(handle,APP_STRING_BTNAME3),strToResId(BtNameBuf));
	winSetVisible(winItem(handle,APP_STRING_BTNAME3),true);
}

