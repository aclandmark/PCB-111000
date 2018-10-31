
/*Proj_3A_transmit_chars
***********************************************************/

/*SENDS CHARACTERS TO THE PC SCREEN


IT INTRODUCES:


1.	The askii code in which numbers are used to represent symbols. 
By incrementing the number from 32 to 126 most of the typewriter symbols from space to ~ are covered.

2.	Project subroutine "Char_to_PC()".
This is used to send a number of <=255 to the PC which can then be displayed as characters.

3.	Subroutine "Num_to_PC_local()".
This converts a 3 digit number to symbols and sends them to the PC screen.

Note:
The PC treats all data sent by "Char_to_PC()" as symbols.
If it receives the number 65 then the symbol A is displayed.
If we want to display the number 65 we must fist convert it to two symbols '6' and '5' 
and then send them separately as 54 and 53.*/




#include "Proj_3A_header_file.h"



int main (void){
char symbol, User_response;

setup_HW;
User_prompt;										//Respond by pressing either 'R' or 'r'

switch(User_response){
case 'r':

newline();
		symbol = '!';								//number 33
		while(symbol <= '~'){						//number 126
		Char_to_PC_local(symbol);					//display symbol on pc screen			
		Timer_T0_10mS_delay_x_m(15);
		symbol++;									//go on to next symbol	
		}break;	
		
case 'R':	
symbol = '!';
		while(symbol <= '~'){
		Num_to_PC_local(symbol);					//display number on the PC screen together			
		Char_to_PC_local(symbol);					//with the corresponding symbol		
		Timer_T0_10mS_delay_x_m(15);
		symbol++;	
		if(!((symbol-'!')%8))newline(); 			//format the output
		else Char_to_PC_local('\t');		
		}		
		break;}										//End of switch block
		
		SW_reset;}     



/**************************************************************************************************/
void Num_to_PC_local(char x)						//convert a number to three chars
{char y; char z;									//Consider x = 115 (the symbol s)
y = x/100;											//y = x/100 = 1					
z = x%100;											//z = x%100 = 15
Char_to_PC_local(y + '0');							//sends char	'1'
Char_to_PC_local(z/10 + '0');						//sends char	15/10 +'0' = '1'
Char_to_PC_local(x%10 + '0'); 						//sends Char	15%10 +'0' = '5'
Char_to_PC_local(' ');}




/**************************************************************************************************/
void Char_to_PC_local(char data)					//Local function only:	normally use "Char_to_PC()"
{while (!(UCSR0A & (1 << UDRE0)));					//Wait here until UDRE0 is set to 1				//
UDR0 = data;}										//Transfer data to UDR0 which will automatically be sent to the PC.

													/*This subroutine talks directly to the USART HW.
													UCSR0A is the USART control and status register A
													UDRE0 is the "USART Data Register Empty" bit.  
													It is set to 1 when the USART is ready to accept more data. 
													UDR0 is the I/O Data Register */