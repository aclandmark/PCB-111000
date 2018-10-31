
/*Proj_5C_Basic_crystal_clock	A crystal controlled clock provided by the mini-OS.
******************************************************************************************/


/*IT INTRODUCES


Mini-OS clock command "I2C_Tx_OS_timer(AT_clock_mode, start_time)".
An AT clock mode"  of 7 starts the clok running.
The various modes (i.e. pause, hode etc.) are saved in the "clock_stop_watch .h" file.


USER INSTRUCTIONS

Having programmed the Atmega 168 run the program and enter the time when requested which is then saved to EEPROM.
It is now possible to transfer the project pcb to an independent 5V supply and the clock will automatically start 
at the time entered.

Note:	If it is power cycled for a second time the display will remain blank until the time is re-entered from 
the PC keyboard.

Note: The short delay following POR is to allow the mini-OS to check OSCCAL (the oscillator calibration). */


#include "Proj_5C_header_file.h"


int main (void){
char start_time[8], input_mode;

setup_HW;												

input_mode = eeprom_read_byte((uint8_t*)0x02);					//Read mode. Default value is 255

switch (input_mode){
case 255:														//If EEPROM contains 255 initiate the clock
String_to_PC("\r\nSend time: hours mins secs\
(24Hr clock, include spaces)");

for (int m = 0;  m < 8; m++)
{start_time[m] = waitforkeypress();							//Obtain time from KBD						
eeprom_write_byte((uint8_t*)(m+3),start_time[m]);	}			//and save each keypress directly to EEPROM
eeprom_write_byte((uint8_t*)(0x02),0);							//Update mode and save in EEPROM location 2
String_to_PC("\r\nPower cycle!:\
Can now use 5V USB charger if required");
break;															//Exit and send Start clock command

case 0:															//If EEPROM location 2 contains 0 start clock immediately
for (int m = 0;  m < 8; m++)
{start_time[m] = eeprom_read_byte((uint8_t*)(m+3));}			//Read start time from EEPROM
eeprom_write_byte((uint8_t*)(0x02),255);						//Restore the EEPROM location 2 to its default value
break;

default: eeprom_write_byte((uint8_t*)(0x02),255);				//If EEPROM ever gets corrupted reset it to 255  (0b11111111)
wdt_enable(WDTO_15MS); while(1);break;}						//Exit and start clock					
	
I2C_Tx_OS_timer(AT_clock_mode, start_time);						//Send Start clock command (AT clock mode is 7)

while(1);}														//Halt ATMEGA 168 operation until it is power cycled.





