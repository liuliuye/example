#ifndef __BTCOM_USER__
#define __BTCOM_USER__


/**
 * cmd：A3. return printer state to app
 *
 */
void Bt_Get_Printer_State(const u8 *data, int len);

/**
 * cmd：A4.  low 4 bit set print level :range 1~5.
 *
 */
void Bt_Set_Quality(const u8 *data, int len);

void Bt_Get_Print_Line_Data(const u8 *data, int len);
void Bt_Get_Print_Line_Data_Zip(const u8 *data, int len);

void Bt_Set_Lcd_Screen(const u8 *data, int len);

void Bt_Feed(const u8 *data, int len);

void Bt_Set_Printer_State(const u8 *data, int len);

u16 reset_pic_data(void);

void Bt_Set_BtName(const u8 *data, int len);




#endif  // __BTCOM_USER__

