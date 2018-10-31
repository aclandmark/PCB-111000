

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Header_files/clock_stop_watch.h"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"		//Responds to switch activity

void Inc_display(void);
void set_time(void);

#define message_1 \
"\r\nCRYSTAL CLOCK with adjustment\r\n\
Press sw1 to toggle the display on and off\r\n\
Press sw3 to pause or resume the clock\r\n\
To adjust the clock:\r\n"

#define message_2 \
"pulse sw2 then press sw1 and sw3 to advance the time\r\n\
or press and hold sw2 (for 500ms)\r\n\
then press sw1 and sw3 to retard the time\r\n\
Always pulse sw2 when the time is correct\r\n"

#define User_instructions \
String_to_PC(message_1);\
String_to_PC(message_2);
