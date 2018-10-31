
/*Proj_2A_reaction_time_tester
***********************************************************/


/*THE LAST of PROJ_1A: A REACTION TIME TESTER


Press SW2 and the leds scan across the display. Release it fast enough when they line up and the display will flash.

Lower leds sweep as for Proj_1A, when the display increments the next segment is illuminated.
At the same time the upper leds increment by 1,2 or 3 leds in random order.
Therefore it is only occasionally that a pair of segments are illuminated. This is when SW2 should be released.


DISCOVERING MORE ABOUT THE ATMEGA HW


1.	Power on Reset (POR).  
This is the condition of the microcontroller immediately after power has been applied.  Data memory is clear 
and operation starts with the "main" routine.
This program like most of them will start when power is applied.  There is no need to connect the PCB to a PC. 

Other resets used in these projects are a reset pin (the external reset) that re-initiates operation 
immediately after programming, the watch dog timer that initiates a reset if it is allowed to time out 
and the brown out reset (BOR) that generates are reset if the supply voltage droops excessively.

2.	EEPROM memory.		
This is memory that can be used to hold data that must not be lost when power is removed or an external reset 
or watch dog timeout occurs.


INTRODUCING RANDOM NUMBER GENERATION

1.	Project subroutine "PRN_16bit_GEN()"
The random number generator project subroutine "PRN_16bit_GEN()" can generate 2^16 -1 (65535) different numbers.
Note there is a similar routine "PRN_8bit_GEN()" that can generate 255 different numbers.

These generators use the top two locations of the EEPROM. 

Google "LFSR" (Linear-Feedback Shift Register) for details of random number generation.


OPERATION

Only use switch_2 the middle switch: Press it to start the display and
when the segments line up release it immediately and they will flash.*/



#include "Proj_2A_header_file.h"


	
int main (void){
unsigned int rand;						//random number
unsigned int PORT_1, PORT_2;

setup_HW;
while(switch_2_up);						//wait for switch 2 to be pressed before starting


PORT_1=0b0000000000000001; 
PORT_2=0b1000000000000000;																		
while (1){								//Infinite loop that could be replaced by a "SW_reset"																			

while(1){								//Infinite loop. 
Timer_T0_10mS_delay_x_m(1);				//The "break" statement is used to exit this loop
while(switch_2_up);						//halt if switch 2 is released
rand = (PRN_16bit_GEN(0))%3 + 1;		//generate a random number (1,2 or 3)
for (int m = 0; m < rand; m++)			//set up a loop 	
{if (PORT_1 == 0b1000000000000000)		//Either reinitialise PORT_1 
PORT_1 = 0b0000000000000001; 			//or
else PORT_1 = PORT_1 << 1; }			//prepare to shift the top row of the display left  	
if (PORT_2 == 0b1000000000000000)		//by 1, 2 or 3 places
PORT_2 = 0b0000000000000001;			//prepare to shift the lower row of the   
else PORT_2 = PORT_2 << 1;				//display left but only by one place

I2C_Tx_2_integers(PORT_1, PORT_2);		//update the display

if(PORT_1 == PORT_2)					//If a single vertical line in illuminated
{Timer_T0_10mS_delay_x_m(35);			//start a timer and pause
if(switch_2_up)break; 					//If switch 2 has been released "break" out of the while loop
else while(switch_2_down);}				//else wait for it to be released

if(PORT_1 != PORT_2) 					//If disjointed segments are illuminated
{Timer_T0_10mS_delay_x_m(8);}}			//momentarily pause before returning  
										//to the top of the while loop
while(switch_2_up){
I2C_Tx_2_integers(PORT_1, 0);			//flash the display until the switch is up
Timer_T0_10mS_delay_x_m(16);
I2C_Tx_2_integers(0, PORT_2);
Timer_T0_10mS_delay_x_m(16);}}}		//When it is pressed again return to the outer while loop


