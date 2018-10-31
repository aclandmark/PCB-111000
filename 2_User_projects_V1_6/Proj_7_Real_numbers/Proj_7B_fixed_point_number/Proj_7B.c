
/*Proj_7B_fixed_point_number
**************************************************/


/*IT INTRODUCES


1.	The fixed point number:
Bits 31 to 16 are on the LHS of the decimal/binary point (DP/BP) and bits 0 to 15 are on the RHS.



2.	Project subroutine "Real_from_KBD(array);"
This is similar to subroutine "Num_from_KBD()" but acquires a real number as an 8 bit string.



3.	Project subroutine "Fixed_pt_number_from_KBD()" which:

A.	Scans the 8 bit string supplied by "Real_from_KBD(array);" looking for a - sign and a DP.

B.	Converts the number on the LHS of the DP to a binary number.

C.	Converts the number on the RHS of the DP plus a denominator of the form 10, 100 or 1000 	
	etc. to a binary number <1 using subroutine "Fraction_to_Binary()".

D.	Assembles the bits on either side of the binary point (BP) into a single 32 bit number.

E.	Complements the bits if the number is negative (see below for details).



4.	Project subroutine "RealNum_to_PC()" which:

A.	Disassembles the real number into components one on the LHS and one on the RHS of the BP.

B.	Uses subroutine "Binary_points_to_Decimal()" to convert the bits on the RHS of the BP to a 	decimal number.

C	Uses subroutine "Num_to_PC()" to send the component on the LHS of the BP to the PC.

D.	Uses subroutine "Decimal_to_PC()" to send the component on the RHS of the BP to the PC 	
	and specify the number of digits to be displayed.


5.	Project subroutine "I2C_Tx_real_num()" which sends a real number to the display.


6.	One's and Two's complement for real numbers: Consider

	1.75 	in binary it is 0000 0000 0000 0001 . 1100 0000 0000 0000 and
	-1.75 	in binary is 	1111 1111 1111 1110 . 0011 1111 1111 1111 which is known as the one's complement.
		
1111 1111 1111 1110 	is -2	(using two's complement)
0.1111 1111 1111 1111	is 1 (as good as!) 
0.1100 0000 0000 0000	is 0.5 + 0.25 = 0.75
Therefore 	0.0011 1111 1111 1111	is 1 - 0.75 = 0.25
and 1111 1111 1111 1110 . 0011 1111 1111 1111	is -2 + 0.25 = -1.75 
							
Dealing with numbers that are below 1:

Consider 0.875
The binary form of 0.875 	is 0000.1110
The binary form of -0.875 	is 1111.0001	= 	-1 + 1 - 0.875 = -0.875
Note:
For signed numbers 11111111 = -1
0.0001 = 0.1111 – 0.1110 = 1 – 0.875
	
In the project subroutines 

0.875 is stored as	0111 0000 0000 0000 and
-0.875 is stored as	1000 1111 1111 1111 i.e. all bits are shifted one place to the right leaving 
the MSB free to be the sign bit.

Dealing with numbers above or equal to 1 and with no component below 1:
Consider 15
The binary form of 15 is 0000 0000 0000 1111 . 0000 0000 0000 0000	(the last 16 zeros are not stored)
The binary form of -15 is 1111 1111 1111 0000 . 1111 1111 1111 1111 =  -16 + 1	= -15.
which is stored as 1111 1111 1111 0001
The MSB is -32,768 remaining bits are 16,384; 8,192; 4,096;........8; 4; 2; 1

Acquiring the negative value of a number	
Integer numbers:	Complement all bits and add 1 (two's complement)
Numbers < 1			Complement all bits MSB is -1. Remaining bits are 1/2, 1/4, 1/8, 1/16,......
Real numbers		Complement all bits (one's complement)

Note:
The complexity of transferring numbers to and from the PC is increasing as fixed point and later floating 
point numbers are encountered.  Project subroutines are used at this stage to demonstrate the steps involved 
in acquiring a string of characters including a decimal point and converting it into a number and also of 
course the reverse process.
However there are C libraries which provide a comprehensive range of input output functions.  These will be 
introduced at a later stage.  It is of course still necessary for the programmer to provide the subroutines 
that interface with the hard ware i.e. acquire or display the next character. 
*/



#include "Proj_7B_header_file.h"



int main (void){

char digits[8], sign = '+', keypress;
unsigned long num_1_RHS=0,Denominator=1, RHS_of_BP, RHSDP;
long number_1, num_1_LHS=0,LHS_of_BP;

int No_dps = 8;

setup_HW;
String_to_PC("Send real number & terminate in cr.\r\n\
Then type * / ~ a or A to do some arithmetic\r\n\
x to escape");


/*******************Convert to subroutine Fixed_pt_number_from_KBD()*******************************************************/

Real_num_string_from_KBD(digits); 									//acquires string and displays it
if(!(digits[0])){SW_reset;}										//Ilegal character?
																	//Scan the display from the LHS
{int m=8; while(!(digits[m-1]))m--;								//Stop at the first character
if (digits[m-1] == '-')sign = '-'; 								//If it is '-' a negative number is to be entered
else num_1_LHS = digits[m-1] - '0'; m--;							//Convert the digit to a number
while (m && (digits[m-1] != '.'))									//Continue scanning until a '.' is detected or the end of the display is reached
{num_1_LHS = 
(10*num_1_LHS) + digits[m-1] - '0'; m--;}							//Continue converting digits and building up the LHS of number
if(m)m--; 
while (m){num_1_RHS = 												//Repeat for the RHS of the number
(10*num_1_RHS) + digits[m-1] - '0'; m--; 
Denominator *=10;}}													//Calculate denominator used to convert RHS to decimal

RHS_of_BP = Fraction_to_Binary_Unsigned(num_1_RHS, Denominator);	//Obtain the decimal part of the number			
number_1 = 
(num_1_LHS << 16) + (RHS_of_BP>>16);								//Assemble the LHS & RHS parts to form a real number	

if(sign == '-')	number_1 = ~number_1;								//If the number is negative invert all the bits

/****************************************************************************************************************************/
 
newline(); Binary_to_PC_Local(number_1); Char_to_PC('\t');			//Print the binary number out and do some arithmetic

while(1){

/******************************Convert to subroutine: RealNum_to_PC() ************************************************************/
if(number_1 < 0){sign = '-'; Char_to_PC('-');
number_1 = ~number_1;}else sign = '+';								//Always display +ve decimal number plus -ve sign if necessary

LHS_of_BP = number_1 >> 16;											//Isolate the two halves of the number
RHS_of_BP = (number_1) << 16;
RHSDP = Binary_points_to_Decimal_Unsigned(RHS_of_BP);				//Convert the binary number on the RHS of the binary point
																	//to a decimal number between 0 and 999999999
if (!(decimalOverflow(10, RHSDP, No_dps)))
{RHSDP = 0; LHS_of_BP++; }											//Check for decimal overflow
Num_to_PC(10, LHS_of_BP); Decimal_to_PC(10, RHSDP, No_dps);		//Send the two halves to the number to the PC.
/****************************************************************************************************************************/


if(sign == '-')number_1 = ~number_1;								//Always do arithmetic on the signed number
I2C_Tx_real_num(number_1);											//Send the number over the I2C bus to the display
if ((keypress=waitforkeypress()) == 'x')
{newline();break;}

switch(keypress){
case '*': number_1 = number_1 << 1; break;
case '/': number_1 = number_1 >> 1; break;
case '~': number_1 = (number_1 * -1)-1; break;						//error is due to insufficient bits on RHS of binary pt
case 'a': number_1 = number_1/5 * -2; break;
case 'A': number_1 = number_1/2 * -5; break;}

newline(); Binary_to_PC_Local(number_1);Char_to_PC('\t'); 

}SW_reset;}


/****************************************************************************************************************************/
void Binary_to_PC_Local(long num){
char counter = 0; unsigned int x;
for (unsigned int m = 16; m; m--)
{if ((num>>16) & (1 << (m-1))) Char_to_PC ('1'); else Char_to_PC('0'); 
counter++; if (!(counter%4))Char_to_PC(' ');}
String_to_PC(" . ");
x=num;
for (unsigned int m = 16; m; m--)
{if (x & (1 << (m-1))) Char_to_PC ('1'); else Char_to_PC('0'); 
counter++;if (!(counter%4))Char_to_PC(' ');}}

