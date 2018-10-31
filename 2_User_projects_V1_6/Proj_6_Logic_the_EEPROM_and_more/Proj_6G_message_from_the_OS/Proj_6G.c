
/*Proj_6G_message_from_the_OS
*********************************************************************/



/*IT INTRODUCES


1.	Project subroutine "Read_Hello_world_string()".

2.	A number of subroutines and macros used by the I2C subroutines.

3.	Programming the mini-OS EEPROM.


User strings can be placed in the ATMEGA 328 EEPROM addresses 0 to 0x199 (the lower half).
This subroutine prints them out in a random order.
It can be included with any of the programs. Sample strings have already been programmed and they can easily be changed.

Note "include statements" may have to be added to the project .h file. 
For example 	#include "../../Resources_V12/Subroutines/Data_to_from_EEPROMs.c" 


Entering the user define messages

1.	Compose the messages in a .txt file (see "6_Text_files\Hello_world (or Hello_world_1)” for 	examples).

2.	Restore the Atmega 168 EEPROM using "Proj_9H_restore_on_chip_EEPROM".  Press P if the text messages are 
	printed out correctly. 	See the file "6_Text_files\ATmega programmer_EEPROM strings.txt" for a copy of 
	the text messages.

3.	Program the Atmega 168 flash with "AT_Programmer_V2.29A_zero_SUT".

4.	Run the program and press PPEW0 at the user prompts.

5.	Send the text file when requested.*/




#include "Proj_6G_header_file.h"



int main (void){

setup_HW;


while(1){

Read_Hello_world_string();newline();
waitforkeypress();}}




/**************************************************************************************************************************/
void Read_Hello_world_string_Local(void){
char num_strings, receive_byte, num_bytes=0;
char string_number;

waiting_for_I2C_master;										//Initiate communications: master requests data
send_byte_with_Ack(num_bytes);								//Slave sends payload size
send_byte_with_Nack('Y');									//Slave sends mode
clear_I2C_interrupt;										//Complete transaction

waiting_for_I2C_master;										//Initiate communications: master to send data
num_strings = receive_byte_with_Nack();						//sends number of strings storred in OS EEPROM
clear_I2C_interrupt;										//Complete transaction

string_number =  (PRN_8bit_GEN()%(num_strings)) + 1;		//Generate random string number

waiting_for_I2C_master;										//Initiate communications: master requests data
send_byte_with_Nack(string_number);							//Slave sends string number
clear_I2C_interrupt;										//Complete transaction

waiting_for_I2C_master;										//Initiate communications:master to send string

do{receive_byte = receive_byte_with_Ack();					//print out string as characters are received
if(receive_byte)Char_to_PC(receive_byte);}
while(receive_byte);										//Detect '\0' character used to terminate a string
clear_I2C_interrupt;}										//Complete transaction


