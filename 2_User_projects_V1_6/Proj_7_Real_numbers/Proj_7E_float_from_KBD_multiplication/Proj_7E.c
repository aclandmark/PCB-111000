
/*Proj_7E_float_from_KBD_multiplication
*****************************************************************/


/*IT INTRODUCES 


1.	Project subroutine "fpn_multiply()".
Multiplication causes problems due to overflow.  Therefore this subroutine takes the reciprocal of one of 
the numbers and divides the other by this reciprocal. A single multiplication step is therefore replaced 
by two divisions.
Note: division only looses the least significant bits due to overflow.

2.	Project subroutine "fpn_divide()" which uses code developed in  "Proj_7D_float_from_KBD_division".

3.	Project subroutine "fpn_reciprocal()". This calls "fpn_divide()".


Note: 
1 is converted to 0.1 E1.  
0.1 is 1/10 = 0001 1001 1001 1001 1001 (obtained using Proj_7A_unsigned_fraction).
The binary pattern is shifted 1 place to the right to make  room for the sign bit. 
Therefore 0.1 becomes 0000 1100 1100 1100 1100...... = 0x0CCCCCCC*/





#include "Proj_7E_header_file.h"


void fpn_divide_Local(long *, signed char*);
long fpn_reciprocal_Local(long, signed char, signed char*);
void fpn_multiply_Local(long* , signed char *);


int main (void){
long number[3];		
signed char  expnt[3];		
char digits[8];

setup_HW; 

while(1){

String_to_PC("Send real number plus cr twice.\r\n\
Press sw2 to pause display and sw1 or 3 to enter new number\r\n\
or zero to exit\r\n");

number[0] = fpn_from_KBD(digits, &expnt[0]);
fpn_to_PC(number[0],expnt[0]); 							//fpn format is +/-0._ _ _ _.... +/-E__

while(1){
Char_to_PC(' '); String_to_PC ("* ");

number[1] = fpn_from_KBD(digits, &expnt[1]);
if(!(number[1]))break;
fpn_to_PC(number[1],expnt[1]); Char_to_PC(' '); String_to_PC ("= ");

fpn_multiply_Local(number,  expnt);

fpn_to_PC(number[2], expnt[2]);Char_to_PC('\t');

I2C_Tx_float_num(number[2], expnt[2]);
I2C_Tx_float_display_control();

newline();
number[0] = number[2]; expnt[0] = expnt[2];}}}



/*****************************************************************************************************************************/

void fpn_multiply_Local(long number[], signed char expnt[]){
signed char expnt_reciprocal;

number[1] = fpn_reciprocal_Local(number[1],expnt[1],&expnt_reciprocal);
expnt[1] = expnt_reciprocal;
fpn_divide_Local(number, expnt);}



/*****************************************************************************************************************************/

void fpn_divide_Local(long num[], signed char  expnt[]){
long Ans_LHS;

Ans_LHS = num[0]/num[1]; expnt[2] = expnt[0] - expnt[1];
if(Ans_LHS != 0)
{num[2] = Fraction_to_Binary_Signed(((num[0]/10)%num[1]), num[1]);
expnt[2]++;}
else num[2] = Fraction_to_Binary_Signed((num[0]%num[1]), num[1]);}




/*****************************************************************************************************************************/

long fpn_reciprocal_Local(long num, signed char num_expnt, signed char *res_expnt){
signed char  local_expnt[3];
long local_num[3]; 

local_num[0] = 0x0CCCCCCC;		//0.1
local_expnt[0] = 1;				//10
local_num[1] = num;
local_expnt[1] = num_expnt;
fpn_divide_Local(local_num, local_expnt);
*res_expnt = local_expnt[2];
return local_num[2];}



