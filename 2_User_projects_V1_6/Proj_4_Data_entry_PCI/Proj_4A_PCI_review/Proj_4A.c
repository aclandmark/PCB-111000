
/*Proj_4A_PCI_review
***********************************************************/

/*A REVIEW OF THE PIN CHANGE INTERRUPT FUNCTION


Note: The ATMEGA devices offer comprehensive Pin Change Interrupt (PCI) functions on 24 input/output (I/O) pins. 
These are in addition to the External Interrupt Request 0 and 1 pins which are not explored here. 

Because of the inevitable complexity and difficulties in interpreting the data sheets unambiguously this program 
has been written so that macros can be composed, tested and confidently used in future programs.


INTRODUCES


1.	Pin Change interrupt Enable bits PCIE0,PCIE1 & PCIE2 of the Pin Change Interrupt Control Register PCICR.

2.	The 24 "Pin change enable mask" bits, PCINT [0:23] held in the "Pin Change Mask" registers PCMSK0, PCMSK1 
and PCMSK2 which enable Pin Change Interrupt on individual I/O pins.

3.	The following locally defined macros required to setup and enable PCI on one or more I/O pins:

	"set_up_PCI" which sets the individual PCIEn bits (i.e. sets them to 1)
	"enable_pci" which sets the individual PCINT bits
	
Both "set_up" and "enable" macros must be run to activate PCI on any pin.

4.	The following locally defined macros used to manage pin change signals:

	"disable_pci" which clears the individual PCINT bits resulting in PCI on individual pins being ignored.
	"hold_PCI" which clears the individual PCIEn bits resulting in interrupts on individual pins being 
	recorded but not acted on.
	"restore_PCI" which is the same as "set_up_PCI" and enables interrupts placed on hold to be acted on. 
	"clear_PCI" which clears bits in the "Pin Change Interrupt Flag Register" and can be used to clear 
	interrupts  (maybe due to switch bounce) without calling the ISR.

Note: Switch bounce should not be a problem because a series resistor/shunt capacitor has been placed 
between each switch and its I/O pin.  
		
5.	Other locally defined macros used to interrogate the switch positions and also the PCICR and PCMSKn registers.		

		
USER INSTRUCTIONS

press sw1,2 & 3 to generate PCI
keypresses 1, 5 and 9 put interrupts on hold (sw activity is remembered)
keypresses 2, 6 and 0 activate any pending interrupts
keypress 3, 7 and disable interrupts (sw activity is ignored)
keypresses 4, 8 and b re-enable interrupts
keypress c gives a status report
Note:
Switch location SW3(PD2) - SW2(PB6) - SW1(PD7)
PD2&7 call ISR(PCINT2_vect)
PB6 calls ISR(PCINT0_vect)*/




#define message_cases_1_and_5 "Command: Hold interrupt on "
#define message_cases_2_and_6 "Command: Restore interrupt on "
#define message_cases_3_and_7 "Command: Dissable Interrupt on "
#define message_cases_4_and_8 "Command: Re-enable interrupt on "


#include "Proj_4A_header_file.h"



char sw_1_mem, sw_3_mem,counter ;
	

int main (void){

sw_1_mem = 'U';												//initial/previous sw position is up
sw_3_mem = 'U';
counter = 0;
setup_HW

String_to_PC("\r\nPCI FUNCTIONS");
User_instructions;
setup_and_enable_PCI;
UCSR0B |= (1 << RXCIE0);									//enable interrupt on keypress
sei();														//all interrupts now active
while(1);}													//wait here for interrupts


/*************************************************************************************************************/
ISR(PCINT2_vect) {											//ISR for sw1 and 3
Timer_T0_10mS_delay_x_m(5);									//switch bounce delay
clear_PCI_on_sw1_and_sw3;									//clear spurious interrupt flags
String_to_PC("PCINT2: ");

if((switch_1_up) && (switch_3_up)){						//interrupt for switch release
if (sw_1_mem == 'D')String_to_PC("switch_1_released\r\n");	//determine which switch has been released
if (sw_3_mem == 'D')String_to_PC("switch_3_released\r\n");
sw_1_mem = 'U';sw_3_mem = 'U';}								//set previous sw position to up.

else														//interrupt for switch press
{if (switch_1_down)
{String_to_PC("switch_1_pressed\r\n");sw_1_mem = 'D';}
if (switch_3_down)
{String_to_PC("switch_3_pressed\r\n");sw_3_mem = 'D';}}
clear_PCI_on_sw1_and_sw3;}									//clear spurious interrupts due to switch bounce



/*************************************************************************************************************/
ISR(PCINT0_vect) { 											//ISR for sw2
Timer_T0_10mS_delay_x_m(5);									//switch bounce delay
clear_PCI_on_sw2;											//clear spurious interrupt flags
String_to_PC("PCINT0: ");
if(switch_2_down)String_to_PC("switch_2_pressed\r\n");		//sw2 pressed 
else String_to_PC("switch_2 released\r\n");					//sw2 released
clear_PCI_on_sw2;}											//clear spurious interrupts due to switch bounce



/*************************************************************************************************************/
//Use keypresses to configure switch operation

ISR(USART_RX_vect){switch(receiveChar()){
case '1': hold_PCI_on_sw1_and_sw3;String_to_PC(message_cases_1_and_5);String_to_PC("sw1_and_sw3\r\n");break;
case '2': restore_PCI_on_sw1_and_sw3;String_to_PC(message_cases_2_and_6);String_to_PC("sw1_and_sw3\r\n");break;
case '3': disable_pci_on_sw1_and_sw3;String_to_PC(message_cases_3_and_7);String_to_PC("sw1_and_sw3\r\n");break;
case '4': enable_pci_on_sw1_and_sw3;String_to_PC(message_cases_4_and_8);String_to_PC("sw1_and_sw3\r\n");break;

case '5': hold_PCI_on_sw2;String_to_PC(message_cases_1_and_5);String_to_PC("sw2\r\n");break;
case '6': restore_PCI_on_sw2;String_to_PC(message_cases_2_and_6);String_to_PC("sw2\r\n");break;
case '7': disable_pci_on_sw2;String_to_PC(message_cases_3_and_7);String_to_PC("sw2\r\n");break;
case '8': enable_pci_on_sw2;String_to_PC(message_cases_4_and_8);String_to_PC("sw2\r\n");break;

case '9': hold_PCI;String_to_PC("Command:PCI on hold\r\n"); break;
case '0': restore_PCI;String_to_PC("Command:PCI restored\r\n");break;
case 'a': disable_pci; String_to_PC ("Command:dissable PCI\r\n");break;
case 'b': enable_pci; String_to_PC ("Command:enable PCI\r\n");break;

case 'c':	if(PCIenabled){String_to_PC("Status: PCI enabled ");
			if(pci_on_sw1_and_sw3_disabled)String_to_PC("but PCI on sw1 and sw3 disabled ");
			if(pci_on_sw2_disabled)String_to_PC("but PCI on sw2 disabled ");}
			else String_to_PC("Status: PCI dissabled  ");
			
			if(PCIset_up){String_to_PC("\r\nStatus: PCI setup ");
			if(PCI_on_sw1_and_sw3_on_hold)String_to_PC("but PCI on sw1 and sw3 placed on hold\r\n");
			if(PCI_on_sw2_on_hold)String_to_PC("but PCI on sw2 placed on hold\r\n");}
			else String_to_PC("Status: PCI placed on hold"); newline();break;

default: break;}
}

