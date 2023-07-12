
/*
 A simple keypress logger which also drives the display. 
 At reset press r to record keypresses or R to replay them 
 */



#include "Keypress_logger_header.h"

volatile int interrupt_detected = 0 ;

int main (void)
{ char keypress;
  int EEP_address = 0x3E8;
  unsigned long PORT_1, PORT_2;

  setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_2_down)wdr();
  
  enable_pci_on_sw2;
  set_up_PCI_on_sw2;
  
 _delay_ms(10);
 sei();
 
  User_prompt;

  if (User_response == 'r')
  { UCSR0B |= (1 << RXCIE0);
  Initialise_display;
    String_to_PC_Basic("Send string and terminate it in a -cr- ?\r\n");
    while (1)
    { if (!(wait_for_interrupt(500, &keypress, &PORT_1, &PORT_2 )))
      {
        Char_to_PC_Basic('?');
      }
      else
      { interrupt_detected = 0;
        EEP_address = Keypress_to_EEPROM(keypress, EEP_address);
      }
    }
  }


  if (User_response == 'R')
  { while (EEP_address > address_of_last_string) {
      while (eeprom_read_byte((uint8_t*)(EEP_address)))
        Char_to_PC_Basic(eeprom_read_byte((uint8_t*)(EEP_address--)));
      EEP_address -= 1;
      waitforkeypress_Basic();
      String_to_PC_Basic("\r\n");
    }
    String_to_PC_Basic("No more strings\r\n");
  }

  SW_reset;
}


ISR(USART_RX_vect) {
  UCSR0B &= (~(1 << RXCIE0));
  {
    interrupt_detected = 1;
  }
}


char wait_for_interrupt (int m , char *keypress, unsigned long *PORT_1, unsigned long *PORT_2  )
{ int n = 0;
  while (1)
  { if (interrupt_detected == 1)
    { *keypress = Char_from_PC_Basic();
      UCSR0B |= (1 << RXCIE0);
      return 1;
    }
    else
    { n++; wdr(); if (n > 32000)
      { inc_display;
        m--;
        n = 0;
      }
      if (m == 0)return 0;
    }
  }
}


int Keypress_to_EEPROM(char keypress, int eep_address)

{ if ((keypress == '\r') || (keypress == '\n'))
  { keypress = '\0';
    save_string_address;
    Char_to_PC_Basic('?');
  }

  eeprom_write_byte((uint8_t*)(eep_address), keypress);
  if (eeprom_read_byte((uint8_t*)(eep_address)))
    Char_to_PC_Basic(eeprom_read_byte((uint8_t*)(eep_address)));
  else String_to_PC_Basic("\r\n");
  return --eep_address;
}



ISR(PCINT0_vect){

while(switch_2_down);
  
}



/************************************************************************************************************/
