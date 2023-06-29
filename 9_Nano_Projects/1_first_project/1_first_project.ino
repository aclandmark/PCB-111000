
#define switch_1_down  ((PIND & 0x80)^0x80)

#include "First_project_header.h"


int main (void)
{                        //start of main routine
unsigned int PORT_1;              
//setup_HW_basic;
setup_HW;

while(1){                   //While loop starts here
  PORT_1=1;                 //In the absence of a "SW_reset", "PORT_1" mast be re-initialised every time the loop is repeated
  for(int m = 1; m < 17; m++){        //for loop starts here
    I2C_Tx_2_integers(PORT_1, PORT_1);
    Timer_T0_10mS_delay_x_m(6);
    PORT_1 = PORT_1 << 1;}          //for loop ends here
    
    if (switch_1_down)
{Num_to_PC(16, eeprom_read_byte((uint8_t*) 0x1FF));String_to_PC("\r\n"); 
Num_to_PC(16, eeprom_read_byte((uint8_t*) 0x1FE)); String_to_PC("\r\n");
 Num_to_PC(16, eeprom_read_byte((uint8_t*) 0x1FD)); String_to_PC("\r\n");}  
    
    }                   //while loop ends here
}                       //end of main loop
