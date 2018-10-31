

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"

#define zero "abcdef"			//chars a,b,c,d,e and f are stored in an array named "zero"
#define one "bc"				//chars b and c are stored in an array named "one"
#define two "abged"
#define three "abcdg"
#define four "fgbc"
#define five "afgcd"
#define six "fegdc"
#define seven "abc"
#define eight "abcdefg"
#define nine "abcfg"

void display_num_string (char*, int);

char isCharavailable_Local (char);