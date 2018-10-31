
/*Proj_2H_Numerical_entry with dimmer control
******************************************************************************/


/*AN ALMOST EXACT COPY OF Proj_2G		


IT INTRODUCES the Program Segment


do{if(isCharavailable_Local (10))digit = receiveChar();				
else continue;
.................} while(...);	


for use in place of "waitforkeypress()" when the WDT is required.


Note:
isCharavailable_Local () includes the statement "wdr();"
If no keypress is made the "continue" statement returns program execution back to the top of the "do-loop".
 
Press sw_2 to change the display brightness and sw_1 to generate a WD time out.
Note:	Switch location SW3 - SW2 - SW1.
	Following the WD time out "digit_num" is reset to zero.  Digits already illuminated will be over written 
	or blanked by new keypresses. Use of WD timer with interrupt could save and restore the value of digit_num 
	so rectifying this.*/




#include "Proj_2H_header_file.h"



char Dimmer_control;

int main (void){

char  digit;
char* string_ptr = 0;	
int digit_num=0;											//Identifies which digit in the display will be illuminated  				

setup_HW;
						
config_sw1_and_sw3_for_PCI;
config_sw2_for_PCI;

 
if (!(watch_dog_reset)) 									//Normal start up i.e. POR of post programming
{String_to_PC("Send digits:\r\n\
Press sw_2 to change display brightness\r\n\
sw1 to generate watchdog timeout");}

else { watch_dog_reset = 0;									//Start up following WD time out (reset watch_dog_reset flag)
 String_to_PC("\r\nWatch-Dog Timeout");
I2C_Tx_any_segment('h', 7);}								//Re-establish any segment mode

wdt_enable(WDTO_250MS);										//Start WD timer

Dimmer_control = 1;
digit = '0';

while(1){
digit_num = 0;	
sei();
do{ 														//"isCharavailable_Local()" continuously resets the WD timer
if(isCharavailable_Local (10))digit = receiveChar();		//Probably all interrupts will occur when isCharavailable(10) is executing			
else continue;												//Program flow jumps from here back to the start of the "do-loop"											
															//until a character is received from the keyboard
switch(digit){												
case '0': string_ptr = zero; break;			
case '1': string_ptr = one; break;
case '2': string_ptr = two; break;
case '3': string_ptr = three; break;
case '4': string_ptr = four; break;
case '5': string_ptr = five; break;
case '6': string_ptr = six; break;
case '7': string_ptr = seven; break;
case '8': string_ptr = eight; break;
case '9': string_ptr = nine; break;
default: continue; break;}									//Any character other than 0-9 sends program flow back to the start of the do-loop 										

display_num_string(string_ptr, digit_num);					//Illuminate the digit
digit_num++;}												//Get ready to illuminate next digit
while (digit_num < 8);										//Exit "do-loop" as soon as display is completely illuminated

while(1){
if(isCharavailable_Local (10))break;}						//continuously resets WD timer.
digit = receiveChar();
I2C_Tx_any_segment_clear_all();}}							//clear display and wait for new keypresses



/****************************************************************************************************/
void display_num_string (char* s, int digit_num){			
int char_ptr=0;						
char letter;
while(1){
letter = (s[char_ptr]);
switch(letter){
case 'a': case 'b': case 'c': 
case 'd': case 'e': case 'f': 
case 'g': I2C_Tx_any_segment(letter, digit_num);break;		
case 0:  return; break;
default: break;}char_ptr++;}}					




/****************************************************************************************************/
char isCharavailable_Local (char m){int n = 0;				//Contains a "wdr()" command
while (!(UCSR0A & (1 << RXC0))){n++;						//Increments "n" from zero to 8000
if (n>8000) {m--;n = 0;wdr();}if (m == 0)return 0;}		//Checks the receiver at each increment
return 1;}													//Returns a 1 as soon as a character is received
															//If no character is sent it pauses a while (15m mS)
															//and eventually returns zero.
															//Note: this subroutine enables us to reset the WDT (wdr();)
															//in a way that is not possible with "waitforkeypress()"


/****************************************************************************************************/
ISR(PCINT0_vect) {											//This ISR momentarily interrupts the main routine
if(switch_2_up)return;
I2C_Tx(1, 'W', &Dimmer_control);							//This I2C subroutine blanks the display
I2C_Tx_any_segment('h', 7);}								//Re-awakens display



/*********************************************************************************************************/
ISR(PCINT2_vect){
if(switch_1_down){while(1); }}								//Generate WD time out

