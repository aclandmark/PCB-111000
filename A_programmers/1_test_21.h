
void Clock_period(void);

#define two_msec_delay;		timer_T0_sub(delay_2ms);
#define five_msec_delay;	timer_T0_sub(delay_5ms);
#define ten_msec_delay;		timer_T0_sub(delay_10ms);
#define twenty_msec_delay;	timer_T0_sub(delay_20ms);

#define	next_char; while(r_pointer == w_pointer);		

void Clock_period(void){for(int p = 0; p<= 3; p++){asm("nop");}}	//P <= 3 AT START

#define PGClock_L  
#define PGClock_H  Clock_period();

#define delay_20ms 5,100
#define delay_10ms 5,183
#define delay_5ms 5,220
#define delay_2ms 4,195

#define RBL 255

/* SW subs prototypes*/

void newline(void);
//char waitforkeypress(void);
char receiveChar(void);
char isCharavailable (int);
void waitforchar(void);
//char Rx_low_speed_data(void);
//void binUnwantedChars (void);
void sendChar(char);
void sendString(char*);
//void sendHex(char, unsigned int);
void USART_init (unsigned char, unsigned char);
//void my_utoa(char, unsigned int, char*, char);
void new_record(void); 
void start_new_code_block(void);
void Program_record(void);
void decode_cmd_write_to_flash_SUB(void);
void decode_HEX_cmd_SUB(void);
void write_page_SUB(int);

/* HW subs prototypes*/

char Atmel_config ( unsigned int, unsigned int );
char Read_write_mem(char, int, char);
void Load_page(char, int, unsigned char);

/* main subs prototypes*/

void Program_Flash (void);
void Verify_Flah_99(void);

//void timer_T0_sub_with_interrupt(char, unsigned char);
//void delay_with_interrupt(char, unsigned char);
void timer_T0_sub(char, unsigned char);

#define inc_r_pointer;\
r_pointer++;\
r_pointer = r_pointer & 0b00111111;

/***********Main variables****************/

signed char line_length;
signed char line_length_old;
signed char short_line;
int Hex_address;
int PIC_address;
signed int page_address;
signed int PAmask;
signed char offset;		//used for different things
signed char space_on_page;
signed int PageSZ;
char Flash_flag;
signed char orphan;
signed char section_break;
signed char page_break;
int prog_counter;
unsigned int Hex_cmd;
signed int FlashSZ;
int line_no;
int test_counter=0;
unsigned int line_counter;


unsigned char cmd_pin, resp_pin, clock_pin, reset_pin;

/***********ISR variables****************/
volatile char w_pointer,r_pointer, w_pointer_copy;
volatile int counter;
volatile int   tempInt1, tempInt2;
volatile int store[80];		//should be 80
volatile int char_count;
//volatile char T0_ovf_flag=0;

/*********variables for the new_record subroutine****************/
volatile char Count_down;
/*********variables for the start_new_code_block subroutine****************/
volatile int write_address;	
volatile signed char line_offset;

#define Prog_enable 0xAC530000
#define Prog_enable_h 0xAC53

#define signature_bit_1 0x30000000
#define signature_bit_1_h 0x3000


#define write_lock_bits 0xACE00000
#define write_lock_bits_h 0xACE0

#define write_fuse_bits_H 0xACA80000
#define write_fuse_bits_H_h 0xACA8

#define write_fuse_bits 0xACA00000
#define write_fuse_bits_h 0xACA0

#define write_extended_fuse_bits 0xACA40000
#define write_extended_fuse_bits_h 0xACA4

#define Chip_erase 0xAC800000
#define Chip_erase_h 0xAC80

/********HW V 1.3 Define target Pin & CC LED definitions START for boot loader**********/
#define PGD_cmd_H PORTB |= cmd_pin
#define PGD_cmd_L PORTB &= ~(cmd_pin)
#define PGD_resp_H PINB & resp_pin
#define PGC_L PORTB &= ~(clock_pin)
#define PGC_H PORTB |= clock_pin
#define Reset_L PORTC &= ~(reset_pin)
#define Reset_H PORTC |= reset_pin

#define boot_target; cmd_pin =  0x08; resp_pin = 0x10; clock_pin =  0x20; reset_pin = 0x08; DDRB |= 0x28; DDRC |= 0x08;

#define LED_2_on; PORTD &= (~(1 << PD7));
#define LED_2_off; PORTD |= (1 << PD7);

#define Atmel_powerup;\
{two_msec_delay;}\
Reset_L;\
PGC_L;\
{two_msec_delay;}\
Reset_H;\
{two_msec_delay;}\
Reset_L;\
{twenty_msec_delay;}

#define Atmel_powerup_and_target_detect;\
Atmel_powerup;\
if((Atmel_config(Prog_enable_h, 0)==0x53) && (Atmel_config(signature_bit_1_h, 0) == 0x1E))target_detected=1;\
else target_detected=0;

