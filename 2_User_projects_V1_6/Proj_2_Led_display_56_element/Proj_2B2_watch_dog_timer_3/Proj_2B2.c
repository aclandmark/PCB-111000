
/*
*Proj_2B2_watch_dog_timer_3
*********************************************************************/

/*SNOW STORM DISPLAY VERSION 3


IT INTRODUCES:


1.	Interrupt Service Routine "ISR (WDT_vect){}":
This ISR is called by the watch dog timer when it is operating in its Interrupt and System Reset Mode.

When the WDT times out it initially issues an interrupt and program execution jumps to "ISR (WDT_vect){}" 
which saves the PRN value to the EEPROM. The WDT then resets itself and when it times out for a second time 
it resets the Atmega 168.  This time the program reads the PRN value from the EEPROM and continues driving 
the display from where it left off.

2.	Global variable "watch_dog_reset".

The first line of "setup_HW" sets up the watchdog and leaves it ready to receive a "wdt_enable()" command. 
The first line of "setup_watchdog" checks the status register and if the current reset is due to the WDT it 
sets "watch_dog_reset" to 1.
The firm ware (FW) therefore knows whether it has just been triggered by the WDT or by some other means 
i.e. a POR or by exiting programming mode. 

3.	Project subroutines "Char_to_EEPROM()" used to save data to the EEPROM and "Char_from_EEPROM()" used to 
read the EEPROM.


Note:  The display always starts with all ones when it is switched on or immediately after programming but 
continues from where it was when interrupted by the WDT.


Press sw_2 to operate the WDT.*/






#include "Proj_2B2_header_file.h"



unsigned int PRN;											//Global memory location used to store "pseudo random numbers"

int main (void){	
setup_HW;													//Sets "watch_dog_reset" if the WDT triggered the current program reset


One_25ms_WDT_with_interrupt;								//Reconfigures the WDT for Interrupt and System Reset Mode
if (!(watch_dog_reset))PRN = 0xFFFF; 						//Normal start up i.e. POR or post programming
else {PRN=read_display();watch_dog_reset = 0;}				//Otherwise we have a WD reset: restore PRN with the number saved by the ISR


config_sw2_for_PCI;
sei();
while(1){													//Infinite while loop
I2C_Tx_2_integers(PRN, (PRN<<1));							//Display two "pseudo random numbers"
PRN = PRN_16bit_GEN (PRN);									//Generate a new PRN
Timer_T0_10mS_delay_x_m(10);								//Pause before repeating
wdr();}}													//Reset the watchdog timer


	

/****************************************************************************************/
ISR(PCINT0_vect) 
{if (switch_2_up)return; else {sei();while(1);}}			//If switch_2 is pressed put program execution on hold
															//The watchdog timer will not be reset and will "time out"
															//Note: Global interrupts are automatically disabled when 
															//an ISR runs.  Hence the "sei()" command so that the 
															//WDT interrupt can be triggered.

/****************************************************************************************/
unsigned int read_display(void){										
return((Char_from_EEPROM(0x1FF) << 8) +					//Read the upper 8 bits shift it 8 bits left
Char_from_EEPROM(0x1FE));}									//then add the lower 8 bits
															//return the result to PRN				

/****************************************************************************************/
ISR (WDT_vect){
Char_to_EEPROM(0x1FF, PRN >> 8);							//Write the upper 8 bits of the PRN to EEPROM address 0x1FF 
Char_to_EEPROM(0x1FE, PRN);	}								//Write the lower 8 bits to address 0x1FE



