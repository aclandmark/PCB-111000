
/*Proj_9D_restore_on_chip_EEPROM
*****************************************************************************/


/*The term on-chip refers to the Atmega 168 the chip which runs user projects.  Its EEPROM is available 
for use by user projects. However if it is to be used to re-program the mini-OS or Atmega 328 EEPROM, 
then its EEPROM should be restored to its default condition so that correct user prompts are generated.


IT INTRODUCES


Project subroutine "Read_AT328_EEPROM()" 
This reads the ATMEGA 328 EEPROM over the I2C bus and either copies the results to the PC screen or 
to the Atmega 168 EEPROM.  Calibration bytes are also sent to the PC as a check that they have not 
got corrupted in the process.


Note:
This program should be ran before the ATMEGA 168 is used to program the ATMEGA 328 EEPROM or flash. 
It ensures that the required strings are available in the 168 EEPROM. 

These strings are backed up on the AT328 EEPROM starting at address 200.
The program reads and displays them and if they are OK they can then be copied across to the ATMEGA 168.

Users may want to reprogram the 328 EEPROM when running "Proj_6G_message_from_the_OS".

The flash will only be re-programmed if the mini-OS gets corrupted or needs updating.

The strings are also saved in "6_Text_files\ATmega programmer_EEPROM strings"
*/


#include "Proj_9D_header_file.h"


void Read_AT328_EEPROM(char);

int main (void){
char text, OSCAL;
int read_address=0;
setup_HW_E;
String_to_PC("\r\nStrings stored on the ATMEGA328 starting at address 0x200 that are \
required by the ATMEGA 168 if it is to run in programmer mode - AK to continue\r\n\r\n");
waitforkeypress();
Read_AT328_EEPROM(read_only);								//Read AT328 EEPROM address 200 and up (Inspection mode)
String_to_PC("\r\nPress P to program the AT168 - AOK to abort\r\n");

if (waitforkeypress() != 'P') {SW_reset;}					//If strings are not OK abort.
Read_AT328_EEPROM(copy);									//Else re-read & program strings into the AT168 EEPROM
newline();

String_to_PC("\r\nEcho ATMEGA 168 EEPROM to screen\r\n\r\n");



/***********Read AT168 EEPROM and print results******************/
Num_to_PC(16,read_address); Char_to_PC('\t');				//Send address of the first string
while(read_address <= 0x1F6){								//Stop at address 0x1F6
text = Text_from_EEPROM(&read_address);						//Read first character and increment read address
if((text == 0) && (read_address <= 0x1F7)) 				//Detect new line
{newline(); Num_to_PC(16,read_address);					//and send its address to the PC 
Char_to_PC('\t');}
else Char_to_PC(text);}										// Sent all data to the PC

String_to_PC ("\r\nATMEGA168 Calibration bytes   ");			//Read and print AT168 calibration bytes

read_address = 0x1F7;
while(read_address <= 0x1F8){ 
OSCAL = Text_from_EEPROM(&read_address);
Num_to_PC (16,OSCAL);
Char_to_PC('\t');}
newline();}




/************************************************************************************************************************/

void Read_AT328_EEPROM(char Op){
char receive_byte, num_bytes=0;
int write_address = 0;
															//Initiate slave I2C
waiting_for_I2C_master;										//master detects slave and requests data
send_byte_with_Ack(num_bytes);								//Slave sends payload size (zero)
send_byte_with_Nack('Z');									//Slave sends mode (Z)
clear_I2C_interrupt;										//Clear I2C interrupt to terminate transaction

waiting_for_I2C_master;										//Initiate new transaction: master to send data

while(write_address <= 0x1F6){								//Download strings (stop after 0x1F6 characters)
receive_byte = receive_byte_with_Ack();						//Receive bytes
if(Op == copy)												//If mode is copy
Text_to_EEPROM(&write_address, receive_byte);				//Save them to the ATMEGA 168 EEPROM
else														//Otherwise (Inspection mode)
{if(receive_byte=='\0') String_to_PC("\r\n");				//Echo them to the PC screen
else Char_to_PC(receive_byte);write_address++;}}

if(Op==copy){newline();
String_to_PC("ATMEGA328 calibration bytes   ");				//Echo calibration bytes
Num_to_PC(16,receive_byte_with_Ack()); 
Char_to_PC('\t');
Num_to_PC(16,receive_byte_with_Nack());}
else{
receive_byte_with_Ack();
receive_byte_with_Nack();}
	
clear_I2C_interrupt;}										//Clear I2C interupt to terminate transaction



