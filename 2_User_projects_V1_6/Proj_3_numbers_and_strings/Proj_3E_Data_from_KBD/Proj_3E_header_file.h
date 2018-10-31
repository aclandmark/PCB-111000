

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Subroutines/Arithmetic.c"
#include "../../Resources_V12/Subroutines/Integers_to_from_PC.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"

long Num_from_KBD_Local(char*);
void display_in_scientific_notation (long, char*, char);
