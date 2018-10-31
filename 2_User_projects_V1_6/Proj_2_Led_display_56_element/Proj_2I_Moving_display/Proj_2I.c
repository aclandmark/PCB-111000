/*Proj_2I_Moving_display
************************************************************************/

/*GENERATES A MOVING DISPLAY	

It makes use of pointers to display digits.

Note: At a latter stage entering numbers will be made much easier because all strings and pointers
will be dealt with in the ATMEGA 328 by the mini-OS. 



IT INTRODUCES


The Project subroutine "decimal_digit (keypress)". 
This returns 1 if keypress is 0 to 9 and zero other wise.


Note: From now on "waitforkeypress()" will normally be used to wait for input from the PC keyboard. 
It is not normally considered good programming technique but does simplify programs.

When writing from scratch it might be worth including the following statement:

#define waitforkeypress() while(1){if(isCharavailable(10)){keypress = receiveChar();break;}}

and ensuring that a "wdr()" is included in the "isCharavailable()" subroutine as in Proj_2E. 

This will enable routine use of the WDT if required.*/



#include "Proj_2I_header_file.h"



#define rotate_display_left  for(int m = 16; m; m--)digit_array[m] = digit_array[m-1];\
digit_array[0] = digit_array[16];




char* string_ptr = 0;											//pointer: will be loaded with the address of a segment string 

int main (void){
char counter=0;					
char digit_array[17];											//Large array used to hold digits as they are rotated
																//(ie the addresses defined by "zero", "one", "two" etc....) 
setup_HW;

for(int m = 0; m < 17; m++)digit_array[m] = 0;					//Clear the array
String_to_PC("Send digits? -x- when complete, AK to rotate");	//User instructions
if (digit_array_to_display(digit_array,  1 ))					//mode is 1: acquire keypresses: returns 1 for 8 digits or 0 for <8 digits
waitforkeypress();												//Only wait if 8 digits were entered


while(1){														
if (counter++ == 8)break;										//Repeat these steps 8 times to clear the display
rotate_display_left;											//Shifts data in "digit_array" one places left
Timer_T0_10mS_delay_x_m(10);									//delay
I2C_Tx_any_segment_clear_all();									//Clear display and then update it from the array "digit_array": 
digit_array_to_display(digit_array,  0 );						//Note: mode is zero (i.e. data is taken from array rather than the keyboard
}waitforkeypress();												//pauses when all digits have shifted off the end of the display

counter=0;

do{
while(1){														//As above but repeat steps 16 times making the number scan across the display
if (counter++ == 16)break;										//Continue shifting digits but rotate display
rotate_display_left;											//so that number re-enters from RH end of display
digit_array_to_display(digit_array,  0 );						//keep rotating digits until user enters 'x'
Timer_T0_10mS_delay_x_m(10);
I2C_Tx_any_segment_clear_all();}counter = 0;}
while(waitforkeypress() !='x');

counter = 0;
while(counter++ <= 8){											//Shift digits back into display
digit_array_to_display(digit_array,  0 );
rotate_display_left;
Timer_T0_10mS_delay_x_m(10);
if(counter != 9)I2C_Tx_any_segment_clear_all();}				//Finnish with number visible on display

waitforkeypress();I2C_Tx_any_segment_clear_all();				//Clear display
newline();SW_reset;}




/************************************************************************************************/
char digit_array_to_display(char digit_array[], char mode)		//"digit_array" is a pointer to a block of memory defined by "main"
{char keypress;
int array_offset = 0;											//takes on values between 0 and 16											

do{
if(mode == 1)													//mode = 1: First of all enter keypresses into "digit_array"
{keypress = waitforkeypress();									//Only keypresses 0 to 9 or x allowed. Otherwise use "continue" to skip input 
if((decimal_digit (keypress)) || (keypress == 'x'))			//mode = 0: Skip keypress entry stage
digit_array[array_offset] = keypress;					
else continue;}

switch(digit_array[array_offset]){								//Read entries stored in "digit_array" and send them to the display.
case '0': string_ptr = zero; break;								//next keypress in "digit_array" is '0': "string_ptr" addresses string "zero" 
case '1': string_ptr = one; break;
case '2': string_ptr = two; break;
case '3': string_ptr = three; break;
case '4': string_ptr = four; break;
case '5': string_ptr = five; break;
case '6': string_ptr = six; break;
case '7': string_ptr = seven; break;
case '8': string_ptr = eight; break;
case '9': string_ptr = nine; break;
case 'x': digit_array[array_offset] = 0; return 0; break;		//Inform "main" that user wants to enter < 8 digits
case 0: string_ptr = null; break;}								//All unused bytes in "digit_array" contain zero: Null generates a space.

{display_num_string(string_ptr, array_offset++);}}				//Increment "array_offset" before executing next line of code
while (array_offset < 8);										//Exit "do" loop when "array_offset" gets to 8

return 1;}														//Inform "main" that user has entered 8 digits



/************************************************************************************************/
void display_num_string (char* s, int digit_num){				//Subroutine requires a pointer to the string		
int char_ptr=0;													//containing segments used to define a digit
char letter;

while(1){
letter = *(s + char_ptr);										//Address of first segment of next digit
switch(letter){
case 'a':
case 'b': 
case 'c': 
case 'd': 
case 'e': 
case 'f': 
case 'g': I2C_Tx_any_segment(letter, digit_num);break;			//update display
case 0:  return; break;
default: break;}char_ptr++;}}									//incrementing "char_ptr" steps through the string
																//Selecting segment letters in turn until the null character:0 is encountered
																
