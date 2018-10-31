
/*Proj_3I_Hex_Data_from_KBD
***********************************************************/

/*MORE ON HEX NUMBERS


INTRODUCES


1.	Project subroutine "hex_from_KBD()" which is similar to "num_from_KBD".

2.	Project subroutine "Num_to_PC_U()" an unsigned version of "Num_to_PC".

3.	Project subroutine "I2C_Tx_long()" which sends a long number to the display.

It explores the operation of 8 bit (char), 16 bit (int) and 32 bit (long) numbers.


Note:
To divide a positive number by 2:	Shift the binary equivalent one place to the right.

To divide a negative number by 2:	record bit[0], shift binary number right, make the MSB 1 
and add 1 if bit[0] was 1 (this ensures that there is always a positive remainder).

To multiply any number by 2: shift the binary number left.

Notice that division looses the LSBs and multiplication looses the MSBs. 
If these are 1s the number cannot be restored by subsequent multiplication or division.


OPERATION

Enter a number in hex format when requested and do some arithmetic on it (see "switch statement block" for options).*/




#include "Proj_3I_header_file.h"



int main (void){
char op;	
char No_bits;
test_Int;															//see .h file for alternatives to test long or char numbers

setup_HW;

while(1){
String_to_PC("Hex Num? (Terminate with Return key)\r\n");
Num_1 = Hex_from_KBD(); newline();									//Similar to num_from_KBD();

Num_to_PC(16, Num_1); String_to_PC(" (");							//Echo number in hex format
Num_to_PC(10, Num_1); String_to_PC(")\t\t");						//Echo number in decimal format									
Binary_to_PC_Local_1(Num_1, No_bits);								//Echo the number in binary format

Char_to_PC('\t');if(Num_1 < 0){Num_2 = 0x100000000 + Num_1;		//If user entered a negative number
Num_to_PC_U(16, Num_2);}											//echo it in unsigned hex format

I2C_Tx_long(Num_1);													//Send the number to the display 

while((op = waitforkeypress()) != 'x'){							//Enter arithmetic operator
newline();
Num_1 = inc_num(Num_1,op);											//Do some arithmetic 
I2C_Tx_long(Num_1);													//Send the result to the display

Num_to_PC(16, Num_1); String_to_PC(" (");							//Send the result to the pc in hex,
Num_to_PC(10, Num_1); String_to_PC(")\t\t");						//decimal and binary formats
Binary_to_PC_Local_1(Num_1, No_bits);

Char_to_PC('\t');if(Num_1 < 0){Num_2 = 0x100000000 + Num_1;		//If the result is negative
Num_to_PC_U(16, Num_2);}}newline();}}								//echo it in unsigned hex format



/**************************************************************************************************/
long inc_num(long Num_1, char op){									//Some basic arithmetic
switch(op){
case 'X': Num_1 *= 2; break;
case '/': Num_1 /= 2; break;
case '-': Num_1 *= -1; break;

case '1':case '2':case '3':case '4':
case '5':case '6': case '7':case '8':
case '9': Num_1 = Num_1 + op - '0'; break;

case 'A':case 'B':case 'C':case 'D':case 'E':
case 'F': Num_1 = Num_1 + op - '0' - 7; break;

case 'G': Num_1 = Num_1 + 0x10;break;
case '!': Num_1 = Num_1 - 1; break;
case '"': Num_1 = Num_1 - 2; break;
case '£': Num_1 = Num_1 - 3; break;
case '$': Num_1 = Num_1 - 4; break;
case '%': Num_1 = Num_1 - 5; break;
case '^': Num_1 = Num_1 - 6; break;
case '&': Num_1 = Num_1 - 7; break;
case '*': Num_1 = Num_1 - 8; break;
case '(': Num_1 = Num_1 - 9; break;

case 'a':case 'b':case 'c':case 'd':
case 'e':case 'f': Num_1 = Num_1 - op + 87; break;
case 'g': Num_1 = Num_1 - 0x10;break;}
return Num_1;}



/**************************************************************************************************/
void Binary_to_PC_Local_1(long num, char No_digits){
char counter = 0;
if(No_digits == 32){Binary_to_PC(num); return;}
for (unsigned int m = (No_digits); m; m--)
{if (num & (1 << (m-1)))Char_to_PC('1');else Char_to_PC('0');
counter++; if(!(counter%4))Char_to_PC(' ');}}


