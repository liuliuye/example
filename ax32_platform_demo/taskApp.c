#include "application.h"
#include "../multimedia/btcom/inc/btcom.h"
#include "../multimedia/btcom/inc/btcom_user.h"
typedef void (*BTCMD_CALLBACK)(const u8 *data, int len);

typedef struct {
	BTCOM_CMD_E cmd;
	BTCMD_CALLBACK fnCmdProc;
} BTCMD_PROC_T;


// must sorted by cmd !!!!
static const BTCMD_PROC_T s_btcmd_procs[] = {
	{BTCOM_SET_BTNAME,		   Bt_Set_BtName}, // = 0x02, //Bt_Set_BtName
	{BTCOM_FEED,               NULL}, // = 0xA1, //Bt_Feed
	{BTCOM_SEND_LINE_DATA,    Bt_Get_Print_Line_Data}, // = 0xA2,
	{BTCOM_GET_STATE,          Bt_Get_Printer_State}, // = 0xA3,
	{BTCOM_SET_QUALITY,        NULL}, // = 0xA4, //Bt_Set_Quality
	{BTCOM_SEND_BITMAP,        NULL}, // = 0xA5,
	{BTCOM_SET_LED_SCREEN,     Bt_Set_Lcd_Screen}, // = 0xA6,
	{BTCOM_GET_DEVINFO,        NULL}, // = 0xA8,
	{BTCOM_FLOW_CONTROL,       NULL}, // = 0xAE,
	{BTCOM_SET_ENERGE25,       NULL}, // = 0xAF,
	{BTCOM_WRITE_DEVICE_ID,    NULL}, // = 0xBB,
	//{BTCOM_PRINT_IGNORE,       NULL}, // = 0xBC,
	{BTCOM_SET_FLAG_BIT,       Bt_Set_Printer_State}, // = 0xBC,
	{BTCOM_SET_MOTOR_STEP_TIME,NULL}, // = 0xBD,
	{BTCOM_SET_PRINT_SPEED,    NULL}, // = 0xBE,
	{BTCOM_SEND_LINE_DATA_ZIP, Bt_Get_Print_Line_Data_Zip}, // = 0xBF,
	{BTCOM_SET_DENSITY,        NULL}, // = 0xF2,
};

static void appCmdProc(BTCOM_CMD_E cmd, const u8 *data, int len)
{
	int begin, end, mid;
	BTCMD_CALLBACK fnProc = NULL;

	// example code
	begin = 0;
	end = sizeof(s_btcmd_procs)/sizeof(s_btcmd_procs[0]) - 1;
	while (begin <= end)
	{
		mid = (begin + end) / 2;
		if (cmd == s_btcmd_procs[mid].cmd) {
			fnProc = s_btcmd_procs[mid].fnCmdProc;
			break;
		} else if (cmd > s_btcmd_procs[mid].cmd) {
			begin = mid + 1;
		} else {
			end = mid - 1;
		}
	}

	if (fnProc != NULL) {
		fnProc(data, len);
	}
}

void taskAppOpen(uint32 arg)
{
	deg_Printf("App open remain: %d\n",hal_sysMemRemain());
	
	btcomInit(appCmdProc);
	uiOpenWindow(&AppWindow,0);
}

void taskAppService(uint32 arg)
{	btcomService();
}
void taskAppClose(uint32 arg)
{
	deg_Printf("App exit.\n");
	btcomUninit();
}
sysTask taskApp=
{
	"App",
	(u32)NULL,
	taskAppOpen,
	taskAppClose,
	taskAppService,
};


