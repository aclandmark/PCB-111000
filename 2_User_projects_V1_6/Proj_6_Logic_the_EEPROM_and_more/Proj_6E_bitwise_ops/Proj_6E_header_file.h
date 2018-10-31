

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Subroutines/Random_and_prime_nos.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/Subroutines/Integers_to_from_PC.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#define Rotate_Right_cyclical  while (n < X) {num = Y & 0b10000000; num = num >> 7; Y = Y <<1; Y = Y | num; n++;}
#define Rotate_left_cyclical  while (n < X) {num = Y & 0b00000001; num = num << 7; Y = Y >>1; Y = Y | num; n++;}



#define dig_0 digits[0]
#define dig_1 digits[1]
#define dig_2 digits[2]


#define message_1 \
"\r\nMore on bitwise ops\r\n\
Press r or R at the user prompt\r\n\
and x or X to escape\r\n\
giving 4 modes of operation\r\n"

#define User_instructions \
String_to_PC (message_1);


char logical_op(char, char, char, char);
char binary_char_from_KBD_Local(void);