//AUTO CAL OF ATMEGA 328 performed on programming PCB
//RESPOND TO USER PROMPT (whatever) by pressing AK.


long UC_from_KBD(void);
long buffer [10];

char cal_mode;
volatile char T1_OVF;
#include "Cal_V1_header_file.h"
#define Version "SW v1"
int main (void){
char dot_counter=0;
//char cal_factor=0;
char OSCCAL_DV, OSCCAL_WV, OSCCAL_approx;
long error_initial, error_mag, error; 
setup_HW;

/*Atmega_328_OSC_CAL;
if(MCUSR){
do{sendString("C  ");}while((isCharavailable(255) == 0));
receiveChar(); binUnwantedChars();
MCUSR = 0;}*/

ASSR = (1 << AS2); 		//initialise T2 for crystal
TCNT2 = 0;				//Reset counter register

Timer_T0_sub(T0_delay_5ms);									//Allows printing to finish before OSCCAL changes
		sei();
		cal_mode = 1;
		OSCCAL_DV = OSCCAL;
		EA_buff_ptr = 0x0F;									//It is automatically read at reset
		TIMSK2 |= (1 << TOIE2);								//Set Timer 2: interrupt on overflow
		TIMSK1 |= (1 << TOIE1);
		initialise_timers_for_cal_error();
		start_timers_for_cal_error();
		for(int m = 0; m < 5; m++)plot_cal_error(0);		//Warm up time
		for(int m = 0x0F; m <= 0xF0; m++){OSCCAL = m; error_initial = plot_cal_error(1);  
		dot_counter += 1; dot_counter = dot_counter%5; if(!(dot_counter))sendChar('.');
		if (error_initial < 0) error_mag = error_initial * (-1); else error_mag = error_initial;
		if(error_mag < 1500)break;
		}
		OSCCAL_approx = OSCCAL;
		
		cal_mode = 5;
		initialise_timers_for_cal_error();
		start_timers_for_cal_error();
		error = calibrate_quick_cal(OSCCAL_approx);			//Modifies OSCCAL
		if (error > 500) error = calibrate_quick_cal(OSCCAL + 4);			//Second iteration required
		if (error > 500)error = calibrate_quick_cal(OSCCAL - 2);
		
		OSCCAL_WV = OSCCAL;									//Save optimised value of OSCCAL
		initialise_timers_for_cal_error();
		TIMSK2 &= (~(1 << TOIE2));							//Dissable Timer 2: interrupt on overflow
		TIMSK1 &= (~(1 << TOIE1));
		
		newline();
		sendLongNum(16,OSCCAL_approx); sendChar('\t');
		sendLongNum(10,error_initial); sendString("\tInitial error\r\n");
		
		sendLongNum(16,OSCCAL_WV); sendChar('\t');
		sendLongNum(10,error); sendString("\tFinal error\r\n");
		
		eeprom_write_byte((uint8_t*)0x3FE, OSCCAL_WV); 
	eeprom_write_byte((uint8_t*)0x3FF, OSCCAL_WV);
	
	sendString("\r\nValues saved to EEPROM\t");				//Echo values back from the EEPROM
	sendLongNum(16,eeprom_read_byte((uint8_t*)0x3FE));
	sendString ("    ");
	sendLongNum(16,eeprom_read_byte((uint8_t*)0x3FF));
	sendString("\r\nAK to repeat\r\n");
	waitforkeypress();SW_reset;}		//keypress to repeat cal process
		//keypress to repeat cal process

/*************************************************************/
ISR(TIMER2_OVF_vect) {
long TCNT1_BKP, overflow = 0, target_res;

target_res = 62500;
TCCR1B = 0;							//Halt T1
TCNT1_BKP = TCNT1;					//Copy the value of TCNT1
TCNT1 = 0;							//Clear TCNT1
TCCR1B = 1;							//Get T1 running again ASAP (Note T2 has not stopped running)
						//Get T1 running again ASAP (Note T2 has not stopped running)
if(EA_counter < cal_mode)T1_OVF = 0;					//Ignore first 5 results
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

