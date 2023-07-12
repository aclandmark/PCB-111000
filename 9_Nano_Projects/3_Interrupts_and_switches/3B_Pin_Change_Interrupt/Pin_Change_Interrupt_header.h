






#include <avr/wdt.h>

char watch_dog_reset = 0;

#define T0_delay_10ms 5,178
#define T1_delay_100ms 3, 0x9E62
#define T2_delay_10ms 7,178

#define switch_1_down  ((PIND & 0x80)^0x80)
#define switch_1_up   (PIND & 0x80)
#define switch_3_down ((PIND & 0x04)^0x04)
#define switch_3_up   (PIND & 0x04)
#define switch_2_down ((PINB & 0x40)^0x40)
#define switch_2_up   (PINB & 0x40)

/*****************************************************************************/
#define setup_HW \
setup_watchdog;\
set_up_I2C;\
ADMUX |= (1 << REFS0);\
set_up_switched_inputs;\
Set_LED_ports;\
Unused_I_O;\
eeprom_write_byte((uint8_t*)(0x1FD),OSCCAL);\
while (!(PIND & (1 << PD1)));\
Timer_T0_10mS_delay_x_m(5);\
OSC_CAL;\
setup_PC_comms_Basic(0,16);\
I2C_Tx_LED_dimmer();



/*****************************************************************************/
#define setup_HW_Extra \
setup_HW;\
\
Timer_T0_10mS_delay_x_m(1);\
I2C_TX_328_check();\
waiting_for_I2C_master;\
if (receive_byte_with_Nack()==1)\
{TWCR = (1 << TWINT);\
String_to_PC_Basic("\r\nPress\r\n\
1 for OS version\r\n\
2 for system data\r\n\
3 Message from the OS (x to escape)\r\n\
4 Default project\r\n\
0 to escape\r\n");\
switch (waitforkeypress_Basic()){\
case '0':break;\
case '1':I2C_Rx_get_version('0');break;\
case '2':I2C_Rx_get_version('1');break;\
case '3':do\
{Read_Hello_world_string();newline_Basic();}\
while (waitforkeypress_Basic() != 'x');\
break;\
case '4':wdt_enable(WDTO_30MS);\
I2C_Tx_display(); break;}}\
else TWCR = (1 << TWINT);



/*****************************************************************************/
#define setup_watchdog \
if (MCUSR & (1 << WDRF))watch_dog_reset = 1;\
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;

#define wdr()  __asm__ __volatile__("wdr")

#define SW_reset {wdt_enable(WDTO_30MS);while(1);}



/*****************************************************************************/
#define set_up_I2C \
TWAR = 0x02;



/*****************************************************************************/
#define set_up_switched_inputs \
MCUCR &= (~(1 << PUD));\
DDRD &= (~((1 << PD2)|(1 << PD7)));\
PORTD |= ((1 << PD2) | (1 << PD7));\
DDRB &= (~(1 << PB6));\
PORTB |= (1 << PB6);



/*****************************************************************************/
#define Unused_I_O \
MCUCR &= (~(1 << PUD));\
DDRB &= (~((1 << PB2)|(1 << PB7)));\
DDRC &= (~((1 << PC0)|(1 << PC1)|(1 << PC2)));\
DDRD &= (~((1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6)));\
PORTB |= ((1 << PB2)|(1 << PB7));\
PORTC |= ((1 << PC0)|(1 << PC1)|(1 << PC2));\
PORTD |= ((1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6));



/*****************************************************************************/
#define Set_LED_ports   DDRB = (1 << DDB0) | (1 << DDB1);
#define LEDs_on       PORTB |= (1 << PB0)|(1 << PB1);
#define LEDs_off      PORTB &= (~((1 << PB0)|(1 << PB1)));
#define LED_1_on      PORTB |= (1 << PB1);
#define LED_1_off     PORTB &= (~( 1<< PB1)); 
#define LED_2_off     PORTB &= (~(1 << PB0));
#define LED_2_on      PORTB |= (1 << PB0);

#define Toggle_LED_1 \
if (PORTB & (1 << PB1)){LED_1_off;}\
else {PORTB |= (1 << PB1);}



/*****************************************************************************/
#define OSC_CAL \
if ((eeprom_read_byte((uint8_t*)0x1FE) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x1FE) < 0xF0) && (eeprom_read_byte((uint8_t*)0x1FE)\
== eeprom_read_byte((uint8_t*)0x1FF))) {OSCCAL = eeprom_read_byte((uint8_t*)0x1FE);}



/*****************************************************************************/
#define diagnostic_mode \




/*****************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);



/*****************************************************************************/
#include "Resources_nano_projects/Subroutines/HW_timers.c"
#include "Resources_nano_projects/PC_comms/Basic_Rx_Tx_Basic.c"
#include "Resources_nano_projects/I2C_Subroutines/I2C_subroutines_1.c"
#include "Resources_nano_projects/I2C_Subroutines/I2C_subroutines_2.c"
#include "Resources_nano_projects/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "Resources_nano_projects/I2C_Subroutines/I2C_diagnostic.c"
#include "Resources_nano_projects/Subroutines/Random_and_prime_nos.c"




/******************************************************************************/
