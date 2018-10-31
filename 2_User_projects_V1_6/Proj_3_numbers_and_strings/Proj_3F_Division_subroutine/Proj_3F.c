
/*Proj_3F_Division_subroutine
***********************************************************/


/*EXPLORES DIVISION AS MULTIPLE SUBTRACTIONS


A repeat of "Proj_3D_decimal_fractions" but the user enters digits at the keyboard.


IT INTRODUCES


1.	Project subroutine "product_AB()" which performs multiplication by doing multiple additions.

2.	Project subroutine "divide_A_by_B()" which performs division by doing multiple subtractions.

3.	Project subroutine "A_modulo_B()" which returns the remainder produced by dividing one number by another.

Note: We usually leave these details to the HW and compiler.
The ATMEGA has an 8 bit multiplier/divider but that still leaves plenty to be done in FW by the compiler for 
16 or 32 bit numbers.*/





#include "Proj_3F_header_file.h"



int main (void){
long Num_1, Num_2;
char digits[8];

setup_HW;
while(1){
String_to_PC("Num_1?  ");
Num_1 = Num_from_KBD(digits);Num_to_PC(10,Num_1); newline();			//"num_from_KBD" subroutine provides a number in binary form

String_to_PC("Num_2?  ");
Num_2 = Num_from_KBD(digits);Num_to_PC(10,Num_2);newline();

String_to_PC("Answer =   ");

if (Num_1 > Num_2)fraction_to_decimal_string_Local(Num_2, Num_1, digits); 
else fraction_to_decimal_string_Local(Num_1, Num_2, digits); 
newline();}SW_reset;}



/**************************************************************************************************/
void fraction_to_decimal_string_Local(long remainder, long denominator, char digits[]){
long digit;
int m=6;
clear_digits;
digits[7]= '0';
digits[6] = '_';
String_to_PC("0.");

while(switch_2_up){
digit = divide_A_by_B(product_AB(remainder,10),denominator);		//uses local routine for divide function
remainder = A_modulo_B(product_AB(remainder,10),denominator);		//uses local function to provide the remainder
Char_to_PC(digit+'0');
if(m){digits[m-1] = (digit+'0');m--;
I2C_Tx_8_byte_array(digits);
Timer_T0_10mS_delay_x_m(12);}
Timer_T0_10mS_delay_x_m(6);}}



/**************************************************************************************************/
long product_AB(long A, long B){									//Product is simply multiple addition
long product = 0;
while(B){product += A; B--;}										//Check with paper and pencil		
return product;}



/**************************************************************************************************/
long divide_A_by_B(long A, long B){									//Division is multiple subtraction while result exceeds zero
long result = 0;
while(A>0){A-=B;result++;}
if(!(A))return result;
else return result-1;}



/**************************************************************************************************/
long A_modulo_B(long A, long B){									//Check with paper and pencil 
return (A - product_AB(divide_A_by_B(A,B),B));}

