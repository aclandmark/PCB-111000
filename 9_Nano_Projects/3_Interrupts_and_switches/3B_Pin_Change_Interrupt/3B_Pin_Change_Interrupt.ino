/*
 Target practice:
 In this example a basic LED driver routine runs in the for ground.
 The user operates user switch SW2 to shoot the LEDs down.
 
 Note SW2 is connected to a pin on the Atmega328 that is configured as input.
 When SW2 is pressed the voltage level on the pin changes from 5V to zero volts.
 A Pin Change Interrupt (PCI) is generated.
 */


#include "Pin_Change_Interrupt_header.h"

unsigned int PORT_1;                                    //Controls display
char scan;                                              //Controls led scan       
char switch_control;                                    //Temporarily disables switch
volatile unsigned int mask;                             //Records which leds haver been shot down


int main (void)
{ setup_HW;
  _delay_ms(10);
  sei();
  set_up_PCI_on_sw2;
  enable_pci_on_sw2;

  mask = 0xFFFF;                                        //No leds shot down yet
  
  while (mask)                                          //Mask is zero when last led has been shot down
  { switch_control = 1;                                 //Disable switch and 
    Timer_T0_10mS_delay_x_m(20);                         //pause at start of each scan
    scan = 1;                                           //Enable scan
    PORT_1 = 1;                                         //Illuminate RH leds at start of new scan
    while (scan)
    { if (PORT_1 == 1)switch_control = 0;               //Enable switch 2 at start of new scan
      I2C_Tx_2_integers
      (PORT_1 & mask, (~mask) ^ PORT_1);                 //Update display as leds are shot down
      Timer_T0_10mS_delay_x_m(10);
      if (PORT_1 & 0x8000)scan = 0;                     //Most LH leds illuminated: Pause scan
      else
        PORT_1 = (PORT_1 << 1);                         //Illuminate next set of LEDS moving left 
    }
  }
  I2C_Tx_2_integers(0, 0xFFFF);                    //Pause display before starting again
  Timer_T0_10mS_delay_x_m(200);
  SW_reset;
}


ISR(PCINT0_vect)
{ if (switch_2_up)return;                             //No action when switch is released
  if (switch_control) return;                         //Switch disabled: No action required
  if (mask & PORT_1)                                  //Led shot down                                 
  { mask &= ~PORT_1;                                  //Update mask to record which one
    scan = 0;                                         //Led shot down: Pause scan
  }
  switch_control = 1;                                //Switch pressed: Disable switch
}





/***********************************************************************************************************/
