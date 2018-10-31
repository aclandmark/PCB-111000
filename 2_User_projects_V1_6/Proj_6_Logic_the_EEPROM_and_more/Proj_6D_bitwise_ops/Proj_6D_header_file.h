

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Subroutines/Random_and_prime_nos.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"		//Responds to switch activity
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../Resources_V12/Subroutines/Integers_to_from_PC.c"

#define line_control {if(line_counter==4) {line_counter = 0;newline();}\
 else {Char_to_PC('\t');line_counter++;}}
