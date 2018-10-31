

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/Header_files/clock_stop_watch.h"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../Resources_V12/Header_files/I_O_from switches.h"		//Responds to switch activity

# define User_instructions \
String_to_PC(message_1);\
String_to_PC(message_2);\
String_to_PC(message_3);\
String_to_PC(message_4);\
String_to_PC(message_5);\

# define message_1 \
"\r\nCOMBINED CLOCK/SW:\r\n\
Conect to pc, press sw1 and enter time (or reset and repeat)\r\n\
Power cycle to 5V supply if wanted and press sw1 then sw3 to start\r\n"

# define message_2 \
"Toggle sw3 to switch between clock and new SW and press sw 2 to select old SW\r\n\
Press sw1 to pause, blank or resume display\r\n"

# define message_3 \
"\r\n\
Start by pressing sw2 in place of sw1 \
to enable 2 stop watches and no clock\r\n\r\n\
To adjust time: Press SW3 to restore clock time "


# define message_4 \
"then immediately press SW1 to induce a WDTout\r\n\
Release sw1 to increment time or sw3 to decrement it then both switches to restore normal operation.\r\n"

# define message_5 \
"\r\n\
Bug: Having pressed SW2 when no timer is running \
it is neccessary to press SW3 twice to restore normal operation\r\n"


#define Time_on_screen \
Char_to_PC(digits[7]);Char_to_PC(digits[6]);Char_to_PC(' ');\
Char_to_PC(digits[5]);Char_to_PC(digits[4]);Char_to_PC(' ');\
Char_to_PC(digits[3]);Char_to_PC(digits[2]);Char_to_PC(' ');\
Char_to_PC('0'); Char_to_PC('0');

#define Update_time_on_screen \
String_to_PC("\b\b\b\b\b\b\b\b\b\b\b");Time_on_screen;
#define ms_on_screen_update  String_to_PC("\b\b");Char_to_PC(digits[1]);Char_to_PC(digits[0]);
#define Update_sec_counter  if(sec_counter_save){cli(); sec_counter = sec_counter_V;\
sei();sec_counter_save=0;}
//Note: sec_counter is long.  The volatile version must remain fixed (i.e no interrupts) while it is being updated

#define setup_64ms_WDT_with_interrupt \
wdr();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = (1<< WDE) | (1 << WDIE) | (1 << WDP1);


volatile char T0_OVF_FLAG, T1_OVF_FLAG = 0, T1_OVF_counter=0;
char  TWI_flag, mode;

volatile char ms_counter;				//Increments every 10mS	
char old_mode;							//Used to restore mode when display is re-activated
char digits[8], charH, charL;			//Holds characters to be displayed
char Hours, Minutes, Seconds;
	
long sec_counter;						//Display time is based on sec_counter 
long sec_counter_V;  					//Volatile version of sec_counter (updated by TWI ISR)
long clock_time_long; 					//Saves time (sec + ms * 100); used to switch between clock * SW 
char payload;							//No of characters to send over I2C (8 every sec & 2 otherwise)
char sec_counter_save;					//Set to 1 every second (indicates that sec_counter is to be updated from sec_counter_V)
char display_clear;						//Records status of display (blank or active)


void set_time(void);					//If mode is 255: 	Enables user to enter start; converts it to seconds and saves it in seconds in EEPROM										//If mode is zero:	Reads time back out from EEPROM locations
										//If mode is 0:		Reads time back from EEPROM locations 3 - 6; saves resut in "sec_counter"
										//Stores mode in EEPROM locarion 2
void Calculate_time(void);				//Converts seconds ("sec_counter") to hours, minutes and seconds 
void timer_utoa(char);					//Converts hours, minutes and seconds to strings

void Timer(void);						//Calls "update_7_seg_display", to close one I2C transaction and sets payload to initiate the next one					
//ISR (TWI_vect)						//Detects 10mS interrupt. Initiates I2C_transaction and responds by sending payload size
void update_7_seg_display(void);		//I2C controler: supervises transmission of timer data to the mini OS.
long clockTOstop_watch(void);			//Saves time before selecting stop watch display (i.e. reseting time to zero)
long restore_Clock(void);				//Restores time by adding "clock_time" to the current SW time
long restore_stop_watch(void);			//Restores SW time by subtracting the curent time from the value stored in "clock_time"
void restore_display(void);				//Re-activate Stop watch/clock display
void clear_display(void);				//Blanks the display
char sw_de_bounce(void);				//User Timer with interrupt to control SW bounce
void print_out_bkp(void);				//For test only: prints out contents of BKP store EEPROM 7 - 18
