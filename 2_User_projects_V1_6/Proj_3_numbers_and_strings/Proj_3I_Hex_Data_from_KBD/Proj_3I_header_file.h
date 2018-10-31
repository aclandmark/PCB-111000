

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Subroutines/Integers_to_from_PC.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"


void Binary_to_PC_Local_1(long, char);
long inc_num(long Num_1, char);


#define test_Char		char Num_1, Num_2;					No_bits = 8;
#define test_S_Char		signed char Num_1; char Num_2;		No_bits = 8;
#define test_Int		int Num_1; unsigned int Num_2;		No_bits = 16;
#define test_U_Int		unsigned int Num_1, Num_2;			No_bits = 16;
#define test_Long		long Num_1; unsigned long Num_2;	No_bits = 32;