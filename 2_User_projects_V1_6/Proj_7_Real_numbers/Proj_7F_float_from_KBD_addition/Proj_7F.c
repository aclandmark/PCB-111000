
/*Proj_7F_float_from_KBD_addition
***********************************************************/


/*IT INTRODUCES 


Project subroutine "add_fp()" which adds and subtracts floating point numbers.

Note:
With both addition and subtraction the numbers have to be adjusted so that they both have the same exponent. 
For example:

3 + 0.4 = 0.3E+1 + 0.4E0		0.3E1 + 0.04E1	=	0.34E1	= 3.4

Multiple divisions by 10 generate small errors resulting in answers like 0.499999 in place of 0.5


USER INSTRUCTIONS

Enter first number plus the return key
Enter op + or -
Enter second number plus the return key

Before entering more data press sw1 or sw3
Then enter op and second number
Enter zero in place of second number to restart*/




#include "Proj_7F_header_file.h"

void add_fp_local(long *, signed char *, char);


int main (void){
long number[3];
signed char  expnt[3];
char digits[8];
char op = '+';

setup_HW; 

while(1){

String_to_PC("Send real number plus cr then + or - followed by second number and cr.\r\n\
Press sw1 or sw3 before entering next op and subsequent number (or 0 to escape)\r\n");

number[0] = fpn_from_KBD(digits, &expnt[0]);
fpn_to_PC(number[0],expnt[0]); 														

while(1){
op = waitforkeypress();
if((op != '+') && (op != '-'))break;
Char_to_PC(' '); Char_to_PC(op);Char_to_PC(' ');
number[1] = fpn_from_KBD(digits, &expnt[1]);
if(!(number[1]))break;
fpn_to_PC(number[1],expnt[1]); Char_to_PC(' '); String_to_PC ("= ");

add_fp_local(number, expnt,  op);

fpn_to_PC(number[2], expnt[2]);Char_to_PC('\t');

I2C_Tx_float_num(number[2], expnt[2]);
I2C_Tx_float_display_control();

newline();
number[0] = number[2]; expnt[0] = expnt[2];}}}




/************************************************************************************************************/

void add_fp_local(long *RHS_of_BP, signed char *expnt, char op){

char mode;

if (expnt[0] == expnt[1])mode = 'A';
if (expnt[0] > expnt[1])mode = 'B';
if (expnt[1] > expnt[0])mode = 'C';

switch (mode){
case 'A':
expnt[2] = expnt[1]; break;

case 'B':
while ((expnt[0] > expnt[1]) &&  (RHS_of_BP[1]))
{RHS_of_BP[1] = RHS_of_BP[1]/10; expnt[1]++;}
expnt[2] = expnt[0]; break;


case 'C':
while ((expnt[1] > expnt[0]) && (RHS_of_BP[0])) 
{RHS_of_BP[0] = RHS_of_BP[0]/10; expnt[0]++;}
expnt[2] = expnt[1]; break;}


if(op == '+'){
if(RHS_of_BP[0] == RHS_of_BP[1]*(-1)){RHS_of_BP[2]=0; return;}
else {	RHS_of_BP[0] /= 10; RHS_of_BP[1] /= 10; expnt[2] = expnt[2] + 1;
		RHS_of_BP[2] = (RHS_of_BP[0]) + (RHS_of_BP[1]);}}

if(op == '-'){
if(RHS_of_BP[0] == RHS_of_BP[1]){RHS_of_BP[2]=0; return;}
else {	RHS_of_BP[0] /= 10; RHS_of_BP[1] /= 10; expnt[2] = expnt[2] + 1;
		RHS_of_BP[2] = (RHS_of_BP[0]) - (RHS_of_BP[1]);}}


while((RHS_of_BP[2] < 0x7FFFFFFF/10) && (RHS_of_BP[2] >= -0x7FFFFFFF/10))	
{RHS_of_BP[2] *= 10; expnt[2]--;}

if(expnt[2] <= (-4)){RHS_of_BP[2]=0;expnt[2]=0;}}


