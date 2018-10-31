volatile char   Return_key;
char  digits[8];

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"		//Responds to switch activity
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"

long number_from_IO(void);
void Tables_sw1_and_3_isr(void);
void Tables_sw2_isr(void);


#define message_1 \
"\r\n\
TABLES Use sw1 to enter multiplicand\r\n\
Use sw3 to shift display left\r\n\
When number is correct press SW2\r\n\
Press sw1 and sw3 alternatively\r\n\
to generate multiplication table\r\n"

#define message_2 \
"hold sw3 down to decement table\r\n\
press sw2 before sw1 to re-initiate display\r\n"



