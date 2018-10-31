

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Subroutines/Integers_to_from_PC.c"	//Obtains numerical input from KBD and switch activity
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"		//Responds to switch activity

#define message_1 \
"\r\n\
Press sw1,2 & 3\r\n\
keypresses 1, 5 and 9 put interrupts on hold\r\n\
2, 6 and 0 restore interrupts\r\n\
3, 7 and a dissable interrupts\r\n"


#define message_2 \
"4, 8 and b re-enable interrupts\r\n\
c gives a status report\r\n"


#define User_instructions \
String_to_PC(message_1);\
String_to_PC(message_2);

