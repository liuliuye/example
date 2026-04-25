#ifndef __BTCOM__
#define __BTCOM__

// 所有支持的命令
typedef enum {
	BTCOM_NONE = 0,
	BTCOM_SET_BTNAME          = 0x02,
	BTCOM_GET_STATE           = 0xA3,
	BTCOM_SET_QUALITY         = 0xA4,
	BTCOM_SEND_BITMAP         = 0xA5,
	BTCOM_FEED                = 0xA1,
	BTCOM_SEND_LINE_DATA      = 0xA2,
	BTCOM_GET_DEVINFO         = 0xA8,
	BTCOM_FLOW_CONTROL        = 0xAE,
	BTCOM_SET_ENERGE25        = 0xAF,
	BTCOM_SEND_LINE_DATA_ZIP  = 0xBF,
	BTCOM_SET_PRINT_SPEED     = 0xBE,
	BTCOM_SET_MOTOR_STEP_TIME = 0xBD,
	BTCOM_PRINT_IGNORE        = 0xBC,
	BTCOM_SET_LED_SCREEN      = 0xA6,
	BTCOM_SET_FLAG_BIT        = 0xBC,
	BTCOM_WRITE_DEVICE_ID     = 0xBB,
	// <app指令发送序列.doc>中列出的指令
	BTCOM_SET_DENSITY         = 0xF2,
}BTCOM_CMD_E;


/**
 * 命令的处理函数
 *
 * cmd - 命令
 * data - 数据
 * len - 数据长度
 *
 */
typedef void (*FN_BTCMD_PROC)(BTCOM_CMD_E cmd, const u8 *data, int len);
#define JPG_LEN_MAX (1024*300)


/**
 * 蓝牙通讯初始化
 *
 * fnCmdProc - 处理命令的回调程序
 *
 */
void btcomInit(FN_BTCMD_PROC fnCmdProc);

/**
 * 蓝牙通讯反初始化
 *
 */
void btcomUninit(void);

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
void btcomCmdRsp(BTCOM_CMD_E cmd, const u8 *data, int len);

/**
 * 蓝牙通讯任务处理
 *
 */
void btcomService(void);


u8 *get_pic_add(void);

u32 get_pic_w(void);

void set_pic_w(u32 pic_w);

#endif // __BTCOM__

