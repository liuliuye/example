#ifndef  _PRINTER_H
#define  _PRINTER_H
#include "../../hal/inc/hal.h"   
#include "../../xos/xos.h"


void printer_init();

//==ret : 0 is ok , 8: no paper , 9: tm hot , 10: w h err
//==level    0~4  max~min
//==print mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery 0~4
u8 printer_print(u8* buf,u16 w,u16 h,u8 level,u8 print_mode,u8 batteryValue);


//==ret: 0 :is ok , 1: mem err ,2: mem err , 3: jpg dec err , 8: no paper ,9 : tm hot
//==fd : jpeg file ==
//==level:  0~4   max~min
//==print_mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery : 0~4
u8 printer_jpeg(int fd,u8 level,u8 print_mode,u8 batteryValue);


//==ret: 0 :is ok , 1: mem err ,2: mem err , 3: jpg dec err , 8: no paper ,9 : tm hot ,10: w ,h err
//==jpeg: jpeg buf ==
//==level:  0~4   max~min
//==print_mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery : 0~4
u8 printer_jpeg_buf(u8 *jpeg,u8 level,u8 print_mode,u8 batteryValue,u32 jpg_size);

int printer_app_buf(u8 *buf,int height);

u8 printer_dot_matrix_handle(u8 *ybuf,u16 w, u16 h);
u8 printer_move_step(void);

#endif

