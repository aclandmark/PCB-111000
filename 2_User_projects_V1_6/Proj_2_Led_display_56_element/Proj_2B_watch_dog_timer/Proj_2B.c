
/*Proj_2B_watch_dog_timer
*******************************************************/

/*SNOW STORM DISPLAY: 
Uses the watch dog timer to prevent the display from getting jammed in a way that sometimes 
affected earlier programs


PROVIDES MORE ON

1.	Random number generation:
Project subroutine "PRN_16bit_GEN (0)" uses a number stored in EEPROM to generate another one 
which it saves in the same EEPROM location. In this way repeated calls to "PRN_16bit_GEN (0)" 
generate a sequence of random numbers.

Note: 
a.	EEPROM memory is not affected by POR or a WD timeout.
b.	"PRN_16bit_GEN (Num)" generates a random number based on "Num" and it is up to the Firm Ware
(FW) to increment "Num" in some way.

2.	The Watch Dog Timer
Once this has been set running it will reset the program after a predetermined time interval
(250mS in this case) unless it is first reset itself using command "wdr()".


OPERATION


Pressing sw_2 calls subroutine "ISR(PCINT0_vect)" which contain the statement "while(1);"
This halts program flow because there are no active interrupts at this point and therefore
the watch dog timer cannot be reset and will time out.
Note:  Interrupts are automatically disabled when program execution enters an ISR. 
They can however be re-enabled using “sei()” in which case one ISR can be interrupted 
by another interrupt.

Note 
1.	The display pauses but continues in sequence, because the random number generator uses 
the EEPROM for its data storage.
2.	In practice the watch dog delay is made as short as possible without interfering with normal
program operation so that any pause is insignificant.
3.	The switch is used to allow us to test the operation of the WDT. Normally we only want it 
to timeout when a real glitch in the system causes the program to crash.
4.	SW_2 also controls the intensity of the display.  The "setup_HW" macro tests switch_2
and if it is pressed it toggles the display intensity.*/




#include "Proj_2B_header_file.h"



int main (void){	
unsigned int PRN;											//Memory location used to store "pseudo random numbers"

setup_HW;
wdt_enable(WDTO_250MS);										//Set up the watchdog timer to generate a reset after 250mS.
config_sw2_for_PCI;
sei();
while(1){													//Infinite while loop	
PRN = PRN_16bit_GEN (0);									//Generate a new PRN (0) tells subroutine to use the EEPROM
I2C_Tx_2_integers(PRN, (PRN<<1));							//Display two "pseudo random numbers"
Timer_T0_10mS_delay_x_m(10);								//Pause before repeating
wdr();}}													//Reset the watchdog timer which avoids the possibility
															//of a reset for another 250mS

ISR(PCINT0_vect) 
{if (switch_2_up)return; else while(1);}					//If switch_2 is pressed put program execution on hold
															//The watchdog timer will not be reset and will "time out"