

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"
#include "../../Resources_V12/Subroutines/scanf_and_printf_customers.c"

void scroll_display_zero(void);
void shift_display_left(void);
double acquire_fpn(void);


int Double_from_IO(FILE *mystr_input);
FILE uart_input_Double_from_IO = FDEV_SETUP_STREAM(NULL, Double_from_IO, _FDEV_SETUP_READ);
FILE  mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);