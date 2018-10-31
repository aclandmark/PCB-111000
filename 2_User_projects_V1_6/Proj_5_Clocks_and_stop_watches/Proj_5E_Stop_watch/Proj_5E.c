
/*Proj_5E_Stop_watch
*********************************************************/


/*IT INTRODUCES

1.	Stop watch (SW) provided by the mini-OS

2.	Project subroutines
	I2C_Tx_Clock_command(one100ms_mode)				Initiates 100mS mode
	I2C_Tx_Clock_command(ten_ms_mode)				Initiates 10mS mode
	I2C_Tx_Clock_command(store_time)				Save time and pause display
	I2C_Tx_Clock_command(display_stored_times)		Displays time saved to memory
	I2C_Tx_Clock_command(display_current_time)		Resume stop watch mode
	

USER INSTRUCTIONS

	sw1 selects 100ms clock, sw3 a 10ms clock
	Press sw3 to pause SW and save time
	Press sw1 the read back saved times
	While pressing sw1 press sw3 to restore SW
	Press sw2 to re-initialise SW



Switch location SW3(PD2) - SW2(PB6) - SW1(PD7)*/




#include "Proj_5E_header_file.h"


volatile char T1_ovf_flag = 0;										//requires volatile label
																	//even though "T1_ovf_flag" is used in "main" 
int main (void){

setup_HW;
User_instructions;

setup_and_enable_PCI;
disable_pci_on_sw1_and_sw3;											//pci on sw1 & 3 not required	
{char digit_num=0; for (int m = 0; m < 8; m++)						//initialise display by
{I2C_Tx_any_segment('d', digit_num++);}}							//illuminating seg "d" on each digit

while(switch_1_up && switch_3_up);									//wait for a keypress
I2C_Tx_any_segment_clear_all();										//clear display
if(switch_1_down){I2C_Tx_Clock_command(one100ms_mode);}				//sw_1 selects 100ms clock
if(switch_3_down){I2C_Tx_Clock_command(ten_ms_mode);}				//sw_3 selects 10ms clock
Timer_T0_10mS_delay_x_m(5);											//50ms delay for switch bounce
while(switch_1_down || switch_3_down);								//wait for switch release

TIMSK1 |= (1 << TOIE1); sei();										//Enable timer 1 interrupt

while(1){															//Infinite loop, requires sw2 interrupt to exit
while(switch_1_up && switch_3_up);									//wait for a keypress

if(switch_3_down)
{I2C_Tx_Clock_command(store_time);									//save time and pause clock display
while (switch_3_down);												//wait for key release
I2C_Tx_Clock_command(display_current_time);}						//clock display continues as normal

Timer_T1_sub_with_interrupt(T1_delay_50ms);							//50mS pause: avoids switch bounce
while(T1_ovf_flag == 0);T1_ovf_flag = 0;							//T1_ovf_flag is reset by T1 ISR after a 50ms delay

if(switch_1_down)													//read stored times at 200ms intervals
{I2C_Tx_Clock_command(display_stored_times);						//if sw1 is held down
Timer_T0_10mS_delay_x_m(20);}		
if(switch_1_down && switch_3_down)									//press sw3 while sw1` is still held down to
{I2C_Tx_Clock_command(display_current_time); 						//continue clock display 
while(switch_1_down || switch_3_down);}							//wait until both switches have been released
}}				


ISR(TIMER1_OVF_vect) {TCCR1B = 0; T1_ovf_flag = 1;}				//stop timer 1 and set overflow flag

ISR(PCINT0_vect) {if(switch_2_up){return;}
I2C_Tx_Clock_command(AT_exit_stop_watch);
SW_reset;}
