/*Proj_2B1_watch_dog_timer_2
*********************************************************************/

/*SNOW STORM DISPLAY VERSION 2

In this version the random number generator is always reset to 0xFFFF 
(1111 1111 1111 1111) and the EEPROM is not used.

When the WDT times out the display continues after a short pause but 
always restarts from the same state (i.e. all ones).

Press sw_2 to operate the WDT.*/






#include "Proj_2B1_header_file.h"


unsigned int PRN;									//Global memory location used to store "pseudo random numbers"


int main (void){	
setup_HW;
wdt_enable(WDTO_250MS);								//Following a WD reset the PRN is re-initialised to 0xFFFF
config_sw2_for_PCI;
sei();

PRN = 0xFFFF;										//Program supplied initial value fror PRN									
while(1){											//Infinite while loop
I2C_Tx_2_integers(PRN, (PRN<<1));					//Display two "pseudo random numbers"
PRN = PRN_16bit_GEN (PRN);							//Generate a new PRN using the previous value as input
Timer_T0_10mS_delay_x_m(10);						//Pause before repeating
wdr();}}											//Reset the watchdog timer which avoids the possibility
													//of a reset for another 250mS

ISR(PCINT0_vect) 
{if (switch_2_up)return; else while(1);}			//If switch_2 is pressed put program execution on hold
													//The watchdog timer will not be reset and will "time out"

