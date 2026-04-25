#include "../../ax32_platform_demo/application.h"
#include "inc/btcom.h"
#include "inc/btcom_inner.h"

static u32 count_printLine = 0;
static u8* BT_user_Pic_Addr;
static u32 Pic_Data_offset=0;
static u32 Deal_Bt_Data_flag=0;
#define FLOW_STOP_DATA 1
#define FLOW_SEND_DATA 0
u16 reset_pic_data(void);

/**
 * cmd：A4.  low 4 bit set print level :range 1~5.
 *
 */
void Bt_Set_Quality(const u8 *data, int len)
{
	/*u8* y_buf;

	y_buf=get_pic_add();
	memset(y_buf,0,JPG_LEN_MAX);
	set_BT_start_flag(0);
	reset_pic_data();
	return ;*/

	if(data==NULL)
		return ;

//	int i;
	u16 Printer_Q;

	deg_Printf("set Quality:data :%x len:%d\n",data,len);
	
	count_printLine=0;		//reset line count 
	set_pic_w(count_printLine);
	Pic_Data_offset=0;

	
	/*BT_Pic_Addr=(u8*)hal_sysMemMalloc(1024*200,32);
	if(NULL==BT_Pic_Addr)
	{
		deg_Printf("bt pic malloc fail\n");
	}else
	{
		deg_Printf("bt pic malloc success\n");
	}*/
	hal_sysMemRemain();

	Printer_Q = data[len-1]&0x0F;
	
	deg_Printf("set Quality: Q:%x\n",Printer_Q);

	#if 1
		switch (Printer_Q)
		{
			case 1: SysCtrl.printer_level = 0;
				break;
			case 2: SysCtrl.printer_level = 1;
				break;
			case 3: SysCtrl.printer_level = 2;
				break;
			case 4: SysCtrl.printer_level = 3;
				break;
			case 5: SysCtrl.printer_level = 4;
				break;
			default:
				SysCtrl.printer_level = 4;
				break;
		}

		deg_Printf("show printer level:%d  %d\n",SysCtrl.printer_level,configValue2Int(/*R_ID_STR_COM_LEVEL_4*/CONFIG_ID_BAT_OLD));
	#endif
}


u8 Rsp_Get_Printer_State()
{
	//低电 > 开盖 > 缺纸 > 过热 > 正在打印
	u8 ret=0;
	if(SysCtrl.battery <= 1)	//low power
	{			
		ret = 0x08;
	}else if(SysCtrl.paper_check==0)	//纸张检测和串口和蓝牙TX复用？？？
	{	
	
		ret = 0x00;
	}else if(114<hal_adcGetChannel(ADC_CH_PA6))//over hot
	{	
	
		ret = 0x02;
	}else	//printing
	{								
		ret = 0x80;
	}
	return ret;
}

/**
 * cmd：A3. return printer state to app
 *
 */
void Bt_Get_Printer_State(const u8 *data, int len)
{
	deg_Printf("app need get printer state.\n");
	deg_Printf("get State:data :%x len:%d\n",data,len);
	//hal_sysMemRemain();
	
	if(data==NULL)
		return ;
	
	u8 data_resp;
	int _len;
	//get state
	data_resp=Rsp_Get_Printer_State();
	_len=sizeof(data_resp);
	//send 
	btcomCmdRsp(BTCOM_GET_STATE,&data_resp,_len);
	
}


/**
 * cmd：AE. Bt flow control
 *
 */
void Bt_Set_Flow_Control(u8 data)
{
	int i;
    u8 bt_flow_stop[9]   = {0x51,0x78 ,0xAE ,0x01 ,0x01 ,0x00 ,0x10 ,0x70 ,0xFF};
    u8 bt_flow_resume[9] = {0x51,0x78 ,0xAE ,0x01 ,0x01 ,0x00 ,0x00 ,0x00 ,0xFF};

	hal_uartIOShareCheck();
	if(data == 1)
	{
		//STOP SEND DATA
		for(i=0;i<9;i++){
			hal_uartSendData(bt_flow_stop[i]);
		}
	}
	else if(data == 0)
	{
		reset_pic_data();
		Deal_Bt_Data_flag = 0;
		//BEGIN SEND DATA
		for(i=0;i<9;i++){
			hal_uartSendData(bt_flow_resume[i]);
		}
	}
}

#ifdef BT_CONTROL_PRINTSTB
static u8 prtPoweroff[7] = {0x48,0x4A,0x58,0x4C,0x45,0x44,0x00};
static u8 prtPoweron[7]  = {0x48,0x4A,0x58,0x4C,0x45,0x44,0x01};//低位有效
u8 Bt_Control_Prt_Power_OnOff(u8 onoff)
{
	int i;
	hal_uartIOShareCheck();
	if(onoff)
	{
		for(i=0;i<7;i++)
		
		{
			hal_uartSendData(prtPoweron[i]);
		}
	}
	else
	{	
		for(i=0;i<7;i++)
		{
			hal_uartSendData(prtPoweroff[i]);
		}
	}
	return 0;
}
#endif

/**
 * cmd：0x02. Bt get BtName
 *
 */
void Bt_Get_BtName(void)
{	
	u8 data_resp;
	int _len;
	//get state
	data_resp=0x00;//Rsp_Get_Printer_State();
	_len=sizeof(data_resp);
	//send 
	btcomCmdRsp(BTCOM_SET_BTNAME,&data_resp,_len);
}

u8 BtNameBuf[25];
int Get_BtName_Flag  = 0;
int Show_BtName_Flag = 0;
void Bt_Set_BtName(const u8 *data, int len)
{
	uart_Printf("in cmd 02,show len:%d\n",len);
	int i;
	uart_Printf("in cmd 02,data: ");
	memset(BtNameBuf,0,sizeof(BtNameBuf));
	for(i=0;i<len;i++)
	{
		//deg_Printf("%x ",data[i]);
		BtNameBuf[i] = data[i];
		
	}

	Get_BtName_Flag = 1;
	uart_Printf("\n");
	uart_Printf("level:%d . mode:%d\n",SysCtrl.printer_level,SysCtrl.printer_print_mode);
}
void Bt_Set_Prt_finish(void)
{
	int i;
    u8 buf[9] = {0x51,0x78 ,0xA4 ,0x01 ,0x01 ,0x00 ,0x01 ,0x07 ,0xFF};
  
	hal_uartIOShareCheck();
	for(i=0;i<9;i++){
			hal_uartSendData(buf[i]);
	}
}


/**
 * cmd：A2. get line data
 *
 */
void Bt_Get_Print_Line_Data(const u8 *data, int len)
{
	int i,j;
	
	BT_user_Pic_Addr = get_pic_add();
	//uart_Printf("Bt_Get_Print_Line_Data \n");

	for(i=0;i<len;i++)
	{
#if 0
		//deg_Printf("%x ",data[i]);
		if(BT_Pic_Addr)
		{
			Pic_Data_offset=count_printLine*len+i;
			*(BT_Pic_Addr+Pic_Data_offset)=data[i];
			#if 0
			for(j=0;j<7;j++)
			{
				*(BT_Pic_Addr+Pic_Data_offset+j)=data[i]&(0x80>>j);
			}
			#endif
				
		}
#else
		if(BT_user_Pic_Addr)
		{
			for(j=0;j<8;j++)
			{
				if(data[i]&(0x01<<j)){
					*(BT_user_Pic_Addr+Pic_Data_offset) = 1;
				}else{
					*(BT_user_Pic_Addr+Pic_Data_offset) = 0;
				}
				Pic_Data_offset++;
			}	
		}
#endif
	}
	set_pic_w(count_printLine++);
	deg_Printf(" line:%d len:%d addr:%x\n",count_printLine,len,BT_user_Pic_Addr);
	//uart_Printf("noZIP Pic_Data_offset:%d\n",Pic_Data_offset);
	//uart_Printf("count_printLine: %d len:%d \n",count_printLine,len);

	if(SysCtrl.printer_print_mode == 1)//dot matrix
	{
		if(Pic_Data_offset > 300000)
		{ 
			if(Deal_Bt_Data_flag == 0)
			{
				Deal_Bt_Data_flag = 1;
				Bt_Set_Flow_Control(FLOW_STOP_DATA);
				deal_BT_Pic_Addr();
			}
		}
	}
}

void Bt_Get_Print_Line_Data_Zip(const u8 *data, int len)
{
	int i,j,line;
	line = 0;
	BT_user_Pic_Addr = get_pic_add();

	for(i=0;i<len;i++)
	{
		if(BT_user_Pic_Addr)
		{
			for(j=0;j<(data[i]&0x7F);j++)
			{
				 line++;
			 	 *(BT_user_Pic_Addr+Pic_Data_offset) = (data[i]>>7)&1;
				 Pic_Data_offset++;
		 	}
			count_printLine = count_printLine+line/384;
		}
	}
	set_pic_w(count_printLine);
	line = 0;
	deg_Printf(" line:%d len:%d addr:%x\n",count_printLine,len,BT_user_Pic_Addr);
	//uart_Printf(" ZIP Pic_Data_offset:%d\n",Pic_Data_offset);

	if(SysCtrl.printer_print_mode == 1)//dot matrix
	{
		if(Pic_Data_offset > 300000)
		{ 
			if(Deal_Bt_Data_flag == 0)
			{
				Deal_Bt_Data_flag = 1;
				Bt_Set_Flow_Control(FLOW_STOP_DATA);
				deal_BT_Pic_Addr();
			}
		}
	}
}


/**
 * cmd：A6. return printer state to app
 *
 */
void Bt_Set_Lcd_Screen(const u8 *data, int len)
{

	

}

/**
 * cmd：A1. feed paper
 *
 */
void Bt_Feed(const u8 *data, int len)
{
	deamon_auto_poweroff(1);
	deamon_screen_save(1);

	SysCtrl.ready_print = 1;
	SysCtrl.ready_print_time = XOSTimeGet();
	
	deg_Printf("bt set lcd screen");
	deg_Printf(" ready print:%d ready time:%d\n",SysCtrl.ready_print,SysCtrl.ready_print_time);

}

void Bt_Set_Printer_State(const u8 *data, int len)
{
	deamon_auto_poweroff(1);
	deamon_screen_save(1);

	deg_Printf("in cmd BC,show len:%d\n",len);
	int i;
	deg_Printf("in cmd BC,data: ");
	for(i=0;i<len;i++)
	{
		deg_Printf("%x ",data[i]);
		if(i==0)
		{
			switch (data[i])
			{
				case 1: SysCtrl.printer_level = 1;
					break;
				case 2: SysCtrl.printer_level = 4;
					break;
				case 3: SysCtrl.printer_level = 7;
					break;

				default:
					SysCtrl.printer_level = 4;
					break;
			}
		}else if(i == 1)
		{
			switch (data[i])
			{
				case 1:SysCtrl.printer_print_mode = 1; // data 1:dot matrix
					break;
				case 2:SysCtrl.printer_print_mode = 0;//gray
					break;

				default:
					SysCtrl.printer_print_mode = 0;
					break;
			}
		}
	}
	deg_Printf("\n");

	deg_Printf("level:%d . mode:%d\n",SysCtrl.printer_level,SysCtrl.printer_print_mode);
	

}


u16 reset_pic_data(void)
{
	set_pic_w(0);
	Pic_Data_offset=0;
	count_printLine = 0;
	deg_Printf("count_printLine[%d] and offset [%d]already reset!\n",count_printLine,Pic_Data_offset);
	return 0;
}


