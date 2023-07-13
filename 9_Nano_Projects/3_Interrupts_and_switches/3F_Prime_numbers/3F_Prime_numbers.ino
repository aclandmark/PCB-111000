
/*Proj_3F_Prime_numbers
****************************************************************/


/*IT INTRODUCES

1.  The Sieve of Eratosthenes a means of isolating the prime numbers first used about 200 BC.

2.  Project subroutine “prime_no_generator()”.

METHOD
Generates an array of numbers (i.e. 1,2,3,4,5...) and then removes any that can be factorised.
Sounds easy, but turns out to be a bit tricky (for some of us any way)!

*/


#include "Prime_numbers_header.h"



char counter;


int main (void){
int I=0;                                        //Zero for first array of 200 numbers, one for second array etc.
int j = 0; 
int array_size = 200;
unsigned int array[200], mask;  
                                                                                                                                    
 setup_HW;
 wdt_enable(WDTO_120MS);
  while((switch_1_down) || (switch_3_down))wdr();
 _delay_ms(10);
 sei();
 
set_up_PCI;
enable_pci;


counter = 10;

while(1){

for(int j =0; j < array_size; j++)              //Populate an array with a sequence of numbers 
{array[j] = j+1+array_size*I;}                  //for example 201, 202, 203, .....400
prime_no_generator(I,array_size,array);         //Sets all non-primes in the array to zero

j = 0;
while(j<array_size){if (array[j] != 0)         //Scan the array for prime numbers
{
if (counter)                                   //Print them out 10 at a time
{if (counter == 10)
String_to_PC_Basic("\r\n");
Num_to_PC_Basic(array[j]); 
Char_to_PC_Basic(' ');
counter -= 1;}                                 //When end of array reached process the next one



/***************************Control display*********************************************************************************/
mask = 0;
{int m = 15; while (!(array [j] & (1 << m)))
{mask |= (1 << m); m -= 1;}}
array[j] = (~array[j]) & ~mask;
pause_pci_on_sw2;
//////_delay_ms(100);
Timer_T0_10mS_delay_x_m(10);

I2C_Tx_2_integers(array[j],reverse (array[j]));
resume_PCI_on_sw2;
}j++;} 



if (I <= 99)I += 1; else I-=100;}             //Process next array of 200 numbers

}




/***************************************************************************************************************************/
void prime_no_generator                        
(int I,int n,  unsigned int reg[]){            //n the array size is 200, I is the array number (zero for the first one)
                                                
int i;                                         //We compute the product i * m
int m;                                         //i always starts at 2 then increments to 3, 4, etc. 
i=2;                                           //m is set to take us to the beginning of the array  
while (i*i < n*(I+1)){                         //Maximum value i can take
wdr();
m=0;
while(i*(i+m) <= n*I)m++;                       //Go to start of the new array for example
while(i*(i+m) <= n*(1+I))                       //2*101, 2*102, 2*103,... etc   3*67,3*68, 3*69,...etc    5*41, 5*42,...etc
{reg[i*(i+m) -1 - n*I] = 0; m++;}               //set appropriate registers to zero (Note the offset of 1 place).
i++;}}




/***************************************************************************************************************************/
void Num_to_PC_Basic (long number)
{ int i = 0;
  char s[12];
   do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');
}



/***************************************************************************************************************************/
unsigned int reverse(unsigned int num)
{unsigned int num_reversed = 0;
  for(int m = 0; m <=15; m++)
  {
    if(num & (1 << m))num_reversed |= 1 << (15 - m);
  }
return num_reversed;}




/***************************************************************************************************************************/
ISR(PCINT0_vect)
{ if (switch_2_up)return;
  counter = 10;                                   //Print out 10 more
}


ISR(PCINT2_vect){while((switch_1_down) || (switch_3_down));}



/****************************************************************************************************************************/
