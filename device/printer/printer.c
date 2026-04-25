//#include "../../ax32xx/inc/ax32xx.h"
#include "printer.h"
#include "../../ax32_platform_demo/application.h"
#include "../../multimedia/interface/userInterface.h"

#define  PRINTER_W 	384
#define PRINTER_GRAY_STEP  32	// 64  // 128 ?��?��?y ��?o��
#define LINE_TIME_FIX_EN 	1	// 0: close line point time fix  ,1: open line point time fix
#define HALF_LINE_PRINTER_EN   0 //  0: max one line hot , 1: max half line hot 
//vddcore ����
// #define  PRINTER_POWER_CH   GPIO_PB		
// #define  PRINTER_POWER_PIN  GPIO_PIN6



//==moto==
//#define  MOTO_NFAULT_CH  GPIO_PF		
//#define  MOTO_NFAULT_PIN GPIO_PIN2
#define  MOTO_SLEEP_CH 	 GPIO_PA		
#define  MOTO_SLEEP_PIN	 GPIO_PIN5		// LOW sleep , HIGH run
#define  MOTO_AIN1_CH 	 GPIO_PB//GPIO_PB
#define  MOTO_AIN1_PIN	 GPIO_PIN6
#define  MOTO_AIN2_CH	 GPIO_PF
#define  MOTO_AIN2_PIN	 GPIO_PIN10
#define  MOTO_BIN1_CH	 GPIO_PA
#define  MOTO_BIN1_PIN	 GPIO_PIN3
#define  MOTO_BIN2_CH 	 GPIO_PF
#define  MOTO_BIN2_PIN	 GPIO_PIN11

#define  MOTO_AIN1_HIGH()  		{GPIO_DATA(MOTO_AIN1_CH) |=	(MOTO_AIN1_PIN);}
#define  MOTO_AIN1_LOW()  		{GPIO_DATA(MOTO_AIN1_CH) &=	~(MOTO_AIN1_PIN);}
#define  MOTO_AIN2_HIGH()  		{GPIO_DATA(MOTO_AIN2_CH) |=	(MOTO_AIN2_PIN);}
#define  MOTO_AIN2_LOW()  		{GPIO_DATA(MOTO_AIN2_CH) &=	~(MOTO_AIN2_PIN);}
#define  MOTO_BIN1_HIGH()  		{GPIO_DATA(MOTO_BIN1_CH) |=	(MOTO_BIN1_PIN);}
#define  MOTO_BIN1_LOW()  		{GPIO_DATA(MOTO_BIN1_CH) &=	~(MOTO_BIN1_PIN);}
#define  MOTO_BIN2_HIGH()  		{GPIO_DATA(MOTO_BIN2_CH) |=	(MOTO_BIN2_PIN);}
#define  MOTO_BIN2_LOW()  		{GPIO_DATA(MOTO_BIN2_CH) &=	~(MOTO_BIN2_PIN);}

//==printer==  // 92 ��
#define  PRINTER_STB_NUM  		1//  1 pin , 2pin 
#define  PRINTER_DATA_CH	 GPIO_PE//PB0
#define  PRINTER_DATA_PIN	 GPIO_PIN15
#define  PRINTER_CLK_CH	 	 GPIO_PE
#define  PRINTER_CLK_PIN	 GPIO_PIN13
#define  PRINTER_LATCH_CH	 GPIO_PB
#define  PRINTER_LATCH_PIN	 GPIO_PIN5
#define  PRINTER_TM_CH	 	 GPIO_PA
#define  PRINTER_TM_PIN	 	 GPIO_PIN6
#ifdef USE_BOARD_DY42
#define  PRINTER_PSENSOR_CH	 GPIO_PD
#define  PRINTER_PSENSOR_PIN GPIO_PIN5
#else
#define  PRINTER_PSENSOR_CH	 GPIO_PF//GPIO_PE//GPIO_PA
#define  PRINTER_PSENSOR_PIN GPIO_PIN3//GPIO_PIN0//GPIO_PIN7
#endif

#if (1==PRINTER_STB_NUM)
#if defined(USB_PWM_LCDBK)||defined(USE_BOARD_DY42)
#define  PRINTER_STB_CH	 	 GPIO_PF
#define  PRINTER_STB_PIN	 GPIO_PIN11  
#else
#define  PRINTER_STB_CH	 	 GPIO_PA
#define  PRINTER_STB_PIN	 GPIO_PIN9
#endif
#define  PRINTER_STB_HIGH()  		{GPIO_DATA(PRINTER_STB_CH)|=(PRINTER_STB_PIN);}
#define  PRINTER_STB_LOW()  		{GPIO_DATA(PRINTER_STB_CH)&=~(PRINTER_STB_PIN);}
#else
#define  PRINTER_STB_CH	 	     GPIO_PF
#define  PRINTER_STB_PIN	 	 GPIO_PIN9
#define  PRINTER_STB2_CH	 	 GPIO_PD
#define  PRINTER_STB2_PIN		 GPIO_PIN13
#define  PRINTER_STB_HIGH()  		{GPIO_DATA(PRINTER_STB_CH)|=(PRINTER_STB_PIN);}
#define  PRINTER_STB_LOW()  		{GPIO_DATA(PRINTER_STB_CH)&=~(PRINTER_STB_PIN);}
#define  PRINTER_STB2_HIGH()  		{GPIO_DATA(PRINTER_STB2_CH)|=(PRINTER_STB2_PIN);}
#define  PRINTER_STB2_LOW()  		{GPIO_DATA(PRINTER_STB2_CH)&=~(PRINTER_STB2_PIN);}
#endif

#define  PRINTER_LATCH_HIGH()  		{GPIO_DATA(PRINTER_LATCH_CH)|=(PRINTER_LATCH_PIN);}
#define  PRINTER_LATCH_LOW()  		{GPIO_DATA(PRINTER_LATCH_CH)&=~(PRINTER_LATCH_PIN);}
#define  PRINTER_CLK_HIGH()  		{GPIO_DATA(PRINTER_CLK_CH)|=(PRINTER_CLK_PIN);}
#define  PRINTER_CLK_LOW()  		{GPIO_DATA(PRINTER_CLK_CH)&=~(PRINTER_CLK_PIN);}
#define  PRINTER_DAT_LOW()  		{GPIO_DATA(PRINTER_DATA_CH)&=~(PRINTER_DATA_PIN);}
#define  PRINTER_DAT_HIGH()  		{GPIO_DATA(PRINTER_DATA_CH)|=(PRINTER_DATA_PIN);}
static void printer_set_int(u8 en);
#if defined(USE_5050IC_CONTROL_LED)
extern void Int_led_timer(void);
extern void Unint_led_timer(void);
#endif
static void printer_delay(u32 n)
{
	volatile u32 i=20*n;
	while(i--)
	{
		asm("l.nop");
	}
}


#if 1
	//==n: 25 is 1ms==
	//==n: 50 is 2ms==
	static void printer_moto_delay(u32 n)
	{
		printer_delay(25*n);
	}
#else
	static void printer_moto_delay(u32 n)
	{
		volatile u32 i=500*n;
		while(i--)
		{
			asm("l.nop");
		}
	}
#endif



void printer_init()
{
	//==close printer power==
//==PB6  PB5 handle clean USB setting==
    USB11CON0=0;
    USB11CON1=0;
    USB11CON0 |= (1<<6);//control by soft
    USB11CON1 &= ~(BIT(4)|BIT(6));    //disable dp,dm 120K pullup
    USB11CON1 &= ~(BIT(7)|BIT(5));    //disable dp,dm 15k pulldown
//==end PB6 handle clean USB setting==
	
	// hal_gpioInit(GPIO_PB,GPIO_PIN6,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	// hal_gpioWrite(GPIO_PB,GPIO_PIN6,GPIO_LOW);
	// int cuu = 0;
	// while(1)
	// {
	// 	ax32xx_wdtClear();
	// 	int a = hal_gpioRead(GPIO_PB, GPIO_PIN6);
	// 	cuu++;
	// 	if(cuu > 2000)
	// 	break;
	// 	deg_Printf("---%d--\n", a);
	// }
	
	//==moto==	
#ifdef BT_CONTROL_PRINTSTB	
	Bt_Control_Prt_Power_OnOff(0);
#else	
	LDOCON &= ~(1<<12);//vddsencore 
#endif 	

	// hal_gpioInit(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	// hal_gpioWrite(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_HIGH);
	// printer_delay(5000);
	// hal_gpioInit(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	// hal_gpioWrite(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_LOW);

	hal_gpioInit(MOTO_AIN1_CH,MOTO_AIN1_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_AIN1_CH,MOTO_AIN1_PIN,GPIO_LOW);
	//������PF10 �����������
	hal_gpioInit(MOTO_AIN2_CH,MOTO_AIN2_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_AIN2_CH,MOTO_AIN2_PIN,GPIO_LOW);

	hal_gpioInit(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_LOW);
	hal_gpioInit(MOTO_BIN2_CH,MOTO_BIN2_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN2_CH,MOTO_BIN2_PIN,GPIO_LOW);
	//==printer==
	//hal_gpioInit(PRINTER_PSENSOR_CH,PRINTER_PSENSOR_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	
	hal_gpioInit(PRINTER_DATA_CH,PRINTER_DATA_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_DATA_CH,PRINTER_DATA_PIN,GPIO_LOW);
	hal_gpioInit(PRINTER_CLK_CH,PRINTER_CLK_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_CLK_CH,PRINTER_CLK_PIN,GPIO_LOW);
	hal_gpioInit(PRINTER_LATCH_CH,PRINTER_LATCH_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_LATCH_CH,PRINTER_LATCH_PIN,GPIO_HIGH);

	hal_gpioInit(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_LOW);
	
#if (1==PRINTER_STB_NUM)
	hal_gpioInit(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_LOW);
	deg_Printf("printer stb 1pin\n");
#else
	hal_gpioInit(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_HIGH);
	hal_gpioInit(PRINTER_STB2_CH,PRINTER_STB2_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB2_CH,PRINTER_STB2_PIN,GPIO_HIGH);
	deg_Printf("printer stb 2pin\n");
#endif

	deg_Printf("printer_init\n");
}

void solve_poweron_prthead_heat(void)
{
	hal_gpioInit(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB_CH,PRINTER_STB_PIN,GPIO_LOW);
}

//==return 0: have paper , 1: have not paper==
static s32 printer_get_paper()
{
	#ifdef USE_BOARD_DY42
	u32 ps = hal_gpioRead(GPIO_PD,GPIO_PIN5);
	#else
	u32 ps = hal_gpioRead(PRINTER_PSENSOR_CH,PRINTER_PSENSOR_PIN);
	#endif
	//u32 ps = hal_gpioRead(GPIO_PA,GPIO_PIN7);
	deg("get paper gpio = %d \n",ps);
	//ps = !ps;
	return ps;
}

//===tm val===
//20C = 37K  pull down, 2.54V,val = 789
//30C = 24K  pull down, 2.26V,val = 701
//40C = 15K  pull down, 1.90V,val = 589
//50C = 10K  pull down, 1.57V,val = 487
//60C = 7.5K pull down, 1.34V,val = 415
//70C = 5.2K pull down, 1.05V,val = 325
extern int	printer_tm_flag;
extern int usb_prinr;
static u16 temprature = 0;
static s32 printer_get_tm()//ע��������Ϊ�����˲�Ҫ�������
{
	hal_gpioInit(PRINTER_TM_CH,PRINTER_TM_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	//printer_delay(100);
	s32 tm = hal_adcGetChannel(ADC_CH_PA6);
	temprature = tm;
	deg("tm=%d printer_tm_flag=%d usb_prinr=%d\n",tm,printer_tm_flag,usb_prinr);

	if(printer_tm_flag)
	{
		#ifdef PRINTTEMP_TEST
		if(tm < 180)
		{
			tm = 20;
		}
		else
		{
			printer_tm_flag = 0;
		}
		#else
		tm = 20;
		#endif
	}
	
	if(usb_prinr)
	{
		tm = 20;
	}

	deg_Printf("tm==%d   <269=1,,<177=2,<236=3,<307=4,<403=5,<512=6,<682=7,>682=8  \n",tm);
	#if 0
	//==new printer==
	if(tm < 269 )// very hot , 75C
	{printer_tm_flag=1;
	if(usb_prinr){printer_tm_flag=0;}
      return 1;
	}
	else if(tm < 280)// 65C
	{
		return 2;
	}
	else if(tm < 300)// 55C
	{
		return 3;
	}
	else if(tm < 307)// 45C
	{
		return 4;
	}
	else if(tm < 403)// 35C
	{
		return 5;
	}
	else if(tm < 512)// 25C
	{
		return 6;
	}
	else if(tm < 682)// 10C
	{
		return 7;
	}
	else if(tm >= 785)// very cool ,0C
	{
		return 8;
	}
	#else//ÿ����ӡͷ�������ܲ�һ�� MTP02-E
	#ifdef PRINTTEMP_TEST
	if(tm < 130 )// very hot , 80C   114 
	{ printer_tm_flag=1;
      return 1;
	}
	#else
	if(tm < 114 )// very hot , 80C   114 
	{ 
      return 1;
	}
	#endif
	else if(tm < 177)// 65C
	{
		return 2;
	}
	else if(tm < 236)// 55C
	{
		return 3;
	}
	else if(tm < 307)// 45C
	{
		return 4;
	}
	else if(tm < 403)// 35C
	{
		return 5;
	}
	else if(tm < 512)// 25C
	{
		return 6;
	}
	else if(tm < 682)// 10C
	{
		return 7;
	}
	else if(tm >= 785)// very cool ,0C
	{
		return 8;
	}

	#endif

	return 0;
}

void FLASH_SWITCH(u8 arg)
{
	if(arg){
		LDOCON |= (1<<12);
		ax32xx_sysLDOSet(SYS_LDO_LSEN,SYS_VOL_V3_3,1);
	}
	else {
		LDOCON &= ~(1<<12);
	}
}

//vddcore ����SLEEP
static void SLEEP(u8 arg)
{
	#ifdef BT_CONTROL_PRINTSTB		
		if(arg)
		{
			Bt_Control_Prt_Power_OnOff(1);
		}
		else 
		{
			Bt_Control_Prt_Power_OnOff(0);
		}
	#else
		if(arg){
			LDOCON |= (1<<12);
			ax32xx_sysLDOSet(SYS_LDO_LSEN,SYS_VOL_V3_3,1);
		}
		else {
			LDOCON &= ~(1<<12);
		}
	#endif
}
//==en 0: disable , 1: enable===
static void printer_moto_enable(u8 en)
{
	//==init moto pin==

	
	hal_gpioWrite(MOTO_AIN1_CH,MOTO_AIN1_PIN,GPIO_LOW);
	hal_gpioWrite(MOTO_AIN2_CH,MOTO_AIN2_PIN,GPIO_LOW);
	hal_gpioWrite(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_LOW);
	hal_gpioWrite(MOTO_BIN2_CH,MOTO_BIN2_PIN,GPIO_LOW);

	if(en)
	{
		SLEEP(1);
		//hal_gpioWrite(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_HIGH);
		printer_moto_delay(20);
	}
	else
	{
		SLEEP(0);
		//hal_gpioWrite(MOTO_SLEEP_CH,MOTO_SLEEP_PIN,GPIO_LOW);
	}
}

/*
//==default==
//dir  0 : forward , 1: reserve===
#define MOTO_TIME  50//50
static s32 printer_moto_move_step(u8 dir)
{
	static u8 half_step = 0;

	if(0 == dir)
	{
		if(0 == half_step)
		{
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			half_step = 1;

		}
		else
		{
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();

			half_step = 0;
		}

	}
	else
	{
		if(0 == half_step)
		{
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			half_step = 1;

		}
		else
		{
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			half_step = 0;
		}
	}
}
*/


//==half step==
#ifdef USE_PRINTER_7_4_SPEED
#define MOTO_TIME  50//9
#else
#define MOTO_TIME  50//9
#endif
//dir  0 : moto forward , 1: moto reserve===
//mode 0 : move 1 line ,1: use hot_time move
//hot_time : mode 1 use it 
//gsetp : mode 1 use it
//line_fix_time : mode 1 use it
//ret : 0 : move finish , 1: not finish
//1-2相   ATP2XSP1RTNEN3
static s32 printer_moto_move_step(u8 dir,u8 mode,u32 hot_time,u8 gstep,u32 line_fix_time)
{
	static u8 half_step = 0;
	s32 ret=0;
	if(0==mode)
	{
		if(0 == dir)
		{
			if(0 == half_step)
			{
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				printer_moto_delay(MOTO_TIME);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_LOW();
				MOTO_BIN2_LOW();
				printer_moto_delay(MOTO_TIME);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				printer_moto_delay(MOTO_TIME);
				MOTO_AIN1_LOW();
				MOTO_AIN2_LOW();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				half_step = 1;
			}
			else
			{
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				printer_moto_delay(MOTO_TIME);
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_LOW();
				MOTO_BIN2_LOW();
				printer_moto_delay(MOTO_TIME);
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				printer_moto_delay(MOTO_TIME);
				MOTO_AIN1_LOW();
				MOTO_AIN2_LOW();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				half_step = 0;
			}

		}
		else
		{
			if(0 == half_step)
			{

				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				printer_moto_delay(50);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_LOW();
				MOTO_BIN2_LOW();
				printer_moto_delay(50);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				printer_moto_delay(50);
				MOTO_AIN1_LOW();
				MOTO_AIN2_LOW();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				half_step = 1;
			}
			else
			{
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				printer_moto_delay(50);
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_LOW();
				MOTO_BIN2_LOW();
				printer_moto_delay(50);
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				printer_moto_delay(50);
				MOTO_AIN1_LOW();
				MOTO_AIN2_LOW();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				half_step = 0;
			}
		}
		return ret;
	}else
	{
		static u8 move_finish=0,mstep=0;
		static s32 all_hot_time,moto_delay_time,move_time;
		if(0==gstep)	// start
		{
			#if HALF_LINE_PRINTER_EN
			all_hot_time=hot_time*PRINTER_GRAY_STEP*2;
			#else
			all_hot_time=hot_time*PRINTER_GRAY_STEP;
			#endif
			moto_delay_time=all_hot_time/4;

		#ifdef USE_PRINTER_7_4_SPEED			
			if(moto_delay_time<MOTO_TIME*25)// MOTO_TIME*25 is 1ms ,50 is 2ms
			{
				moto_delay_time=MOTO_TIME*25;
			}
		#else
			if(moto_delay_time<MOTO_TIME*23)// MOTO_TIME*25 is 1ms ,50 is 2ms
			{
				moto_delay_time=MOTO_TIME*23;
			}
		#endif

			move_finish=0;
			mstep=0;
			move_time=hot_time;
		}
		else
		{
			move_time=move_time+hot_time;
			if(move_time>=moto_delay_time)
			{
				mstep++;
				if(mstep>3)	
				{
					mstep=4;
					move_finish=1;
					if(half_step)
					{
						half_step=0;
					}
					else
					{
						half_step=1;
					}
				}
				move_time-=moto_delay_time;
			}
		}

		if(0 == dir)
		{
			if(0 == half_step)
			{
				if(0 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time+line_fix_time);
				}else if(1 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_LOW();
					MOTO_BIN2_LOW();
					printer_delay(hot_time+line_fix_time);
				}else if(2 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time+line_fix_time);
				}else if(3 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_LOW();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time+line_fix_time);
				}
			}
			else
			{
				if(0 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time+line_fix_time);
				}else if(1 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_LOW();
					MOTO_BIN2_LOW();
					printer_delay(hot_time+line_fix_time);
				}else if(2 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time+line_fix_time);
				}else if(3 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_LOW();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time+line_fix_time);
				}
			}

		}else
		{
			if( 0 == half_step)
			{
				if(0 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time+line_fix_time);
				}else if(1 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_LOW();
					MOTO_BIN2_LOW();
					printer_delay(hot_time+line_fix_time);
				}else if(2 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time+line_fix_time);
				}else if(3 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_LOW();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time+line_fix_time);
				}
			}else
			{
				if(0 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time+line_fix_time);
				}else if(1 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_LOW();
					MOTO_BIN2_LOW();
					printer_delay(hot_time+line_fix_time);
				}else if(2 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time+line_fix_time);
				}else if(3 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_LOW();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time+line_fix_time);
				}
			}
		}

		if(move_finish)
		{
			return 0;
		}else
		{
			return 1;
		}
	}
}


/*
//==one step==
//dir  0 : forward , 1: reserve===
#define MOTO_TIME  100//50
static s32 printer_moto_move_step(u8 dir)
{
	if(0 == dir)
	{
		MOTO_AIN1_HIGH();
		MOTO_AIN2_LOW();
		MOTO_BIN1_LOW();
		MOTO_BIN2_HIGH();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_HIGH();
		MOTO_AIN2_LOW();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_LOW();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_LOW();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_LOW();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_LOW();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_LOW();
		MOTO_BIN2_HIGH();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_HIGH();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_HIGH();
	}
	else
	{
		MOTO_AIN1_LOW();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_LOW();
		MOTO_BIN2_HIGH();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_LOW();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_LOW();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_HIGH();
		MOTO_AIN2_LOW();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_LOW();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_HIGH();
		MOTO_AIN2_LOW();
		MOTO_BIN1_LOW();
		MOTO_BIN2_HIGH();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_HIGH();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_HIGH();
	}
}
*/

static void printer_strobe(u32 n)
{
	// u16 counter = n;
	// u8 flag = 0;
#if (1==PRINTER_STB_NUM)

	PRINTER_STB_HIGH();
	printer_delay(n);
	PRINTER_STB_LOW();

	//�в�ͨ
	// while(counter > 0)//�����Ҫ��ô�� ��Ȼ��������
	// {
	// 	if((counter % 3) == 0)
	// 	{
	// 		PRINTER_STB_LOW();
	// 	}
	// 	else
	// 	PRINTER_STB_HIGH()
	// 	printer_delay(1);
	// 	counter -= 1;
	// }
#else

	PRINTER_STB_LOW();
	PRINTER_STB2_LOW();
	printer_delay(n);
	PRINTER_STB_HIGH();
	PRINTER_STB2_HIGH();
	
#endif
}
u8 is_printting = 0;
//==mode : 0 hot n time , 1 hot and moto move==
//==gstep : mode 1 use it==
//==line_fix_time : mode 1 use it==
//==ret : 0 : is moto move line finish , 1: moto moving
static s32 printer_strobe_moto_move(u8 mode,u32 n,u8 gstep,u32 line_fix_time)
{
	s32 ret=0;
#if (1==PRINTER_STB_NUM)
	PRINTER_STB_HIGH();
//	printer_delay(n);
	if(0==mode)
		printer_delay(n);
	else
	{
		ret=printer_moto_move_step(1,1,n,gstep,line_fix_time);
	}

	PRINTER_STB_LOW();
#else
	PRINTER_STB_HIGH();
	PRINTER_STB2_HIGH();
	if(0==mode)
		printer_delay(n);
	else
	{
		ret=printer_moto_move_step(1,1,n,gstep,line_fix_time);
	}
	PRINTER_STB_LOW();
	PRINTER_STB2_LOW();
#endif
	return ret;
}



#if (MXWAPP_PRINTER_SUPPORT == 1)
int app_get_paper_sta = 0;
u8 printer_move_step(void)
{
	int k;

	//if(app_get_paper_sta == 1){
	//	app_get_paper_sta = 0;
	//	return 1;
	//}
	printer_moto_enable(1);
//	for(k = 0;k < 160;k++)//84
	for(k = 0;k < 84;k++)//84
	{
		//printer_moto_move_step(1);
		printer_moto_move_step(1,0,0,0,0);
	}
	//printer_moto_move_step(0);
	//printer_moto_move_step(0,0,0,0);
	printer_moto_enable(0);
	return 0;
}

u8 pi_app=0;
u8 printer_print_app(u8* buf,u16 w,u16 h,u8 level,u8 batteryValue)
{
	s32 tm;
	u32 tm_hot_time=0;
//	u32 tm_fix = 0;
	u32 tm_battery=0; 
	u32 all_hot_time,delay_time;
	s16 i,j,k;
	u8 ret=0;
	
	//if(app_get_paper_sta == 1){
	//	return 1;
	//}

	if(SysCtrl.battery <= 2)
	{
		ret = 15;
		goto APP_PRINTER_END;
	}
	
	//pi_app++;
	#if (2 == SENSOR_NUM)
	printer_init();
	#endif

	hal_gpioInit(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_LOW);

	printer_moto_enable(1);

	hal_gpioInit(MOTO_BIN2_CH,MOTO_BIN2_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN2_CH,MOTO_BIN2_PIN,GPIO_LOW);
	//PA7 ��ֽ�ż��  ��ȥ��TX   �˲���֮��Ҫ�д�ӡ
	ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_POS_NONE);
	hal_gpioInit(PRINTER_PSENSOR_CH,PRINTER_PSENSOR_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);

	hal_gpioInit(MOTO_AIN2_CH,MOTO_AIN2_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_AIN2_CH,MOTO_AIN2_PIN,GPIO_LOW);
	printer_delay(500); 

	if(printer_get_paper())	// check paper
	{
		ret=8;
		goto APP_PRINTER_END;
	}

	/*u8 time_cnt = 8;
	if(pi_app != 1)  //��һ�ν��벻��Ҫ��ʱ
	while(time_cnt--)//��ʱ��ȴ
	{
		PRINTER_STB_LOW();//�����ͣ���ֹһֱ�����ջ�
		hal_sysDelayMS(1000);
		ax32xx_wdtClear();
	}*/

	// 1 - 12 Խ��Խ�� 
	tm = printer_get_tm();//check tm


	if(1 == tm) // hot
	{
		if(usb_prinr)			
		{
			ret =12;
			goto APP_PRINTER_END;
		}

		ret=9;
		goto APP_PRINTER_END;
	}
	#if 1
	if(2 == tm)	
	{
		tm_hot_time=0;
	}
	else if(3 == tm)
	{
		tm_hot_time=4;
	}
	else if(4 == tm)
	{
		tm_hot_time=8;
	}
	else if(5 == tm)
	{
		tm_hot_time=12;
	}
	else if(6 == tm)
	{
		tm_hot_time=16;
	}
	else
	{
		tm_hot_time=18;
	}
	#else
	if(2 == tm)	
	{
		tm_hot_time=0;
	}
	else if(3 == tm)
	{
		tm_hot_time=2;
	}
	else if(4 == tm)
	{
		tm_hot_time=4;
	}
	else if(5 == tm)
	{
		tm_hot_time=8;
	}
	else if(6 == tm)
	{
		tm_hot_time=10;
	}
	else if(7 == tm)
	{
		tm_hot_time=14;
	}
	else if(8 == tm)
	{
		tm_hot_time=20;//16;
	}
	else if(9 == tm)
	{
		tm_hot_time=25;//18;
	}
	else if(10 == tm)
	{
		tm_hot_time=32;//20;
	}
	else if(11 == tm)
	{
		tm_hot_time=36;//25;
	}
	else if(12 == tm)
	{
		tm_hot_time=40;//30
	}
	else
	{
		tm_hot_time=38;//30
	}
	#endif
	//==set battery==
	deg_Printf("batteryValue =%d,???4-(0),3-(4),2-(8),1-(12),0-(16)\n",batteryValue);
	if( batteryValue >= 4)
	{
       tm_battery = 0;
	}
	else if (3 == batteryValue)
	{
       tm_battery = 4;
	}
	else if(2 == batteryValue)
	{
       tm_battery = 8;
	}
	else if(1 == batteryValue)
	{
       tm_battery = 12;
	}
	else if(0 == batteryValue)
	{
       tm_battery = 16;
	}

	//tm_battery = 4;
	//level=0;
	//==set level==
	if(level>6)
	{
       level = 6;
	}

	is_printting = 1;

	if(0)	// DOT_MATRIX
	{
		u8 *p = buf;

		printer_moto_enable(1);

		all_hot_time = (16+tm_battery+tm_hot_time+level*7)*16;
		
		if(all_hot_time >= (16+tm_battery+tm_hot_time+7*7)*16)
		{
			delay_time = 0;
		}
		else
		{
			delay_time = (16+tm_battery+tm_hot_time+7*7)*16 - all_hot_time;
		}

		deg_Printf("????tm_battery=%d,tm_hot_time=%d,level=%d,levelx12=%d\n",tm_battery,tm_hot_time,level,level*12);//h = 384
		deg_Printf("???? ???????all_hot_time==%d\n",all_hot_time);//h = 384
		//==first move 2 line==
		printer_moto_move_step(1,0,0,0,0);
		printer_delay(delay_time+all_hot_time+50);
		printer_moto_move_step(1,0,0,0,0);
		printer_delay(delay_time+all_hot_time+50);

		if(PRINTER_W==w)
		{
			for(j=0;j<h;j++)
			{
				printer_moto_move_step(1,0,0,0,0);
				
				for(i=0;i<PRINTER_W;i++)
				{
					if(*(p+i)<=128)
					{
						PRINTER_DAT_HIGH();
					}
					else
					{
						PRINTER_DAT_LOW();
					}
					PRINTER_CLK_HIGH();
					PRINTER_CLK_LOW();
				}
				PRINTER_LATCH_LOW();
				PRINTER_LATCH_HIGH();

				printer_strobe(all_hot_time);
				//printer_delay(delay_time);//ȷ��ʱ��һ��
				hal_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto APP_PRINTER_END;
				}
			}
		}
		else if(PRINTER_W==h)
		{
			u32 len;
			u32 offset;
			len=w*h;
			for(j=0;j<w;j++)
			{
				p = buf+j;
				printer_moto_move_step(1,0,0,0,0);
				for(offset=len-w;offset<len;offset-=w)
				{
					if(*(p+offset)<=128)
					{
						PRINTER_DAT_HIGH();
					}
					else
					{
						PRINTER_DAT_LOW();
					}
					PRINTER_CLK_HIGH();
					PRINTER_CLK_LOW();
				}
				PRINTER_LATCH_LOW();
				PRINTER_LATCH_HIGH();
				printer_strobe(all_hot_time);
				//printer_delay(delay_time);//ȷ��ʱ��һ��
				hal_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto APP_PRINTER_END;
				}
			}
		}
		else
		{
			ret=10;
			goto APP_PRINTER_END;
		}

		//==set paper align==
		for(i= 0;i<80;i++)//?����?�䨰��???���?������DD
		{
			 printer_moto_move_step(1,0,0,0,0);
			 printer_delay(delay_time+all_hot_time+20);
			 printer_moto_move_step(1,0,0,0,0);
			 printer_delay(delay_time+all_hot_time+20);
		}

		
	}
	else	// GRAY LEVEL
	{
		s16 /*i,j,k,*/pnum;
		s16 gray_step = 256/PRINTER_GRAY_STEP;
		u8 *p = buf;
		printer_moto_enable(1);
		u16 gray_tab[PRINTER_GRAY_STEP];
		for(k=0;k<PRINTER_GRAY_STEP;k++)
		{
			gray_tab[k]=k*gray_step;
		}

		u16 line_point_cnt,line_fix_step;
		u16 line_fix_time[PRINTER_W+1];
#undef PRINTER_GRAY_TIME
#if 1//def PRINTER_3_7V
#define PRINTER_GRAY_TIME  42//16//25//16//32//64//64//96//192//?��?��?����?����??
		#if LINE_TIME_FIX_EN
		#if HALF_LINE_PRINTER_EN
		line_point_cnt=PRINTER_W/2;
		#else
		line_point_cnt=PRINTER_W;
		#endif
		if(2 == tm) 
		{
			tm_hot_time=0;
			line_fix_step=65;
		}
		else if(3 == tm)
		{
			tm_hot_time=8;
			line_fix_step=60;
		}
		else if(4 == tm)
		{
			tm_hot_time=16;
			line_fix_step=55;
		}
		else if(5 == tm)
		{
			tm_hot_time=24;
			line_fix_step=50;
		}
		else if(6 == tm)
		{
			tm_hot_time=32;
			line_fix_step=45;
		}
		else
		{
			tm_hot_time=40;
			line_fix_step=40;
		}

		for(i=0;i<=line_point_cnt;i++)
		{
			if(i>56)
			{
				line_fix_time[i]=(i-56)*10/line_fix_step;
			}
			else
			{
				line_fix_time[i]=0;
			}
		}
		#endif
#else
#define PRINTER_GRAY_TIME  48
#endif

		//all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time+level*12+tm_battery)/2; //there are 4 levels 

		//== 5或以上会比原来更浓
		//if(level>3) 
		//	level+=2;

		#ifdef PRINTHEAD_3_7V
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*15+tm_battery)/2;  //there are 7 levels , but max is same
		if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*15+tm_battery)/2)
		{
			delay_time = 0;
		}
		else
		{
			delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*11+tm_battery)/2 - all_hot_time;
		}
		#else
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*8+tm_battery)*2;  //there are 7 levels , but max is same
        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*8+tm_battery)*2)
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*5+tm_battery)*2 - all_hot_time;
        }
		#endif


		//==first move 2 line==
	/*	printer_moto_move_step(1);
		printer_delay((delay_time+all_hot_time)*PRINTER_GRAY_STEP+50);
		printer_moto_move_step(1);
		printer_delay((delay_time+all_hot_time)*PRINTER_GRAY_STEP+50);  */

		if(PRINTER_W==w)
		{
			for(j=0;j<h;j++)
			{
				//printer_moto_move_step(1);
				ret =1;
				for(k=0;k<PRINTER_GRAY_STEP;k++)
				{
					for(i=0;i<PRINTER_W;i++)
					{
						if(*(p+i)==1)
						{
							PRINTER_DAT_HIGH();
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
	
					// printer_strobe(all_hot_time);
					// printer_delay(delay_time);//ȷ��ʱ��һ��
					if(ret)
						ret = printer_strobe_moto_move(1,all_hot_time,k,0);
					else
						ret = printer_strobe_moto_move(0,all_hot_time,k,0);
				}
				p += w;
				while(ret)
				{
					ret = printer_moto_move_step(1,1,all_hot_time,k,0);
				}

				hal_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto APP_PRINTER_END;
				}
			}
		}
		else if(PRINTER_W==h)
		{
			u32 len;
			len=w*h;
			for(j=0;j<w;j++)
			{
				p = buf+j;
				//printer_moto_move_step(1);
				ret =1;
				for(k=0;k<PRINTER_GRAY_STEP;k++)
				{
					u32 offset;
					for(offset=len-w;offset<len;offset-=w)
					{
						if(*(p+offset)<=gray_tab[k])
						{
							PRINTER_DAT_HIGH();
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
					// printer_strobe(all_hot_time);
					// printer_delay(delay_time);//ȷ��ʱ��һ��
					
					if(ret)
						ret=printer_strobe_moto_move(1,all_hot_time,k,0);
					else
						ret=printer_strobe_moto_move(0,all_hot_time,k,0);					
				}
				// wait moto move finish
				while(ret)
				{
					ret=printer_moto_move_step(1,1,all_hot_time,k,0);
				}

				hal_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto APP_PRINTER_END;
				}
			}
		}
		else
		{
			ret=10;
			goto APP_PRINTER_END;
		}
		//==set paper align==
	/*	for(i= 0;i<80;i++)//?����?�䨰��???���?������DD
		{
			 printer_moto_move_step(1);
			 printer_delay((delay_time+all_hot_time+50)*PRINTER_GRAY_STEP);
			 printer_moto_move_step(1);
			 printer_delay((delay_time+all_hot_time+50)*PRINTER_GRAY_STEP);
		}    */
	}


		/*
			for(i= 0;i<80;i++)//?����?�䨰��???���?������DD
			{
				printer_moto_move_step(1,0,0,0);
				printer_delay((delay_time+all_hot_time+50)*PRINTER_GRAY_STEP);
				printer_moto_move_step(1,0,0,0);
				printer_delay((delay_time+all_hot_time+50)*PRINTER_GRAY_STEP);
			}	
		*/
		if(get_BT_start_flag()==0)
		{
			for(i= 0;i<50;i++)
			{
				printer_moto_move_step(1,0,0,0,0);
				printer_moto_delay(200);
				printer_moto_move_step(1,0,0,0,0);
				printer_moto_delay(200);
			}
		}


	
APP_PRINTER_END:
	//��ԭ
	printer_moto_enable(0);
	hal_adcInit();
	hal_gpioInit(GPIO_PE,GPIO_PIN0,GPIO_INPUT,GPIO_PULLE_FLOATING);
	ax32xx_adcEnable(ADC_CH_PE0,1);
	//��ʼ��Ϊadc����
	// hal_gpioInit(GPIO_PE,GPIO_PIN0,GPIO_INPUT,GPIO_PULL_FLOATING);
	// ax32xx_adcEnable(6, 1);
	
	// SysCtrl.PE0_stat = (1<<1) | (1 << (1 + 4)); 
	//pa7  pf11 tx rx  
    hal_uartInit();
	hal_sysDelayMS(10);
	deg("??? tm_battery=%d,tm_hot_time=%d,level=%d=%d\n",tm_battery,tm_hot_time,level,level*7);//h = 384

	deg("TTT---%d--\n", temprature);
	//PA3���������
	//hal_gpioInit(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	is_printting = 0;

	return ret;

}

#if (MXWAPP_CHECK_PRTER_STA == 1)
void AppPrintCheckPrintSta(int *low_power,int *no_paper,int *is_hot)
{	
	//app_lcdCsiVideoShowStop();
	//dev_ioctrl(SysCtrl.dev_fd_sensor, DEV_SENSOR_UINIT, hardware_setup.cmos_sensor_sel);

	dispLayerSetPIPMode(0);

	printer_set_int(0);
	printer_init();

	GPIO_DATA(PRINTER_POWER_CH) |=  PRINTER_POWER_PIN;

	printer_delay(500);

	if(SysCtrl.dev_stat_battery <= BATTERY_STAT_2){
		*low_power = 1;
	}else {
		*low_power = 0;
	}

	if(printer_get_paper()){
		*no_paper = 1;
	}else{
		*no_paper = 0;
	}

	if(printer_get_tm()==1){
		*is_hot = 1;
	}else{
		*is_hot = 0;
	}
	
	GPIO_DATA(PRINTER_POWER_CH) &= ~PRINTER_POWER_PIN;
	hal_gpioInit(PRINTER_PSENSOR_CH,PRINTER_PSENSOR_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_PSENSOR_CH,PRINTER_PSENSOR_PIN,GPIO_LOW);
	printer_set_int(1);

	//dev_sensor_init();
	//dev_ioctrl(SysCtrl.dev_fd_sensor, DEV_SENSOR_INIT, hardware_setup.cmos_sensor_sel);
	app_resume_cam_set();
}
#endif

//extern int app_tips_type;
int printer_app_buf(u8 *buf,int height)
{
	int print_ret ;

	deg_Printf("======printer_app_buf======\n");
	printer_set_int(0);
	print_ret = printer_print_app((u8* )buf,PRINTER_W,height,SysCtrl.printer_level,SysCtrl.battery);
	printer_set_int(1);

	/*if(8==print_ret)
	{
		app_tips_type = 1;
	}
	else if(9==print_ret)
	{
		app_tips_type = 2;
	}
	else if(12 == print_ret)
	{
		app_tips_type = 3;
	}
	else if(15 == print_ret)
	{
		app_tips_type = 4;
	}*/

	return print_ret;
}

#endif



//==ret : 0 is ok , 8: no paper , 9: tm hot , 10: w h err 15 low bat
//==level    0~4  max~min
//==print mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery 0~4
u8 pi=0;
#ifdef USE_COLOR_PAPER
u8 gray_test_tap[32] =  {0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144,152,160,168,176,184,192,200,208,216,224,232,240,248,256};
#endif

#ifdef USE_BOARD_DY42
extern int flicker_on ;
extern int flicker_slow ;
#endif
u8 printer_print(u8* buf,u16 w,u16 h,u8 level,u8 print_mode,u8 batteryValue)
{
	s32 tm;
	u32 tm_hot_time=0;
//	u32 tm_fix = 0;
	u32 tm_battery=0; 
	u32 all_hot_time,delay_time;
	s16 i,j,k;
	u8 ret=0;
	#if defined(USE_BOARD_DY42)
	if(SysCtrl.battery <= 1)
	#else
	if(SysCtrl.battery <= 2)
	#endif
	{
		ret = 15;
		goto PRINTER_END;
	}
	pi++;
	#if (2 == SENSOR_NUM)
	printer_init();
	#endif

	deg_Printf("after  printer_init!\n");
	
	deg_Printf("=========================[%d]==========================\n",pi);
	deg("w=%d,h=%d,level=%d,print_mode=%d\n",w,h,level,print_mode);
	//��ӡ
	hal_gpioInit(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_LOW);

	printer_moto_enable(1);

	hal_gpioInit(MOTO_BIN2_CH,MOTO_BIN2_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN2_CH,MOTO_BIN2_PIN,GPIO_LOW);
	//PA7 ��ֽ�ż��  ��ȥ��TX   �˲���֮��Ҫ�д�ӡ
	#ifdef USE_APP_PRINTER
		ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_POS_NONE);
	#endif
	hal_gpioInit(PRINTER_PSENSOR_CH,PRINTER_PSENSOR_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);

	hal_gpioInit(MOTO_AIN2_CH,MOTO_AIN2_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_AIN2_CH,MOTO_AIN2_PIN,GPIO_LOW);
	printer_delay(500); 

	if(printer_get_paper())	// check paper
	{
		ret=8;
		goto PRINTER_END;
	}
	deg_Printf("after  check paper!\n");

	
	/*u8 time_cnt = 8;
	if(pi != 1)  //��һ�ν��벻��Ҫ��ʱ
	while(time_cnt--)//��ʱ��ȴ
	{
		PRINTER_STB_LOW();//�����ͣ���ֹһֱ�����ջ�
		hal_sysDelayMS(1000);
		ax32xx_wdtClear();
	}*/

	// 1 - 12 Խ��Խ�� 
	tm = printer_get_tm();//check tm

	
	//deg_Printf("tm=%d,???2-(0),3-(4),4-(8),5-(12),6-(16),7-(18)\n",tm);

	if(1 == tm) // hot
	{	
		if(usb_prinr)			
		{
			ret =12;
			goto PRINTER_END;
		}
			
		ret=9;
		goto PRINTER_END;
	}
	#if 1
	if(2 == tm)	
	{
		tm_hot_time=0;
	}
	else if(3 == tm)
	{
		tm_hot_time=4;
	}
	else if(4 == tm)
	{
		tm_hot_time=8;
	}
	else if(5 == tm)
	{
		tm_hot_time=12;
	}
	else if(6 == tm)
	{
		tm_hot_time=16;
	}
	else
	{
		tm_hot_time=18;
	}
	#else
	if(2 == tm)	
	{
		tm_hot_time=0;
	}
	else if(3 == tm)
	{
		tm_hot_time=2;
	}
	else if(4 == tm)
	{
		tm_hot_time=4;
	}
	else if(5 == tm)
	{
		tm_hot_time=8;
	}
	else if(6 == tm)
	{
		tm_hot_time=10;
	}
	else if(7 == tm)
	{
		tm_hot_time=14;
	}
	else if(8 == tm)
	{
		tm_hot_time=20;//16;
	}
	else if(9 == tm)
	{
		tm_hot_time=25;//18;
	}
	else if(10 == tm)
	{
		tm_hot_time=32;//20;
	}
	else if(11 == tm)
	{
		tm_hot_time=36;//25;
	}
	else if(12 == tm)
	{
		tm_hot_time=40;//30
	}
	else
	{
		tm_hot_time=38;//30
	}
	#endif
	//==set battery==
	deg_Printf("batteryValue =%d,???4-(0),3-(4),2-(8),1-(12),0-(16)\n",batteryValue);
	if( batteryValue >= 4)
	{
       tm_battery = 0;
	}
	else if (3 == batteryValue)
	{
       tm_battery = 4;
	}
	else if(2 == batteryValue)
	{
       tm_battery = 8;
	}
	else if(1 == batteryValue)
	{
       tm_battery = 12;
	}
	else if(0 == batteryValue)
	{
       tm_battery = 16;
	}

	//tm_battery = 4;
	//level=0;
	//==set level==
	if(level>6)
	{
       level = 6;
	}

	is_printting = 1;
	
#if defined(USE_5050IC_CONTROL_LED)
	Unint_led_timer();
#endif

	if(1==print_mode)	// DOT_MATRIX
	{
		u8 *p = buf;

		printer_moto_enable(1);
		//all_hot_time = (32+tm_battery+tm_hot_time+level*12)*20; // - 28 // - 20
		//all_hot_time = (16+tm_battery+tm_hot_time+level*12)*16; //6.1
		//all_hot_time = (16+tm_battery+tm_hot_time+level*4)*16; //6.2
		//all_hot_time = (16+tm_battery+tm_hot_time+level*4)*12; //6.5

		//all_hot_time = (16+tm_battery+tm_hot_time+level*12)*16;//there are 4 levels 
		#if defined(PRINTHEAD_3_7V)
			all_hot_time = (16+tm_battery+tm_hot_time+level*7)*16;//there are 7 levels , but max is same
			
			if(all_hot_time >= (16+tm_battery+tm_hot_time+7*7)*16)
			{
				delay_time = 0;
			}
			else
			{
				delay_time = (16+tm_battery+tm_hot_time+7*7)*16 - all_hot_time;
			}
		#else
			all_hot_time = (42+tm_battery+tm_hot_time+level*15)*23;//there are 7 levels , but max is same
			
			if(all_hot_time >= (42+tm_battery+tm_hot_time+7*15)*23)
			{
				delay_time = 0;
			}
			else
			{
				delay_time = (42+tm_battery+tm_hot_time+7*15)*23 - all_hot_time;
			}
		#endif

		deg_Printf("????tm_battery=%d,tm_hot_time=%d,level=%d,levelx12=%d\n",tm_battery,tm_hot_time,level,level*12);//h = 384
		deg_Printf("???? ???????all_hot_time==%d\n",all_hot_time);//h = 384
		//==first move 2 line==
		//printer_moto_move_step(1);
		printer_moto_move_step(1,0,0,0,0);
		printer_delay((delay_time+all_hot_time)*42+50);
		//printer_moto_move_step(1);
		printer_moto_move_step(1,0,0,0,0);
		printer_delay((delay_time+all_hot_time)*42+50);

		if(PRINTER_W==w)
		{
			for(j=0;j<h;j++)
			{
				//printer_moto_move_step(1);
				printer_moto_move_step(1,0,0,0,0);
				for(i=0;i<PRINTER_W;i++)
				{
					if(*(p+i)<=128)
					{
						PRINTER_DAT_HIGH();
					}
					else
					{
						PRINTER_DAT_LOW();
					}
					PRINTER_CLK_HIGH();
					PRINTER_CLK_LOW();
				}
				PRINTER_LATCH_LOW();
				PRINTER_LATCH_HIGH();

				printer_strobe(all_hot_time);
				//printer_delay(delay_time);//ȷ��ʱ��һ��
				hal_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto PRINTER_END;
				}
			}
		}
		else if(PRINTER_W==h)
		{
			u32 len;
			u32 offset;
			len=w*h;
			for(j=0;j<w;j++)
			{
				p = buf+j;
				printer_moto_move_step(1,0,0,0,0);
				
				for(k=0;k<3;k++)
				{
					for(offset=len-w;offset<len;offset-=w)
					{
						if(*(p+offset)<=128)
						{
							PRINTER_DAT_HIGH();
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
					printer_strobe(all_hot_time);
				}
				hal_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto PRINTER_END;
				}
			}
		}
		else
		{
			ret=10;
			goto PRINTER_END;
		}

	}
	else	// GRAY LEVEL
	{
		s16 /*i,j,k,*/pnum;
		s16 gray_step = 256/PRINTER_GRAY_STEP;
		u8 *p = buf;
		printer_moto_enable(1);
		u16 gray_tab[PRINTER_GRAY_STEP];
		for(k=0;k<PRINTER_GRAY_STEP;k++)
		{
			#ifdef USE_COLOR_PAPER
			gray_tab[k] = gray_test_tap[k];
			#else
			gray_tab[k]=k*gray_step;
			#endif
		}

		u16 line_point_cnt,line_fix_step;
		u16 line_fix_time[PRINTER_W+1];

#undef PRINTER_GRAY_TIME
#if 1//def PRINTER_3_7V
#define PRINTER_GRAY_TIME  42//16//25//16//32//64//64//96//192//?��?��?����?����??
		#if LINE_TIME_FIX_EN
		#if HALF_LINE_PRINTER_EN
		line_point_cnt=PRINTER_W/2;
		#else
		line_point_cnt=PRINTER_W;
		#endif
		if(2 == tm) 
		{
			tm_hot_time=0;
			line_fix_step=65;
		}
		else if(3 == tm)
		{
			tm_hot_time=8;
			line_fix_step=60;
		}
		else if(4 == tm)
		{
			tm_hot_time=16;
			line_fix_step=55;
		}
		else if(5 == tm)
		{
			tm_hot_time=24;
			line_fix_step=50;
		}
		else if(6 == tm)
		{
			tm_hot_time=32;
			line_fix_step=45;
		}
		else
		{
			tm_hot_time=40;
			line_fix_step=40;
		}

		for(i=0;i<=line_point_cnt;i++)
		{
			if(i>56)
			{
				line_fix_time[i]=(i-56)*10/line_fix_step;
			}
			else
			{
				line_fix_time[i]=0;
			}
		}
		#endif
#else
#define PRINTER_GRAY_TIME  48
#endif

		//all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time+level*12+tm_battery)/2; //there are 4 levels 

		//== 5或以上会比原来更浓
		//if(level>3) 
		//	level+=2;
		#if defined(DY05_OPOS_PRINTHEAD_3_7V)
	    all_hot_time = (PRINTER_GRAY_TIME-10+tm_hot_time*2+level*1+tm_battery)*3/2;
        if(all_hot_time >= (PRINTER_GRAY_TIME-10+tm_hot_time*2+7*1+tm_battery)*3/2)
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME-10+tm_hot_time*2+7*1+tm_battery)*3/2- all_hot_time;
        }
		#elif defined(DY43_PRINTHEAD_3_7V)
		/*all_hot_time = (PRINTER_GRAY_TIME-10+tm_hot_time*2+level*1+tm_battery)*3/2;
        if(all_hot_time >= (PRINTER_GRAY_TIME-10+tm_hot_time*2+7*1+tm_battery)*3/2)
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME-10+tm_hot_time*2+7*1+tm_battery)*3/2- all_hot_time;
        }*/	
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*15+tm_battery)*1;//1  //34/25 there are 7 levels , but max is same
        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*15+tm_battery)*1)  //level*3+tm_battery)*2
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*15+tm_battery)*1 - all_hot_time;
        }
		#elif defined(DY152_PRINTHEAD_7_4V)
				all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*15+tm_battery)*1;//1  //34/25 there are 7 levels , but max is same
        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*15+tm_battery)*1)  //level*3+tm_battery)*2
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*15+tm_battery)*1 - all_hot_time;
        }
		#elif defined(PRINTHEAD_3_7V)  //PRINTHEAD_DY06_3_7V
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*15+tm_battery)/2;  //there are 7 levels , but max is same
		if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*15+tm_battery)/2)
		{
			delay_time = 0;
		}
		else
		{
			delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*11+tm_battery)/2 - all_hot_time;
		}
		#elif defined(DY19_PRINTHEAD_7_4V)
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*3+tm_battery)*3/2;  //there are 7 levels , but max is same
        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*3/2)
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*3/2 - all_hot_time;
        }
		#elif defined(DY45_PRINTHEAD_7_4V)
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*40+tm_battery)*1;  //there are 7 levels , but max is same
        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*1)
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*1 - all_hot_time;
        }
		#elif defined(DY16_LY_PRINTHEAD_7_4V)
			#ifdef USE_STICKER_PRTPAPER //不干胶纸打印浓度深一点
				if(configGet(CONFIG_ID_PRINTER_TYPE) == R_ID_STR_PRINT_PT_PAPER)
				{
					all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*3+level*10+tm_battery*4)*9/10;  //7/5
			        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*3+7*10+tm_battery*4)*9/10)
			        {
			            delay_time = 0;
			        }
			        else
			        {
			            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*3+7*10+tm_battery*4)*9/10 - all_hot_time;
			        }
				}
				else if(configGet(CONFIG_ID_PRINTER_TYPE) == R_ID_STR_PRINT_BGJ_PAPER)
				{
					all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*3+level*10+tm_battery*4)*3/2;  //7/5
			        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*3+7*10+tm_battery*4)*3/2)
			        {
			            delay_time = 0;
			        }
			        else
			        {
			            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*3+7*10+tm_battery*4)*3/2 - all_hot_time;
			        }
				}
			#else
				all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*3+level*30+tm_battery*4)*1;//10-9/10  //7/5
		        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*3+7*30+tm_battery*4)*1)
		        {
		            delay_time = 0;
		        }
		        else
		        {
		            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*3+7*30+tm_battery*4)*1 - all_hot_time;
		        }
			#endif	
		#elif defined(DY16_LY_PRINTHEAD_3_7V)
				all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*3+level*10+tm_battery*4)*5/10;//10-9/10  //7/5
		        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*3+7*10+tm_battery*4)*5/10)
		        {
		            delay_time = 0;
		        }
		        else
		        {
		            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*3+7*10+tm_battery*4)*5/10 - all_hot_time;
		        }
		#elif defined(DY128_LY_PRINTHEAD_7_4V)	
				all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*3+level*30+tm_battery*4)*9/10;//10-9/10  //7/5
		        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*3+7*30+tm_battery*4)*9/10)
		        {
		            delay_time = 0;
		        }
		        else
		        {
		            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*3+7*30+tm_battery*4)*9/10 - all_hot_time;
		        }
		#elif defined(J589_LY_PRINTHEAD_7_4V)
			all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*40+tm_battery)*1;  //20/10
	        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*1)
	        {
	            delay_time = 0;
	        }
	        else
	        {
	            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*1 - all_hot_time;
	        }
		#elif defined(DY116_LY_PRINTHEAD_7_4V)
			all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*40+tm_battery)*1;  //20/10
			if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*1)
			{
				delay_time = 0;
			}
			else
			{
				delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*1 - all_hot_time;
			}
		#elif defined(DY79_LY_PRINTHEAD_7_4V)
			#ifdef ADD_PRINT_PAPER_TYPE
				if(configGet(CONFIG_ID_PRINTER_TYPE2)==R_ID_STR_PRINT_COL_PAPER){			
					all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*40+tm_battery)*10/6;//彩色整体加浓一个等级
			        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*10/6)
			        {
			            delay_time = 0;
			        }
			        else
			        {
			            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*10/6 - all_hot_time;
			        }
				}else{
					all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*40+tm_battery)*3/2;  //1-old
					if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*3/2)
					{
						delay_time = 0;
					}
					else
					{
						delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*3/2 - all_hot_time;
					}
				}
			#else 
				all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*40+tm_battery)*1;//3/2-NEW(add two level)  //1-old
		        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*1)
		        {
		            delay_time = 0;
		        }
		        else
		        {
		            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*40+tm_battery)*1 - all_hot_time;
		        }
			#endif		
		#elif defined(DY20_OPOS_PRINTHEAD_7_4V)//沃博思
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*3+tm_battery)*7/4;//9/4,*5/2  //there are 7 levels , but max is same
        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*7/4)
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*7/4 - all_hot_time;
        }
		#elif defined(DY05_OPOS_PRINTHEAD_7_4V)//沃博思
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*3+tm_battery)*2;  //there are 7 levels , but max is same
        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*2)
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*2 - all_hot_time;
        }
		#elif defined(DY26_OPOS_PRINTHEAD_7_4V)//沃博思
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*3+tm_battery)*5/2;  //there are 7 levels , but max is same
        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*5/2)
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*5/2 - all_hot_time;
        }
		#elif defined(DY42_PRINTHEAD_7_4V)
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*3+tm_battery)*14/5;  //there are 7 levels , but max is same
        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*14/5)
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*14/5 - all_hot_time;
        }
		#else//  PRINTHEAD_DY19_7_4V
		all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time*2+level*3+tm_battery)*2;  //there are 7 levels , but max is same
        if(all_hot_time >= (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*2)
        {
            delay_time = 0;
        }
        else
        {
            delay_time = (PRINTER_GRAY_TIME+tm_hot_time*2+7*3+tm_battery)*2 - all_hot_time;
        }
		#endif
		
		//deg_Printf("???????all_hot_time==%d\n",all_hot_time);//h = 384
		deg_Printf("??? tm_battery=%d,tm_hot_time=%d,level=%d=%d\n",tm_battery,tm_hot_time,level,level*12);//h = 384
		deg_Printf("??? ???????all_hot_time==%d\n",all_hot_time);//h = 384

		//==first move 2 line==
		printer_moto_move_step(1,0,0,0,0);
		printer_delay((delay_time+all_hot_time)*PRINTER_GRAY_STEP+50);
		printer_moto_move_step(1,0,0,0,0);
		printer_delay((delay_time+all_hot_time)*PRINTER_GRAY_STEP+50);


		if(PRINTER_W==w)
		{
			for(j=0;j<h;j++)
			{
				#if HALF_LINE_PRINTER_EN
				u16 hpoint;
				ret =1;
				//==half line==
				for(k=0;k<PRINTER_GRAY_STEP;k++)
				{
					#if LINE_TIME_FIX_EN
					pnum=0;
					#endif
					hpoint=0;
					for(i=0;i<PRINTER_W;i++)
					{
						if(*(p+i)<=gray_tab[k])
						{
							hpoint++;
							if(1==hpoint%2)
							{
								PRINTER_DAT_HIGH();
								#if LINE_TIME_FIX_EN
								pnum++;
								#endif
							}
							else
							{
								PRINTER_DAT_LOW();
							}
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
					#if LINE_TIME_FIX_EN
					if(ret)
						ret =  printer_strobe_moto_move(1,all_hot_time,k,line_fix_time[pnum]);
					else
						ret = printer_strobe_moto_move(0,all_hot_time,k,line_fix_time[pnum]);
					#else
					if(ret)
						ret =  printer_strobe_moto_move(1,all_hot_time,k,0);
					else
						ret = printer_strobe_moto_move(0,all_hot_time,k,0);
					#endif
				}

				//==left hal line==
				for(k=0;k<PRINTER_GRAY_STEP;k++)
				{
					#if LINE_TIME_FIX_EN
					pnum=0;
					#endif
					hpoint=0;
					for(i=0;i<PRINTER_W;i++)
					{
						if(*(p+i)<=gray_tab[k])
						{
							hpoint++;
							if(0==hpoint%2)
							{
								PRINTER_DAT_HIGH();
								#if LINE_TIME_FIX_EN
								pnum++;
								#endif
							}
							else
							{
								PRINTER_DAT_LOW();
							}
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
					#if LINE_TIME_FIX_EN
					if(ret)
						ret =  printer_strobe_moto_move(1,all_hot_time,k,line_fix_time[pnum]);
					else
						ret = printer_strobe_moto_move(0,all_hot_time,k,line_fix_time[pnum]);
					#else
					if(ret)
						ret =  printer_strobe_moto_move(1,all_hot_time,k,0);
					else
						ret = printer_strobe_moto_move(0,all_hot_time,k,0);
					#endif
				}

				#else
				
				ret =1;
				for(k=0;k<PRINTER_GRAY_STEP;k++)
				{
					#if LINE_TIME_FIX_EN
					pnum=0;
					#endif
					for(i=0;i<PRINTER_W;i++)
					{
						if(*(p+i)<=gray_tab[k])
						{
							PRINTER_DAT_HIGH();
							#if LINE_TIME_FIX_EN
							pnum++;
							#endif
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
					#if LINE_TIME_FIX_EN
					if(ret)
						ret =  printer_strobe_moto_move(1,all_hot_time,k,line_fix_time[pnum]);
					else
						ret = printer_strobe_moto_move(0,all_hot_time,k,line_fix_time[pnum]);
					#else
					if(ret)
						ret =  printer_strobe_moto_move(1,all_hot_time,k,0);
					else
						ret = printer_strobe_moto_move(0,all_hot_time,k,0);
					#endif
				}

				#endif

				p += w;
				// wait moto move finish
				while(ret)
				{
					ret = printer_moto_move_step(1,1,all_hot_time,k,0);
				}
				hal_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto PRINTER_END;
				}
			}
		}
		else if(PRINTER_W==h)
		{
			u32 len;
			len=w*h;
			for(j=0;j<w;j++)
			{
				p = buf+j;
				ret =1;

				#if HALF_LINE_PRINTER_EN
				u16 hpoint;
				//==half line==
				for(k=0;k<PRINTER_GRAY_STEP;k++)
				{
					s32 offset;
					#if LINE_TIME_FIX_EN
					pnum=0;
					#endif
					hpoint=0;
					for(offset=len-w;offset>=0;offset-=w)
					{
						if(*(p+offset)<=gray_tab[k])
						{
							hpoint++;
							if(1==hpoint%2)
							{
								PRINTER_DAT_HIGH();
								#if LINE_TIME_FIX_EN
								pnum++;
								#endif
							}
							else
							{
								PRINTER_DAT_LOW();
							}
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
					#if LINE_TIME_FIX_EN
					if(ret)
						ret = printer_strobe_moto_move(1,all_hot_time,k,line_fix_time[pnum]);
					else
						ret=printer_strobe_moto_move(0,all_hot_time,k,line_fix_time[pnum]);
					#else
					if(ret)
						ret = printer_strobe_moto_move(1,all_hot_time,k,0);
					else
						ret=printer_strobe_moto_move(0,all_hot_time,k,0);
					#endif
				}

				//==left half line==
				for(k=0;k<PRINTER_GRAY_STEP;k++)
				{
					s32 offset;
					#if LINE_TIME_FIX_EN
					pnum=0;
					#endif
					hpoint=0;
					for(offset=len-w;offset>=0;offset-=w)
					{
						if(*(p+offset)<=gray_tab[k])
						{
							hpoint++;
							if(0==hpoint%2)
							{
								PRINTER_DAT_HIGH();
								#if LINE_TIME_FIX_EN
								pnum++;
								#endif
							}
							else
							{
								PRINTER_DAT_LOW();
							}
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
					#if LINE_TIME_FIX_EN
					if(ret)
						ret = printer_strobe_moto_move(1,all_hot_time,k,line_fix_time[pnum]);
					else
						ret=printer_strobe_moto_move(0,all_hot_time,k,line_fix_time[pnum]);
					#else
					if(ret)
						ret = printer_strobe_moto_move(1,all_hot_time,k,0);
					else
						ret=printer_strobe_moto_move(0,all_hot_time,k,0);
					#endif
				}

				#else

				for(k=0;k<PRINTER_GRAY_STEP;k++)
				{
					s32 offset;
					#if LINE_TIME_FIX_EN
					pnum=0;
					#endif
					for(offset=len-w;offset>=0;offset-=w)
					{
						if(*(p+offset)<=gray_tab[k])
						{
							PRINTER_DAT_HIGH();
							#if LINE_TIME_FIX_EN
							pnum++;
							#endif
						}
						else
						{
							PRINTER_DAT_LOW();
						}
						PRINTER_CLK_HIGH();
						PRINTER_CLK_LOW();
					}
					PRINTER_LATCH_LOW();
					PRINTER_LATCH_HIGH();
					#if LINE_TIME_FIX_EN
					if(ret)
						ret = printer_strobe_moto_move(1,all_hot_time,k,line_fix_time[pnum]);
					else
						ret = printer_strobe_moto_move(0,all_hot_time,k,line_fix_time[pnum]);
					#else
					if(ret)
						ret = printer_strobe_moto_move(1,all_hot_time,k,0);
					else
						ret=printer_strobe_moto_move(0,all_hot_time,k,0);
					#endif
				}

				#endif
				// wait moto move finish
				while(ret)
				{
					ret=printer_moto_move_step(1,1,all_hot_time,k,0);
				}

				hal_wdtClear();
				if(printer_get_paper()) // no paper
				{
					ret=8;
					goto PRINTER_END;
				}
			}
		}
		else
		{
			ret=10;
			goto PRINTER_END;
		}

	}

			//==set paper align==
		if(print_mode==1)
		{
			for(i= 0;i<80;i++)
			{
				printer_moto_move_step(1,0,0,0,0);
				printer_moto_delay(200);
				printer_moto_move_step(1,0,0,0,0);
				printer_moto_delay(200);
			}

		}else
		{
			for(i= 0;i<80;i++)//?����?�䨰��???���?������DD
			{
				printer_moto_move_step(1,0,0,0,0);
				printer_delay(1000/*(delay_time+all_hot_time+50)*PRINTER_GRAY_STEP*/);
				printer_moto_move_step(1,0,0,0,0);
				printer_delay(1000/*(delay_time+all_hot_time+50)*PRINTER_GRAY_STEP*/);
			}
		}
		
PRINTER_END:
	//��ԭ
	printer_moto_enable(0);
	//re init ,for adc - bat detective
	#ifndef USE_BOARD_DY42
	hal_adcInit();
	
	hal_gpioInit(GPIO_PE,GPIO_PIN0,GPIO_INPUT,GPIO_PULL_FLOATING);
	ax32xx_adcEnable(ADC_CH_PE0, 1);
	#endif

	
	// SysCtrl.PE0_stat = (1<<1) | (1 << (1 + 4)); 
	
//#if HAL_CFG_EN_DBG == 0
//	hal_gpioInit(PRINTER_PSENSOR_CH,PRINTER_PSENSOR_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
//	hal_gpioWrite(PRINTER_PSENSOR_CH,PRINTER_PSENSOR_PIN,GPIO_LOW);
//#else 
	////re init uart,when need  用作TX  debug 
	#ifdef USE_APP_PRINTER
		hal_uartInit();//不带蓝牙ic板子不需要走,否则会导致打印完菜单设置不了彩灯
	#endif
//#endif
   
	hal_sysDelayMS(10);
	deg("T-T-T---%d-- show ret[%d]\n", temprature,ret);
	deg("total time[%d] tm_hot_time[%d] level[%d] tm_battery[%d]\n",(PRINTER_GRAY_TIME+tm_hot_time+level*7+tm_battery)/2,tm_hot_time,level,tm_battery);
	deg("??? tm_battery=%d,tm_hot_time=%d,level=%d level*7=%d\n",tm_battery,tm_hot_time,level,level*7);//h = 384

	//PA3���������
	//hal_gpioInit(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	is_printting = 0;
#if defined(USE_5050IC_CONTROL_LED)
	hal_gpioInit(GPIO_PA,GPIO_PIN7,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	Int_led_timer();
#endif

	return ret;

}




//==en 0: is close , 1 : is open==
static void printer_set_int(u8 en)
{
	static u8 csi,jpga,jpgb,lcdc,de,rotate,emi,usb1,usb2,timer0,timer1,timer2,timer3;

	if(0 == en)
	{
		if(SPR_PICMR&BIT(IRQ_CSI))
		{
			deg_Printf("IRQ_CSI en\n");
			csi = 1;
		}
		else
		{
			csi = 0;
		}
		if(SPR_PICMR&BIT(IRQ_JPGA))
		{
			deg_Printf("IRQ_JPGA en\n");
			jpga = 1;
		}
		else
		{
			jpga = 0;
		}
		if(SPR_PICMR&BIT(IRQ_JPGB))
		{
			deg_Printf("IRQ_JPGB en\n");
			jpgb = 1;
		}
		else
		{
			jpgb = 0;
		}
		if(SPR_PICMR&BIT(IRQ_LCDC))
		{
			deg_Printf("IRQ_LCDC en\n");
			lcdc = 1;
		}
		else
		{
			lcdc = 0;
		}
		if(SPR_PICMR&BIT(IRQ_DE))
		{
			deg_Printf("IRQ_DE en\n");
			de = 1;
		}
		else
		{
			de = 0;
		}
		if(SPR_PICMR&BIT(IRQ_ROTATE))
		{
			deg_Printf("IRQ_ROTATE en\n");
			rotate = 1;
		}
		else
		{
			rotate = 0;
		}
		if(SPR_PICMR&BIT(IRQ_EMI))
		{
			deg_Printf("IRQ_EMI en\n");
			emi = 1;
		}
		else
		{
			emi = 0;
		}
		if(SPR_PICMR&BIT(IRQ_USB11))
		{
			deg_Printf("IRQ_USB11 en\n");
			usb1 = 1;
		}
		else
		{
			usb1 = 0;
		}

		if(SPR_PICMR&BIT(IRQ_USB20))
		{
			deg_Printf("IRQ_USB20 en\n");
			usb2 = 1;
		}
		else
		{
			usb2 = 0;
		}
		if(SPR_PICMR&BIT(IRQ_TIMER0))
		{
			deg_Printf("IRQ_TIMER0 en\n");
			timer0 = 1;
		}
		else
		{
			timer0 = 0;
		}
		if(SPR_PICMR&BIT(IRQ_TIMER1))
		{
			deg_Printf("IRQ_TIMER1 en\n");
			timer1 = 1;
		}
		else
		{
			timer1 = 0;
		}
		if(SPR_PICMR&BIT(IRQ_TIMER2))
		{
			deg_Printf("IRQ_TIMER2 en\n");
			timer2 = 1;
		}
		else
		{
			timer2 = 0;
		}
		if(SPR_PICMR&BIT(IRQ_TIMER3))
		{
			deg_Printf("IRQ_TIMER3 en\n");
			timer3 = 1;
		}
		else
		{
			timer3 = 0;
		}

		ax32xx_intEnable(IRQ_CSI,0);
		ax32xx_intEnable(IRQ_JPGA,0);
		ax32xx_intEnable(IRQ_JPGB,0);
		ax32xx_intEnable(IRQ_LCDC,0);
		ax32xx_intEnable(IRQ_DE,0);
		ax32xx_intEnable(IRQ_ROTATE,0);
		ax32xx_intEnable(IRQ_EMI,0);
		ax32xx_intEnable(IRQ_USB11,0);
		ax32xx_intEnable(IRQ_USB20,0);
		ax32xx_intEnable(IRQ_TIMER0,0);
		ax32xx_intEnable(IRQ_TIMER1,0);
		ax32xx_intEnable(IRQ_TIMER2,0);
		ax32xx_intEnable(IRQ_TIMER3,0);
	}
	else
	{
		if(csi)
		ax32xx_intEnable(IRQ_CSI,1);
		if(jpga)
		ax32xx_intEnable(IRQ_JPGA,1);
		if(jpgb)
		ax32xx_intEnable(IRQ_JPGB,1);
		if(lcdc)
		ax32xx_intEnable(IRQ_LCDC,1);
		if(de)
		ax32xx_intEnable(IRQ_DE,1);
		if(rotate)
		ax32xx_intEnable(IRQ_ROTATE,1);
		if(emi)
		ax32xx_intEnable(IRQ_EMI,1);
		if(usb1)
		ax32xx_intEnable(IRQ_USB11,1);
		if(usb2)
		ax32xx_intEnable(IRQ_USB20,1);
		if(timer0)
		ax32xx_intEnable(IRQ_TIMER0,1);
		if(timer1)
		ax32xx_intEnable(IRQ_TIMER1,1);
		if(timer2)
		ax32xx_intEnable(IRQ_TIMER2,1);
		if(timer3)
		ax32xx_intEnable(IRQ_TIMER3,1);

	}
}

//==scale jpg to printer width height==
//==ret : 0 is ok , other err==
//==src_jpg: input jpg==
static u8 printer_jpg_to_printer_wh(u8* src_jpg,u32 *dst_ybuf,u16* dst_w,u16* dst_h,u32 jpg_size)
{
	u8 *yuvbuf=NULL;
	u32 ybuf_size,yuvbuf_size;
	u8 *enjpg_buf=NULL;
	u32 enjpg_w=0,enjpg_h=0;
	u32 en_ybuf_size;
	u32 en_yuvbuf_size;
	u8 *en_yuvbuf=NULL;
	u16 src_width,src_height,l_width,l_heigth;
	u8 ret,need_encode;
	u32 point = 0;
		
	INT32S ret_jpg;
	INT8U *thumbBuffer_old = NULL ,*tempBuff =NULL;
	  
	ret=0;
	need_encode=0;
	*dst_ybuf=0;
	*dst_w=0;
	*dst_h=0;

#if 0
	while(hal_BackRecDecodeStatusCheck()||!ax32xx_csiScalerCheck());

	if(api_backrec_status())
	{
		backrec_adpt_set(0);
	}
#else
	while(!ax32xx_csiScalerCheck());
#endif
	hal_lcdSetPIPEnable(0);
#if 0
	//==check jpg w h==
	hal_mjpegHeaderParse(src_jpg,NULL);
#else
	ret_jpg = 0;
	ret_jpg = hal_mjpegHeaderParse(src_jpg,&thumbBuffer_old);
	if(ret_jpg == -10)
	{
		INT8U app[12];
		
		point = jpg_size - sizeof app;
		memcpy(app,src_jpg + point,sizeof app);
		
		if (app[4] == 'J' && app[5] == 'R' && app[6] == 'X')
		{
			u32 thumbOffset;
			thumbOffset = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];

			src_jpg = src_jpg+thumbOffset;
			ret = hal_mjpegHeaderParse(src_jpg,NULL);
		}else
		{
			tempBuff = src_jpg;
			src_jpg = thumbBuffer_old;
			deg("Decode thumb\n");
			ret = hal_mjpegHeaderParse(src_jpg, NULL);
		}
			
	}
	if(ret_jpg < 0)
	{
		deg_Printf("headparse fail .ret_jpg:%d\n",ret_jpg);
		ret_jpg = -1;
		
    }else{
		deg_Printf("printer_wh ret_jpg:%d\n",ret_jpg);
    	}
#endif
	hal_mjpegDecodeGetResolution(&src_width,&src_height);
	hal_mjpeglDecodeGetResolution(&l_width,&l_heigth);
	deg_Printf("printer_wh:src_width:%d ,src_height:%d ,l_width:%d ,l_height:%d\n",src_width,src_height,l_width,l_heigth);
	if(/*l_width < 4032 &&*/ src_height >= 384){
		if((src_width==PRINTER_W)||(src_height==PRINTER_W))
		{
			*dst_w=src_width;
			*dst_h=src_height;
		}
		else if((src_width>PRINTER_W)&&(src_height>PRINTER_W))
		{
			if(src_width>=src_height)
			{
					if(src_width==4032){*dst_w=((src_width*PRINTER_W)/2270)&(~0x1f);}else
				*dst_w=((src_width*PRINTER_W)/src_height)&(~0x1f);
				*dst_h=PRINTER_W;
			}
			else
			{
				*dst_w=PRINTER_W;
				*dst_h=(src_height*PRINTER_W)/src_width;
			}
		}
		else if((src_width>PRINTER_W)||(src_height>PRINTER_W))
		{
			if(src_width>PRINTER_W)
			{
				*dst_w=PRINTER_W;
				*dst_h=(src_height*PRINTER_W)/src_width;
			}
			else
			{
				*dst_w=((src_width*PRINTER_W)/src_height)&(~0x1f);
				*dst_h=PRINTER_W;
			}
		}
		else
		{
			need_encode=1;
			*dst_w=src_width;
			*dst_h=src_height;
			if(src_width>=src_height)
			{
				enjpg_w=((src_width*PRINTER_W)/src_height)&(~0x1f);
				enjpg_h=PRINTER_W;
			}
			else
			{
				enjpg_w=PRINTER_W;
				enjpg_h=(src_height*PRINTER_W)/src_width;
			}
		}
	}else{
	#if PIC_16_9
	*dst_w = 672;
	#else
	*dst_w = 512;
	#endif
	*dst_h = 384;
	}
	//==decode==
	ybuf_size=(*dst_w)*(*dst_h);
	yuvbuf_size=(ybuf_size*3)/2;
	yuvbuf=(u8*)hal_sysMemMalloc(yuvbuf_size,32);
	

	if(NULL==yuvbuf)
	{
		ret=1;
		goto PRINTER_SCALE_JPG;
	}
	deg_Printf("*dst_w:%d,*dst_h:%d,ybuf_size=%d,yuvbuf_size=%d\n",*dst_w,*dst_h,ybuf_size,yuvbuf_size);

	*dst_ybuf=(u32)yuvbuf;

	if(/*l_width < 4032 && */src_height >= 384){
		if(hal_mjpegDecodeNoIsr(src_jpg,(u8*)(yuvbuf),(u8*)(yuvbuf+ybuf_size),*dst_w,*dst_h)<0)
		{
			ret=3;
			goto PRINTER_SCALE_JPG;
		}

		if(!jpeg1_decode_check())
		{
			ret=4;
			goto PRINTER_SCALE_JPG;
		}
		ax32xx_sysDcacheFlush((u32)yuvbuf,yuvbuf_size);
		deg_Printf("pass decode:%d\n",need_encode);

		if(need_encode)
		{
			enjpg_buf=(u8*)hal_sysMemMalloc(100*1024,32);	// i guess no over 100KB
			if(NULL==enjpg_buf)
			{
				ret=2;
				goto PRINTER_SCALE_JPG;
			}

			deg_Printf("enjpg_w:%d,enjpg_h:%d\n",enjpg_w,enjpg_h);

			//==encode==
			ax32xx_mjpB_Ctl_init(0,JPEG_Q_75,*dst_w,*dst_h,enjpg_w,enjpg_h);
			ax32xx_mjpB_Linebuf_cfg((u8*)(yuvbuf),(u8*)(yuvbuf+ybuf_size));
			ax32xx_mjpB_dma_cfg((u32)enjpg_buf,(u32)(enjpg_buf+100*1024));
			hal_watermarkEnable(0,0);
			ax32xx_mjpB_on_2();
			if(ax32xx_mjpegB_encode_check()<0)
			{
				ret=5;
				goto PRINTER_ENCODE_ERR;
			}
			ax32xx_sysDcacheFlush((u32)enjpg_buf,100*1024);
			deg_Printf("pass encode\n");

			//==decode==
			en_ybuf_size=enjpg_w*enjpg_h;
			en_yuvbuf_size=en_ybuf_size*3/2;
			en_yuvbuf=(u8*)hal_sysMemMalloc(en_yuvbuf_size,32);
			if(NULL==en_yuvbuf)
			{
				ret=1;
				goto PRINTER_ENCODE_ERR;
			}
			
			if(hal_mjpegDecodeNoIsr(enjpg_buf,(u8*)(en_yuvbuf),(u8*)(en_yuvbuf+en_ybuf_size),enjpg_w,enjpg_h)<0)
			{
				ret=3;
				goto PRINTER_ENCODE_ERR;
			}
			
			if(!jpeg1_decode_check())
			{
				ret=4;
				goto PRINTER_ENCODE_ERR;
			}
			ax32xx_sysDcacheFlush((u32)en_yuvbuf,en_yuvbuf_size);
			deg_Printf("pass decode\n");

			*dst_w=enjpg_w;
			*dst_h=enjpg_h;
			*dst_ybuf=(u32)en_yuvbuf;
	PRINTER_ENCODE_ERR:

			if(yuvbuf)
			{
				hal_sysMemFree(yuvbuf);
				yuvbuf=NULL;
			}
		}
	}else{
	ret = decodePic(src_jpg,yuvbuf,yuvbuf+ybuf_size,*dst_w,*dst_h,jpg_size);
			if(ret){
				deg_Printf("decodePic ret:%d dst_w:%d dst_h:%d\n",ret,*dst_w,*dst_h);
				ret = 0;
			}else{
				deg_Printf("decode Pic fail !\n");
				if(yuvbuf){
					hal_sysMemFree(yuvbuf);
					yuvbuf=NULL;
				}
				ret = -1;
			}

	}
PRINTER_SCALE_JPG:

	if(need_encode)
	{
		if(enjpg_buf)
		{
			hal_sysMemFree(enjpg_buf);
			enjpg_buf=NULL;
		}
	}
	(void)tempBuff;

	return ret;
}



u8 printer_dot_matrix_handle(u8 *ybuf,u16 w, u16 h)
{
	int ret=-1;
	deg_Printf("dot_matrix ybuf w:%d,w:%d\n",w,h);
	if((384==w)&&(672==h))
	{
		ret=CLAHE(ybuf, w, h, 0, 255, w/48, h/84, 64, 4.0f);//2.0f
	}
	else if((672==w)&&(384==h))
	{
		ret=CLAHE(ybuf, w, h, 0, 255, w/84, h/48, 64, 4.0f);//2.0f
	}
	else if((384==w)&&(216==h))
	{
		ret=CLAHE(ybuf, w, h, 0, 255, w/48, h/27, 64, 4.0f);//2.0f
	}
	else if((216==w)&&(384==h))
	{
		ret=CLAHE(ybuf, w, h, 0, 255, w/27, h/48, 64, 4.0f);//2.0f
	}
	else if((320==w)&&(180==h))
	{
		ret=CLAHE(ybuf, w, h, 0, 255, w/32, h/18, 64, 4.0f);//2.0f
	}
	else if((320==w)&&(240==h))
	{
		ret=CLAHE(ybuf, w, h, 0, 255, w/32, h/24, 64, 4.0f);//2.0f
	}else if((384 == h)&& (512 == w))
	{
		ret =CLAHE(ybuf, w, h, 0, 255,w/64,h/48,64, 4.0f);
	}

	//?��?�C????????43?o??64?��?-?????3?��?????o?????-?????��?????2?��???��?�C?????��?3?????��???????????3??	//???????????��?��??????
	//?��?�C?????????????��???��?��???��?��???????????��?3???��?��???��???��???��???��???��???????2?o?��?o?��???��???��???��???��???�C???��???��???��?????????��?o??
	//?��?1?????��???????????�C?��?��?-???????��?????��???????��?��?��???��?3256?��???��???????��?��???????1???��ram?��?3?????��???��?2???a?????????��?�C???????????��?��?a?��???????�C?��?��?????????????��
	//?��?�C?????��???????��???��???????��???????��?????��?��?a?��??ram?��???��?3

	if(0==ret)
	{
		//BayerDithering(ybuf, w, h);
		FloydSteinbergDithering(ybuf, w, h, 0);
	}
	else 
	{
		deg_Printf("clahe err:%d\n",ret);
		ret =1;
	}

	return ret;
}

//==ret: 0 :is ok , 1: mem err ,2: mem err , 3: jpg dec err , 8: no paper ,9 : tm hot
//==fd : jpeg file ==
//==level:  0~4   max~min
//==print_mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery : 0~4
#define ALLOC_JPEG_SIZE 100*1024
u8 printer_jpeg(int fd,u8 level,u8 print_mode,u8 batteryValue)
{
	u32 ybuf/*,y_buf_size*/,res;
	s32 jpg_size;
	u16 printer_w,printer_h;
	u8 *jpg_y_buf=NULL;
	u8 *jpeg=NULL;
	u8 ret;

	jpg_size=fs_size(fd);
	deg_Printf("jpg_size=%d\n",jpg_size);



	if(jpg_size<512)	// too mall , jpg file not ok 
	{
		if(jpeg)
		{
			hal_sysMemFree(jpeg);
		}
		ret=2;
		goto PRINTER_END;
	}

	//----------Printer read thumb-------
		
	//if(jpg_size > (hal_sysMemRemain()+64))		//fsSize large than remain space
	{
		jpeg = (INT8U *)hal_sysMemMalloc(ALLOC_JPEG_SIZE,64);		//malloc space
		read(fd,jpeg,ALLOC_JPEG_SIZE);
		ax32xx_sysDcacheFlush((u32)jpeg,ALLOC_JPEG_SIZE);
	}

	res = hal_mjpegHeaderParse(jpeg,NULL);
	deg_Printf("printer jpg:check res[%d]\n",res);
	if(res == -10 ||res == 0)
	{
		
		INT8U app[12];
		lseek(fd, jpg_size - sizeof app, 0);
		read(fd, app, sizeof app);
		if (app[4] == 'J' && app[5] == 'R' && app[6] == 'X')
		{
			u32 thumbOffset;
			thumbOffset = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];
			deg_Printf("thumbOffset=0x%x \n", thumbOffset);
			lseek(fd, thumbOffset, 0);
			read(fd,jpeg,ALLOC_JPEG_SIZE); 
			ax32xx_sysDcacheFlush((u32)jpeg,ALLOC_JPEG_SIZE);
			deg_Printf("Decode thumb\n");
			deg_Printf("read jpg data : jpgSize:%d thumbSize:%d\n",jpg_size,jpg_size-thumbOffset);
		}else{

			if((hal_sysMemRemain()-672*384*3/2-23*1024+ALLOC_JPEG_SIZE)>jpg_size)
			{
				deg_Printf("outside pic print\n");
				deg_Printf("---[%d]---",(hal_sysMemRemain()-672*384*3/2-23*1024+ALLOC_JPEG_SIZE));
				hal_sysMemFree(jpeg);
				jpeg = (INT8U *)hal_sysMemMalloc(jpg_size,64);
				if(jpeg==NULL)
					goto PRINTER_END;
				lseek(fd, 0, 0);
				read(fd,jpeg,jpg_size); 
				ax32xx_sysDcacheFlush((u32)jpeg,jpg_size);

				/*deg_Printf("\n\n");
				hal_sysMemPrint();
				deg_Printf("\n\n");*/
			}else{
				deg_Printf("---[%d]---",(hal_sysMemRemain()-672*384*3/2-23*1024+ALLOC_JPEG_SIZE));
				deg_Printf("alloc fail!! \noutdside pic Size %d > %d:remainSize\n",jpg_size,hal_sysMemRemain());
				ret=2;
				goto PRINTER_END;
			}
		}
	}else
	{
		deg_Printf("res else!!\n");
		ret=1;
		goto PRINTER_END;
	}

	//-------------------------------

	
	ret=printer_jpg_to_printer_wh(jpeg,&ybuf,&printer_w,&printer_h,jpg_size);
	jpg_y_buf=(u8*)ybuf;
	deg_Printf("printer_w=%d,printer_h=%d,ret=%d\n",printer_w,printer_h,ret);
	if(0==ret)
	{
		if(1==print_mode)
		{
			printer_dot_matrix_handle(jpg_y_buf,printer_w,printer_h);
		}
		//printer_set_int(0);
		

		ret=printer_print((u8* )jpg_y_buf,printer_w,printer_h,level,print_mode,batteryValue);
		//printer_set_int(1);
	}

PRINTER_END:

	if(jpg_y_buf)
	{
		hal_sysMemFree(jpg_y_buf);
	}
	
	if(jpeg)
	{
		hal_sysMemFree(jpeg);
	}

	dispLayerSetPIPMode(DISP_PIP_FRONT);
	deg_Printf("ret=%d\n",ret);
	return ret;
}
#ifdef USE_BOARD_DY42
extern int deal_photoApp_fstMsg;
#endif
//==ret: 0 :is ok , 1: mem err ,2: mem err , 3: jpg dec err , 8: no paper ,9 : tm hot ,10: w ,h err
//==jpeg: jpeg buf ==
//==level:  0~4   max~min
//==print_mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery : 0~4
u8 printer_jpeg_buf(u8 *jpeg,u8 level,u8 print_mode,u8 batteryValue,u32 jpg_size)
{
	u32 ybuf/*,y_buf_size*/;
	u16 printer_w,printer_h;
	u8 *jpg_y_buf;
	u8 ret;

	jpg_y_buf=NULL;
	ret=printer_jpg_to_printer_wh(jpeg,&ybuf,&printer_w,&printer_h,jpg_size);
	jpg_y_buf=(u8*)ybuf;
	deg_Printf("printer_w=%d,printer_h=%d,ret=%d\n",printer_w,printer_h,ret);

	if(0==ret)
	{
		if(1==print_mode)
		{
			printer_dot_matrix_handle(jpg_y_buf,printer_w,printer_h);
		}

		printer_set_int(0);
		ret=printer_print((u8* )jpg_y_buf,printer_w,printer_h,level,print_mode,batteryValue);
		printer_set_int(1);
	}

//PRINTER_END:

	if(jpg_y_buf)
	{
		hal_sysMemFree(jpg_y_buf);
	}

	dispLayerSetPIPMode(DISP_PIP_FRONT);
	#ifdef USE_BOARD_DY42
	deal_photoApp_fstMsg = 0;
	#endif
	deg(" in printer_jpeg_buf: ret=%d\n",ret);
	return ret;

}


