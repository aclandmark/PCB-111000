
/*Proj_7C_fixed_point_arithmetic
******************************************************************************/


/*Tests the fixed point entry and display subroutines by doing some more arithmetic. 
Note: the decimal point is not explicitly present in the binary representation of the number.  
It is always assumed to be between bits 15 and 16.


IT INTRODUCES


1.	Project subroutine "Fraction_to_Binary_Signed()".
This is a development of "Fraction_to_Binary()", in which either numerator or denominator can be signed and 
the MSB of the result is the sign bit.

2.	Multiplication of two fixed point numbers.
Each number is initially shifted 8 places right to allow for overflow. This may loose some of the bits of 
low significance.

3.	Division of fixed point numbers.
The number on the LHS of the BP is generated using the compiler (i.e. Num_1/Num_2). The number on the RHS of the BP 
is determined by processing the remainder (i.e. Num_1%Num_2) and denominator (i.e. Num_2) in subroutine 
"Fraction_to_Binary_Signed()".

Note:	
Addition and subtraction of two fixed point numbers is completely managed by the compiler.

USER INSTRUCTIONS

Enter number and terminate in the return key
Enter op: +, -, * or /
Enter second number and terminate in the return key and the result will be given
Enter a second op the third number plus the return key.
Continue with more arithmetic


Note: Performance may be OK for some applications but is disappointing (especially for multiplication) when 
compared with a calculator.*/




#include "Proj_7C_header_file.h"



int main (void){

char digits[8],  decimal_sign = '+', op;
long number_1, number_2;
unsigned long RHS_of_BP;
int No_dps = 3;

setup_HW;
String_to_PC("Send numbers and op (terminate numbers in cr)\r\n");

number_1 = Fixed_pt_number_from_KBD(digits);								//Subroutine developed in proj_8B
RealNum_to_PC(number_1, 4); 													//Subroutine developed in proj_8B
 
while(1){																	//infinite loop

op = waitforkeypress();														//enter arithmetical op
switch(op){
case '+': case '-': case '*':  
case '/': case 'x':break;
default: op = 'y'; Char_to_PC('!'); break;}
if(op == 'y')continue;														//branch straight back to start of the infinite loop
if(op == 'x')break;															//escape from the infinite loop
Char_to_PC(op); Char_to_PC('\t');

number_2 = Fixed_pt_number_from_KBD(digits);								//Get second number from KBD
RealNum_to_PC(number_2, 4);													//Echo it to PC
switch(op){
case '+': number_1 += number_2; break;
case '-': number_1 -= number_2; break;
case '*': number_1 = (number_1 >> 8) * (number_2 >> 8); break;				//>>8 to make allowance for overflow
case '/': 	
RHS_of_BP = Fraction_to_Binary_Signed((number_1)%(number_2), number_2);	//Determine number on RHS of binary point
if((RHS_of_BP >> 16) & (1<<15)) decimal_sign = '-'; 						//Test bit 31 which is the decimal sign bit
else decimal_sign = '+';													//and record the sign

RHS_of_BP = RHS_of_BP << 1;													//remove the decimal sign bit
number_1 /= number_2;														//determine LHS of binary point

if(decimal_sign == '-')number_1--;											//Compiler added 1 to form a negative number; this must be removed
number_1 = (number_1 << 16) + (RHS_of_BP>>16);								//Re-assemble number_1 so that binary point is between bits 15 and 16

break;}

String_to_PC("\t=");RealNum_to_PC(number_1, No_dps);							//Send the real number to the PC
I2C_Tx_real_num(number_1);													//and over the I2C bus to the display
}SW_reset;}



