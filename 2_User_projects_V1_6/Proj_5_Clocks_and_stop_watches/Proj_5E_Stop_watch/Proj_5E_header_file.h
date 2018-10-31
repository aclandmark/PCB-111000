

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

#define User_instructions \
String_to_PC(message_1);\
String_to_PC(message_2);


#define message_1 \
"\r\nSTOP WATCH: sw1 selects 100ms clock, sw3 a 10ms clock\r\n\
Press sw3 to pause SW and save time\r\n\
Press sw1 the read back saved times\r\n"

#define message_2 \
"While pressing sw1 press sw3 to restore SW\r\n\
Press sw2 to re-initialise SW\r\n"
