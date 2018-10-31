

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"
#include "../../Resources_V12/Subroutines/Data_to_from_EEPROMs.c"
#include "../../Resources_V12/Subroutines/Random_and_prime_nos.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"

void Read_on_chip_EEPROM_local(int);

#define message_1 0xA
#define message_2 0x16
#define message_3 0x3A
#define message_4 0x64
#define message_5 0x9E
#define message_6 0xDE
#define message_7 0x114
#define message_8 0x142
#define message_9 0x185
#define message_10 0x1CB
#define message_11 0x1F1

#define message_A 0xA
#define message_B 0x16
#define message_C 0x1A
#define message_D 0x1F
#define message_E 0x25
#define message_F 0x2A
#define message_G 0x2E
#define message_H 0x33
#define message_I 0x37


#define client_program_1 \
Read_on_chip_EEPROM_local(message_1); waitforkeypress();\
Read_on_chip_EEPROM(message_2); waitforkeypress();\
Read_on_chip_EEPROM_local(message_3); waitforkeypress();\
Read_on_chip_EEPROM(message_4); waitforkeypress();\
Read_on_chip_EEPROM_local(message_5); waitforkeypress();\
Read_on_chip_EEPROM(message_6); waitforkeypress();\
Read_on_chip_EEPROM_local(message_7); waitforkeypress();\
Read_on_chip_EEPROM(message_8); waitforkeypress();\
Read_on_chip_EEPROM_local(message_9); waitforkeypress();\
Read_on_chip_EEPROM(message_10); waitforkeypress();\
Read_on_chip_EEPROM_local(message_11); waitforkeypress();


#define client_program_2 \
Read_on_chip_EEPROM(message_A); waitforkeypress();\
Read_on_chip_EEPROM_local(message_B); waitforkeypress();\
Read_on_chip_EEPROM(message_C); waitforkeypress();\
Read_on_chip_EEPROM_local(message_D); waitforkeypress();\
Read_on_chip_EEPROM(message_E); waitforkeypress();\
Read_on_chip_EEPROM_local(message_F); waitforkeypress();\
Read_on_chip_EEPROM_local(message_H); waitforkeypress();\
Read_on_chip_EEPROM(message_I); waitforkeypress();

//Only use ths subroutine if correct user cal values are inserted in place of 0x8E
/*#define rescue_calibration \
eeprom_write_byte((uint8_t*)(0x1F7), 0x8E);\
Timer_T0_sub(T0_delay_4ms);\
eeprom_write_byte((uint8_t*)(0x1F8), 0x8E);\
Timer_T0_sub(T0_delay_4ms);*/
