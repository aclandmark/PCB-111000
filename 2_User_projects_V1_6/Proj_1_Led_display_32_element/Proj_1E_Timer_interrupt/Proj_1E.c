
/*Proj_1E_Timer_interrupt
***********************************************************/


/*A PROGRAM THAT DOES MULTI-TASKING

The program continues to drive the display but the vast majority of time which was spent 
waiting to increment the display is used to do something useful.  In this case doing some 
arithmetic and sending the results to the PC to be displayed.



IT INTRODUCES


1.	Project subroutine "T1_65ms_clock_tick()":
This generates an interrupt every 65 mS.

2.	ISR(TIMER1_OVF_vect){}:
This is the Timer1 interrupt service routine. Here, it is the ISR that increments the LED display 
rather than the main routine.

3. 	Code other than a timer subroutine than can safely be interrupted.  Here the code is executed
indefinitely by a "while(1)" loop.

4.	Project subroutines "Char_to_PC()" and "Num_to_PC_U()"
These send data to the PC and will be considered in more detail later on.

5. 	Timer T2:
This slows down data flow to the PC.
Note: Timer_T2_sub(T2_delay_2ms) has also been introduced to highlight an important point.
Use of multiple timers can be problematic.   Problems are usually resolved by editing the 
"makefile": change line 61 from "OPT = s" to "OPT = 1".

6.	long numbers:
These occupy 32 bits of data memory and can hold numbers between -2^31 and 2^31 – 1 or 
between 0 and 2^32 - 1 = 4,294,967, 295.

Note: The logic is necessary for the display and may be found interesting but is not really 
the important thing here.*/




//THE PROGRAM STARTS HERE




#include "Proj_1E_header_file.h"




int m;
unsigned int PORT_1, PORT_2;	

int main (void){	
unsigned long counter_squared, counter=1;		//32 bits are reserved for each of these variables
	
m=0;							
setup_HW; 
PORT_1=0xFFFF;									//Equivalent to 0b1111111111111111
PORT_2 = 0;					
I2C_Tx_2_integers(PORT_1, PORT_2);
sei();							
T1_65ms_clock_tick();							//This subroutine starts HW clock Timer 1 that generates an interrupt every 65mS


while(1){										//Interrupts enable several process to take place  simultaneously
counter_squared = counter*counter;				//In this case squares are calculated some of which are printed out
if((!(counter%33))&& (switch_2_down)){			//"counter%33" is only zero when counter is 33, 66, 99 etc.
Num_to_PC_U(10, counter); Char_to_PC('\t');	//Only print out results if Switch_2 has been pressed
Num_to_PC_U(10, counter*counter); newline();	//Code in this while-loop could be interrupted at any point
 }counter = (counter + 1)%0x10000;				//limits the value of counter to avoid overflow and garbage out.
Timer_T2_sub(T2_delay_2ms);}
}


/**********************ISR called evry 65mS to increment the display******************************************/



ISR(TIMER1_OVF_vect) {

if(m <=15){PORT_2 = (PORT_2 << 1) | 1;			//PORT_2 is shifted one place to the left 
		PORT_1 = PORT_1 << 1;}					//An additional 1 is placed in the most RH bit using "|1". 
		
		else
		{PORT_2 = (PORT_2 >> 1);				//PORT_2 now shifts right 
		PORT_1 = ~(~PORT_1 >> 1);}				//see below	for quick look at the logic		
		
		I2C_Tx_2_integers(PORT_1, PORT_2);m++; //increment "m";
if (m==32)m=0;}								//Reset "m" when it gets to 32




/*
A QUICK LOOK AT SOME OF THE LOGIC

Consider the statement "PORT_1 = ~(~PORT_1 >> 1);"
If PORT_1 = 0 
then "(~PORT_1 >> 1)" = 0b0111111111111111
and "~(~PORT_1 >> 1)" = 0b1000000000000000
The next time the loop runs 
"(~PORT_1 >> 1)" = 0b0011111111111111 and
"~(~PORT_1 >> 1)" = 0b1100000000000000
*/



