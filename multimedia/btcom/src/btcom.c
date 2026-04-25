#include "../../../ax32_platform_demo/application.h"
#include "../inc/btcom.h"
#include "../inc/btcom_inner.h"


// 全局数据
static FRAME_MANAGE_T *sFrmManager = NULL;
static FN_BTCMD_PROC  sCmdProc = NULL;


//static u32 test_buf_offset = 0;
static u8 start_jpg_flag = 0;

/**
 * 打印串口数据
 *
 */
static void prvOutputStream(const u8 *data, int len)
{
	// example code
	char buf[100], *p;
	int i, outlen;
	u8 hi, lo;

	p = buf;
	outlen = len > 33 ? 33 : len;
	for (i = 0; i < outlen; i++) {
		hi = (data[i]>>4) & 0x0F;
		lo = data[i] & 0x0F;
		*p++ = (hi >= 10) ? 'A'+(hi-10) : '0'+hi;
		*p++ = (lo >= 10) ? 'A'+(lo-10) : '0'+lo;
		*p++ = ' ';
	}
	*p = '\0';
	deg("STREAM:len=%d,[%s]\n", len,buf);
}


/**
 * 串口数据接收
 *
 */

//u8 tmpdata[1024*300];
static u8 *BT_Pic_Addr = NULL;
static u32 dataLen = 0;

static void prvRxIsr(u8 data)
{
	int i,line;
	//uart_Printf("data = %x \n",data);
	if (sFrmManager != NULL) 
	{
		// 先分配一个接收buffer
		if (sFrmManager->working == NULL) 
		{
			btcomRecycleBuf(sFrmManager);
			if (btcomGetIdleBuf(sFrmManager) == false) 
			{
				deg_Printf("[RxIsr]No buf left\n");
				return;
			}
		}
		
		// 超时检测
		static uint32 lasttick = 0;
		uint32 tick = XOSTimeGet();
		FRAME_BUF_T *buf = sFrmManager->working;
		if (lasttick > 0 && tick > lasttick && (tick - lasttick) >= 5000 && buf->len > 0) {
			//deg_Printf("Timeout data:");
			//prvOutputStream(buf->data, buf->len);
			buf->len = 0;
		}
		lasttick = tick;

		
#if 1
		if(start_jpg_flag==1)
		{	if(SysCtrl.printer_print_mode == 0){
			if (dataLen < JPG_LEN_MAX)	BT_Pic_Addr[dataLen++] = data;
			}
			
		}
		else if(start_jpg_flag == 2)
		{

		} 
#endif

		// 是否数据部份
		if (buf->len >= 6) {
			buf->data[buf->len++] = data;
			// 判断是否接收完一帧数据
			if (buf->len >= buf->total_len) {
			//	uart_Printf("btcomPutBusy dataLen=%d \n",dataLen);
				btcomPutBusy(sFrmManager);
			}
		} else if (buf->len == 5) {
			if(buf->data[2] == 0xBC)
			{
				//buf->data[buf->len++] = data;
				buf->data[buf->len++]=0x00;
				buf->total_len = 8 + ((0x00<<8)|(0x02)); // 2byte
			}else
			{
				buf->data[buf->len++] = data;
				// 计算帧长度
				buf->total_len = 8 + ((buf->data[5]<<8)|(buf->data[4]));
				if (buf->total_len > sizeof(buf->data)) {
					//deg_Printf("[RxIsr]length err(%d)\n",buf->total_len);
					buf->len = 0;
				}
			}
		} else if (buf->len >= 3) {
			buf->data[buf->len++] = data;
		} else if (buf->len == 2) {
			if (btcomCmdSupport(data)) {
				//deg_Printf("data:%x\n",data);
				buf->data[buf->len++] = data;
				if(data == 0xA2)
				{
					//if(SysCtrl.printer_print_mode == 0)
					{
						start_jpg_flag = 1;
						//memset(tmpdata,0,300*1024);
						//dataLen=0;
					}
				}
				else if(data == 0xBF)
				{
					if(SysCtrl.printer_print_mode == 1)
					{
						start_jpg_flag = 1;
					}
				}
				else if(data == 0xA1)
				{
					uart_Printf("end jieshou shuju \n");
					//if(SysCtrl.printer_print_mode == 0)
					{
						start_jpg_flag = 0;
						SysCtrl.ready_print = 1;
						SysCtrl.ready_print_time = XOSTimeGet();
						ax32xx_sysDcacheWback((u32)BT_Pic_Addr,dataLen);
					}
				}else if(data == 0xBC)
				{
					buf->data[buf->len++] = 0x00;
				}
			} else {
				buf->len = 0;
				//deg_Printf("[RxIsr]unknown cmd:%x\n", data);
			}
		} else if (buf->len == 1) {
			if (data == 0x78) {
				buf->data[buf->len++] = data;
				
			} else {
				buf->len = 0;
				//deg_Printf("[RxIsr]second byte is not 0x78[0x%X]\n", data);
			}
		} else /*if (buf->len == 0)*/ {
			if (data == 0x51) {
				buf->data[buf->len++] = data;
//				buf->start_tick = XOSTimeGet();
			}
		}

	}
}

/**
 * 蓝牙通讯初始化
 *
 * fnCmdProc - 处理命令的回调程序
 *
 */
void btcomInit(FN_BTCMD_PROC fnCmdProc)
{
	// 创建接收buffer
	sFrmManager = btcomBufCreate();
	if (sFrmManager == NULL) {
		return;
	}
	BT_Pic_Addr = hal_sysMemMalloc(JPG_LEN_MAX, 64);
	if (BT_Pic_Addr == NULL) {
		btcomBufDestroy(sFrmManager);
		sFrmManager = NULL;
		return ;
	}
	memset(BT_Pic_Addr,0,sizeof(BT_Pic_Addr));
	dataLen = 0;
	// 命令处理callback
	sCmdProc = fnCmdProc;

	// 命令排序
	btcomParserInit();

	// 注删蓝牙数据接收
	hal_uartRXIsrRegister(prvRxIsr);
	hal_uartInit();
}

/**
 * 蓝牙通讯反初始化
 *
 */
void btcomUninit(void)
{
	// 不再接收数据
	hal_uartRXIsrRegister(NULL);

	// 释放内存
	if (sFrmManager != NULL) {
		btcomBufDestroy(sFrmManager);
		sFrmManager = NULL;
	}
	if (BT_Pic_Addr != NULL) {
		hal_sysMemFree(BT_Pic_Addr);
		BT_Pic_Addr = NULL;
	}

	sCmdProc = NULL;
}

/**
 * 蓝牙通讯任务处理
 *
 */
void btcomService(void)
{
	// 未进行蓝牙通讯
	if (sFrmManager == NULL) {
		return;
	}

	// 数据处理
	FRAME_BUF_T *buf = btcomTestBusyBuf(sFrmManager);
	if (buf != NULL) {
		u8 *data = buf->data+6;
		int data_len = buf->total_len-8;
#if 0
		u32 chksum;
		u8 crc;

		crc = buf->data[buf->total_len-2];
		chksum = btcomCheckSum(data, data_len, 0);
		if ((chksum&0xFF) == crc)
#endif
		{
			prvOutputStream(buf->data,buf->total_len);
			if (sCmdProc != NULL) {
				sCmdProc(buf->data[2], data, data_len);
			}
		}
		btcomMarkConsumed(buf);
	}
}

/**
 * 指令处理结果的回复
 *
 * cmd - 指令
 * data - 回复的数据
 * len - 数据长度
 *
 * 返回:true - 支持；false - 不支持
 *
 */
void btcomCmdRsp(BTCOM_CMD_E cmd, const u8 *data, int len)
{
	const u8 *fragment[3];
	int frag_len[3];
	u8 header[6];
	u8 tail[2];
	int i, j;
//    0   1     2     3     4     5    6      7     8
//{0x51,0x78 ,0xAE ,0x01 ,0x01 ,0x00 ,0x10 ,0x70 ,0xFF};
//{0x51,0x78 ,0xAE ,0x01 ,0x01 ,0x00 ,0x00 ,0x00 ,0xFF};

	// header
	header[0] = 0x51;
	header[1] = 0x78;
	header[2] = cmd;
	header[3] = 0;
	header[4] = len & 0x0F;
	header[5] = (len>>8) & 0xFF;
	// tail
	tail[0] = (btcomCheckSum(data, len, 0) && 0xFF);
	tail[1] = 0xFF;

	fragment[0] = header;
	frag_len[0] = 6;
	fragment[1] = data;
	frag_len[1] = len;
	fragment[2] = tail;
	frag_len[2] = 2;
	hal_uartIOShareCheck();
	for (i = 0; i < sizeof(fragment)/sizeof(fragment[0]); i++) {
		for (j = 0; j < frag_len[i]; j++) {
			hal_uartSendData(fragment[i][j]);
		}
	}
}

u8 *get_pic_add(void)
{
	if(BT_Pic_Addr)
		return BT_Pic_Addr;
	return NULL;
}

u32 get_pic_w(void)
{
	return dataLen;
}

void set_pic_w(u32 pic_w)
{
	dataLen = pic_w;
}

void deal_BT_Pic_Addr(void)
{
	//start_jpg_flag = 0;
	SysCtrl.ready_print = 1;
	SysCtrl.ready_print_time = XOSTimeGet();
	ax32xx_sysDcacheWback((u32)BT_Pic_Addr,dataLen);
}

U8 get_BT_start_flag(void)
{
	return start_jpg_flag;
}

U8 set_BT_start_flag(u8 value)
{
	start_jpg_flag = value;
}




