
/*
Interrupts enable several process to run at the same time.
Here the main task is to do some simple arithmetic and send the results to the PC
However a timer interrupt that occurs every 100ms is used to drive the LED display.
Code to setup a keypress interrupt is also given.  This enables the user to adjust 
the display.

When an interrupt occurs program control jumps to the Interrupt Service Routine (ISR)

Note: Arduino reserves the ISR(USART_RX_vect) subroutine which is therefore not easily available
to the user.  For these examples "setup_328_HW_Basic_IO" is used and all subroutines using 
the Arduino Serial library are commented out.
*/


#define reset_display \
  PORT_1= ~0;\
  PORT_2= 0;\
  m = 0;


#include "UART_timer_interrupt_header.h"

int m, num_leds = 8;
unsigned int PORT_1, PORT_2;

int main (void)
{ unsigned long counter;
  
  setup_HW;

wdt_enable(WDTO_120MS);
  while(switch_3_down)wdr();
  
  _delay_ms(10);
  sei();
  reset_display;

  
  UCSR0B |= (1 << RXCIE0);
  T1_100ms_clock_tick();
  counter = 1;
  newline_Basic();
  while((switch_2_up) && (switch_3_up))wdr();
  
  if(switch_3_down){cli(); while(1);}
  
  while(switch_3_up)
  { 
    if((!(counter%33)) && (switch_2_down)){
Num_to_PC_Basic(counter); Char_to_PC_Basic('\t');
Num_to_PC_Basic((counter * counter)); newline_Basic();
 }
 counter = (counter + 1)%(unsigned long)0x8000;
    _delay_us(250);
         wdr();
  }
   
    while(switch_3_down);
}



ISR(TIMER1_COMPA_vect)                                            //Routine called every 100 mS
{ UCSR0B &= (~(1 << RXCIE0));                               //Disable Serial interrupt 
  TCNT1 = 0;                                                      //Update display
  if (m < num_leds)
  { PORT_2 = (PORT_2 << 1) | 1;
    PORT_1 = PORT_1 & (~(1 << m));
  }
  else
  { PORT_2 = (PORT_2 >> 1);
    PORT_1 |= (1 << (num_leds * 2 - m - 1));
  }
  for (int m = 15; m >= num_leds; m--)
    PORT_1 &= ~(1 << m);
  sei(); I2C_Tx_2_integers(PORT_1, PORT_2);
  m += 1;
  if (m == num_leds * 2)m = 0;
  UCSR0B |= (1 << RXCIE0);
}


void T1_100ms_clock_tick(void)                                      //Start the T1 clock
{ TCNT1 = 0;
OCR1A = 12500;
  TIMSK1 |= (1 <<  OCIE1A);
  TCCR1B = 0x03;
}



ISR(USART_RX_vect)
{ switch (Char_from_PC_Basic())
  { case '2':   num_leds = 2; break;
    case '4':   num_leds = 4; break;
    case '6':   num_leds = 6; break;
    case '8':   num_leds = 8; break;
    case 'A':   num_leds = 10; break;
    case 'C':   num_leds = 12; break;
    case 'E':   num_leds = 14; break;
    case '0':   num_leds = 16; break;
  }
  reset_display;
}





void Num_to_PC_Basic (unsigned long number)
{ int i = 0;
  char s[12];
  
  do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');
}




/***************************************************************************************************/
