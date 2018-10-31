
/*Proj_6A_Prime_numbers
****************************************************************/


/*IT INTRODUCES

1.	The Sieve of Eratosthenes a means of isolating the prime numbers first used about 200 BC.

2.	Project subroutine “prime_no_generator()”.

METHOD
Generates an array of numbers (i.e. 1,2,3,4,5...) and then removes any that can be factorised.


USER INSTRUCTIONS

Press and hold sw3 to scroll through prime numbers from 0 to 200.
Pulse sw2 to increase start point by 200.

Switch location SW3(PD2) - SW2(PB6) – SW1(PD7).*/




#include "Proj_6A_header_file.h"


volatile char mode;
volatile int l;
volatile int array_size = 200;
int array[200];
char digit_array[8];

int main (void){

int j = 0;                                                                                                                                          
setup_HW;
User_instructions;
setup_and_enable_PCI;
l=0; mode = 1;  
sei();

while(1){											//Infinite loop: instantaneously  responds to mode change
switch(mode){										//Mode is controlled using switch 3.
case 0: break;										//Mode 0: Pause mode (do nothing)

case 1: for(int j =0; j < array_size; j++) 		//mode 1:  Populate an array with a sequence of numbers 
{array[j] = j+1+array_size*l;} 					//for example 200, 201, 202, .....400
prime_no_generator(l,array_size,array); 			//Sets all non-primes in the array to zero
j=0; while(array[j] == 0)j++;						//Start to scan the array skipping the first zero entries
I2C_Tx_long(array[j]); mode = 0; break;			//Display the first prime number and pause (set mode to 0)

case 2: if(j<array_size){if (array[j] != 0)		//Mode 2: Continue scanning the array displaying the prime numbers
{I2C_Tx_long(array[j]);Num_to_PC(10,array[j]);
Char_to_PC(' ');
Timer_T0_10mS_delay_x_m(16);}j++; } break;}}}




/*************************************************************************************/
ISR (PCINT2_vect){									//Not used for mode 1 (the start-up mode)
if(switch_1_down)return;							//Switch 1 never used
if((mode == 0) && (switch_3_up))return;
if((mode == 0) && (switch_3_down))
{mode = 2; return;}									//Press SW3 to resume scanning (mode = 2)
if((mode==2) && (switch_3_up))	
{mode = 0; return;}}								//Release SW3 to pause scanning (mode = 0)




/*************************************************************************************/
ISR (PCINT0_vect){
int j;
if(switch_2_up) return;								//Ignore sw2 release
l++;												//move sequence of numbers on by 200

for(int j =0; j < array_size; j++) 				//Duplicates mode 1;
{array[j] = j+1+array_size*l;} 					//Not efficient on code space or time but gives
prime_no_generator(l,array_size,array);				//the required output  
j=0; while(array[j] == 0)j++;
I2C_Tx_long(array[j]);
mode = 1;}

