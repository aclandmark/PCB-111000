

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"
#include "../../Resources_V12/Subroutines/Data_to_from_EEPROMs.c"
#include "../../Resources_V12/Subroutines/Random_and_prime_nos.c"

#define Two_sec_WDT_with_interrupt \
wdr();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = (1<< WDE) | (1 << WDIE) |  (1 << WDP2) |  (1 << WDP1) |  (1 << WDP0);

void backup_the_display(char, char);
void restore_display(char*);