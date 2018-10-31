

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../Resources_V12/Subroutines/Integers_to_from_PC.c"	//Obtains numerical input from KBD and switch activity



void fraction_to_decimal_string_Local(long, long, char*);
long product_AB(long, long);
long divide_A_by_B(long, long);
long A_modulo_B(long, long);
