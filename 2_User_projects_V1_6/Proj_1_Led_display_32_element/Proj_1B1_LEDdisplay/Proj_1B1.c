
/*Proj_1B1_LEDdisplay
**********************************************************/


/*
As Proj_1B: But uses project subroutine "waitforkeypress();" to increment the display.
*/



#include "Proj_1B1_header_file.h"

int main (void){

unsigned int PORT_1=1;		//PORT_1 defines the display and initialises it
char m=1;					//m  is only active for the first 5 cycles of the "while-loop"
char overflow=0;			//overflow is set to 1 when the most left hand leds are illuminated
								
setup_HW;
while(1){	
		
		
	I2C_Tx_2_integers(PORT_1, ~PORT_1);				//the ~ symbol inverts the ones and zeros
	waitforkeypress();									
	if (m<=5){PORT_1 |= (PORT_1 << 1);m += 1;}	 //m += 1; is shorthand for m = m+1;			
	else PORT_1 = PORT_1 << 1;						//once "m" is 6 simply shift the display left				
	if(overflow)PORT_1 |= 1;						//if overflow is 1 execute "PORT_1 |= 1;".
	if (PORT_1 & 0x8000) overflow=1;				//0x8000 = binary 1000000000000000
	else overflow = 0;
	}}												//if PORT_1 is greater than or equal to 0x8000 
													//place 1 in its least significant bit
													//this effectively cycles the display round and round 
		

/*
MORE ON THE LOGIC
Note: a|b and a&b operate on the bits defined by a and b
00001010 | 10101000 = 10101010
11101010 & 10100011 = 10000010   

If  PORT_1 starts of as 00000111  
then the statement PORT_1 = PORT_1 << 1 changes it to 00001110
and the statement PORT_1 |= (PORT_1 << 1)
changes it to  
00000111 | 00001110 which equals 00001111

if PORT_1 = 1xxxxxxxxxxxxxxx then "overflow" is set to save LH 1
which will otherwise be lost at the next shift left
and subsequently place it in the most RH location
Note x either 0 or 1; LH left hand, RH right hand
*/

