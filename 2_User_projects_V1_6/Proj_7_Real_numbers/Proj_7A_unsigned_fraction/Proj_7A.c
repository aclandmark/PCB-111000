
/*Proj_7A_unsigned_fraction
*******************************************************************/


/*IT  INTRODUCES



1.	The three processes that are essential to dealing with real numbers which are:

A	Convert a fraction or a decimal number below 1 to a binary number. 
Consider 0.125.  This is converted to the fraction 125/1000 and then processed as a fraction.
B	Convert the binary number back to a decimal number.
C	Convert the decimal number to a string of digits.


2.	Project subroutine "Fraction_to_Binary()".

This returns the variable "RHS_of_BP". It multiplies the numerator by 2 and divides it by the denominator.  
It then replaces the numerator with the remainder and repeats the process. It saves the results of each 
division (0 or 1) in an intger variable  "RHS_of_BP" starting with bit 15 and working towards bit 0.

3.	Project subroutine "Binary_points_to_Decimal(RHS_of_BP)".

This returns the variable "RHS_of_DP"	. It Converts the binary number "RHS_of_BP" to a decimal number 
between 0 and 999,984,742 (or 999,999,999 for 32 bit precision).

The bits of "RHS_of_BP" are equivalent to 1/2, 1/4, 1/8, 1/16, 1/32...........1/2^16.........1/2^32.

In this subroutine these numbers are multiplied by 1E9 giving:
500,000,000; 250,000,000; 125,000,000; 62,500,000; 31,250,000; 15,625,000........61,035; 30518; 15259.........0.

Therefore binary number 0.1010100000000100 is equal to the decimal number 656,311,035 * 1E-9
(i.e. 500,000,000 + 125,000,000 + 31,250,000 + 61,035).

4.	Project subroutine "Decimal_to_PC(RHS_of_DP)".

This converts the decimal number on the RHS of the decimal point to a string and then sends this to the PC. 
Because the required result might be 0.00xxxxx the subroutine starts by adding number 1000,000,000 to the 
decimal number. When this is converted to a string the initial '1' is replaced by a decimal point.   
Note: the number of decimal places to be displayed, can set with this subroutine and the result is then rounded up.


USER INSTRUCTIONS

Enter the Numerator and press return then enter the denominator and press return.
Press l for 16 bit precision of h for 32 bit precision. 

Try fractions 5/7, 1/8, 1/64, 75/100, 875/1000 etc.  View Binary versions.*/




#include "Proj_7A_header_file.h"



int main (void){
char digits[8], precision;
unsigned long number_1, number_2;
unsigned long RHS_of_DP, RHS_of_BP;

setup_HW; 
String_to_PC("Send positive fraction(i.e. 5/7, 1/8, 75/100, 273/771 etc.)\r\nTerminate each number in cr\
 and press h or l at the = sign\r\n");newline();
while(1){								

number_1 = Num_from_KBD (digits);										//Get number from KBD and	
Num_to_PC(10,number_1); 												//Echo it to the screen
String_to_PC(" / ");
number_2 = Num_from_KBD (digits);
Num_to_PC(10,number_2);				 
String_to_PC(" ="); Char_to_PC('\t');						

if((number_2 <= number_1)){String_to_PC("Vulgar fraction\r\n");break;}

else{
precision = waitforkeypress();											//user enters precision 16 or 8 bit
String_to_PC("0.");														//LHS of decimal point is always zero

switch(precision){

case 'l':																//16 bit precision uses local functions
RHS_of_BP = Fraction_to_Binary_Local(number_1,number_2);
RHS_of_DP = Binary_points_to_Decimal_Local(RHS_of_BP);
String_to_PC("\t0");
Decimal_to_PC_local(10,(RHS_of_DP), 4); String_to_PC("  ?"); break;

case 'h':																//See Arithmetic subroutines for 32 bit precision
RHS_of_BP = Fraction_to_Binary_Unsigned(number_1,number_2); 
Binary_to_PC(RHS_of_BP);												//See "Integers_to_from_PC" subroutines for sendBinary
RHS_of_DP = Binary_points_to_Decimal_Unsigned(RHS_of_BP);
String_to_PC("\t0");
Decimal_to_PC(10,(RHS_of_DP), 7); String_to_PC("  ?"); break;	

default: newline();SW_reset; break;}
}
newline();}SW_reset;}


/**************************************************************************************************************************************/

unsigned long Fraction_to_Binary_Local(unsigned long rem,unsigned long Den)	
{unsigned int Result = 0;
char counter=0;
for(int n = 0; n <= 15; n++){											//Repeat to obtain 16 bits on the RHS of the binary point
	if ((2*(rem))/Den) 												//Obtain individual bits and assemble in variale "Result"
{(Result) |= (1 << (15-n));											//First bit (n==0) or subsequent bits (n!=0)
Char_to_PC(1 + '0');}													//Echo result to the screen
	else Char_to_PC('0');												
counter++; if(!(counter%4))Char_to_PC(' ');							//format the output in groups of 4 bits
(rem) = (2*(rem))%Den;}												//update the remainder before calculating the next bit
return Result;}



/**************************************************************************************************************************************/

unsigned long Binary_points_to_Decimal_Local(unsigned long RHSofBP)
{unsigned long  RHSofDP, decimal_digits;
RHSofDP = 0;
decimal_digits = 500000000;												//Equivalent to 0.5 in decimal (0.1 in binary)
for(int n = 0; n <= 15; n++){											//Obtain each bit on the RHS of the binary point
if (RHSofBP & (1 << (15-n)))RHSofDP += decimal_digits;					//If 1 increment number on RHSD of decimal point 
decimal_digits /= 2;} 													//Half decimal_digits for the next bit
return RHSofDP;}
/*
0.1 in binary = 0.500000000 in decimal
0.01 in binary = 0.250000000 in decimal
0.001 in binary = 0.125000000 in decimal
0.111 in binary = 0.875000000 in decimal
*/


/**************************************************************************************************************************************/

void Decimal_to_PC_local(char radix, unsigned long Hex, int No_dps){ char print_out_string[12];
long inc = 1;
if(Hex == 0) {String_to_PC(".0");return;} 
Hex += 1000000000;														//The 1 defines the location of the decimal point
for(int k = 0; k < (8-No_dps); k++)inc = inc*10;						//"inc" will be used to round "hex" (for 8 decimal places inc = 1)
inc = inc*5;											
Hex += inc;											
SBtoAL(print_out_string, Hex, radix);									//convert "Hex" to a string (starts by clearing the string to all zeros)
for(int k = 0; k <= (8-No_dps); k++)		
{print_out_string[k] = ' ';}											//blank out unwanted decimal places		
print_out_string[9] = '.'; 												//replace the "1" of 100000000 with a decimal point

{int m = 0;
while((print_out_string[m] == '0')||(print_out_string[m] == ' '))
{print_out_string[m] = ' ';m++; }}										//blank out any trailing zeros
NumericString_to_PC(print_out_string);String_to_PC(" ");}				//send the string to the PC (note: it is in the reverse order)



