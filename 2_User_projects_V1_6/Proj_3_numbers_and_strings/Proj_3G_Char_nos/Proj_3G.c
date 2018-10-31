
/*Proj_3G_Char_nos
***********************************************************/

/*USES THE DISPLAY TO EXPLORE THE OPERATION OF BINARY NUMBERS



IT INTRODUCES



1.	Binary notation for unsigned integers from 0 to 255 and signed integers between -128 and +127.

2.	Project subroutine "I2C_Tx_display_char(digit, mode)" where digit is any binary number between 0 and 11111111 
and mode is either "u" for unsigned or "s" for signed.


OPERATION

The 4 LHS digits of the display are used to display the binary number and the 4 RHS digits for the decimal number.
Press sw1 to increment the binary number or sw3 to decrement it.
sw2 toggles the display between "signed" and "unsigned" numbers.

Switch location on PCB:	 SW3 - SW2 - SW1

Note: 
This project has been compiled with Optimisation level 1 in an attempt to minimise occaisional unstable operation.*/




#include "Proj_3G_header_file.h"




char digit=1, op, mode;

int main (void){

op=0;
mode = 'u';													//mode: signed or unsigned
setup_HW;
I2C_Tx_display_char(digit,mode);
setup_and_enable_PCI;
sei();


while(1){
switch(op){
case 0: break;												//Static display
case 1: I2C_Tx_display_char(++digit,mode);break;			//Increment "digit" before calling "I2C_Tx....."
case 2: I2C_Tx_display_char(--digit, mode);break;}			//Decrement "digit" before calling "I2C_Tx....."

Timer_T0_10mS_delay_x_m(15);}}



/**************************************************************************************************/
ISR(PCINT2_vect) {											//sw1 and sw3 interrupt
if(switch_2_down)return;									//Ignore if sw2 is still down
if((switch_1_up) && (switch_3_up)){op = 0; return;}		//Both switches up

if(switch_1_down) {op = 1; return;}						//Digits increment
if(switch_3_down) {op = 2; return;}}						//Digits decrement


/**************************************************************************************************/
ISR(PCINT0_vect){											//sw2 interrupt service routine
if(switch_2_up)return;										//Ignore sw2 key release
if((switch_1_down) || (switch_3_down))return;				//Ignore if sw1 or 3 are still down
if (mode == 's')	
{mode = 'u'; I2C_Tx_display_char(digit,mode);return;}		//toggle display from signed to unsigned
if (mode == 'u')
{mode = 's'; I2C_Tx_display_char(digit,mode);return;}}		//and visa-versa
