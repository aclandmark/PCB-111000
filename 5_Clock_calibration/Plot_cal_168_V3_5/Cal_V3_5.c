long UC_from_KBD(void);

long buffer [5];//unused

volatile char T1_OVF;
#include "Cal_V3_5_header_file.h"
#define Version "SW V3_5"
int main (void){
char cal_factor=0;
char OSCCAL_DV, OSCCAL_WV;
long error;

setup_HW;
Atmega_168_OSC_CAL;
OSCCAL_WV = OSCCAL;

if(MCUSR){
while(1){do{sendString("C  ");}while((isCharavailable(255) == 0)); if(receiveChar() == 'C')break;}}
MCUSR = 0;
ASSR = (1 << AS2); 		//initialise T2 for crystal
TCNT2 = 0;				//Reset counter register


sendString("\r\nGenerate plot of ATMEGA168 calibration factors  ");
sendString(Version); newline(); sendString("Please wait 10 seconds");newline();

if (cal_factor==1){sendString("UC value  "); sendLongNum(16, OSCCAL); newline();
sendString("DC value  "); sendLongNum(16, (eeprom_read_byte((uint8_t*)0x3FD))); }
if (cal_factor==0){sendString("No UC value\r\nDC value  "); sendLongNum(16, OSCCAL); newline();}

newline();
Timer_T0_sub(T0_delay_5ms);									//Allows printing to finish before OSCCAL changes
		sei();
		OSCCAL_DV = OSCCAL;
		EA_buff_ptr = 0x0F;									//It is automatically read at reset
		TIMSK2 |= (1 << TOIE2);								//Set Timer 2: interrupt on overflow
		TIMSK1 |= (1 << TOIE1);
		initialise_timers_for_cal_error();
		start_timers_for_cal_error();
		for(int m = 0; m < 5; m++)plot_cal_error(0);		//Warm up time
		for(int m = 0x0F; m <= 0xF0; m++){OSCCAL = m; error = plot_cal_error(0);
		OSCCAL = OSCCAL_WV;
		sendLongNum(10,m); sendChar('\t');
		sendLongNum(10,error); sendChar('\t');
		sendLongNum(10,error*100/62500);
		sendChar('%');newline();
		Timer_T0_sub(T0_delay_5ms);
		}
		
		initialise_timers_for_cal_error();  				//dissable timers
		TIMSK2 &= (~(1 << TOIE2));							//Dissable Timer 2: interrupt on overflow
		TIMSK1 &= (~(1 << TOIE1));

waitforkeypress(); SW_reset;}								//keypress to repeat cal process

/*************************************************************/
ISR(TIMER2_OVF_vect) {
long TCNT1_BKP, overflow = 0, target_res;

target_res = 62500;
TCCR1B = 0;							//Halt T1
TCNT1_BKP = TCNT1;					//Copy the value of TCNT1
TCNT1 = 0;							//Clear TCNT1
TCCR1B = 1;							//Get T1 running again ASAP (Note T2 has not stopped running)
						//Get T1 running again ASAP (Note T2 has not stopped running)
if(EA_counter < 1)T1_OVF = 0;					//Ignore first 5 results
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

