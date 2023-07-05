
/*
ATMEGA 168 EEPROM reservations
0x1FF		User cal value
0x1FE		User cal value
0x1FD		Default cal value
0x1FC		Used by PRN_8bit_GEN()
0x1FC/B		Used by PRN_16bit_GEN()
*/



int Mux_cntl_2;
int test;
char watch_dog_reset = 0;
char User_response;

#define T0_delay_5ms 5,220
#define T0_delay_4ms 4,227
#define T0_delay_10ms 5,178



#define T1_delay_250ms 5,0xF85F

#define T1_delay_10ms 3, 0xF63C
#define T1_delay_20ms 3, 0xEC79
#define T1_delay_30ms 3, 0xE2B6
#define T1_delay_50ms 3, 0xCF31
#define T1_delay_100ms 3, 0x9E62
#define T1_delay_500ms 5,0xF0BE


#define T1_delay_1sec 5,0xE17D
#define T1_delay_2sec 5,0xC2FB
#define T1_delay_3sec 5, 0xA479
#define T1_delay_4sec 5,0x85F7

#define T2_delay_2ms 4,0
#define T2_delay_8ms 7,192
#define T2_delay_32ms 7,0


/*****************************************************************************/
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdint.h>
#include <stdio.h>


/*****************************************************************************/
#define SW_reset {wdt_enable(WDTO_30MS);while(1);}
#define clear_digits {for(int m = 0; m<=7; m++)digits[m]=0;}
#define pause {String_to_PC(" AK");waitforkeypress();}
#define shift_digits_left {for (int n = 0; n < 7; n++){digits[7-n] = digits[6-n];}}
#define shift_digits_right \
{int n = 0; while(digits[n+1] && (n<=6)){digits[n] = digits[n+1]; n++;} digits[n]=0;}


/*****************************************************************************/
#define setup_HW_basic \
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
setup_PC_comms_Basic(0,16);

#define setup_HW \
setup_HW_basic;\
\
Timer_T0_10mS_delay_x_m(1);\
I2C_Tx_LED_dimmer();\
diagnostic_mode;


/*****************************************************************************/
#define setup_watchdog \
if (MCUSR & (1 << WDRF))watch_dog_reset = 1;\
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;

#define wdr()  __asm__ __volatile__("wdr")

#define wd_timer_off \
wdr();\
MCUSR &= (~(1 << WDRF));\
WDTCSR |= (1<<WDCE) | (1<<WDE);\
WDTCSR = 0x00;


/*****************************************************************************/
#define One_25ms_WDT_with_interrupt \
wdr();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = (1<< WDE) | (1 << WDIE) |  (1 << WDP0)  |  (1 << WDP1);


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



#define Set_LED_ports		DDRB = (1 << DDB0) | (1 << DDB1);
#define LEDs_on				PORTB |= (1 << PB0)|(1 << PB1);
#define LEDs_off			PORTB &= (~((1 << PB0)|(1 << PB1)));
#define LED_1_on			PORTB |= (1 << PB1);
#define LED_1_off			PORTB &= (~( 1<< PB1));	
#define LED_2_off			PORTB &= (~(1 << PB0));
#define LED_2_on			PORTB |= (1 << PB0);

#define Toggle_LED_1 \
if (PORTB & (1 << PB1)){LED_1_off;}\
else {PORTB |= (1 << PB1);}

/*****************************************************************************/
#define OSC_CAL \
if ((eeprom_read_byte((uint8_t*)0x1FE) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x1FE) < 0xF0) && (eeprom_read_byte((uint8_t*)0x1FE)\
== eeprom_read_byte((uint8_t*)0x1FF))) {OSCCAL = eeprom_read_byte((uint8_t*)0x1FE);}


/*****************************************************************************/
#define User_prompt \
while(1){\
do{String_to_PC("R?    ");}	 while((isCharavailable(250) == 0));\
User_response = receiveChar();\
if((User_response == 'R') || (User_response == 'r'))break;} String_to_PC("\r\n");



/*****************************************************************************/
#define diagnostic_mode \
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
case '4':I2C_Tx_display(); break;}}\
else TWCR = (1 << TWINT);




/*****************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);




