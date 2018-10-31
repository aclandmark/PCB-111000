
/*Proj_5D_Crystal_clock_with adjustment
*********************************************************/


/*As for Proj_5C the mini-OS provides a crystal controlled clock with moderately good accuracy
which can be operated independently of a PC.

IT INTRODUCES

The following mini-OS clock commands

	I2C_Tx_Clock_command(hide_clock);
	I2C_Tx_Clock_command(pause_clock);
	I2C_Tx_Clock_command(display_clock);
	I2C_Tx_Clock_command(increment_seconds);
	I2C_Tx_Clock_command(increment_minutes);
	I2C_Tx_Clock_command(decrement_seconds);
	I2C_Tx_Clock_command(decrement_minutes);


USER INSTRUCTIONS

During operation the three switches are continuously polled.

To control the display:
	Press sw1 to toggle the display ON and OFF
	Press sw3 to pause or resume the clock

To adjust the clock:
	Pulse sw2 then press sw1 and sw3 to advance the time
	or press and hold sw2 (for 500ms)
	then press sw1 and sw3 to retard the time
	Always pulse sw2 when the time is correct


Switch location SW3(PD2) - SW2(PB6) – SW1(PD7)*/





#include "Proj_5D_header_file.h"


char keypress = 0;
	
int main (void){
char start_time[8], input_mode, display_mode;

setup_HW;												
User_instructions;

input_mode = eeprom_read_byte((uint8_t*)0x02);					//Read mode. Default value is 255

switch (input_mode){
case 255:														//If EEPROM contains 255 initiate the clock
String_to_PC("\r\nSend time: hours mins secs\
(24Hr clock, include spaces)");

for (int m = 0;  m < 8; m++)
{start_time[m] = waitforkeypress();							//Obtain time from KBD						
eeprom_write_byte((uint8_t*)(m+3),start_time[m]);}				//and save each keypress directly to EEPROM
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


display_mode = 0;




while(1){														//Operation continuously cycles around this loop 

switch (display_mode){											//Beware of switch bounce

case 0: if(switch_2_down){Timer_T0_10mS_delay_x_m(50); display_mode = 'A';}
		if(switch_1_down){display_mode = 'B';I2C_Tx_Clock_command(hide_clock);while(switch_1_down);}
		if(switch_3_down){display_mode = 'C';I2C_Tx_Clock_command(pause_clock);while(switch_3_down);}break;

case 'A': if(switch_2_up)display_mode = 1; else {while(switch_2_down); display_mode = 2;} break;
case 'B':if(switch_1_down){display_mode = 0;I2C_Tx_Clock_command(display_clock);while(switch_1_down);}break;
case 'C':if(switch_3_down){display_mode = 0; I2C_Tx_Clock_command(display_clock);while(switch_3_down);}break;


case 1: if (switch_1_down){I2C_Tx_Clock_command(increment_seconds);Timer_T0_10mS_delay_x_m(20);}
		if (switch_3_down){I2C_Tx_Clock_command(increment_minutes);Timer_T0_10mS_delay_x_m(20);}
		if(switch_2_down){ while (switch_2_down);display_mode = 0; }break;
		
case 2: if (switch_1_down){I2C_Tx_Clock_command(decrement_seconds);Timer_T0_10mS_delay_x_m(20);}
		if (switch_3_down){I2C_Tx_Clock_command(decrement_minutes);Timer_T0_10mS_delay_x_m(20);}
		if(switch_2_down){while (switch_2_down);display_mode = 0; }break;}}}

