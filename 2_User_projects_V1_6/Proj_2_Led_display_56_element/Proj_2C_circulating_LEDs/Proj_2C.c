

/*Proj_2C_circulating_LEDs
***************************************************************/


/*OPTIONS FOR A NEW PATTERN GENERATOR THAT USES ALL THE LEDS 



IT INTRODUCES

The project I2C subroutines "I2C_Tx_any_segment(segment, digit_num)" and "I2C_Tx_any_segment_clear_all()".

Note: 
1.	The segments are labelled "a" to "g" and "segment - 'a'" takes on the values 0 to 6
Digits are labelled 0 to 7 and writing to a led toggles it on or off.

2.	Logic required to backup the display looks tricky, requires paper and pencil to get it.
Beginners: Why not concentrate on doing a decent display? The switch block can have any length.


OPERATION

Press sw_2 to reverse the display and any key to activate the WDT.*/




#include "Proj_2C_header_file.h"



int n;													//Provided for use in "while(n < 20){}" loop
char display_bkp[7];									//One element to backup each segment letter	


int main (void){
char segment=0, digit_num=0;	

setup_HW;
Two_50ms_WDT_with_interrupt;							//See .h file


if (!(watch_dog_reset)) 								//Normal start up i.e. POR or post programming
{for(int m = 0; m < 8; m++)
Char_to_EEPROM(m,0);}									//clear backup space in EEPROM

else {for(int m = 0; m < 7; m++)						//New start following Watch dog timeout
{display_bkp[m] = Char_from_EEPROM(m);}				//Read data saved in EEPROM back to "display_bkp"
restore_display(display_bkp);							//Restore the display
watch_dog_reset = 0;}

UCSR0B |= (1 << RXCIE0); 								//Set Interrupt on key press (for test purposes only)
sei();													//Global enable interrupt

while(1){												//Generate pattern
if(Char_from_EEPROM(7))
{n = Char_from_EEPROM(7)+1;Char_to_EEPROM(7,0);}		//Re-instate value of "n" following WDT
else n = 0;
while(n < 20){											//We cannot use (int n = 0; n < 20; n++) {} 
switch(n){												//because n would have to be used outside the loop
case 0: segment = 'a'; digit_num = 0; break;
case 1: segment = 'a'; digit_num = 1; break;
case 2: segment = 'a'; digit_num = 2; break;
case 3: segment = 'a'; digit_num = 3; break;
case 4: segment = 'a'; digit_num = 4; break;
case 5: segment = 'a'; digit_num = 5; break;
case 6: segment = 'a'; digit_num = 6; break;
case 7: segment = 'a'; digit_num = 7; break;
case 8: segment = 'f'; digit_num = 7; break;
case 9: segment = 'e'; digit_num = 7; break;
case 10: segment = 'd'; digit_num = 7; break;
case 11: segment = 'd'; digit_num = 6; break;
case 12: segment = 'd'; digit_num = 5; break;
case 13: segment = 'd'; digit_num = 4; break;
case 14: segment = 'd'; digit_num = 3; break;
case 15: segment = 'd'; digit_num = 2; break;
case 16: segment = 'd'; digit_num = 1; break;
case 17: segment = 'd'; digit_num = 0; break;
case 18: segment = 'c'; digit_num = 0; break;
case 19: segment = 'b'; digit_num = 0; break;}

I2C_Tx_any_segment(segment, digit_num);					//Update display
backup_the_display(segment, digit_num);					//keep backup up to date
Timer_T0_10mS_delay_x_m(10); n +=1; wdr();}			//delay and reset watch dog

if (switch_2_up) {I2C_Tx_any_segment_clear_all(); 		//Normally clear display before repeating it
for(int m = 0; m < 7; m++)display_bkp[m] = 0;}			//and clear display_bkp
														//If switch 2 pressed do not clear display
Timer_T0_10mS_delay_x_m(10);}}							//Just pause before toggling leds off one at a time




/*****************************************************************/
void backup_the_display(char segment, char digit_num){
display_bkp[segment - 'a'] = display_bkp[segment - 'a'] ^ (1 << digit_num);}



/*****************************************************************/
void restore_display(char* array){
	for (int m = 0; m < 7; m++){
	for (int p = 0; p < 8; p++){
	if((array[m]) & (1 << p))I2C_Tx_any_segment(m+'a', p);}}}



/*****************************************************************/
ISR(USART_RX_vect){receiveChar();
I2C_Tx_any_segment_clear_all();
sei();while(1);}	



/*****************************************************************/
ISR (WDT_vect){											//save "display_bkp" to EEPROM before applying reset
for(int m = 0; m < 7; m++){
Char_to_EEPROM(m,display_bkp[m]);}
Char_to_EEPROM(7,n);}									//save value of n



