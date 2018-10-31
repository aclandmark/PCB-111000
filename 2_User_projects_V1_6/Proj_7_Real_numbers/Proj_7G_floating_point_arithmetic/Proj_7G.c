
/*Proj_7G_floating_point_arithmetic
******************************************************/


/*IT INTRODUCES
Basic calculator using floating point arithmetic for comparison with "Proj_7C_fixed_point_arithmetic"

Note that for this calculator, floating point numbers are given an additional 8 bits (to store the exponent) 
as well as making more efficient use of the 32 bits used for fixed point numbers. */



#include "Proj_7G_header_file.h"



int main (void){

char digits[8],  op;
long number[3];		
signed char  expnt[3];		


setup_HW;
String_to_PC("\r\nSend real numbers and op (terminate numbers in cr)"); 
String_to_PC("\r\nPress sw2 to pause display.");  
String_to_PC("\r\nPress sw1 or 3 then new op and new number");
String_to_PC("\r\n( or press x in place of op to reinitialise everything\r\n"); 
number[0] = fpn_from_KBD(digits,&expnt[0]);
fpn_to_PC(number[0],expnt[0]);  
 
while(1){

op = waitforkeypress();
switch(op){
case '+': case '-': case '*':  
case '/': case 'x':break;
default: op = 'y'; Char_to_PC('!'); break;}
if(op == 'y')continue;
if(op == 'x')break;
Char_to_PC(op); Char_to_PC('\t');

number[1] = fpn_from_KBD(digits,&expnt[1]);			//Print the number out and do some arithmetic
fpn_to_PC(number[1],expnt[1]); 
switch(op){
case '+': add_fp(number,  expnt, '+'); break;
case '-': add_fp(number,  expnt, '-'); break;
case '*': fpn_multiply(number,  expnt); break;
case '/': fpn_divide(number,  expnt);	break;}

String_to_PC("\t=");fpn_to_PC(number[2],expnt[2]);  

I2C_Tx_float_num(number[2],expnt[2]);
I2C_Tx_float_display_control();

number[0] = number[2];
expnt[0] = expnt[2];
}SW_reset;}



