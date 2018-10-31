
/*Proj_1F_PCI_and_T1_interrupt
****************************************************************/


/*MULTI PATTERN DISPLAY


Uses switch presses to select any one of six displays. A more ambitious program in which the inevitable 
complexity is handled by two subroutines (see below) and two interrupt service routines one triggered 
by a timer and the other by a switch press. One of the subroutines is used to initiate the display and 
the other to increment it.


IT INTRODUCES


1.	The Watch Dog Timer:
An Atmega HW device that can be programmed to generate a reset if program execution unexpectedly halts 
for any reason. It will be studied in more detail later on. 

2.	Interupts from multiple sources:
Combining a timer interrupt to increment the display with a PCI interrupt to select the display 
and initialise it.

3.	Lots of logic once again the study of most of it is probably best postponed. 

4.	Subroutines:		 
A subroutine executes a segment of code on behalf of the main routine. Subroutines may be used to make a 
program more readable especially if the code segment is required several times. Alternatively the subroutine 
may provide a service like some arithmetic that will be required by many programs.

Consider the project subroutine "void Inc_Display(char mode)".
 
It defines a variable and gives it the name "mode".  The initial value of "mode" is supplied by the statement 
"Inc_Display(switch_press)" in the "main" routine in which "switch_press" is defined as a char variable. 
The subroutine can change the value of "mode" but these changes will not effect the variable "switch_press" 
in the main routine.  The term "void" indicates that the subroutine will not be returning any value to the 
"main" routine as might be the case for an arithmetic function for example.

5. Volatile variables: 		Every variable used by the main routine is allocated permanent storage space in 
data memory. Variables only used by the subroutines or ISRs are allocated shared memory space in data memory.

However there may be some variables that are not used by the main routine but that must not be allowed to 
loose their value or be overwritten by other variables.  They also require permanent storage and are defined 
as "volatile".

Note: things do not always appear as simple as this and it is often worth checking whether or not variables 
should be declared as volatile if problems occur.*/




//THE PROGRAM STARTS HERE




#include "Proj_1F_header_file.h"



volatile char T1_OVF;							//T1_OVF is set to 1 by TIMER 1 ISR or alternatively by a keypress ISR
unsigned int PORT_1,PORT_2,n, n_max;			//"n" records the increment number and "n_max" records the value at which it is reset to 0		
char switch_press, p;							//"switch_press" is used to remember which switches have most recently been pressed
												//here it can vary between 1 and 5 resulting in 5 different displays
int main (void){
setup_HW;

setup_and_enable_PCI;							//All three user switched generate PCI
UCSR0B |= (1 << RXCIE0);						//Set up key press interrupt to trace logic more easily
T1_OVF = 0;
sei();
T1_65ms_clock_tick();							//Timer_1 interrupt generated every 65mS
initialise_display();
wdt_enable(WDTO_120MS);							//Set up watch dog timer: This will be studied shortly

while(1)										//Start a continuous loop
{Inc_Display(switch_press);						//Increment the display depending upon which user switches have been pressed
while(!(T1_OVF));T1_OVF = 0;					//Wait for the timer or keypress interrupt and then reset it.
wdr();}}										//Watchdog reset (prevents it from timing out).
												//a watchdog timeout re-establishes "key_press" as 0 which cannot be selected using the switches


/************Interrupt Service routines****************************/


//ISR(USART_RX_vect){receiveChar();T1_OVF = 1;} //Replace timer ISR with keypress ISR if required to check operation of logic

	
ISR(TIMER1_OVF_vect) {T1_OVF = 1;}				//Timer interrupt ISR


ISR(PCINT0_vect) {								//ISR for PCI on sw2
if(switch_2_up)return;							//No interrupts on switch release
if(switch_2_down) switch_press = 5;				//Depending on the switch presses initialise the display 
initialise_display();}							//and then increment it appropriately


ISR(PCINT2_vect) {								//ISR for PCI on sw_1 and sw_3
if(switch_1_down) {if(switch_press == 5)switch_press = 2; else switch_press = 1; initialise_display();}
if(switch_3_down) {if(switch_press == 5)switch_press = 4; else switch_press = 3; initialise_display();}}



/***********initialise_display() subroutine***********************/


void initialise_display(void){
n=0;
switch(switch_press){

case 0: PORT_1 = 1<<p; PORT_2 = 0x8000>>p; n_max = 16-(2*p); break;
case 1: PORT_1 = 1; PORT_2 = 1; n_max = 16; break;
case 2: n=1;
case 3: PORT_1=0b0000000000000001;
		PORT_2=0b1000000000000000; 
		n_max = 16;break;
case 4: 
		PORT_1 = (0xFFFF >> 1) ^ (0xFFFF << 1); PORT_2 = ~PORT_1;
		n_max = 24;n=1;break;
case 5: PORT_1=0b0000000000000001;
		PORT_2 = 0;
		n_max = 32;
		break;}
I2C_Tx_2_integers(PORT_1, PORT_2);}



/***********Inc_Display() subroutine*****************************/



void Inc_Display(char mode) {

switch (mode){
case 0: PORT_1= PORT_1 << 1;	PORT_2 = PORT_2 >> 1;	break;
case 1: PORT_1 = PORT_1 << 1; PORT_2 = PORT_1; break;

case 2: 
if(n<8){PORT_1 |= 1 << (2*n); PORT_2 |= 0x8000 >> (2*n);}
else {PORT_1 = PORT_1 << 2; PORT_2 = PORT_2 >> 2;}break;

case 3: if(n < 5){PORT_1 |= PORT_1 << 1; PORT_2 |= PORT_2 >> 1;}
		else{PORT_1= PORT_1 << 1;PORT_2 = PORT_2 >> 1;}break;
	
case 4: 
		if ((n <= 7)){PORT_1 = ((0xFFFF >> (n+1)) ^ (0xFFFF << (n+1)));PORT_2 = ~PORT_1;}
		if(n==8){PORT_1= PORT_1<<1; PORT_2 = 1;}
		if(n > 8){PORT_1 = PORT_1<< 1;PORT_2 |= (PORT_2 << 1);}break;

case 5: if (n<16){PORT_1= PORT_1 << 1;}
		if (n>=15) {PORT_2 = ((~(~PORT_2 >> 1)) & (0xFFFF >> (n-15)));}break;}

I2C_Tx_2_integers(PORT_1, PORT_2); n++; 
if (n == n_max) {n=0; p += 1; p = p%8; initialise_display();}}


