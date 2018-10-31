
/*Proj_1C1_Pin_Change_Interupt
**************************************************************/


/*
A SLIGHT VARIATION ON PROGRAM 3 THE REACTION TIME TESTER.
Test your reactions by attempting to shoot the LEDS down using the middle pcb switch (switch_2).

INTRODUCES the statement "if(m == 1)" which means execute the next statement if m equals 1, and
if it equals anything else skip the next statement.
*/



#include "Proj_1C1_header_file.h"



unsigned int PORT_1, mask;			//Do NOT define mask here as 0xFFFF as programmer then fails
char switch_control;				//Prevents rapid switch_2 presses from shooting leds down at random

int main (void){
setup_HW;
config_sw2_for_PCI;					//Enable PCI interrupt on switch_2				 

mask = 0xFFFF;						//0xFFFF = 0b1111111111111111 indicating that none of the leds have yet been shot down
switch_control = 0;
sei();								//Global enable all interrupts (required in addition to "config_sw2_for_PCI")						
while(mask){						//Exit the "while-loop" as soon as mask gets set to zero
PORT_1=1;							//Initialise display to 0000 0000 0000 0001

for(int m = 1; m < 17; m++){		//Repeat "for-loop" 16 times
if(m ==	1)switch_control = 0;		//LOGIC: "Dead" leds are transferred to the bottom row
I2C_Tx_2_integers(PORT_1 & mask, (~mask) ^ PORT_1);
 
Timer_T0_10mS_delay_x_m(10);		//Program execution spends most time waiting here, so this is where the interrupt almost always occurs						
PORT_1 = (PORT_1 << 1);			//Move on to next display location			
}}I2C_Tx_2_integers(0, 0xFFFF);	//When all leds are dead illuminate all the bottom leds and then
Timer_T0_10mS_delay_x_m(100);		//pause for 1 sec before starting all over again.			 
SW_reset;}



/**************Routine executed every time that switch is  operated**************************************/
									
ISR(PCINT0_vect) {					//This ISR momentarily interrupts the main routine
if(switch_2_up)return;				//It notes which LED has just been shot down and 
if (switch_control) return;			//Return if "switch_control" is not zero
mask &= ~PORT_1;					//writes zero at its location in the "mask" register
switch_control = 1;}


