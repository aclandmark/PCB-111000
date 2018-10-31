
#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"	

#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"

volatile char   Return_key, no_reset;
char  digits[9];							//SHOULD be [9]!!!!!!!!!!!!
char Disp_bkp[8];

void accumulator_sw2_isr(void);
void accumulator_sw1_and_sw3_isr(void);

#define flash_display for(int m = 0; m<=3; m++){Disp_bkp[m] = digits[m]; digits[m] = 0;}\
I2C_Tx_accumulator_op(digits, 'D');Timer_T0_10mS_delay_x_m(50);for(int m = 0; m<=3; m++)digits[m] = Disp_bkp[m];\
I2C_Tx_accumulator_op(digits, 'D');



#define message_1 \
"\r\n\
ACCUMULATOR program\r\n\
repeatedly press sw2 to enter and shift data\r\n\
press sw1 to enter data ending in zero (i.e.10,20 etc)\r\n"

#define message_2 \
"otherwise press sw1 while selecting final integer\r\n\
release SW2 when integer is correct\r\n\
Then release sw1\r\n"

#define message_3 \
"Press sw1 to add number to accumulator\r\n\
Press sw3 to subtract it from accumulator\r\n\
press sw2 just before sw1 to enter new data\r\n\
press sw3 rather than enterning new data to reset everything\r\n"


#define User_instructions \
String_to_PC(message_1);\
String_to_PC(message_2);\
String_to_PC(message_3);

