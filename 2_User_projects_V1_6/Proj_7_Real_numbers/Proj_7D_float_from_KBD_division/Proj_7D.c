
/*Proj_7D_float_from_KBD_division
**********************************************************************/


/*IT INTRODUCES


1.	The notation used here for a floating point number.
For example a number like 123.456 is converted to 0.123456 x 10^3. 
0.875 is converted to binary as 0111 0000 0000 0000 0000 0000 0000 0000 where the MSB is the sign bit.

2.	Project subroutine "fpn_from_KBD()".
This is used to acquire floating point numbers from the keyboard. Consider the number 0.123456. 
It is converted to binary using the subroutine "Fraction_to_Binary_Signed(123456,1000000)".
Note: The exponent must be stored separately.

3.	Division of the floating point numbers.
As for fixed point numbers:
The number on the LHS of the DP is generated using the compiler (i.e. Num_1/Num_2). The number on the RHS of 
the binary point is determined by processing the remainder and denominator in "Fraction_to_Binary_Signed()".

Note that the improved precision comes at the cost of 40 bits: 8 for the exponent and 32 for the number 0.123456,
as well as more efficient use of the 32 bits.

4.	Project subroutine "fpn_to_PC()".

5.	Project subroutine "I2C_Tx_float_num()".
	Floating point numbers can easily acquire more than 8 characters. This sends them to the display in a rotating format.

6.	Project subroutine "I2C_Tx_float_display_control()".
This enables users to pause the display using sw2.*/





#include "Proj_7D_header_file.h"



long fpn_from_KBD_local(char*, signed char*);
void fpn_to_PC_Local(long, signed char);


int main (void){
long number_1, number_2, number_3, Ans_LHS;
signed char  expnt_1, expnt_2, expnt_3;
char digits[8];

setup_HW; 

while(1){

String_to_PC("Send real number plus cr twice.\r\n\
Press sw2 to pause display and sw1 or 3 to enter new number\r\n\
or zero to exit\r\n");

number_1 = fpn_from_KBD_local(digits, &expnt_1);
fpn_to_PC_Local(number_1,expnt_1); 											//fpn format is +/-0._ _ _ _.... +/-E_ _
																			//All numbers have 32 bits on the RHS of the decimal point
while(1){
Char_to_PC(' '); String_to_PC ("/ ");
number_2 = fpn_from_KBD_local(digits, &expnt_2);
if(!(number_2))break;
fpn_to_PC_Local(number_2,expnt_2); Char_to_PC(' '); String_to_PC ("= ");

Ans_LHS = number_1/number_2; expnt_3 = expnt_1 - expnt_2;

if(Ans_LHS != 0)															//Maintain fpn format (i.e. answer must be +/-0._ _ _ +/-E_ _
{number_3 = Fraction_to_Binary_Signed(((number_1/10)%number_2), number_2);
expnt_3++;}																	//"Fraction_to_Binary" subroutines ignore the binary point
else 																		//This is equivalent to multiplying both numerator and denominator by 10 exp 9													
number_3 = Fraction_to_Binary_Signed((number_1%number_2), number_2);

fpn_to_PC_Local(number_3, expnt_3);Char_to_PC('\t');						//sendBinary(number_3);
I2C_Tx_float_num(number_3, expnt_3);
I2C_Tx_float_display_control();



newline();
number_1 = number_3; expnt_1 = expnt_3;}}}



/***********************************************************************************************************************/

long fpn_from_KBD_local(char * digits, signed char *expnt){
long num_1=0,Denominator=1;
long RHS_of_BP;

char sign = '+';
*expnt = 0;
Real_num_string_from_KBD(digits); if(!(digits[0])){return 0;}				//Ilegal character?
																			//Scan the display from the LHS
{int m=8; while(!(digits[m-1]))m--;										//Stop at the first character
if (digits[m-1] == '-')sign = '-'; 										//If it is '-' a negative number 
else {num_1 = digits[m-1] - '0'; 											//is to be entered
if(num_1){Denominator *=10;(*expnt)++;}}m--;	
while (m && (digits[m-1] != '.'))											//Continue scanning until a '.' is detected 
{num_1 = 																	//or the end of the display is reached
(10*num_1) + digits[m-1] - '0'; 
Denominator *=10;(*expnt)++; m--;}											//Continue converting digits and
																			//building up the LHS of number
if(m)m--; 
while (m){num_1 = 															//Repeat for the RHS of the number
(10*num_1) + digits[m-1] - '0'; m--; 
if(num_1)Denominator *=10; if(!(num_1))(*expnt)--;	}					//Calculate denominator used to convert RHS to decimal
		
while(Denominator/num_1 > 10)
{Denominator /= 10; (*expnt)--;}}											//Ensures -0.05 ends up as -0.5E-1 and not -0.05E0


if(sign == '-')	num_1 = -num_1;												//Standard negation: complement and add 1
RHS_of_BP = Fraction_to_Binary_Signed(num_1, Denominator);					//Obtain the decimal part of the number			
return RHS_of_BP;}



/***********************************************************************************************************************/

void fpn_to_PC_Local(long number, signed char expnt){						//Receives fpn as +/-0._ _ _ _ .... 
long  RHSDP;
char No_dps = 7;															//with a decimal exponent
																			//Convert the RHS of the binary point
RHSDP = Binary_points_to_Decimal_Signed(number);							//to a decimal number
if(number < 0){String_to_PC("-0"); RHSDP = ~RHSDP;}
else Char_to_PC('0');	
if (!(decimalOverflow(10, RHSDP, No_dps))){RHSDP = 0;
Char_to_PC('\b');Char_to_PC('1');Char_to_PC('c');}
Decimal_to_PC(10, RHSDP, No_dps);Exp_to_PC(expnt);}





