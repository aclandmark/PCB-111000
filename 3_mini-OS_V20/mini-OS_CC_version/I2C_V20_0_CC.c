/*********************
Projects version 9
I2C program version 4

V9.9 Prevents users applying impractical user-cal values to the ATMEGA 328
Performs scan of the ATMEGA 328 clock cal factor
Following POR check cal factor and perform auto clock recovery if necassary

*EEPROM reservations
0x3FF	user cal if set
0x3FE	user cal if set
0x3FD	Default cal supplied by Atmel
0x3FC	If 1: press 'x' diagnostic mode else press 'r' normal mode
0x3FB	If 0 use multiplexter (T0) period of 4ms else use period of 2mS (std)	?????THIS IS THE NEW FEATURE


***********************/

# include "I2C_V20_0_CC.h"
# include "I2C_V20_0_CC_display_subroutines.c"

# include "../Resources/I2C_V20_0_IO_subroutines.c"
# include "../Resources/I2C_V20_0_Arithmetic_subroutines.c"
# include "../Resources/I2C_V20_0_ISR_subroutines.c"
# include "../Resources/I2C_V20_0_osccal_calibration subroutintes.c"
# include "../Resources/I2C_modes_B_to_F.c"
# include "../Resources/I2C_mode_definitions.h"
# include "../Resources/I2C_clock_stop_watch_subroutines.c"
# include "../Resources/I2C_Eeprom_subroutines.c"



int main (void){

signed char digit_num=0;
int letter=0;
char payload_size, sign;
long L_number;
signed char expnt;
char sign_bit;
char test_num, test_digit;

char *SW_Version = "OS: I2C_V20_0_CA\r\n";
char *SW_info = "SW information: Operating system I2C_V20_0_CA\
  Projects V1_9 and Bootloader V4_22_CC. (zero_SUT & auto clock recovery) \
  External programmer V2_30A\r\n";
	

/****Watchdog initiated for mode F only (user clock/stop watch with
external 10mS crystal interrupt).*********/

/***********Brown-out:  This is set (using config bits only) for 2.9V*************/

if(MCUSR & (1 << BORF)){									//Detect brown-out
MCUSR &= (~(1 << BORF));}									//Reset brown-out flag 

ADMUX |= (1 << REFS0);										//select internal ADC ref and remove external supply on AREF pin
setup_watchdog;	
initialise_IO;												//Ensures that all IO is initially set to WPU
set_digit_drivers;
clear_digits;
clear_display;
initialise_Arithmetic_variables;

					
TWBR = 32;													//gives 100KHz I2C clock for TWSR 
ASSR = (1 << AS2); 										//initialise T2 for crystal 
timer_2_counter=0;											//Initialsise timer_2_counter to zero


if((MCUSR & (1 << PORF)))									//If POR check OSC Calibration and recal if necessary
{Cal_at_Power_on_Reset ();}							
OSCCAL_WV = OSCCAL;
OSCCAL_DV = eeprom_read_byte((uint8_t*)0x3FD);				//Save OSCALL working and default values

	
if((eeprom_read_byte((uint8_t*)0x3FB) == 0xFF) ||\
(eeprom_read_byte((uint8_t*)0x3FB) == 0x01));				
else eeprom_write_byte((uint8_t*)0x3FB,0x01);				//set display brightness

sei();	
T0_interupt_cnt = 0;										//Start multiplexer
TIMSK0 |= (1 << TOIE0);									//T0 interrupt enabled
MUX_cntl = 0;
if(eeprom_read_byte((uint8_t*)0x3FB) == 0xFF)
{timer_T0_sub_with_interrupt(T0_delay_2ms);}				//Normal Display brightness
else {timer_T0_sub_with_interrupt(T0_delay_500us);}


while(1){													//main loop in which the program resides

while((mode == 'F')\
||(mode == 'K')\
 || (mode == 'U'));	



while(1){													//Repetitively poll the PIC until it responds
TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);			//send a start condition  (this identical to a "repeated start")
while (!(TWCR & (1 << TWINT)));							//Wait for TWINT flag 
TWDR = 0x03;												//Address of slave (master read operation)  SLA + R
TWCR = (1 << TWINT) | (1 << TWEN);							//Clear TWINT bit to start transmission of address
while (!(TWCR & (1 << TWINT)));							//Wait for TWINT flag 
if (TWSR == 0x40)break;										//SLA + R successfully transmitted ACK recveived 
if(clock_flag==1){refresh_clock_display;}}

clock_flag=0; 

payload_size = I2C_master_receive(1);
if(payload_size)mode = I2C_master_receive(1);
else mode = I2C_master_receive(0);

if (mode != 8) display_mask=0;								//dissable flashing digits

if(payload_size){											//payload is zero for mode 'G'

for (int m = 0; m < payload_size; m++){					//payload_size = Num digits to be downloaded
if (m == (payload_size-1))
I2C_data[m] = I2C_master_receive(0);
else I2C_data[m] = I2C_master_receive(1);}
TWCR = (1 << TWINT ) | (1 << TWEN ) | (1 << TWSTO );}		//Send stop bit	

switch(mode){
case 3: case 8: case 7: 
case 'F':  case 'I':  case 'Q': break;
default: {clear_display_buffer;}break;}

switch (mode){

case 1:		I2C_Tx_2_integers; break;						//Uses the 32 vertical segments to display two binary numbers

case 2:														//Illuminates/clears any of the 56 segments
case 3: 	I2C_Tx_any_segment; break;														


case 4: 	I2C_Tx_8_byte_array;break;						//Display a string of 8 digits														


case 5: 	I2C_Tx_display_char; break;						//Displays +/- char as number and binary																											


case 6: 	I2C_Tx_long; break;								//Displays 8 digit number																


case 7: 	I2C_Tx_Compile_tables();break;					//I2C_Tx_Initiate_tables() and I2C_Tx_Inc/dec_tables()														


case 8:		I2C_Tx_2URNs_from_IO ();break;					//Generates numbers of type 0.1234 With an exponent									 


	
case 9:		I2C_Tx_Uarithmetic_OP ();break;					//Takes numbers entered using mode 8 and does some arithmetic																												

	
case 'A':	I2C_Tx_accumulator_1();break;					//Implements add, subtract and clear modes													

case 'B':	I2C_Tx_accumulator_2();break;					//Aquires data for use by accumulator												



case 'C':	basic_clock(); break;							//I2C_Tx_OS_timer(AT_clock_mode, start_time): Starts the timer/clock																

case 'D':	multi_mode_clock(); break;						//Used with subroutine I2C_Tx_Clock_command(char timer_mode, char command): Controls the timer/clock													

	
case 'E':	stop_watch(); break;							//Used by I2C_Tx_Clock_command(one100ms_mode) and I2C_Tx_Clock_command(ten_ms_mode)													


case 'F':	I2C_initiate_10mS_ref; break;					//Ten_mS_interrupt for combined clock and stopwatch;														

	
case 'G':	I2C_Tx_BWops;	break;							//Used to illustarte bit wise logic operations														


case 'H':	Message_from_the_OS(); break;					//Messages compiled by user and saved to pcb_A Atmega 328 EEPROM																			


case 'I':	I2C_displayToLong; break;						//Converts display to long number and transmits it to the Atmega 168														

case 'J':	I2C_Tx_real_num; break;							//Displays a real number													

case 'K':	I2C_Tx_float_num; break;						//Scrolls scientific number accross the display															


case 'L':	Multiplexer_demo; break;



case 'M':	cal_plot_328(); break;							//Scans 328 cal-factor fronmm 0x10 to 0xF0									


case 'N':	manual_cal_PCB_A_device(); break;

	
case 'O':	PCB_test; break;								//For manufacturing test: Dissables the multiplexer
	
case 'P':	I2C_Rx_get_version; break;

case 'Q':	I2C_Tx_LED_dimmer; break;

case 'U':	start_T2_for_ATMEGA_168_cal(1); break;			  


/**********Modes 'R', 'S' and 'T' are not available***************************/

case 'V':	set_diagnostic_mode; break;						//0x3FC is set to 1 when the user enters 'x' at the p/r prompt


case 'W':	restore_168_EEPROM_strings; break;				//Used to restore AT168 EEPROM from backup version on AT328
	

case 'Y':	spot_check_328_cal(); break;}}}




	

	void I2C_master_transmit(char data){
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));}		

	char I2C_master_receive(char mode){
	char data;
	if(!(mode))TWCR = (1 << TWINT) | (1 << TWEN);				
	else TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);					
	while (!(TWCR & (1 << TWINT)));
	data =  TWDR;
	return data;}



