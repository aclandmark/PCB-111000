
/*Proj_3E_Data_from_KBD
***********************************************************/

/*EXPLORES SCIENTIFIC NOTATION


IT INTRODUCES


1.	Project subroutine "Num_from_KBD_Local()".

This subroutine acquires numerical keypresses and echoes them to the PC and display. Note that the display 
is shifted one place to the left every time that a new digit is entered (rather than acquiring a string, 
reversing it and then sending it to the display).

It then calls "I2C_displayToNum" and returns the result to "main".

2.	Project subroutine "Num_to_PC()".
This performs a binary to askii conversion using a radix of either 10 or 16. The resulting string is then 
sent to the PC using subroutine  "NumericString_to_PC()" which reverses the order of the digits. 

3.	Project subroutine "wait_for_return_key()".  
This traps symbols '\r' (return) and '\n' (new line) and ensures that if both are present they are combined 
into a single '\r'. 

4. 	Project subroutine "display_in_scientific_notation(number)"
This generates a denominator of the form 10, 100 or 1000 etc.  It then divides the number by the 
denominator to give a result in the form 1.2345 or 0.12345 and adds an exponent.  (The latter may be 
preferred for arithmetic, the former for display).

5.	Project subroutine "Divide_and_round()" for integer number division.

6.	A method of trapping overflow that occurs with multiplication.*/



#include "Proj_3E_header_file.h"

long Divide_and_round(long A, int B);										//subroutine prototype


int main (void){
long Num_1;
char digits[8], OVF;
char User_response, mode;
int m;

setup_HW;
User_prompt;
if(User_response == 'r')mode = 1; else mode = 0;


String_to_PC("Enter positive number & terminate with Return key.\r\n");
Num_1 = Num_from_KBD_Local(digits);											//Acquires data and Echoes keypresses to screen
Num_to_PC(10,Num_1); newline();											//Converts number back to a string and sends it to the pc.

String_to_PC("Scientific notation  ");newline();
OVF=0;																		//Set to 1 when multiplication would generate overflow
m = 1;																		//Used for arithmetic
do {																		
if(!(OVF))Num_1 *= m; 														//Do arithmetic
else Num_1 = Divide_and_round(Num_1,m);										//depending on value to OVF												
m++; 
display_in_scientific_notation (Num_1, digits,mode);						//Displays result in scientific notation
if((!OVF)&&((Num_1 > 0xFFFFFFFF/(m+1)) || ((Num_1 * (m+1)) < 0)))		//Check for a potential overflow
OVF = 1;																	//If A*B > 0xFFFF then A > 0xFFFF/B and overflow will occur
																			//Also if A*B is a negative number overflow has occurred


if (!OVF)String_to_PC(" x "); else String_to_PC(" / ");					//Finnish Printing out equation
Num_to_PC(10,m);String_to_PC(" = ");

if(!(Num_1)) String_to_PC("Zero");newline();								

Timer_T0_10mS_delay_x_m(25);
if(waitforkeypress() == 'x') break;										//press -x- to escape early or AOK to continue

}while (Num_1);


SW_reset;}	




/******************************************************************************************/
long Num_from_KBD_Local(char digits[]){						//Local version, ignores negative number
char keypress;												//library version does not.
for(int n = 0; n<=7; n++) digits[n] = 0; 					//Clear array
String_to_PC("Echo keypresses ");
do
{keypress =  waitforkeypress();}							//wait for first keypress
while (!(decimal_digit(keypress)));						//Ignore illegal characters
digits[0] = keypress;										//Save first one to array
Char_to_PC(digits[0]);
I2C_Tx_8_byte_array(digits);								//send array to display

while ((keypress = wait_for_return_key()) != '\r'){		//Continue until return key is pressed

if (decimal_digit (keypress))								//Ignore illegal characters
{for(int n = 7; n>=1; n--)digits[n] = digits[n-1];		//Shifts display left for each keypress
digits[0] = keypress;										//Places new digit in array[0]
Char_to_PC(digits[0]);										//Updates the PC
I2C_Tx_8_byte_array(digits);}}								//Updates the display
newline();
String_to_PC("Number entered   ");
return I2C_displayToNum();}




/******************************************************************************************/
void display_in_scientific_notation(long Numerator,  char digits[], char mode){

long digit, denominator=1;
int m=6, n=0;
char exp=0;

while (Numerator >= 1000000000){Numerator /=10; exp++;}		//Avoid overflow of denominator

clear_digits;													//For each number, generate a denominator (such as 1,10,100 etc)							
if(!(Numerator)){Char_to_PC('0');return;}						//and corresponding exponent
while(denominator <= Numerator)									//The denominator should have one zero for each digit in the number -1
{denominator *=10; exp++;}										//10 to the power of the exponent should equal the denominator	
if(mode){denominator /= 10;	exp--;}							//If this line is omitted the 1234 is converted to the form 0.1234 E4
n = exp;														//rather than 1.234 E3


digits[7]=Numerator/denominator + '0';
digits[6] = '_';												//place first digit followed by a decimal point in the "data array"
Numerator =Numerator%denominator;								//Update the numerator			
Char_to_PC(digits[7]);											//Send data to pc
Char_to_PC('.');


if(!(n)){digits[5] = '0';										//Tidy up display if exponent is zero 
Char_to_PC('0');}

while(n){														//Only calculate significant decimal places
if (Numerator >= 0xCCCCCCC)
{Numerator /= 10; denominator /=10;}							//0xCCCCCCC x 10 0x7FFF FFF8
digit = (Numerator *10/denominator);
Numerator = ((Numerator *10)%denominator);						//check with pencil and paper

Char_to_PC(digit+'0');
if(m){digits[m-1] = (digit+'0');m--;							//Update the display: six characters max
I2C_Tx_8_byte_array(digits);
Timer_T0_10mS_delay_x_m(12);}
Timer_T0_10mS_delay_x_m(6);
n--;}

String_to_PC(" E ");Num_to_PC(10, exp);
digits[0] = exp +'0';											//Overwrite the two LSBs of the display
digits[1] = 'X';												//Displays 'E'.
I2C_Tx_8_byte_array(digits);}




/******************************************************************************************/
long Divide_and_round(long A, int B){
long result;
int error;
result = A/B;
error = (A%B)*10/B;
if (error > 5) result += 1;
return result;}




