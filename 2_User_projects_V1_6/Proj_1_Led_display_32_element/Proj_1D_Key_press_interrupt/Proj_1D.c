
/*Proj_1D_Key_press_interrupt
****************************************************************/


/*ANOTHER VARIATION ON Proj_1A
This time not all sets of vertical LEDs are illuminated in any given sweep.


DISCOVERING A BIT MORE ABOUT THE ATMEGA HW


1.	The Serial Port (UART):
This is also known as the Universal Asynchronous Receiver Transmitter or just 
Receiver Transmitter (Rx/Tx). It enables communication with the PC.  
Because PCs now requires a USB connection a USB bridge is included on the PCB.  
The provides an interface between the Atmega serial port and PC USB port.

2.	HW register UCSR0B:
This is known as the UART Control and Status Register B.
Bit 7 of this register: RXCIE0 (Rx complete interrupt enable).
User code must set this bit to 1 if an interrupt is required when a character is received.
Note also the registers used in "waitforkeypress()" and "receiveChar()".

3.	"ISR(USART_RX_vect){}"
This is the ISR that responds to iterrupts generated Using keypresses made at the PC keyboard.



INTRODUCING SOME NEW C TOPICS 

1.	The "switch block":
This includes “switch” statements and the "break" instruction that causes program 
execution to exit the "switch block".
	
2.	Instructions cli(); used in combination with sei(); to temporarily disable interrupts.

3.	The statement “row = row%2;”:	
This gives the remainder of dividing the variable "row" by 2, it only takes on the values 0 and 1. 
(A%2 is spoken as A modulo 2).

4.	The Statement  "while(A);":
This halts program execution while “A” is non-zero.
Similarly statement "while(!(A));"	halts program execution while “A” is zero.
"A" can be changed by an interrupt or by the hardware when for example the UART receives a character.


PROGRAM OPERATION	


The main process that of controlling the display is interrupted for a brief instant by a keypress.
This calls an ISR that affects the main process, changing the number of leds that are skipped 
every time that the display increments.  


WHAT REALLY MATTERS HERE	
	

The Rx interrupt (the keypress interrupt) and setting it up, subroutine ISR(USART_RX_vect){},  
and the "switch" statement block.


POSSIBLY for LATTER REVIEW	


The division and logic might possibly be worth revisiting latter on but really they have just 
been included in an attempt to do something different.*/





//THE PROGRAM STARTS HERE




#include "Proj_1D_header_file.h"



int p;												//p is defined for both main and ISR routines. It is the
													//number of leds that will be skipped every time that
													//the display increments.
int main (void){
long PORT_1=1;									
char row=0;											//set row to 0 for the top row of leds or 1 for the bottom 
setup_HW;
UCSR0B |= (1 << RXCIE0); 							//Enables the serial port Rx interrupt. Sets "RCIE0" with needing to know which bit it is

p=1;												//Set "p" to 1 so the the display increments without 	
sei();												//skipping any leds											
	while(1){
	if (p <= 8){									//If "p" is less than 9
	if (!(row))I2C_Tx_2_integers(PORT_1, 0);		//Illuminate the Upper half of display first and then the lower 
	else I2C_Tx_2_integers(0, 0x8000/PORT_1);}		//and  ensure that the lower half is a mirror image of the upper
	
	else 											//If "p" is greater than 8 illuminate both upper and lower
	I2C_Tx_2_integers(PORT_1, 0x8000/PORT_1);		//halves of the display together
	
	Timer_T0_10mS_delay_x_m(12);
	
	if (p==7)										//If p == 7 disable the interrupts and 
		{cli();if (waitforkeypress()== 'x')		//make keypresses available to increment display manually
		{sei(); p = 8;}}							//For a keypress of "x" re-enable interrupts and increment "p"
	
	PORT_1 = PORT_1 << p;							//Increment the display by "p"
	
	if(PORT_1 >= 0x10000){row += 1; row = row%2;	//0x10000 corresponds to leds that do not exist and therefore 
	PORT_1 = PORT_1 >> 16;}}}						//PORT_1 is shifted 16 places to the right to to a led that does exist
													//and the row increments



/*****************Routine executed by a keypress at the PC keyboard if global interrupts are set*******************/



ISR(USART_RX_vect){
switch(receiveChar()){	
case '1': if (p==1)p = 15; else p=1; break;		//If user presses key "1": set p to 1 or 15 and exit break block
case '2': if (p==2)p = 14; else p=2; break;		//If user presses key "2": set p to 2 or 14 and exit break block		
case '3': if (p==3)p = 13; else p=3; break;		//Continue for keypresses 3 to 7
case '4': if (p==4)p = 12; else p=4; break;		//Other keypresses are ignored
case '5': if (p==5)p = 11; else p=5; break;
case '6': if (p==6)p = 10; else p=6; break;
case '7': p=7; break;}}





/************Local versions of waitforkeypress() and receiveChar()

char waitforkeypress_local(void){
while (!(UCSR0A & (1 << RXC0)));	//Bit 7 (RXC0 Receive complete) of UART Control and Status Register A (UCSR0A)
return UDR0;}						//is set when a character is received by the UART
									//Repeat the while-loop endlessly until the bit is set
									//then return the contents of UDR0 the I/O register


char receiveChar_local(void)		//We know that a character is ready in UDR0 because of the interrupt
{return UDR0;}						//and immediately return it to the calling routine (i.e. the ISR)

********************************************************/
