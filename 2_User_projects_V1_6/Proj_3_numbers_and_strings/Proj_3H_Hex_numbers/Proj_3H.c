
/*Proj_3H_Hex_numbers
***********************************************************/

/*QUICK INTRODUCTION TO HEX NUMBERS


IT INTRODUCES


1.	Hex number format.

2.	Project subroutine  "Binary_to_PC_local()".*/



#include "Proj_3H_header_file.h"



int main (void){
int Num;
char digit_array[4], radix = 16, Inc;

setup_HW;

	String_to_PC("Number then Inc? 0-9 then AK or x\r\n");	
		Num = waitforkeypress()-'0';
		Inc = waitforkeypress()-'0';
do{ 
	Local_BtoA(Num, digit_array);   					//Convert binary number to an askii string
	NumericString_to_PC_local(digit_array);				//send the string to the pc
	Char_to_PC('\t');
	Num_to_PC(radix, Num);Char_to_PC('\t');			//Send the number to the pc with a radix of 16.
	Binary_to_PC_local(Num);							//Send the number to the pc in binary form
	String_to_PC("\r\n");
	Num= Num + Inc;} while(waitforkeypress() != 'x');	//increment the number
	SW_reset;}
	
	

/**************************************************************************************************/
void NumericString_to_PC_local(char* s){
int n=0;
while (s[n] != '\0')n++;								//scan through the string until the '\0' is found
for(int m = n; m; m--)Char_to_PC(*(s + m-1));			//send characters to the pc finishing with s[0]
Timer_T0_10mS_delay_x_m(4);}							//rather than starting with it



/**************************************************************************************************/
void Local_BtoA(long num,char digit_array[]){			//Local function only: check with paper and pencil
int m=0;
do  {digit_array[m++] = (num%10) + '0';}
while((num = num/10) > 0);
digit_array[m] = 0;}



/**************************************************************************************************/
void Binary_to_PC_local(int x){							//check with paper and pencil
for (int m = 0; m<16; m++){
	if(x & (1 << (15-m))) Char_to_PC('1');				//test each bit in turn starting with the most significant
	else Char_to_PC('0'); 	
	if(m%4==3)String_to_PC("  ");}}

