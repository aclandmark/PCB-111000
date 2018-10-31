
/*Proj_6F_program_on_chip_EEPROM
***************************************************************************/


/*IT INTRODUCES


1.	Programming the Atmega 168 EEPROM with text string for use by other programs (known as client programs).

2.	Project subroutine "Text_to_EEPROM()".
This saves blocks of text rather than individual characters.

3.	Project subroutine "Text_from_EEPROM()".
This reads blocks of text rather than individual characters.

4.	Project subroutine "binUnwantedChars_dot()".
When a file is being read by project code and its length exceeds a defined value, this subroutine can be used 
to discard the remaining characters echoing each with a '.' so that the file download can complete properly.
Note: there is also a similar subroutine "binUnwantedChars()": 
This does not echo discarded characters.

5.	Project subroutine "Read_on_chip_EEPROM()".
This is used by the client program to read the text strings.




PROGRAM OPERATION


Proj_6F allows users to put text strings in the EEPROM rather than in program memory.  Text string are composed 
in a .txt file.  This is downloaded and the strings copied to EEPROM.

The strings are echoed to the PC screen along with their addresses.  These must be copied manually into the header 
file along with the names of the messages for use by the client programs.  See "Proj_6F_header_file.h" for examples.

For test purposes two .txt files are used with this program.
Segments of the client programs are included in this program for illustrative purposes.

Note: 	The first 10 locations are reserved for use by the client programs for non text based use.
This number can be increased or reduced even to zero if needed using the variable “write_address”.
		
Locations 0x1F7 and 0x1F8 are used for 8MHz clock oscillator calibration bytes.  If these get corrupted they can 
be restored using "Proj_9B_168_auto_cal".*/




#include "Proj_6F_header_file.h"



int main (void){

char text, User_response,message_file;
int read_address=10, write_address = 10;								//Leave 10 bytes of EEPROM for application use
char  next_char=0;														

setup_HW;
String_to_PC("Message_file?  1 or 2");									//select the required .txt program
message_file = waitforkeypress();										//Note attempts to download one and read the other
if((message_file != '1') && (message_file != '2'))						//will generate meaningless results
{newline();SW_reset;}



String_to_PC("\r\nPress -r- to read the EEPROM\r\n\
or -R- to reprogram it\r\n");
User_prompt;



/***********client programs_1 and 2 will read the strings stored in the EEPROM***************
*****************These program segments are included here for test purposes only*************/

if ((User_response == 'r') && (message_file == '1'))
{client_program_1; SW_reset;}

if ((User_response == 'r') && (message_file == '2'))
{client_program_2; SW_reset;}

/**************************************************************************************/



/********************Proj_6F requests the relevant .txt file****************************/

String_to_PC("Set baud rate to 2.4k then AK\r\n");	
USART_init(1,160);
waitforkeypress();

switch(message_file){
case '1': String_to_PC("Send message file_1\r\n");break;
case '2': String_to_PC("Send message file_2\r\n");break;
default: SW_reset;break;}




/***************Strings in the .txt file are saved to EEPROM**************************/

while(1){next_char = waitforkeypress();								//wait for first character from file
if ((next_char != '\r') && (next_char != '\n'))						//Ignore leading carriage returns
break;}							
Text_to_EEPROM(&write_address, next_char);								//save first letter to EEPROM

while(write_address < 0x1F6) 											//Exits before cal bytes can be overwritten
	{if(isCharavailable(1)) 											//returns 1 if a new letter is available (0 at the end of the file) 
	{text = receiveChar(); 												//Temporary storage

	switch (text){														//Test the new character	
		case '\r':														//If it is '\r' and or '\n' 
		case '\n':														//ignore it or replace it with with a single '\0'
		if(next_char == '\0')break; 
		else 	{next_char = '\0'; 
				Text_to_EEPROM(&write_address, next_char);}break;
		default : 	next_char = text; 									//save the letter
					Text_to_EEPROM(&write_address, next_char);			//increments the write address
					break;}

	}else break; }														//End of file reached

if(write_address == 0x1F6)												//If text file was too long
{Text_to_EEPROM(&write_address, '\0');									//Place '\0' in 0x1F6 to terminate the string
binUnwantedChars_dot();}												//Send dots to pc to indicate lost characters




/****************Echo text file to screen with the address of each string**********************/

Num_to_PC(16,read_address); Char_to_PC('\t');							//Send address of first line of text
do{																		//Read back text one line at a time
while(1){text = Text_from_EEPROM(&read_address);						//Increments the read address
if(text)Char_to_PC(text); else break;} 								//When '\0' is detected start next line
newline();Num_to_PC(16,read_address); 									//Send address of next line
Char_to_PC('\t');}
while(read_address < write_address);									//Exit when read address equals write address


String_to_PC("\r\nBAUD RATE 57.6k!!\r\npress any key to continue\r\n");

USART_init(0,16);

waitforkeypress();
SW_reset;}



/************Local version of "Read_on_chip_EEPROM()"*************************************************************************************************/

void Read_on_chip_EEPROM_local(int EEPROM_address){
char temp_char;
while(1){
temp_char =  eeprom_read_byte((uint8_t*)(EEPROM_address++));   		// Read_write_mem('O',(EEPROM_address--),0);
if (temp_char != '\0') Char_to_PC(temp_char);
else {newline(); break;}}}