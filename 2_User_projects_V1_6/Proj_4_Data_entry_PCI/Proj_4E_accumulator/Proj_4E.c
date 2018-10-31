
/*Proj_4E_accumulator
**********************************************/


/*IT INTRODUCES 

An accumulator program for multiple additions and subtractions of integer numbers


USER INSTRUCTIONS

repeatedly press sw2 to enter and shift data
press sw1 to enter data ending in zero (i.e.10,20 etc.)
otherwise press sw1 while selecting final integer
release SW2 when integer is correct
Then release sw1
Press sw1 to add number to accumulator
Press sw3 to subtract it from accumulator
press sw2 just before sw1 to enter new data
press sw3 rather than entering new data to reset everything




Switch location SW3(PD2) - SW2(PB6) – SW1(PD7)*/



#include "Proj_4E_header_file.h"



int main (void){

setup_HW;
User_instructions;
setup_and_enable_PCI;
disable_pci_on_sw1_and_sw3;

Return_key=0;											//Initialise return key
I2C_Tx_accumulator_op(digits, 'I');						//initialise display to "_ _ _ _ _ _ _ 0"

while(1){

sei(); 													//Enable interrupts

while(1){if ((switch_3_down) && (!(no_reset)))		//clear accumulator and reset
{I2C_Tx_accumulator_op(digits, 'C');SW_reset;}			//only allowed when new number is zero 	

if(!(Return_key)); else {no_reset = 0; break;}}		//Line A wait here for data to be entered using sw2 and sw2_isr
Return_key=0;cli();										//Clear interrupts so that sw2 can be used for other purposes

while(1){ 
while ((switch_1_up) && (switch_3_up));				//wait for switch_1 or 3 to be pressed.
if (switch_2_down)break;								//If switch_2 down exit and reinitialise new number

if(switch_1_down)I2C_Tx_accumulator_op(digits, 'A');	//Add new number to accumulator

if(switch_3_down)I2C_Tx_accumulator_op(digits, 'S');	//Subtract new number from accumulator
while((switch_1_down) || (switch_3_down));				//wait for switch_1 or 3 to be released (inside while loop)
Timer_T0_10mS_delay_x_m(5);}							//switch bounce delay

while((switch_1_down) || (switch_3_down));				//wait for switch_1 or 3 to be released (outside while loop)
while(switch_2_down);									//wait for switch 2 to be released

I2C_Tx_accumulator_op(digits, 'N');						//Prepare accumulator for new value
disable_pci_on_sw1;										//Needed for 2 or 3 digit numbers
Return_key=0;}}											//Reset return key




/****************************************************************************************************************/
ISR(PCINT0_vect){accumulator_sw2_isr();}
ISR(PCINT2_vect){accumulator_sw1_and_sw3_isr();}




/****************************************************************************************************************/
void accumulator_sw2_isr(void){			
Timer_T0_10mS_delay_x_m(5);								//sw bounce delay
clear_PCI_on_sw2;										//clear spurious interrupts
if (switch_2_up) return;								//Ignore key release

no_reset = 1;											//prevents sw_reset using sw3
while(1){												//scrolling digit[0]
do{														//Keypress on SW2 scrolls digits[0] through digits 0-9	
switch(digits[0]){										//Test digits[0]		
case '9': digits[0] = '0';break;						//If 9 set to 0
default: digits[0]++; break;}							//Else increment
I2C_Tx_accumulator_op(digits, 'D');						//Data input to accumulator
Timer_T0_10mS_delay_x_m(25);	
}while(switch_2_down);									//Continue while sw2 is down							

Timer_T0_10mS_delay_x_m(50); 							//wait 500ms after sw2 has been released
if(switch_2_up)break;}									//if sw2_down again continue scrolling the digit
														//When sw2 is finally released:
if(switch_1_down){Return_key=1; 						//Set return key if sw1 is down (data entry is complete)					 
while(switch_1_down);									//wait for sw1 release 
Timer_T0_10mS_delay_x_m(20);							//anti sw bounce delay
return;}  												//return to Line A

shift_digits_left;										//Ongoing acquisition?(sw1_up) then shift display left
digits[0] = '0'; 										//Initialise digit[0] to '0'						
digits[3] = 0;
enable_pci_on_sw1;										//pci_on_sw1 is only needed to enter numbers like 10, 20 100
I2C_Tx_accumulator_op(digits, 'D');}					//Update display and return to line A without setting the return key




/****************************************************************************************************************/
void accumulator_sw1_and_sw3_isr(void){

Timer_T0_10mS_delay_x_m(5);								//switch bounce delay
clear_PCI_on_sw1_and_sw3;								//clear spurious interrupts

if ((switch_1_up) && (switch_3_up))return;
if (switch_1_down) Return_key=1;
disable_pci_on_sw1;	
{flash_display;}										//sw1 only needed to enter numbers like 10, 20, 50, 100
while(switch_1_down);}									//wait for sw release




