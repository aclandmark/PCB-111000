
/*Proj_8E_fp_nos_from_IO
**************************************************************/

/*A look at getting floating point numbers from the user switches.


IT INTRODUCES


1.	Project subroutine "scroll_display_zero()".
	This is used to acquire the next digit (i.e. 0-9, d.p. - or E)

2.	Project subroutine "shift_display_left()".
	This is used to enter the latest digit and note any subsequent digits which may now be 
	illegal (i.e. only one decimal point is allowed). 
	
3.	Project subroutine "Double_from_IO()".
	This is where program execution pauses while the user is entering data at the keyboard. 
	A return key press causes program execution to 	leave this subroutine.
	
4.	Project subroutine "acquire_fpn()" the subroutine used to initiate the acquisition of a fpn.

*/



#include "Proj_8E_header_file.h"
#include <math.h>

void scroll_display_zero(void);
void shift_display_left(void);
double acquire_fpn(void);

volatile char Data_Entry_complete, digit_entry;
char scroll_control;
char digits[8];



int main (void){
double x1;

setup_HW;

if((!watch_dog_reset)){
String_to_PC("Press: sw_1 to populate digit_0, sw3 to shift the display left\r\n\
sw_2 to enter the number and sw1 to clear the display.\r\n\
Note: display flashes to indicate number has been entered.\r\n");}

watch_dog_reset = 0;

x1 = acquire_fpn();
printf(" = %g  ?\r",x1);
while(switch_1_up);
SW_reset;}



/********************************************************************************/
double acquire_fpn(void)
{double x1;
setup_and_enable_PCI;
Init_display_for_pci_data_entry;
Data_Entry_complete = 0;
digit_entry = 0;
scroll_control = 0b00011110;
/*
bit 0: exponent disabled
bit 1: decimal point enabled
bit 2: negative sign enabled
bit 3: LHS of exponent
bit 4: Waiting fot first character
*/
stdout = &mystdout;
stdin = &uart_input_Double_from_IO;
sei();
scanf("%lf", &x1);
disable_pci_on_sw1_and_sw3;
disable_pci_on_sw2
return x1;}



/*************************************************************************/
int Double_from_IO(FILE *mystr_input){
char keypress = 0, data = 0;
while((!(Data_Entry_complete)) && (!(digit_entry)));
if((digit_entry) && (!(Data_Entry_complete)))keypress = 1;
if((Data_Entry_complete) && (digit_entry))keypress = 2;
if((Data_Entry_complete) && (!(digit_entry)))keypress = 3;

switch(keypress){
case 1: data = digits[1]; digit_entry = 0; break;
case 2: data = digits[0]; digit_entry = 0; break;
case 3: data ='\r'; Data_Entry_complete = 0; break;}

return data;}



/*************************************************************************/
ISR(PCINT0_vect){
char disp_bkp[8];
if(switch_2_up)return;								//Ignore switch release
digit_entry = 1;
for(int m = 0; m<=7; m++){disp_bkp[m]=digits[m]; digits[m]=0;}I2C_Tx_8_byte_array(digits);
Timer_T0_10mS_delay_x_m(25);						//Flash display
for(int m = 0; m<=7; m++){digits[m]=disp_bkp[m];}I2C_Tx_8_byte_array(digits);
Data_Entry_complete=1;}	



/*************************************************************************/
ISR(PCINT2_vect){
if((switch_1_up) && (switch_3_up))return;
while(switch_1_down){scroll_display_zero();Timer_T0_10mS_delay_x_m(20);}
if(switch_3_down)shift_display_left();
Timer_T0_10mS_delay_x_m(20);
clear_PCI_on_sw1_and_sw3;}



/*************************************************************************/
void shift_display_left(void){
scroll_control &= (~(0x14));						//negative sign & first char disabled

switch(digits[0]){
case '-':	break;
case '.':	scroll_control &= (~(0x3)); break;		//exponent & dp disabled	
case 'e':	scroll_control &= (~(0xB));			//Set RHS and disable d.p. 
			scroll_control |= 0x04; break;			//Enable neg sign
default: 	if(scroll_control & 8)					//If LHS but not RHS
			scroll_control |= 1;break;} 			//enable exponent

shift_digits_left; digits[0] = '0'; 
I2C_Tx_8_byte_array(digits);
digit_entry = 1;}



/*************************************************************************/
void scroll_display_zero(void){ 					//display scrolls 0 to 9 then minus symbol d.p. E and back to 0
switch (digits[0]){
case '9':
switch(scroll_control){
case 0b11110: digits[0] = '-'; break;				//Waiting for first character: digits[0] = '\0'
case 0b01010: digits[0] = '.'; break;				//Waiting for second character: negative number digits[0] = '-'
case 0b01000: digits[0] = '0'; break;				//LHS waiting for first digit (0 to 9)
case 0b01011: digits[0] = '.'; break;				//digits[0] = 0 to 9: can receive d.p. e or additional digit
case 0b01001: digits[0] = 'e'; break;				//Real number: can only receive e or additional digits
case 0b00000: digits[0] = '0'; break;				//RHS: Can only receive digits
case 0b00100: digits[0] = '-'; break;} break;		//RHS: can receive a - or a digit
		

case '-':
switch(scroll_control){
case 0b11110: digits[0] = '.'; break;				//Waiting for first character: digits[0] = '\0'
case 0b00100: digits[0] = '0'; break;} break;		//RHS: can receive a - or a digit

case '.':
switch(scroll_control){
case 0b11110: digits[0] = '0'; break;				//Waiting for first character: digits[0] = '\0'
case 0b01010: digits[0] = '0'; break;				//Waiting for second character: negative number digits[0] = '-'
case 0b01011: digits[0] = 'e'; break;} break;		//digits[0] = 0 to 9: can receive d.p. e or additional digit
	

case 'e':
switch(scroll_control){
case 0b01011: digits[0] = '0'; break;				//digits[0] = 0 to 9: can receive d.p. e or additional digit
case 0b01001: digits[0] = '0'; break;} break;		//Real number: can only receive e or additional digits

default: digits[0] += 1; break;}

I2C_Tx_8_byte_array(digits);}
