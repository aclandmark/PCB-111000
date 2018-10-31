
/*Proj_6B_Prime_numbers_plus
****************************************************************/


/*IT INTRODUCES


1.	An attempt to have a bit of fun with prime numbers.

2.	Project subroutine "prime_no_generator_plus()".

This generates prime numbers plus a few extras that do look like primes.
Try and guess which are primes and which are not.


USER INSTRUCTIONS

The numbers are sent to the PC screen.
Type in any that look non-prime terminating each with a "return" keypress.
Enter zero to exit and press sw2 when requested.

Switch location SW3(PD2) - SW2(PB6) – SW1(PD7).*/



#include "Proj_6B_header_file.h"


volatile int start_1=0;

int main (void){
int l, start, counter_1, counter_2, array_size=100;	
int  array[array_size];
int factor[10], number;
char digit_array[8], line_counter=0;


setup_HW;
setup_and_enable_PCI;
User_instructions;
sei();
start = 0;

while(1){
counter_1 = 0;														//Counts non-prime numbers
counter_2 = 0;														//Counts all non zero numbers
l=start;
while(l < (start+5)){ 
for(int k =0; k < array_size; k++)
{array[k] = k+1+array_size*l;}										//populates array with sequence of numbers i.e. 100, 101, 102,....199
prime_no_generator_plus(l,array_size,array);						//sets non primes_plus to zero
{int j=0; 
while(j<array_size){if (array[j] != 0)
{Num_to_PC(10,array[j]);I2C_Tx_long(array[j]);						//Display non primes_plus
counter_1++; counter_2++; Char_to_PC(' ');							//Count the numbers
Timer_T1_sub(T1_delay_100ms);}	
j++;}}
l++;} newline();													//repeat 5 times (i.e. for numbers 0 to 499)
	
l=start;
while(l < (start+5)){ 												//Repeat but obtain real prime numbers only
for(int k =0; k < array_size; k++) 
{array[k] = k+1+array_size*l;}
prime_no_generator(l,array_size,array);
{int j=0; while(j<array_size)
{if (array[j] != 0)counter_1--;									//Adjust counter_1 for number of non_prime numbers
j++;}}l++;}

Num_to_PC(10,counter_2);String_to_PC (" numbers including ");
Num_to_PC(10,counter_1); String_to_PC (" non prime numbers\r\n");
String_to_PC("Enter number (prime or non-prime?) or 0 to exit\r\n");

line_counter=0;

while(1){

	
number = Num_from_KBD(digit_array);									//Enter trial number at the keyboard

if(!(number)) {String_to_PC("Sw2 to continue.\r\n");break;}
Num_to_PC(10, number);String_to_PC("  ");							//Echo the number
{int  n = 0; int m=0;
do{
factor[n] = Product_search(number);								//Search for factors
number = number/factor[n];n++;} while (number != 1);	
if (n==1) {String_to_PC("Prime"); line_control;}					//Only one factor: Print Prime
else {m=n; for(n=0; n<m; n++){Num_to_PC(10,factor[n]);
Char_to_PC(' ');}line_control;}}} 									//Several factors: print them out
while(start == start_1);											//wait for sw2 keypress
start = start_1;}}




/*************************************************************************************/
ISR(PCINT2_vect){ 
if(switch_1_up)return; while(switch_1_down);return;}				//Press sw_1 to pause display



/*************************************************************************************/
ISR (PCINT0_vect){
if(switch_2_up) return;
while (switch_2_down){start_1+=5; Num_to_PC(10,start_1); 			//Hold sw_2 down to increment start point
Char_to_PC(' ');Timer_T1_sub(T1_delay_1sec);}						//for random number generator
newline(); 
return;}




 

