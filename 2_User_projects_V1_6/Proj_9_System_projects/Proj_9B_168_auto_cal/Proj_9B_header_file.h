

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"	
#include "../../Resources_V12/Subroutines/Integers_to_from_PC.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_diagnostic.c"


void I2C_initiate_7_8125mS_ref(void);
long emergency_cal(void);
long compute_error(void);
long Calibration_tuning(char);

#define waiting_for_I2C_master_with_ISR \
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
