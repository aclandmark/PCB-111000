//Manual calibration of ATMEGA 328 Performed on propgramming PCB


long UC_from_KBD(void);
int buffer[45];


volatile char T1_OVF;
#include "Cal_V2_header_file.h"
#define Version "SW V2"
int main (void){
long percentage_error;
char osccal_MIN, cal_factor=0;
char OSCCAL_WV;
char  New_UC_value;  
int error;
setup_HW;
Atmega_328_OSC_CAL;
OSCCAL_WV = OSCCAL;
if(MCUSR){	
while(1){do{sendString("C  ");}while((isCharavailable(255) == 0)); if(receiveChar() == 'C')break;}}
MCUSR = 0;
ASSR = (1 << AS2); 		//initialise T2 for crystal
TCNT2 = 0;				//Reset counter register

sendString("\r\nPerforming  manual calibration on ATMEGA328  ");
sendString(Version); newline(); sendString("Please wait 10 seconds");newline();

if (cal_factor==1){sendString("UC value  "); sendLongNum(16, OSCCAL_WV); newline();
sendString("DC value  "); sendLongNum(16, (eeprom_read_byte((uint8_t*)0x3FD))); }
if (cal_factor==0){sendString("No UC value\r\nDC value  "); sendLongNum(16, OSCCAL); newline();}

newline();
Timer_T0_sub(T0_delay_5ms);									//Allows printing to finish before OSCCAL changes

		sei();
		EA_buff_ptr = 0;									//It is automatically read at reset
		TIMSK2 |= (1 << TOIE2);								//Set Timer 2: interrupt on overflow
		TIMSK1 |= (1 << TOIE1);
		initialise_timers_for_cal_error();
		start_timers_for_cal_error();
		OSCCAL -=20;
		osccal_MIN = OSCCAL;
		for(int m = 0; m <= 40; m++){compute_error(1);OSCCAL++;}
		initialise_timers_for_cal_error();  				//dissable timers
		TIMSK2 &= (~(1 << TOIE2));							//Dissable Timer 2: interrupt on overflow
		TIMSK1 &= (~(1 << TOIE1));
		OSCCAL = OSCCAL_WV;

for(int m = 0; m <= 40; m++)							//Print table of values
{sendLongNum(16,osccal_MIN); osccal_MIN++;
sendChar('\t');sendLongNum(10,buffer[m]); 
sendChar('\t');
percentage_error = buffer[m];
sendLongNum(10,percentage_error*100/62500);sendChar('%'); 
newline();}

sendString("\r\nEnter new user cal value, FF to delete it or 0 to abort");					//Request new OSCCAL_User_Value

if((New_UC_value = UC_from_KBD()))						//zero aborts

{sendString ("\r\nPress y if OK or AOK to abort  ");
if (waitforkeypress() == 'y')

	{if ((New_UC_value != 0xFF) &&
	((error = compute_single_error(New_UC_value, OSCCAL_WV)) >= 1800)){
	newline(); sendLongNum(10, error); sendString("  Error too great, try again");
	newline();SW_reset;}	
	
	eeprom_write_byte((uint8_t*)0x3FE, New_UC_value); 
	eeprom_write_byte((uint8_t*)0x3FF, New_UC_value); 
	if(New_UC_value == 0xFF) OSCCAL = OSCCAL_WV; else OSCCAL = New_UC_value;}
else {SW_reset;}}
		
sendString("\r\nValues saved to EEPROM\t");				//Echo values back from the EEPROM
sendLongNum(16,eeprom_read_byte((uint8_t*)0x3FE));
sendString ("    ");
sendLongNum(16,eeprom_read_byte((uint8_t*)0x3FF));
sendString("\r\nAK to repeat\r\n");
waitforkeypress();SW_reset;}		//keypress to repeat cal process


/*************************************************************/
ISR(TIMER2_OVF_vect) {
long TCNT1_BKP, overflow = 0, target_res;

target_res = 62500;
TCCR1B = 0;							//Halt T1
TCNT1_BKP = TCNT1;					//Copy the value of TCNT1
TCNT1 = 0;							//Clear TCNT1
TCCR1B = 1;							//Get T1 running again ASAP (Note T2 has not stopped running)
						//Get T1 running again ASAP (Note T2 has not stopped running)
if(EA_counter < 5)T1_OVF = 0;					//Ignore first 5 results
else
{
switch(T1_OVF){
case 0: overflow = 0; break;
case 1: overflow = 65546; break;
case 2: overflow = 131072; break;}
error_SUM = error_SUM + (TCNT1_BKP - target_res + overflow);
T1_OVF = 0;}
EA_counter++;}

/*************************************************************/
ISR(TIMER1_OVF_vect) {T1_OVF += 1;}

/*************************************************************/
long UC_from_KBD(void){
char keypress;// sign = '+';
long number;

do
{keypress =  waitforkeypress();} 
while (!(hex_digit(keypress)));//(non_hex_char(keypress));

if(keypress == '0')return 0;
newline(); sendChar(keypress);
keypress -= '0';
if (keypress > 9)keypress -= 7;
number =  keypress;

while(1){
if ((keypress = wait_for_return_key())  =='\r')break;
if (hex_digit (keypress))	
{sendChar(keypress);
keypress -= '0';
if (keypress > 9)keypress -= 7;
number = number * 0x10 + keypress;}}
return number;}

