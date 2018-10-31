
/*Proj_1G_LED_dimmer
**************************************************/


/*A TEST ROUTINE FOR THE DISPLAY BRIGHTNESS CONTROL.


A repeat of Proj_1A with an Interrupt on key press that toggles the brightness control.



IT INTRODUCES


1.	Project subroutine "I2C_Tx()".
This transmits data to the Atmega 328 over the I2C bus.  

2.	Pointers: 	 
Look at the ISR: It makes the following subroutine call I2C_Tx(1, 'W', &Dimmer_control); 

The "&" before the variable "Dimmer_control" means that the subroutine "I2C_Tx" expects the 
calling routine to provide the data memory location (i.e. its address) and uses and possibly 
modifies whatever data it finds in that memory location.

In this case Proj_1G defines a memory location and calls it "Dimmer_control". The subroutine 
call hands the address of "Dimmer_control" to the subroutine rather than the actual value.  


3. Subroutine "void I2C_Tx(char num_bytes, char mode, char* s){}".

This is the subroutine being called by "I2C_Tx(1, 'W', &Dimmer_control);"
It provides memory locations "num_bytes" and "mode" for the 1 and W.
The "*" signifies that it does not provide memory for variable "s" but expects the 
calling routine to provide it.

Note: "W" is the mini-OS mode that toggles the display brightness. 
The pointer is used because "I2C_Tx()" is also used to send arrays of data (to be considered later).
*/




//THE PROGRAM STARTS HERE




#include "Proj_1G_header_file.h"




char Dimmer_control;

int main (void){
unsigned int PORT_1;
setup_HW;

UCSR0B |= (1 << RXCIE0); 
sei();
Dimmer_control = 1;

while(1){
PORT_1=1;
for(int m = 1; m < 17; m++){
cli();I2C_Tx_2_integers(PORT_1, PORT_1);sei();
Timer_T0_10mS_delay_x_m(6);
PORT_1 = PORT_1 << 1;
}}}



ISR(USART_RX_vect){
receiveChar();
I2C_Tx(1, 'W', &Dimmer_control);}	




/*

//Local version of subroutine "I2C_Tx()"

void I2C_Tx_local(char num_bytes, char mode, char* s){
waiting_for_I2C_master;										//Turn on I2C slave and await call from master
send_byte_with_Ack(num_bytes);								//send data byte, request acknowledgement
send_byte_with_Ack(mode);
for (int m = 0;  m < num_bytes; m++){
if (m==num_bytes-1){send_byte_with_Nack(s[m]);}				//Last byte, no ackowledgement needed
else {send_byte_with_Ack(s[m]);}}
TWCR = (1 << TWINT);}										//Clear interrupt and close I2C slave

*/