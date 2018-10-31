//long buffer[45];////////////////////////////////////////////////////////////OVERFLOW on 168

//long buffer [10];

volatile int EA_counter, EA_buff_ptr;
volatile long error_SUM;
//unsigned char cmd_pin, resp_pin, clock_pin, reset_pin, reset_only_pin;

#define timer_T0_sub Timer_T0_sub
#define delay_2ms 4,195
#define delay_20ms 5,100

#define T0_delay_5ms 5,220
#define T0_delay_10ms 5,178
#define T1_delay_100ms 5,0xFCF2
#define delay_2us 1,254
#define delay_10ms 5,183

/*****************************************************************************/
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdint.h>


/*****************************************************************************/
#define SW_reset; {wdt_enable(WDTO_30MS);while(1);}
#define clear_digits; {for(int m = 0; m<=7; m++)digits[m]=0;}
#define pause {sendString(" AK");waitforkeypress();}
#define shift_digits_left; {for (int n = 0; n < 7; n++){digits[7-n] = digits[6-n];}}
#define shift_digits_right;\
{int n = 0; while(digits[n+1] && (n<=6)){digits[n] = digits[n+1]; n++;} digits[n]=0;}


/*****************************************************************************/

#define setup_HW;\
setup_watchdog;\
while (!(PIND & (1 << PD1)));\
USART_init(0,16);\
nop_delay(10);\
ADMUX |= (1 << REFS0);\
set_up_switched_inputs;\
Timer_T0_10mS_delay_x_m(1);\
Unused_I_O;


#define setup_watchdog;\
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;

#define wdr()  __asm__ __volatile__("wdr")

#define wd_timer_off;\
wdr();\
MCUSR &= (~(1 << WDRF));\
WDTCSR |= (1<<WDCE) | (1<<WDE);\
WDTCSR = 0x00;

#define set_up_switched_inputs;\
MCUCR &= (~(1 << PUD));\
DDRD &= (~((1 << PD2)|(1 << PD7)));\
PORTD |= ((1 << PD2) | (1 << PD7));\
DDRB &= (~(1 << PB6));\
PORTB |= (1 << PB6);


/*****************************************************************************/
#define Unused_I_O;\
MCUCR &= (~(1 << PUD));\
DDRB &= (~((1 << PB2)|(1 << PB7)));\
DDRC &= (~((1 << PC0)|(1 << PC1)|(1 << PC2)));\
DDRD &= (~((1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6)));\
PORTB |= ((1 << PB2)|(1 << PB7));\
PORTC |= ((1 << PC0)|(1 << PC1)|(1 << PC2));\
PORTD |= ((1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6));



#define Atmega_168_OSC_CAL;\
eeprom_write_byte((uint8_t*)0x1F9, OSCCAL);\
if ((eeprom_read_byte((uint8_t*)0x1F7) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x1F7) < 0xF0) && (eeprom_read_byte((uint8_t*)0x1F7)\
== eeprom_read_byte((uint8_t*)0x1F8))) {OSCCAL = eeprom_read_byte((uint8_t*)0x1F7);\
cal_factor=1;} else {cal_factor=0;}

#define Atmega_328_OSC_CAL;\
eeprom_write_byte((uint8_t*)0x3FD, OSCCAL);\
if ((eeprom_read_byte((uint8_t*)0x3FE) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x3FE) < 0xF0) && (eeprom_read_byte((uint8_t*)0x3FE)\
== eeprom_read_byte((uint8_t*)0x3FF))) {OSCCAL = eeprom_read_byte((uint8_t*)0x3FE);\
cal_factor=1;} else {cal_factor=0;}

