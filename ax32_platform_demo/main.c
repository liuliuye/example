
/****************************************************************************
**
 **                              MAIN 
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MAIN ENTER  OF SYSTEM
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : main.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : 
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h" 
#include "../hal/inc/hal_uart.h" 
#include "version.h"


System_Ctrl_T SysCtrl;

#if (4 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3};

#elif (18 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4,
 								  RES_FRAME5,RES_FRAME6,RES_FRAME7,RES_FRAME8,RES_FRAME9,
 								  RES_FRAME10,RES_FRAME11,RES_FRAME12,RES_FRAME13,
 								  RES_FRAME14,RES_FRAME15,RES_FRAME16,RES_FRAME17
 								  };
#elif (9 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4,
 								  RES_FRAME5,RES_FRAME6,RES_FRAME7,RES_FRAME8
 								  };
#elif (10 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4,
 								  RES_FRAME5,RES_FRAME6,RES_FRAME7,RES_FRAME8,RES_FRAME9
 								  };
#elif (11 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4,
 								  RES_FRAME5,RES_FRAME6,RES_FRAME7,RES_FRAME8,RES_FRAME9,
 								  RES_FRAME10
 								  };

#elif (15 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4,
								  RES_FRAME5,RES_FRAME6,RES_FRAME7,RES_FRAME8,RES_FRAME9,
								  RES_FRAME10,RES_FRAME11,RES_FRAME12,RES_FRAME13,RES_FRAME14 								  
 								  };

#elif (16 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4,
								  RES_FRAME5,RES_FRAME6,RES_FRAME7,
								  RES_FRAME8,RES_FRAME9,RES_FRAME10,
 								  RES_FRAME11,RES_FRAME12,RES_FRAME13,RES_FRAME14,RES_FRAME15
 								  };
#elif (17 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4,
								  RES_FRAME5,RES_FRAME6,RES_FRAME7,RES_FRAME8,RES_FRAME9,
								  RES_FRAME10,RES_FRAME11,RES_FRAME12,RES_FRAME13,
 								  RES_FRAME14,RES_FRAME15,RES_FRAME16
 								  };

#elif (20 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME10,RES_FRAME11,RES_FRAME12,
								  RES_FRAME13,RES_FRAME14,RES_FRAME15,RES_FRAME16,RES_FRAME17,
								  RES_FRAME18,RES_FRAME19,RES_FRAME2,RES_FRAME3,RES_FRAME4,
 								  RES_FRAME5,RES_FRAME6,RES_FRAME7,RES_FRAME8,RES_FRAME9
 								  };
#elif (22 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4,
 								  RES_FRAME5,RES_FRAME6,RES_FRAME7,RES_FRAME8,RES_FRAME9,
 								  RES_FRAME10,RES_FRAME11,RES_FRAME12,RES_FRAME13,RES_FRAME14,
 								  RES_FRAME15,RES_FRAME16,RES_FRAME17,RES_FRAME18,RES_FRAME19,
 								  RES_FRAME20,RES_FRAME21/*,RES_FRAME22,RES_FRAME23,RES_FRAME24,
 								  RES_FRAME25,RES_FRAME26*/
 								  };

#elif (27 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0,RES_FRAME1,RES_FRAME2,RES_FRAME3,RES_FRAME4,
 								  RES_FRAME5,RES_FRAME6,RES_FRAME7,RES_FRAME8,RES_FRAME9,
 								  RES_FRAME10,RES_FRAME11,RES_FRAME12,RES_FRAME13,RES_FRAME14,
 								  RES_FRAME15,RES_FRAME16,RES_FRAME17,RES_FRAME18,RES_FRAME19,
 								  RES_FRAME20,RES_FRAME21,RES_FRAME22,RES_FRAME23,RES_FRAME24,
 								  RES_FRAME25,RES_FRAME26
 								  };

#else // 20


#endif

static MSG_T sysDeviceQStack[SYSDEVICE_Q_SIZE];
extern sysTask taskPowerOff;
extern sysTask taskVideoRecorde;
extern sysTask taskPhotoEncode;
extern sysTask taskPlayBack;
extern sysTask taskAudioPlayer;
extern sysTask taskUsbDevice;
extern sysTask taskMainMenu;
extern sysTask taskGameMenu;
extern sysTask taskGame;
extern sysTask taskSettingMenu;
extern sysTask taskApp;


#if 0 //(MXWAPP_PRINTER_SUPPORT == 1)
XWork_T *  Mxw_liukong_handle=NULL;
#endif

#ifdef USE_GPIO_CONTROL_LED
u8 Control_GpioLed_OnOff(u8 onoff)
{
	int i;
	
	if(onoff)
	{
		hal_gpioWrite(GPIO_PB,GPIO_PIN0,GPIO_HIGH);//pf11
	}
	else
	{	
		hal_gpioWrite(GPIO_PB,GPIO_PIN0,GPIO_LOW);
	}
	return 0;
}
#endif

#if defined(USE_5050IC_CONTROL_LED)
XWork_T *  Led_5050IC_handle=NULL;
void led5050ICTimeout(void);

void Int_led_timer(void)
{
	if(Led_5050IC_handle == NULL)
	{
		Led_5050IC_handle = XWorkCreate(200*X_TICK,led5050ICTimeout);//10ms
	}
}

void Unint_led_timer(void)
{
	if(Led_5050IC_handle)
	{
		XWorkDestory(Led_5050IC_handle);
		Led_5050IC_handle = NULL;
	}
}
#endif

#ifdef USE_5050IC_CONTROL_LED
static int led_col_index = 0;
static int jj;
int close_5050_led = 0;
int led_5050_isclose = 0;
#define LEDS_NUM	(2)

#ifdef USE_RGB_MODE_DEAL_LED
#define RGB_MAX_LEVEL  0xf0///0x80///最大占空比
#define RGB_STEP       1//8///渐变步进，要能被RGB_MAX_LEVEL整除
#define RGB_STEP1_2    16///渐变步进，要能被RGB_MAX_LEVEL整除
#define RGB_STEP2      4///渐变步进，要能被RGB_MAX_LEVEL整除

#define RGB_LEVEL1      (RGB_MAX_LEVEL/2)///每个灯间隔RGB值，最好为整数
#define RGB_LEVEL1_2    (RGB_MAX_LEVEL/2)///每个灯间隔RGB值，最好为整数
#define RGB_LEVEL2      (RGB_MAX_LEVEL/2)///每个灯间隔RGB值，最好为整数
typedef struct rgb_info_s {
    unsigned int color;
} rgb_info_t;
rgb_info_t g_rgb_info[LEDS_NUM];
#endif

#if 0
#define COLOR_BLACK		(0x000000)
#define COLOR_WHITE		(0xffffff)

#define COLOR_GREEN		(0xff0000)//GREEN GRB

#define COLOR_GREEN_RED1 (0xdf7b07)
#define COLOR_GREEN_RED2 (0xadbc09)
#define COLOR_GREEN_RED3 (0x6ce808)

#define COLOR_RED		(0x00ff00)//RED

#define COLOR_RED_BLUE1 (0x3FBE63)
#define COLOR_RED_BLUE2 (0x4777A5)
#define COLOR_RED_BLUE3 (0x4C32E1)

#define COLOR_BLUE	    (0x0000ff)//BLUE

#define COLOR_BLUE_YELLOW1 (0x4C32E1)
#define COLOR_BLUE_YELLOW2 (0x935AD0)
#define COLOR_BLUE_YELLOW3 (0xB3A7B5)
#define COLOR_BLUE_YELLOW4 (0xD7CD84)

#define COLOR_YELLOW	(0xFDFF55)// 

#define COLOR_YELLOW_PURPLE1 (0xD7CD84)
#define COLOR_YELLOW_PURPLE2 (0xB3A7B5)
#define COLOR_YELLOW_PURPLE3 (0x6891D5)

#define COLOR_PURPLE	(0X096EFF)// 

#define COLOR_PURPLE_ORANGE1 (0X6891D5)
#define COLOR_PURPLE_ORANGE2 (0X81BF99)

#define COLOR_ORANGE    (0X9BF059)

#define COLOR_ORANGE_CHING1    (0XD7CD84)

#define COLOR_CHING     (0XFBA18E) 

#define COLOR_CHING_GREEN1 (0XFE7E6D) 
#define COLOR_CHING_GREEN2 (0XFF5346)
#else
#define COLOR_BLACK		(0x000000)
#define COLOR_WHITE		(0xffffff)

/*                    	        	      
#define COLOR_GREEN_RED1		0xf74d00
#define COLOR_GREEN_RED2		0xee6d00
#define COLOR_GREEN_RED3		0xe48600
#define COLOR_GREEN_RED4		0xd79e00
#define COLOR_GREEN_RED5		0xc6b400
#define COLOR_GREEN_RED6		0xb2c800
#define COLOR_GREEN_RED7		0x9cd900
#define COLOR_GREEN_RED8		0x85e500
#define COLOR_GREEN_RED9		0x6bf000
#define COLOR_GREEN_RED10		0x4bf800
*/

#define COLOR_RED				0x00ff00
#define COLOR_RED_GREEN1		0x22ff00
#define COLOR_RED_GREEN2		0x44ff00
#define COLOR_RED_GREEN3		0x66ff00
#define COLOR_RED_GREEN4		0x88ff00
#define COLOR_RED_GREEN5		0xaaff00
#define COLOR_RED_GREEN6		0xccff00
#define COLOR_RED_GREEN7		0xeeff00
#define COLOR_RED_GREEN8		0xffff00
#define COLOR_RED_GREEN9		0xffee00
#define COLOR_RED_GREEN10		0xffcc00
#define COLOR_RED_GREEN11		0xffaa00	
#define COLOR_RED_GREEN12		0xff8800
#define COLOR_RED_GREEN13		0xff6600
#define COLOR_RED_GREEN14		0xff4400
#define COLOR_RED_GREEN15		0xff2200
#define COLOR_GREEN				0xff0000//GREEN GRB
#define COLOR_GREEN_BLUE1		0xff0022
#define COLOR_GREEN_BLUE2		0xff0044
#define COLOR_GREEN_BLUE3		0xff0066
#define COLOR_GREEN_BLUE4		0xff0088
#define COLOR_GREEN_BLUE5		0xff00aa
#define COLOR_GREEN_BLUE6		0xff00cc
#define COLOR_GREEN_BLUE7		0xff00ee
#define COLOR_GREEN_BLUE8		0xff00ff
#define COLOR_GREEN_BLUE9		0xee00ff
#define COLOR_GREEN_BLUE10		0xcc00ff
#define COLOR_GREEN_BLUE11		0xaa00ff
#define COLOR_GREEN_BLUE12		0x8800ff
#define COLOR_GREEN_BLUE13		0x6600ff
#define COLOR_GREEN_BLUE14		0x4400ff
#define COLOR_GREEN_BLUE15		0x2200ff
#define COLOR_BLUE				0x0000ff

/*
#define COLOR_RED_BLUE1			0x31e940
#define COLOR_RED_BLUE2			0x42d45e
#define COLOR_RED_BLUE3			0x4cbf74
#define COLOR_RED_BLUE4			0x51ab88
#define COLOR_RED_BLUE5			0x539799
#define COLOR_RED_BLUE6			0x5283aa
#define COLOR_RED_BLUE7			0x4f6abe
#define COLOR_RED_BLUE8			0x464fd3
#define COLOR_RED_BLUE9			0x3834e5
#define COLOR_RED_BLUE10		0x261df4
                                      
                                      
#define COLOR_BLUE_YELLOW1		0x4000f9
#define COLOR_BLUE_YELLOW2		0x5d03f2
#define COLOR_BLUE_YELLOW3		0x721dec
#define COLOR_BLUE_YELLOW4		0x8134e5
#define COLOR_BLUE_YELLOW5		0x9047df
#define COLOR_BLUE_YELLOW6		0x9e5cd6
#define COLOR_BLUE_YELLOW7		0xae74cb
#define COLOR_BLUE_YELLOW8		0xc091bd
#define COLOR_BLUE_YELLOW9		0xd2b1a9
#define COLOR_BLUE_YELLOW10		0xe5d28e
                                      
#define COLOR_YELLOW			0xfdff55
                                      
#define COLOR_YELLOW_PURPLE1	0xebeb7f
#define COLOR_YELLOW_PURPLE2	0xdedd95
#define COLOR_YELLOW_PURPLE3	0xd1cfa6
#define COLOR_YELLOW_PURPLE4	0xc4c2b4
#define COLOR_YELLOW_PURPLE5	0xb8b6bf
#define COLOR_YELLOW_PURPLE6	0xabacc9
#define COLOR_YELLOW_PURPLE7	0x9da0d3
#define COLOR_YELLOW_PURPLE8	0x8a93de
#define COLOR_YELLOW_PURPLE9	0x7385e9
#define COLOR_YELLOW_PURPLE10	0x5279f4
                                      
#define COLOR_PURPLE			0x096eff
                                      
#define COLOR_PURPLE_ORANGE1	0x3d78f0
#define COLOR_PURPLE_ORANGE2	0x4f81e5
#define COLOR_PURPLE_ORANGE3	0x5e8bd9
#define COLOR_PURPLE_ORANGE4	0x6b98cb
#define COLOR_PURPLE_ORANGE5	0x78a8ba
#define COLOR_PURPLE_ORANGE6	0x81b8a8
#define COLOR_PURPLE_ORANGE7	0x89c698
#define COLOR_PURPLE_ORANGE8	0x8fd288
#define COLOR_PURPLE_ORANGE9	0x94dd7a
#define COLOR_PURPLE_ORANGE10	0x97e56c
                                      
#define COLOR_ORANGE			0x9bf059

//橘色→浅绿色                                      
#define COLOR_ORANGE_GREEN1		0xa6eb5f
#define COLOR_ORANGE_GREEN2		0xb2e565
#define COLOR_ORANGE_GREEN3		0xc0dd6c
#define COLOR_ORANGE_GREEN4		0xd0d175
#define COLOR_ORANGE_GREEN5		0xdec47d
#define COLOR_ORANGE_GREEN6		0xebb785

//浅绿                                      
#define COLOR_LIGHT_GREEN		0xfba18e
                                      
#define COLOR_LIGHT_GREEN1		0xfc8a78
#define COLOR_LIGHT_GREEN2		0xfd7766
#define COLOR_LIGHT_GREEN3		0xfe6051
#define COLOR_LIGHT_GREEN4		0xff4237
*/
	// 将HSL颜色转换为RGB
#if 1
#include "math.h"
static uint8 outr,outg,outb;

#define TOTAL_STEPS 360//360	// 渐变总步数

static float my_fabs(float x) 
{
    return (x < 0) ? -x : x;
}

// 自定义取模函数
static float my_fmod(float x, float y) 
{
    if (y == 0) return 0;  // 避免除零错误
    return x - (int)(x / y) * y;
}

static unsigned int hsl_to_rgb(float h, float s, float l) 
{
	float c = (1 - my_fabs(2 * l - 1)) * s;
	float x = c * (1 - my_fabs(my_fmod(h / 60, 2) - 1));
	float m = l - c / 2;
	unsigned int outrgb;
	float r, g, b;
	
	if (h >= 0 && h < 60) {
		r = c; g = x; b = 0;
	} else if (h < 120) {
		r = x; g = c; b = 0;
	} else if (h < 180) {
		r = 0; g = c; b = x;
	} else if (h < 240) {
		r = 0; g = x; b = c;
	} else if (h < 300) {
		r = x; g = 0; b = c;
	} else {
		r = c; g = 0; b = x;
	}
	
	outr = (uint8)((r + m) * 255);
	outg = (uint8)((g + m) * 255);
	outb = (uint8)((b + m) * 255);

	if(outr == 0) outr = 0x01;
	if(outg == 0) outg = 0x01;
	if(outb == 0) outb = 0x01;
	
	outrgb = ((outg << 16)|(outr << 8)|(outb << 0));
	//deg_Printf("outg=: %x,outr=: %x,outb: %x,outrgb: %x\n",outg,outr,outb,outrgb);
	
	return outrgb;
}

static unsigned int rainbow_gradient(int step, int total_steps) 
{
    float hue = (float)step / total_steps * 360.0f;
    return hsl_to_rgb(hue, 1.0f, 0.5f);
}

#endif
#endif

#define DIN_PORT	GPIO_PA//GPIO_PF		//5050RGB DIN Port
#define DIN_PIN		GPIO_PIN7//GPIO_PIN11  //5050RGB DIN Pin

unsigned int rgbflow[][LEDS_NUM]={
#if 0
	{COLOR_GREEN,COLOR_GREEN},

	{COLOR_GREEN_RED1,COLOR_GREEN_RED1},
	{COLOR_GREEN_RED2,COLOR_GREEN_RED2},
	{COLOR_GREEN_RED3,COLOR_GREEN_RED3},

	{COLOR_RED,COLOR_RED},

	{COLOR_RED_BLUE1,COLOR_RED_BLUE1},
	{COLOR_RED_BLUE2,COLOR_RED_BLUE2},
	{COLOR_RED_BLUE3,COLOR_RED_BLUE3},

	{COLOR_BLUE,COLOR_BLUE},

	{COLOR_BLUE_YELLOW1,COLOR_BLUE_YELLOW1},
	{COLOR_BLUE_YELLOW2,COLOR_BLUE_YELLOW2},
	{COLOR_BLUE_YELLOW3,COLOR_BLUE_YELLOW3},
	{COLOR_BLUE_YELLOW4,COLOR_BLUE_YELLOW4},

	{COLOR_YELLOW,COLOR_YELLOW},

	{COLOR_YELLOW_PURPLE1,COLOR_YELLOW_PURPLE1},
	{COLOR_YELLOW_PURPLE2,COLOR_YELLOW_PURPLE2},
	{COLOR_YELLOW_PURPLE3,COLOR_YELLOW_PURPLE3},

	{COLOR_PURPLE,COLOR_PURPLE},

	{COLOR_PURPLE_ORANGE1,COLOR_PURPLE_ORANGE1},
	{COLOR_PURPLE_ORANGE2,COLOR_PURPLE_ORANGE2},

	{COLOR_ORANGE,COLOR_ORANGE},

	{COLOR_ORANGE_CHING1,COLOR_ORANGE_CHING1},

	{COLOR_CHING,COLOR_CHING},

	{COLOR_CHING_GREEN1,COLOR_CHING_GREEN1},
	{COLOR_CHING_GREEN2,COLOR_CHING_GREEN2},
#else
	
	/*{COLOR_GREEN_RED1,COLOR_GREEN_RED1},			
	{COLOR_GREEN_RED2,COLOR_GREEN_RED2},			
	{COLOR_GREEN_RED3,COLOR_GREEN_RED3},			
	{COLOR_GREEN_RED4,COLOR_GREEN_RED4},			
	{COLOR_GREEN_RED5,COLOR_GREEN_RED5},			
	{COLOR_GREEN_RED6,COLOR_GREEN_RED6},			
	{COLOR_GREEN_RED7,COLOR_GREEN_RED7},			
	{COLOR_GREEN_RED8,COLOR_GREEN_RED8},			
	{COLOR_GREEN_RED9,COLOR_GREEN_RED9},			
	{COLOR_GREEN_RED10,COLOR_GREEN_RED10},*/

	{COLOR_RED,COLOR_RED},
	
	{COLOR_RED_GREEN1,COLOR_RED_GREEN1},  
	{COLOR_RED_GREEN2,COLOR_RED_GREEN2},  
	{COLOR_RED_GREEN3,COLOR_RED_GREEN3},  
	{COLOR_RED_GREEN4,COLOR_RED_GREEN4},  
	{COLOR_RED_GREEN5,COLOR_RED_GREEN5},  
	{COLOR_RED_GREEN6,COLOR_RED_GREEN6},  
	{COLOR_RED_GREEN7,COLOR_RED_GREEN7},  
	{COLOR_RED_GREEN8,COLOR_RED_GREEN8},  
	{COLOR_RED_GREEN9,COLOR_RED_GREEN10}, 
	{COLOR_RED_GREEN10,COLOR_RED_GREEN11},
	{COLOR_RED_GREEN11,COLOR_RED_GREEN12},
	{COLOR_RED_GREEN12,COLOR_RED_GREEN13},
	{COLOR_RED_GREEN13,COLOR_RED_GREEN14},
	{COLOR_RED_GREEN14,COLOR_RED_GREEN15},
	{COLOR_RED_GREEN15,COLOR_RED_GREEN15},

	{COLOR_GREEN,COLOR_GREEN},
	
	{COLOR_GREEN_BLUE1,COLOR_GREEN_BLUE1},	 
	{COLOR_GREEN_BLUE2,COLOR_GREEN_BLUE2},	 
	{COLOR_GREEN_BLUE3,COLOR_GREEN_BLUE3},	 
	{COLOR_GREEN_BLUE4,COLOR_GREEN_BLUE4},	 
	{COLOR_GREEN_BLUE5,COLOR_GREEN_BLUE5},	 
	{COLOR_GREEN_BLUE6,COLOR_GREEN_BLUE6},	 
	{COLOR_GREEN_BLUE7,COLOR_GREEN_BLUE7},	 
	{COLOR_GREEN_BLUE8,COLOR_GREEN_BLUE8},	 
	{COLOR_GREEN_BLUE9,COLOR_GREEN_BLUE10}, 
	{COLOR_GREEN_BLUE10,COLOR_GREEN_BLUE11},
	{COLOR_GREEN_BLUE11,COLOR_GREEN_BLUE12},
	{COLOR_GREEN_BLUE12,COLOR_GREEN_BLUE13},
	{COLOR_GREEN_BLUE13,COLOR_GREEN_BLUE14},
	{COLOR_GREEN_BLUE14,COLOR_GREEN_BLUE15},
	{COLOR_GREEN_BLUE15,COLOR_GREEN_BLUE15},

	{COLOR_BLUE,COLOR_BLUE},

	/*{COLOR_RED_BLUE1,COLOR_RED_BLUE1},				
	{COLOR_RED_BLUE2,COLOR_RED_BLUE2},				
	{COLOR_RED_BLUE3,COLOR_RED_BLUE3},				
	{COLOR_RED_BLUE4,COLOR_RED_BLUE4},				
	{COLOR_RED_BLUE5,COLOR_RED_BLUE5},				
	{COLOR_RED_BLUE6,COLOR_RED_BLUE6},				
	{COLOR_RED_BLUE7,COLOR_RED_BLUE7},				
	{COLOR_RED_BLUE8,COLOR_RED_BLUE8},				
	{COLOR_RED_BLUE9,COLOR_RED_BLUE9},				
	{COLOR_RED_BLUE10,COLOR_RED_BLUE10},


	{COLOR_BLUE_YELLOW1,COLOR_BLUE_YELLOW1},		
	{COLOR_BLUE_YELLOW2,COLOR_BLUE_YELLOW2},		
	{COLOR_BLUE_YELLOW3,COLOR_BLUE_YELLOW3},		
	{COLOR_BLUE_YELLOW4,COLOR_BLUE_YELLOW4},		
	{COLOR_BLUE_YELLOW5,COLOR_BLUE_YELLOW5},		
	{COLOR_BLUE_YELLOW6,COLOR_BLUE_YELLOW6},		
	{COLOR_BLUE_YELLOW7,COLOR_BLUE_YELLOW7},		
	{COLOR_BLUE_YELLOW8,COLOR_BLUE_YELLOW8},		
	{COLOR_BLUE_YELLOW9,COLOR_BLUE_YELLOW9},		
	{COLOR_BLUE_YELLOW10,COLOR_BLUE_YELLOW10},

	{COLOR_YELLOW,COLOR_YELLOW},

	{COLOR_YELLOW_PURPLE1,COLOR_YELLOW_PURPLE1},	
	{COLOR_YELLOW_PURPLE2,COLOR_YELLOW_PURPLE2},	
	{COLOR_YELLOW_PURPLE3,COLOR_YELLOW_PURPLE3},	
	{COLOR_YELLOW_PURPLE4,COLOR_YELLOW_PURPLE4},	
	{COLOR_YELLOW_PURPLE5,COLOR_YELLOW_PURPLE5},	
	{COLOR_YELLOW_PURPLE6,COLOR_YELLOW_PURPLE6},	
	{COLOR_YELLOW_PURPLE7,COLOR_YELLOW_PURPLE7},	
	{COLOR_YELLOW_PURPLE8,COLOR_YELLOW_PURPLE8},	
	{COLOR_YELLOW_PURPLE9,COLOR_YELLOW_PURPLE9},	
	{COLOR_YELLOW_PURPLE10,COLOR_YELLOW_PURPLE10},

	{COLOR_PURPLE,COLOR_PURPLE},

	{COLOR_PURPLE_ORANGE1,COLOR_PURPLE_ORANGE1},	
	{COLOR_PURPLE_ORANGE2,COLOR_PURPLE_ORANGE2},	
	{COLOR_PURPLE_ORANGE3,COLOR_PURPLE_ORANGE3},	
	{COLOR_PURPLE_ORANGE4,COLOR_PURPLE_ORANGE4},	
	{COLOR_PURPLE_ORANGE5,COLOR_PURPLE_ORANGE5},	
	{COLOR_PURPLE_ORANGE6,COLOR_PURPLE_ORANGE6},	
	{COLOR_PURPLE_ORANGE7,COLOR_PURPLE_ORANGE7},	
	{COLOR_PURPLE_ORANGE8,COLOR_PURPLE_ORANGE8},	
	{COLOR_PURPLE_ORANGE9,COLOR_PURPLE_ORANGE9},	
	{COLOR_PURPLE_ORANGE10,COLOR_PURPLE_ORANGE10},

	{COLOR_ORANGE,COLOR_ORANGE},

	{COLOR_ORANGE_GREEN1,COLOR_ORANGE_GREEN1},
	{COLOR_ORANGE_GREEN2,COLOR_ORANGE_GREEN2},
	{COLOR_ORANGE_GREEN3,COLOR_ORANGE_GREEN3},
	{COLOR_ORANGE_GREEN4,COLOR_ORANGE_GREEN4},
	{COLOR_ORANGE_GREEN5,COLOR_ORANGE_GREEN5},
	{COLOR_ORANGE_GREEN6,COLOR_ORANGE_GREEN6},

	{COLOR_LIGHT_GREEN,COLOR_LIGHT_GREEN},

	{COLOR_LIGHT_GREEN1,COLOR_LIGHT_GREEN1},
	{COLOR_LIGHT_GREEN2,COLOR_LIGHT_GREEN2},
	{COLOR_LIGHT_GREEN3,COLOR_LIGHT_GREEN3},
	{COLOR_LIGHT_GREEN4,COLOR_LIGHT_GREEN4},*/
	
#endif
};

unsigned int rgbblack[][LEDS_NUM]={

	{COLOR_BLACK,COLOR_BLACK},
	{COLOR_BLACK,COLOR_BLACK},
	{COLOR_BLACK,COLOR_BLACK},
	{COLOR_BLACK,COLOR_BLACK},
	{COLOR_BLACK,COLOR_BLACK},
	{COLOR_BLACK,COLOR_BLACK},
	{COLOR_BLACK,COLOR_BLACK},
};

void delay_100ns(u32 dtime)
{
	if(dtime == 0) return;
	volatile int cnt=dtime;
	while(cnt--)
	{
		;
	}
}
void Delay_5ms(u32 n)
{
	volatile u32 cnt=5000*n;
	while(cnt--)
	{
		;
	}
}
static int rcnt =1;
void wr_onepixel(unsigned int color)
{
	unsigned int tmp = color;//(((color&0x00ff00)<<8)|((color&0xff0000)>>8)|(color&0xff));  //color

	// T0H 0码，高电平时间 	0.3us	
	// T0L 0码，低电平时间	0.9us	
	// T1H 1码，高电平时间 	0.9us	
	// T1L 1码，低电平时间	0.3us	
	// RES 帧单位，低电平时间 	200us以上
#if 0
	int i;
	for(i=23; i>=0; i--)
	{
		if ((tmp>>i)&0x01) {
			GPIO_DATA(DIN_PORT) |=  DIN_PIN;
			delay_100ns(10);//9

			GPIO_DATA(DIN_PORT) &= ~(DIN_PIN);
			delay_100ns(2);//3

		} else {
			GPIO_DATA(DIN_PORT) |=  DIN_PIN;
			delay_100ns(2);//3
			
			GPIO_DATA(DIN_PORT) &= ~(DIN_PIN);
			delay_100ns(10);//9
		}
	}

#else
	int i=0,k1=2,k2=10;

	for(i=24; i>=1; i--)
	{
	
		if((i==24)&&rcnt)
		{
			k1 = 0;
			k2 = 3000;			
			rcnt = 0;
		}
		else
		{
			k1 = 2;
			k2 = 10;
		}

		if ((tmp>>(i-1))&0x01) {
			GPIO_DATA(DIN_PORT) |=  DIN_PIN;
			delay_100ns(k2);//9

			GPIO_DATA(DIN_PORT) &= ~(DIN_PIN);
			delay_100ns(k1);//3

		} else {
			GPIO_DATA(DIN_PORT) |=  DIN_PIN;
			delay_100ns(k1);//3
			
			GPIO_DATA(DIN_PORT) &= ~(DIN_PIN);
			delay_100ns(k2);//9
		}
	}
	#endif
}


void wr_trst(void)//Trst
{
	GPIO_DATA(DIN_PORT) &= ~(DIN_PIN);
	//delay_100ns(3000);//300us
	Delay_5ms(1);//50
}

static int led_show_reverse = 0;
static int offcnt = 0;

void led5050ICTimeout(void)
{
	//int j;
	u64 cur_color;
	#ifdef USE_RGB_MODE_DEAL_LED
		hjx_rgb_mode_4_deal();
	#else
		wr_trst();
		//close_5050_led = 0;
		if(close_5050_led==0)
		{
			#if 1
			rcnt =1;
			cur_color = rainbow_gradient(led_col_index,TOTAL_STEPS);
			for(jj=1; jj<11; jj++)
			{
				wr_onepixel(cur_color);
			}
			if(++led_col_index > TOTAL_STEPS)
			{
				led_col_index = 0;
			}
			wr_trst();
			#else
			for(jj=0; jj<LEDS_NUM; jj++)
			{
				wr_onepixel(rgbflow[led_col_index][jj]);
			}

			wr_trst();

			if(led_show_reverse == 0)
			{		
				led_col_index++;
				if(led_col_index>((sizeof(rgbflow)/sizeof(rgbflow[0]))-1))
				{
					led_show_reverse = 1;
					led_col_index--;
				}

			}
			else if(led_show_reverse == 1)
			{
				if(led_col_index > 0)
				{
					led_col_index--;	
				}
				else
				{
					led_show_reverse = 0;
				}					
			}
			#endif
			
			led_5050_isclose  = 0;
		}
		else if(close_5050_led==1)
		{
			//wr_trst();
			//if(led_5050_isclose == 0)
			if(offcnt++ > 1)
			{
				//offcnt = 0;
				rcnt =1;
				for(jj=1; jj<11; jj++)
				{
					wr_onepixel(COLOR_BLACK);
				}
				//wr_trst();
				
				//hal_gpioInit(DIN_PORT,DIN_PIN,GPIO_OUTPUT,GPIO_PULL_DOWN);
				//hal_gpioWrite(DIN_PORT,DIN_PIN,GPIO_LOW);
			}
			led_col_index = 0;
			led_5050_isclose = 1;
		}			
	#endif
}
u8 Control_5050Led_OnOff(u8 onoff)
{
	//int i;
	
	if(onoff)
	{
		close_5050_led = 0;
	}
	else
	{	
		close_5050_led = 1;
	}
	return 0;
}

#ifdef USE_RGB_MODE_DEAL_LED
void rgb_set_all_led(void)
{
	wr_trst();
    for(jj=0; jj<LEDS_NUM; jj++){
		wr_onepixel(g_rgb_info[jj].color);
    }
	wr_trst();
}

void rgb_mode1_buff_deal(u8 led,u32 cnt)
{
    if(cnt<RGB_MAX_LEVEL){
        g_rgb_info[led].color = ((0 << 16)|(RGB_MAX_LEVEL << 8)|cnt);
    }else if(cnt<(2*RGB_MAX_LEVEL)){
        g_rgb_info[led].color = ((0 << 16)|((2*RGB_MAX_LEVEL-cnt) << 8)|RGB_MAX_LEVEL);
    }else if(cnt<(3*RGB_MAX_LEVEL)){
        g_rgb_info[led].color = (((cnt-2*RGB_MAX_LEVEL) << 16)|(0 << 8)|RGB_MAX_LEVEL);
    }else if(cnt<(4*RGB_MAX_LEVEL)){
        g_rgb_info[led].color = ((RGB_MAX_LEVEL << 16)|(0 << 8)|(4*RGB_MAX_LEVEL-cnt));
    }else if(cnt<(5*RGB_MAX_LEVEL)){
        g_rgb_info[led].color = ((RGB_MAX_LEVEL << 16)|((cnt-4*RGB_MAX_LEVEL) << 8)|0);
    }else if(cnt<(6*RGB_MAX_LEVEL)){
        g_rgb_info[led].color = (((6*RGB_MAX_LEVEL-cnt) << 16)|(RGB_MAX_LEVEL << 8)|0);
    }
}

void hjx_rgb_mode_1_deal(void)
{
    static u16 cnt=0;
    static u16 temp=0;
    static u16 temp1=0;

    u8 i;
    cnt++;
    //if(cnt>=speed_cnt)
    {
        cnt=0;
        //temp++;
        temp=temp+RGB_STEP;
        if(temp>=6*RGB_MAX_LEVEL){
            temp=0;
        }

		
        for(i = 0; i < LEDS_NUM;i++){
            temp1 = temp + i*RGB_LEVEL1;
            if(temp1>=6*RGB_MAX_LEVEL){
                temp1=temp1%(6*RGB_MAX_LEVEL);
            }

            //rgb_mode1_buff_deal(HJX_RGB_LED_NUM-1-i,temp1);
            rgb_mode1_buff_deal(i,temp1);
			//rgb_set_all_led(i);
        }
		
		wr_trst();
        for(jj=0; jj<LEDS_NUM; jj++){
			wr_onepixel(g_rgb_info[0].color);
        }
		wr_trst();
    }
}

static u8 hxd_flag = 0;
static u8 hxd_r_cnt = 0;
static u8 hxd_g_cnt = 0;
static u8 hxd_b_cnt = 0;
void hjx_rgb_mode_3_deal(void)
{
    #define RGB_MODE1_LEVEL   1//2//3///渐变等级
    #define RGB_MODE1_MAX_LEVEL 0xff///灯的最大等级，总等级0xff
    u8 i;

    if(hxd_flag==0){
        if(hxd_r_cnt>(RGB_MODE1_MAX_LEVEL-RGB_MODE1_LEVEL)){
            hxd_r_cnt=RGB_MODE1_MAX_LEVEL;
            hxd_flag=1;
        }else{
            hxd_r_cnt=hxd_r_cnt+RGB_MODE1_LEVEL;
        }
    }else if(hxd_flag==1){
        if(hxd_g_cnt>(RGB_MODE1_MAX_LEVEL-RGB_MODE1_LEVEL)){
            hxd_g_cnt=RGB_MODE1_MAX_LEVEL;
            hxd_flag=2;
        }else{
            hxd_g_cnt=hxd_g_cnt+RGB_MODE1_LEVEL;
        }
    }else if(hxd_flag==2){
        if(hxd_r_cnt<RGB_MODE1_LEVEL){
            hxd_r_cnt=0;
            hxd_flag=3;
        }else{
            hxd_r_cnt=hxd_r_cnt-RGB_MODE1_LEVEL;
        }
    }else if(hxd_flag==3){
        if(hxd_b_cnt>(RGB_MODE1_MAX_LEVEL-RGB_MODE1_LEVEL)){
            hxd_b_cnt=RGB_MODE1_MAX_LEVEL;
            hxd_flag=4;
        }else{
            hxd_b_cnt=hxd_b_cnt+RGB_MODE1_LEVEL;
        }
    }else if(hxd_flag==4){
        if(hxd_g_cnt<RGB_MODE1_LEVEL){
            hxd_g_cnt=0;
            hxd_flag=5;
        }else{
            hxd_g_cnt=hxd_g_cnt-RGB_MODE1_LEVEL;
        }
    }else if(hxd_flag==5){
        if(hxd_r_cnt>(RGB_MODE1_MAX_LEVEL-RGB_MODE1_LEVEL)){
            hxd_r_cnt=RGB_MODE1_MAX_LEVEL;
            hxd_flag=6;
        }else{
            hxd_r_cnt=hxd_r_cnt+RGB_MODE1_LEVEL;
        }
    }else if(hxd_flag==6){
        if(hxd_b_cnt<RGB_MODE1_LEVEL){
            hxd_b_cnt=0;
            hxd_flag=1;
        }else{
            hxd_b_cnt=hxd_b_cnt-RGB_MODE1_LEVEL;
        }
    }else{
        hxd_r_cnt=RGB_MODE1_MAX_LEVEL;
        hxd_g_cnt=0;
        hxd_b_cnt=0;
        hxd_flag=1;
    }

    for (i = 0; i < LEDS_NUM; i++) {
        ///g_rgb_info[i].color = 0x0;///清所有灯数据

        g_rgb_info[i].color=((hxd_r_cnt << 16)|(hxd_g_cnt << 8)|hxd_b_cnt);
    }

   // rgb_set_all_led();

	wr_trst();
    for(jj=0; jj<LEDS_NUM; jj++){
		wr_onepixel(g_rgb_info[jj].color);
    }
	wr_trst();

}

void hjx_rgb_mode_5_deal(void)
{
    #define MIN_DUTY  0x10
    #define MAX_DUTY  0xff
    #define DUTY_LEVEL 1
    static u8 up_flag=1;
    static u8 duty_cnt=MIN_DUTY;
    static u16 cnt=0;
    static u16 temp=0;
    static u16 temp1=0;
    u8 led_r_per,led_g_per,led_b_per;

    u8 i;


    cnt++;
    //if(cnt>=speed_cnt)
    {
        cnt=0;

        if (up_flag) {
            if (duty_cnt < MAX_DUTY-DUTY_LEVEL) {
                duty_cnt=duty_cnt+DUTY_LEVEL;
            } else {
                duty_cnt=MAX_DUTY;
                up_flag=0;
            }
        } else {
            if (duty_cnt <= MIN_DUTY+DUTY_LEVEL) {
                duty_cnt = MIN_DUTY;
                up_flag = 1;
            } else {
                duty_cnt = duty_cnt - DUTY_LEVEL;
            }
        }

    #if 0
        //temp++;
        temp=temp+RGB_STEP2;
        if(temp>=6*RGB_MAX_LEVEL){
            temp=0;
        }
    #endif

        for(i = 0; i < LEDS_NUM;i++){
            temp1 = temp + i*RGB_LEVEL2;
            if(temp1>=6*RGB_MAX_LEVEL){
                temp1=temp1%(6*RGB_MAX_LEVEL);
            }

            //rgb_mode1_buff_deal(HJX_RGB_LED_NUM-1-i,temp1);
            rgb_mode1_buff_deal(i,temp1);

            led_g_per = (((u8)(g_rgb_info[i].color >> 8) & 0xff)*duty_cnt/MAX_DUTY);
            led_r_per = (((u8)(g_rgb_info[i].color >> 16) & 0xff)*duty_cnt/MAX_DUTY);
            led_b_per = (((u8)(g_rgb_info[i].color) & 0xff)*duty_cnt/MAX_DUTY);
            g_rgb_info[i].color=((led_r_per << 16)|(led_g_per << 8)|led_b_per);
        }

       // rgb_set_all_led();
       	wr_trst();
	    for(jj=0; jj<LEDS_NUM; jj++){
			wr_onepixel(g_rgb_info[jj].color);
	    }
		wr_trst();

    }
}

void hjx_rgb_mode_4_deal(void)
{
    #define MIN_DUTY  0x10
    #define MAX_DUTY  0xff
    #define DUTY_LEVEL 1
    static u8 up_flag=1;
    static u8 duty_cnt=MIN_DUTY;
    static u8 rgb_num=0;
    u32 rgb_tab[]={0xff0000,0x00ff00,0x0000ff,0xffff00,0x00ffff,0xff00ff};
    u8 i;

    if (up_flag) {
        if (duty_cnt < MAX_DUTY-DUTY_LEVEL) {
            duty_cnt=duty_cnt+DUTY_LEVEL;
        } else {
            duty_cnt=MAX_DUTY;
            up_flag=0;
        }
    } else {
        if (duty_cnt <= MIN_DUTY+DUTY_LEVEL) {
            duty_cnt = MIN_DUTY;
            up_flag = 1;

            rgb_num++;
            if(rgb_num>=sizeof(rgb_tab)/sizeof(rgb_tab[0])){
                rgb_num=0;
            }
        } else {
            duty_cnt = duty_cnt - DUTY_LEVEL;
        }
    }

    for (i = 0; i < LEDS_NUM; i++) {
        g_rgb_info[i].color = rgb_tab[rgb_num]*duty_cnt/MAX_DUTY;///0x0;
    }

    rgb_set_all_led();

}


#endif
	
#endif

extern msgDealInfor sysUnshieldedMsgDeal[];
void registerTask(void)
{
	taskRegister(TASK_POWER_OFF,&taskPowerOff);
	taskRegister(TASK_VIDEO_RECORD,&taskVideoRecorde);
	taskRegister(TASK_PHOTO_ENCODE,&taskPhotoEncode);
	taskRegister(TASK_PLAY_BACK,&taskPlayBack);
	taskRegister(TASK_AUDIO_PLAYER,&taskAudioPlayer);
	taskRegister(TASK_USB_DEVICE,&taskUsbDevice);
	taskRegister(TASK_MAIN_MENU,&taskMainMenu);
	taskRegister(TASK_GAME_MENU,&taskGameMenu);
	taskRegister(TASK_GAME,&taskGame);
	taskRegister(TASK_SETTING_MENU,&taskSettingMenu);
	taskRegister(TASK_APP,&taskApp);
}

bool sysPowerOn=false;
/*******************************************************************************
* Function Name  : init
* Description    : initial system hardware & task
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
extern int uninit(void); //taskPowerOff.c
extern void recordTimeCount1S(void);
XWork_T *  recordIncrease1S=NULL;
extern int usb_bat_draw_first_pic;
#ifdef USE_BOARD_DY42
XWork_T *Indicator_light;
extern int deal_photoApp_fstMsg;
int flicker_flag = 0;
int flicker_on = 0;
int flicker_slow = 0;
int flicker_cnt = 4;
int flicker_total_cnt = 0;
int flicker_is_battery_low = 0;
void IndicatorLight_flicker(void)//指示灯闪烁定时器
{
	if(flicker_on == 1)
	{
		if(flicker_slow == 1)
		{
			flicker_cnt--;
			if(flicker_cnt>0)
				return ;
			else if(flicker_cnt==0)
				flicker_cnt = 4;
		}
		else
		{
			flicker_cnt = 4;
		}

		if(flicker_flag == 0)
		{
			flicker_flag=1;
			ax32xx_VDDGSENEnable(1);
		}
		else if(flicker_flag == 1)
		{
			flicker_flag=0;
			ax32xx_VDDGSENEnable(0);
		}

		if(flicker_is_battery_low == 0)
		{
			flicker_total_cnt++;
			if(flicker_total_cnt>20)
			{
				flicker_on = 0;
				flicker_total_cnt = 0;
				flicker_flag = 1;
				ax32xx_VDDGSENEnable(1);
			}
		}
	}
	else
	{
		if(deal_photoApp_fstMsg==1)
		{
			if(flicker_flag == 0)
			{
				flicker_flag = 1;
				ax32xx_VDDGSENEnable(1);
			}
		}
	}
}

void close_IndicatorLight(void)
{
	XWorkDestory(Indicator_light);
	Indicator_light = NULL;
}
#endif


#if	0// (MXWAPP_PRINTER_SUPPORT == 1)
void app_resume_cam_set(void)
{
	dispLayerSetPIPMode(DISP_PIP_DISABLE);//app_lcdCsiVideoShowStop();
	hal_sysDelayMS(100);
	dispLayerSetPIPMode(SysCtrl.pip_mode);//app_lcdCsiVideoShowStart();
}
#endif

static int FLASH_INIT(void)
{
	#ifdef USE_BOARD_DY79
		configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_OFF);
		hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
		hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_HIGH);
	#elif defined(USE_BOARD_DY152) 
		configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_OFF);
		ax32xx_WKI0OutputEnable(1,0);
	#else
		configSet(CONFIG_ID_CAP_FLASH,R_ID_STR_COM_OFF);
		FLASH_SWITCH(0);
	#endif
	
	configSys(CONFIG_ID_CAP_FLASH);
	userConfigSave();
}

int init(void)
{
   // debgchar('0');
   	debg("****************CLKTUN:%x, OUT %x, IN %x***************\n",CLKTUN,(CLKTUN>>7)&0x1f,CLKTUN&0x1f);
	debg("****************DLLCON:%x, CH1 %x, CH2 %x***************\n",DLLCON,(DLLCON>>25)&0xf,(DLLCON>>21)&0xf);
	debg("CHIP_ID:%x\n",CHIP_ID);
	deg_Printf("read 720P or VGA:[%d]/t read watermark x[%d] y[%d]",RESOLUTION,WATERMAKE_SET_X_POS,WATERMAKE_SET_Y_POS);
	hal_vddWKOEnable(1);  // hold power
    hal_wkiWakeup(0);
	
	#if 0// WKI0_WAKEUP_EN
    	hal_wki0Wakeup(1);
	#endif
	
    SysCtrl.PE0_stat = 0;
	hal_sysInit();	//initial system for free run
#if (IO_PA7_NOT_INPUT)
	hal_uartInit();//注意作为输入的时候不要有串口打印
#else
	hal_gpioInit(GPIO_PA,GPIO_PIN7,GPIO_INPUT,GPIO_PULL_FLOATING);
#endif
	uart_Printf("[V%d.%d]build time:%s\n",VERSION_MAIN,VERSION_SUB,VERSION_TIME);
//----------initial system work queue.work queues are isr callback function	
	taskInit();
	registerTask();
	XOSInit();	
//----------start system work queue for system check & deamon service	
	hal_timerStart(HAL_CFG_OS_TIMER,X_CFG_TICK,XOSTickService);  // 1000 hz 
	#if HAL_CFG_ISP_SUPPORT
	XWorkCreate(15*X_TICK,isp_process_check);  // isp process
	#endif
	recordIncrease1S=XWorkCreate(1000*X_TICK,recordTimeCount1S);
	deg_Printf("system power on\n");
	memset(&SysCtrl,0,sizeof(System_Ctrl_T));
	taskInfor(1);

    DAC_PA_MUTE_INIT();
	DAC_PA_MUTE_OFF();
//----------board initial,LCD,LED,SPI,ADC,DAC....	
	boardInit(NULL);   // initial main board	
//----------get board device ioctrl handler	

	SysCtrl.bfd_battery = boardOpen(DEV_NAME_BATTERY);
//    SysCtrl.bfd_gsensor = boardOpen(DEV_NAME_GSENSOR);	
	SysCtrl.bfd_usb 	= boardOpen(DEV_NAME_USB);
	SysCtrl.bfd_key 	= boardOpen(DEV_NAME_ADKEY);   
    SysCtrl.bfd_sdcard  = boardOpen(DEV_NAME_SDCARD);		
	SysCtrl.bfd_led     = boardOpen(DEV_NAME_LED);
	SysCtrl.bfd_lcd     = boardOpen(DEV_NAME_LCD);
//SysCtrl.bfd_ir 		= boardOpen(DEV_NAME_IR);
//	SysCtrl.bfd_usensor= boardOpen(DEV_NAME_USENSOR);
	SysCtrl.powerflag = POWERON_FLAG_MAX;

	if(SysCtrl.bfd_battery<0)
	{
		//uninit();
	}

//-----------welcome
	deg_Printf("\n");
	deg_Printf(">>---------------------------------------------<<\n");
	deg_Printf(">>- Welcome.BuildWin SZ LTD.CO-----------------<<\n");
	deg_Printf(">>- VIDEO PROJECT TEAM.VISION------------------<<\n");
	deg_Printf(">>- %s --------------------<<\n",SYSTEM_VERSION);
	deg_Printf(">>---------------------------------------------<<\n");
//----------nv fs for read only resource
	nv_init();	
//----------user menu configure set
	userConfigInitial();

	//----------power on flag check
	int ret;
	int32 temp;
	// if(SysCtrl.powerflag==POWERON_FLAG_KEY) // power key is not hold
	// {
	// 	temp = 0;
	// 	boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
	// 	if(temp==0)
	// 	{
	// 		//uninit();
	// 	}
	// }
	// hal_csiEnable(0);
	// XOSTimeDly(50);  //等待一会儿让数据流结束
	// 	_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
	// 	csiSensor->p_fun_adapt.fp_rotate(1);//打开旋转

	// 	hal_csiEnable(1);
	#if (1 == SENSOR_NUM)
	printer_init();
	#endif

	#ifdef SOLVE_PRINTHEAD_POWERON_HEAT
	solve_poweron_prthead_heat();
	#endif
	#if defined(USE_BOARD_DY20)||defined(USE_BOARD_DY05)
	SysCtrl.key_down_long = 0;
	#endif
	SysCtrl.printer_en = configGet(CONFIG_ID_PRINTER_EN);
	SysCtrl.printer_level = configGet(CONFIG_ID_PRINTER_DENSITY);
	SysCtrl.printer_print_mode=configValue2Int(CONFIG_ID_PRINTER_MODE);
	SysCtrl.volume_level=configGet(CONFIG_ID_VOLUME);
	deg_Printf("SysCtrl.printer_en=%d\n",SysCtrl.printer_en);
	deg_Printf("SysCtrl.printer_level=%d\n",SysCtrl.printer_level);
	deg_Printf("SysCtrl.printer_print_mode=%d\n",SysCtrl.printer_print_mode);
	deg_Printf("SysCtrl.volume_level=%d\n",SysCtrl.volume_level);

	#if defined(USB_PWM_LCDBK)
	set_bright_level(configValue2Int(CONFIG_ID_LCD_BRIGHTNESS));
	#endif

	#if defined(USE_BOARD_DY19)
	hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_LOW);
	#endif
	

	#if defined(USE_BOARD_DY42) 
	Indicator_light = XWorkCreate(50*X_TICK,IndicatorLight_flicker);//10ms
	#endif
	//Bt_Control_Led_OnOff(1);
	hal_dacSetVolume(100);

	boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
	if(temp == 0)
	{
		usb_bat_draw_first_pic = 1;
		layout_logo_show(1,0,1);  // power on.music en,do not wait music end	//---->show logo here,can speed start logo show.make user feeling system starting faster
		//layout_logo_show(0,0,1);  // power on.music en,do not wait music end	//---->show logo here,can speed start logo show.make user feeling system starting faster

	}
	else
	{
		layout_logo_show(0,0,2);
	}
	
	yuv_rgb_table();

	
#ifdef USE_5050IC_CONTROL_LED
	hal_gpioInit(GPIO_PA,GPIO_PIN7,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	if(configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
	{
		Control_5050Led_OnOff(1);
	}
	else
	{
		Control_5050Led_OnOff(0);
	}
	Int_led_timer();
#endif 

#ifdef USE_BT_CONTROL_LED
	if(configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
	{
		Bt_Control_Led_OnOff(1);
	}
	else
	{
		Bt_Control_Led_OnOff(0);
	}
#endif	

#ifdef USE_GPIO_CONTROL_LED
	hal_gpioInit(GPIO_PB,GPIO_PIN0,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	if(configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
	{
		Control_GpioLed_OnOff(1);
	}
	else
	{
		Control_GpioLed_OnOff(0);
	}
#endif

#if defined(USE_FLASH_SET)
	FLASH_INIT();//解决复位后开机闪光灯还是开启状态
#endif	

//--------initial resource manager for fs
	managerInit();
//--------initial font  & ui & configure
	fontInit();
    iconInit();
//---------update time RTC time

//	deg_Printf("main : backlight on.%d ms\n",XOSTimeGet()-tick);
//	int fd = boardOpen(DEV_NAME_BATTERY);

	//#if(USE_TEST_FCC == 1)
	//if((USB_STAT_NULL==SysCtrl.usb))
	//{
	//#endif
		boardNameIoctrl("cmos-sensor",IOCTRL_CMOS_INIT,0);
	//#if(USE_TEST_FCC == 1)
	//}
	//#endif
	
//    deg_Printf("main : csi end.%d ms\n",XOSTimeGet()-tick);
	SysCtrl.t_wait = XOSTimeGet();
    DATE_TIME_T *rtcTime = hal_rtcTimeGet();	
	if(/*userConfigInitial() || */(rtcTime->year<2020)) // user configure reset  
	{
    #if DATETIME_LOAD_AUTO >0	
        rtcTime->year = configGet(CONFIG_ID_YEAR);
		rtcTime->month = configGet(CONFIG_ID_MONTH);
		rtcTime->day = configGet(CONFIG_ID_MDAY);
		rtcTime->hour = configGet(CONFIG_ID_HOUR);
		rtcTime->min = configGet(CONFIG_ID_MIN);
		rtcTime->sec = configGet(CONFIG_ID_SEC);
    #else
		rtcTime->year = 2021;
		rtcTime->month =9;
		rtcTime->day = 9;
		rtcTime->hour = 0;
		rtcTime->min = 0;
		rtcTime->sec = 0;
    #endif		
		hal_rtcTimeSet(rtcTime); // default time ->2017/01/01 00:00:00
	}	
    SysCtrl.pip_mode = 0;
	SysCtrl.f_update = 0;
    SysCtrl.avi_list = -1;
	SysCtrl.jpg_list = -1;
	SysCtrl.wav_list = -1;
	SysCtrl.powerOnTime=0;;
	SysCtrl.f_keysound = configValue2Int(CONFIG_ID_KEYSOUND);
	SysCtrl.sysQ = XMsgQCreate(sysDeviceQStack,SYSDEVICE_Q_SIZE);
	SysCtrl.bat_charg_flag = 1;
/*
	SysCtrl.ir_setting= configValue2Int(CONFIG_ID_IR_LED);
	if(2==SysCtrl.ir_setting)
	{
		boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,1);
	}
*/
	SysCtrl.sdcard = SDC_STAT_NULL;//SDC_STAT_UNSTABLE;
	if(configGet(CONFIG_ID_BAT_CHECK_FLAG) == R_ID_STR_COM_ON){
		#ifdef POWER_ON_DELAY_SHOWBAT	
			SysCtrl.battery = BATTERY_STAT_5;
		#else
			SysCtrl.battery = BATTERY_STAT_4;  
		#endif
		deg_Printf("show value: use DEFAULT\n");
		deg_Printf("density:[%d]\n",configValue2Int(CONFIG_ID_PRINTER_DENSITY));
	}else
	{
		SysCtrl.battery = configValue2Int(CONFIG_ID_BAT_OLD);
		deg_Printf("show value: sys[%d]  2value:oldbat[%d] flag[%d]\n",SysCtrl.battery,configValue2Int(CONFIG_ID_BAT_OLD),configValue2Int(CONFIG_ID_BAT_CHECK_FLAG));
	}
	/*configSet(CONFIG_ID_BAT_OLD,R_ID_STR_COM_LEVEL_6);
	//configSys(CONFIG_ID_BAT_OLD);
	userConfigSave();
	deg_Printf("set old bat [%d]\n",configValue2Int(CONFIG_ID_BAT_OLD));*/
//--------initial fs	
	fs_init();
//--------board check ,the first time.
	taskSysScanDev(0); // check board state
//--------wait power on music end.	
    ret = XOSTimeGet();
	volatile int i = 0x08fffff;
	i = 0x08fffff;
    while((audioPlaybackGetStatus() == MEDIA_STAT_PLAY)) // wait music end
	{
		i--;
		if(i==0)
			break;
		if((i&0xffff)==0)
			deg_Printf(".");
	}
	deg_Printf("pass music\n");
	while(XOSTimeGet()-ret <= 1500)	// for check usb 
	{
		XOSTimeDly(2);
		taskSysScanDev(0); // check board state
		hal_wdtClear();
	}
	deg_Printf("pass check usb\n");
	
    INT16U width,height;
    dispLayerInit(DISP_LAYER_OSD0);                                    // enable ui display layer
    dispLayerGetResolution(DISP_LAYER_OSD0,&width,&height); // get osd size
	initDrawBuffWH(width,height);   // intitial ui manager
	deg_Printf("pass initDrawBuffWH\n");
    R_loadResource((void *)User_Icon_Table,R_ICON_MAX);       // load icon table
    winInit();
	deg_Printf("pass winInit\n");
    layout_keysound_load(NULL);
	deg_Printf("pass layout_keysound_load\n");
	
	//if all device is working , baterry may low, must off sysytem
	return 0;
}

/*******************************************************************************
* Function Name  : main
* Description    : 
* Input          : *
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{	
	

      usb_phyreset_as_typeb();  
//--------------power on--------	 
    init();       // system power on configure
	configSystem();
//----fireware upgrade
	upgrade();
/*	

TASK_POWER_OFF	has highest priority,if other place start TASK_POWER_OFF task,
the task started here will be ignored;
*/	
		// hal_csiEnable(0);
		// XOSTimeDly(50);  //等待一会儿让数据流结束
		// _Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
		// csiSensor->p_fun_adapt.fp_rotate(1);//打开旋转
		// hal_csiEnable(1);
	hal_custom_frame_init();

	#ifdef USE_BOARD_DY42
	taskStart(TASK_PHOTO_ENCODE,0);
	#else
	if(USB_STAT_NULL==SysCtrl.usb)
	{
		taskStart(TASK_MAIN_MENU,0);
	}
	else
	{
		taskStart(TASK_USB_DEVICE,USB_DEV_PCCAMERA);
	}
	#endif
	
	unshieldedMsgFuncRegister(sysUnshieldedMsgDeal);
	// LDOCON |= (1<<12);
	// ax32xx_sysLDOSet(SYS_LDO_LSEN,SYS_VOL_V2_8,1);
	sysPowerOn=true;
	XMsgQFlush(SysCtrl.sysQ);
	XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,SysCtrl.sdcard));
#ifndef POWER_ON_DELAY_SHOWBAT	
	XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_BAT,SysCtrl.battery));//开机不发送电量检测,避免出现开机先满电后掉电的情况
#endif	
	if(SysCtrl.usb!=USB_STAT_NULL)
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_USB,SysCtrl.usb));
	taskService();
	//self_test();
	return 2; // for usb upgrade
}
/*******************************************************************************
* Function Name  :  systemService
* Description    : system event service for event and key get
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void systemService(void)
{
	hal_wdtClear();
	taskSysScanDev(1);
	msgDeal();
	drawUIService(false);
}

void sendDrawUIMsg(void)
{
	msgDealByType(SYS_DRAW_UI,winGetCurrent(),0,NULL);
};


