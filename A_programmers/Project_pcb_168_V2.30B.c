

/*****ATMEGA programmer V2.30B developed specially for the bootloader pcb project 
********runs on the ATMEGA168 and programs the ATMEGA 328**********
At the user prompt press P to program the OS+bootloader FW or X to program single applications
If programming the EEPROM press 
"E" to program addresses 5 to 0x1FF or
"e" to program addresses 0x200 to 0x3FD or
"D" to delete the entire EEPROM up to 0x3FD (preserves cal bytes)
When verifying the target EEPROM press "r" or "R".

ON_CHIP EEPROM:  	User cal bytes if set are stored in addresses 0x1F7 and 0x1F8.
ATMEGA 168			Default OSCCAL (the value built into the HW) is stored in
					address 0x1F9.
					Text strings required by the family of programmers are stored in 
					addresses 0 to 0x1F6.	

TARGET CHIP EEPROM reservations (ATMEGA 328)
0x0	Top of programmed text space  MSB				
0x1	Top of programmed text space  LSB				
0x2 Number of programmed data items					
0x3 Bottom of application EEPROM space	MSB
0x4 Bottom of application EEPROM space	LSB
Note: the ATMEGA 328 OS and 
bootloaderstrings are all stored in Flash so
it is not essential that its EEPROM be programmed

NOTE One extra line:
This Writes to the EEPROM of the target device to indicate 
that it has just been programmed and ensure that the auto cal routine runs
*/

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "Project_pcb_168_V2.30B.h"
#include "Project_pcb_168_V2.30B_SW_subs.c"
#include "Project_pcb_168_V2.30B_HW_subs.c"
#include "1_Basic_Timer_IO_subs.c"

#define wdr()  __asm__ __volatile__("wdr")
#define Version "Project_pcb_168_V2.30B" 


int main (void){ 
unsigned int target_type = 0, target_type_M;	// temp_config_int=0, temp_config_cmd = 0;
char temp_char;
char  my_cal_value=0, op_code;	// op_code_2, op_code_3, 
unsigned char fuse_H;
int long cal_error;
char OSCCAL_WV;




wdr();
MCUSR &= ~(1<<WDRF);
WDTCSR |= (1 <<WDCE) | (1<< WDE);
WDTCSR = 0;

if ((eeprom_read_byte((uint8_t*)0x1F7) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x1F7) < 0xF0) && (eeprom_read_byte((uint8_t*)0x1F7)\
== eeprom_read_byte((uint8_t*)0x1F8))) {OSCCAL = eeprom_read_byte((uint8_t*)0x1F7);my_cal_value=1;}

/*************This program works with 8MHz clock and EEPROM preserved*********************************/
ADMUX |= (1 << REFS0);			//select internal ADC ref and remove external supply on AREF pin
while (!(PIND & (1 << PD1)));	//wait for DPDT switch
USART_init(0,16);				//use 16 for 57.6K, 	51 for 19.2k or 12 for 76.8k, 25 for 38.4
Set_LED_ports;	
LEDs_off;		
/*****************Power-up and make contact with target****************************/
op_code =0;		
while(1){
do{sendString("P  ");} while((isCharavailable(255) == 0)); //op_code = receiveChar();
switch(receiveChar()){
case 'P': case 'p':	fuse_H = 0xD0; FF_cmd_counter_limit = 500;	op_code =1;break;
case 'X': case 'x':	fuse_H = 0xD7; FF_cmd_counter_limit = 10000;op_code =1;break; 
case 'v': case 'V': sendString (Version);newline();wdt_enable(WDTO_60MS); while(1);break;
case 'R': case 'r':
TWAR = 0x02;
waiting_for_I2C_master;									
OSCCAL_WV = receive_byte_with_Ack();					
cal_error = receive_byte_with_Ack();					
cal_error = (cal_error << 8) + receive_byte_with_Nack();
clear_I2C_interrupt;
sendString("\r\nTarget cal factor  ");
sendHex(16, OSCCAL_WV);sendChar('\t');
sendString("error ");sendHex(10, cal_error); sendString("\r\n");
break;



default:break;}if(op_code)break;}	
		
boot_target;
Atmel_powerup_and_target_detect;        //waits for keypress to continue  Holds target in reset state

newline();sendString("ATMEGA");
switch (target){
case 168: sendString ("168");  PageSZ = 0x40; PAmask = 0x1FC0; FlashSZ=0x2000; EEPROM = 0x200; text_start = 0x5; break;		//no cal bits
case 328: sendString ("328"); PageSZ = 0x40; PAmask = 0x3FC0; FlashSZ=0x4000; EEPROM = 0x200; text_start = 0x5; break;		//For normal strings
default: wdt_enable(WDTO_1S); while(1);break;}
Text_Press_P_R_or_H;

while(1){
op_code = waitforkeypress();
if ((op_code == 'P')||(op_code == 'p'))  {Atmel_powerup; Atmel_config(Prog_enable_h, 0); break;}}

Text_SendEorAOK

while(1){
op_code = waitforkeypress();
switch (op_code){
case 'e':	if(target==328){EEPROM = 0x400; text_start = 0x200;}Prog_EEPROM(); break;		//Place backup strings between 0x200 and 0x400
case 'E':  Prog_EEPROM(); break;//No return from here

case 'D':
sendString("Reset EEPROM! D or AOK");newline();
if(waitforkeypress() == 'D'){
sendString("10 sec wait");
if(target == 168){for (int m = 0; m <= 0x1FF;m++)
{Read_write_mem('I', m, 0xFF);}}
if(target == 328){for (int m = 0; m <= 0x3FD;m++)			//Do not delete Cal data
{Read_write_mem('I', m, 0xFF);}}
sendString(" Done\r\n");}wdt_enable(WDTO_60MS); while(1);break;

case 'p': 
case 'P': sendString ("8MHz");break;
case 'x':
case 'X': wdt_enable(WDTO_60MS); while(1);break;
default: break;} 
if ((op_code == 'P') || (op_code == 'p')) break;}		

temp_char = (Atmel_config(Chip_erase_h, 0)); 
Atmel_config(write_fuse_bits_H_h,fuse_H );
if(fuse_H == 0xD0){Atmel_config(write_extended_fuse_bits_h,0xFD );
Atmel_config(write_fuse_bits_h,0xC2 );}
if(fuse_H == 0xD7){Atmel_config(write_extended_fuse_bits_h,0xFF );
Atmel_config(write_fuse_bits_h,0xE2 );}

Atmel_config(write_lock_bits_h,0xEB );

prog_counter=0; line_length_old=0; line_counter = 0; cmd_counter = 0; 
Flash_flag = 0;  PIC_address = 0;  section_break = 0; orphan = 0; 

newline();Text_Send_Hex
Program_Flash();
Verify_Flash_99();  
Read_write_mem('I', 0x3F9, 1); 				//EXTRA LINE: See notes above

sendString (Version);newline();
Text_ATMEGA168_configBits; newline();  
sendHex(16, Atmel_config(read_extended_fuse_bits_h, 0));
sendHex(16, Atmel_config(read_fuse_bits_H_h,0));	//Target config bits
sendHex(16, Atmel_config(read_fuse_bits_h, 0));
sendHex(16, Atmel_config(read_lock_bits_h, 0));

Text_on_chip_cal_bit
if (my_cal_value==1) sendHex(16, OSCCAL);
sendHex(16, OSCCAL);

newline();
Text_Hex_file_size
sendHex(10,cmd_counter); sendSpace();sendString("d'loaded:"); sendSpace();
sendHex(10,prog_counter); sendString(" in:"); sendSpace();
sendHex(10,read_ops); sendString(" out");newline();

sendString("Power cycle then press -r- to auto cal the target device.\r\n");

wdt_enable(WDTO_60MS); while(1);
return 1;  }









void Prog_EEPROM(void){					
int EEP_address_counter = 0,   Saved_UART_counter_value = 0,   EEP_buffer_pointer, EEP_data_counter = 0;
char temp_char, Down_Load_Flag = 0, down_load_status, op_code;	
unsigned char  EEPROM_buffer[265];
char reservation[4];
int App_reservation;

EEP_address_counter = text_start;
Text_Press_W_or_R;
while(1){if (!(isCharavailable(150))) sendString("?");	else break;}
switch(receiveChar()){

case 'R':
if(((Read_write_mem('O', 0x0, 0)) ==0xFF)  &&  ((Read_write_mem('O', 0x1, 0)) ==0xFF))
{sendString("No Data!\r\n");break;}

EEPROM = ( ( (Read_write_mem('O', 0x3, 0))<<8 ) +  (Read_write_mem('O', 0x4, 0))) ;
Upload_text((Read_write_mem('O', 0x0, 0x0) << 8) + Read_write_mem('O', 0x1, 0x0));	
if(Read_write_mem('O', 0x2, 0x0) && (Read_write_mem('O', 0x2, 0x0)!= 0xFF)){sendString("1,2 or 3");	
op_code =  waitforkeypress();	binUnwantedChars();		
switch (op_code){
case '1':  Upload_data_1 (((Read_write_mem('O', 0x0, 0x0) << 8) + Read_write_mem('O', 0x1, 0x0))  ,Read_write_mem('O', 0x2, 0x0)); break;
case '2':	 Upload_data_2 (((Read_write_mem('O', 0x0, 0x0) << 8) + Read_write_mem('O', 0x1, 0x0))  ,Read_write_mem('O', 0x2, 0x0)); break;	
default:	Upload_data (((Read_write_mem('O', 0x0, 0x0) << 8) + Read_write_mem('O', 0x1, 0x0))  ,Read_write_mem('O', 0x2, 0x0)); break;}}


if ((((Read_write_mem('O', 0x3, 0)) <<8 ) +  (Read_write_mem('O', 0x4, 0)))  == 0xFFFF); else	
{ if((((Read_write_mem('O', 0x3, 0)) <<8 ) +  (Read_write_mem('O', 0x4, 0))) < 0x200)
{sendString("\r\nApp: Start addr's  ");sendHex(16,(((Read_write_mem('O', 0x3, 0)) <<8 ) +  (Read_write_mem('O', 0x4, 0)))) ;}}
break;		

case 'r':
text_start_mem = text_start;
text_start = 0x200;
Upload_text(0x3F7);
text_start = text_start_mem;
break;

case 'W':
case 'w':
Text_int_0_FF; 		
reservation[0] = '0';		//MSB
reservation[2] = '\0';
while(1){
reservation[1] = waitforkeypress();
if (non_numeric_char (reservation[1]))sendChar ('?'); else break;}

if (isCharavailable(150)){while(1){
temp_char = receiveChar();
if (non_numeric_char (temp_char)) {sendChar ('?'); while (!(UCSR0A & (1 << RXC0)));} else break;}  binUnwantedChars();		//NEW LINE
reservation[0] = reservation[1]; 
reservation[1] = temp_char;}
	
App_reservation =  askiX2_to_hex(reservation);

if(App_reservation > 0){
sendString("\r\nApp: Start addr's  ");sendHex(16, (EEPROM - App_reservation));newline();
sendString ("X to escape or AOK\r\n");
if(waitforkeypress() == 'X') {binUnwantedChars();	wdt_enable(WDTO_1S); while(1);}}
binUnwantedChars();		
EEPROM = EEPROM - App_reservation;
temp_char = Read_write_mem('I', 0x3, (EEPROM >> 8)); 	
temp_char = Read_write_mem('I', 0x4, (EEPROM & 0x00FF));

Text_Send_Text_File;
EEP_data_counter = 0;
do {
down_load_status = (Download_text(&EEP_address_counter, &Saved_UART_counter_value,\
&Down_Load_Flag, &EEP_buffer_pointer,  EEPROM_buffer));
if (!(down_load_status)){Down_Load_Flag = 0;  
LED_1_off;	
Text_Send_file_again}}
while (!(down_load_status));
LED_1_off;	

if (Down_Load_Flag == 2){
while (!(Download_data(&EEP_address_counter, &Saved_UART_counter_value,\
&Down_Load_Flag, &EEP_data_counter,  &EEP_buffer_pointer, EEPROM_buffer)))
{LED_1_off;							
Text_Send_file_again;}}
binUnwantedChars ();
LED_1_off;

sendString("AK?");	
binUnwantedChars();		
temp_char = Read_write_mem('I', 0x0, (EEP_address_counter >> 8));  	
temp_char = Read_write_mem('I', 0x1, (EEP_address_counter & 0x00FF));
temp_char = Read_write_mem('I', 0x2, EEP_data_counter);	temp_char = waitforkeypress();
Upload_text(EEP_address_counter);  
if (EEP_data_counter > 0) Upload_data (EEP_address_counter, EEP_data_counter); 
break;  //End of Read case
default: break;}
wdt_enable(WDTO_60MS); while(1);}



char Download_text(int *ptr_EEP_address_counter,\
int *ptr_Saved_UART_counter_value,\
char *ptr_Down_Load_Flag,\
int *ptr_EEP_buffer_pointer, unsigned char EEPROM_buffer[]){	
int UART_counter = 0;
char temp_char, text_char;

*ptr_EEP_buffer_pointer = 0;
text_char = waitforkeypress();	UART_counter++;											
while(1){text_char = Rx_data(); UART_counter++; if (text_char == '"') break;}	
	
while(!(*ptr_Down_Load_Flag)){//loop1
text_char = Rx_data(); UART_counter++; 
if (text_char != '"')	{//loop2
if (UART_counter > *ptr_Saved_UART_counter_value){//loop 3  Only save text if downloaded for the first time

switch (text_char){	
case '\n':
case '\r':
if (   EEPROM_buffer[ ((*ptr_EEP_buffer_pointer)-1)] != '\0')
{text_char = '\0';} else {((*ptr_EEP_buffer_pointer)--); text_char = '\0';}
default: break;}

EEPROM_buffer[((*ptr_EEP_buffer_pointer)++)] = text_char;
if 	((*ptr_EEP_buffer_pointer >= RBL) && (text_char != '\r') && (text_char != '\0')) *ptr_Down_Load_Flag = 1;	}//loop 3																														
}//loop2
else break;}   	//loop1	Exit Loop 1 when the second " is received

if (*ptr_Down_Load_Flag == 1){binUnwantedChars();	 
*ptr_Saved_UART_counter_value = UART_counter; 
EEPROM_buffer[(*ptr_EEP_buffer_pointer)++] = '\0';

LED_1_on;
{int n=0;
while (((*ptr_EEP_address_counter) +n) < EEPROM){
temp_char = Read_write_mem('I', ((*ptr_EEP_address_counter) + n),EEPROM_buffer[n]); 
n++;
if (n==(*ptr_EEP_buffer_pointer))break;}
if (((*ptr_EEP_address_counter) +n) == EEPROM){*ptr_Down_Load_Flag = 3; // EEPROM overflow occurs: Save text and exit
temp_char = Read_write_mem('I', (EEPROM-1), '\0'); 
*ptr_EEP_address_counter = EEPROM;  
return 1;}
(*ptr_EEP_address_counter) += n-1; 
*ptr_EEP_buffer_pointer = 0;}  
return 0;}

else // " char encountered to terminate text input
LED_1_on;							
EEPROM_buffer[((*ptr_EEP_buffer_pointer)++)] = '\0';
*ptr_Saved_UART_counter_value = UART_counter;
(*ptr_EEP_address_counter) += (*ptr_EEP_buffer_pointer); //addres of first number 
if((*ptr_EEP_address_counter) >= EEPROM){*ptr_Down_Load_Flag = 3; //EEPROM overflow occurs: Save text and exit
{int n=0;\
while (((*ptr_EEP_address_counter) - (*ptr_EEP_buffer_pointer) +n) < (EEPROM-1)){\
temp_char = Read_write_mem('I', ((*ptr_EEP_address_counter) - (*ptr_EEP_buffer_pointer) + n),EEPROM_buffer[n]);
n++;}}
temp_char = Read_write_mem('I', (EEPROM-1), '\0'); 
*ptr_EEP_address_counter = EEPROM; return 1;}
*ptr_Down_Load_Flag = 2; 
return 1;}



char Download_data(int *ptr_EEP_address_counter,int *ptr_Saved_UART_counter_value,\
char *ptr_Down_Load_Flag, int *ptr_EEP_data_counter,\
int *ptr_EEP_buffer_pointer, unsigned char EEPROM_buffer[]){

int data_int=0, UART_counter=0, ADDR_last_string=0, next_data_address=0;
char data_text,  temp_char;

if(*ptr_EEP_buffer_pointer == 0) {next_data_address = *ptr_EEP_address_counter + *ptr_EEP_data_counter + *ptr_EEP_data_counter;
data_text = waitforkeypress();	UART_counter++;	
while 	(UART_counter < *ptr_Saved_UART_counter_value) {data_text = Rx_data(); UART_counter++; }}

else {UART_counter = *ptr_Saved_UART_counter_value; ADDR_last_string = *ptr_EEP_address_counter - *ptr_EEP_buffer_pointer;}
	
while((*ptr_Down_Load_Flag ==0) || (*ptr_Down_Load_Flag ==2) ){ //LOOP 1
data_text = Rx_data(); UART_counter++; 
if(!data_text)break;
switch (data_text){
case '-': case '+': case '0': case '1': case '2':  case '3': case '4': case '5':  case '6': case '7': case '8': case '9':  
data_int = decimal_conversion(data_text, &UART_counter); 
EEPROM_buffer[(*ptr_EEP_buffer_pointer)++] = (data_int >> 8);
EEPROM_buffer[(*ptr_EEP_buffer_pointer)++] = (data_int & 0x00FF);
(*ptr_EEP_data_counter)++;
if((*ptr_EEP_buffer_pointer >= RBL)){
if(*ptr_Down_Load_Flag == 2) *ptr_Down_Load_Flag = 3;
if(*ptr_Down_Load_Flag == 0) *ptr_Down_Load_Flag = 4;}break;

case '$':  data_int = Hex_conversion(data_text, &UART_counter);
EEPROM_buffer[(*ptr_EEP_buffer_pointer)++] = (data_int >> 8);
EEPROM_buffer[(*ptr_EEP_buffer_pointer)++] = (data_int & 0x00FF);
(*ptr_EEP_data_counter)++;
if((*ptr_EEP_buffer_pointer >= RBL)){								
if(*ptr_Down_Load_Flag == 2) *ptr_Down_Load_Flag = 3;
if(*ptr_Down_Load_Flag == 0) *ptr_Down_Load_Flag = 4;}break;

default: break;}
} //LOOP 1

binUnwantedChars();	

switch (*ptr_Down_Load_Flag){
case 2:	//RAM buffer only partially full:  Text and numbers or just text
LED_1_on;
{int n=0;\
while ((ADDR_last_string +n) < (EEPROM)){\
temp_char = Read_write_mem('I', (ADDR_last_string + n),EEPROM_buffer[n]);
n++;\
if (n==(*ptr_EEP_buffer_pointer))break;}} 
return 1; break;

case 3: //RAM buffer completely full:  Text and numbers
LED_1_on;
{int n=0;\
while ((ADDR_last_string +n) < (EEPROM)){\
temp_char = Read_write_mem('I', (ADDR_last_string + n),EEPROM_buffer[n]); 
n++;\
if (n==(*ptr_EEP_buffer_pointer))break;}
if((ADDR_last_string +n) == (EEPROM)) {
LED_1_off;	
return 1;} } //EEPROM overflow occurs: Save text and exit
(*ptr_EEP_buffer_pointer) = 0;  *ptr_Down_Load_Flag = 0; *ptr_Saved_UART_counter_value = UART_counter;
return 0; break;

case 0:	//RAM buffer only partially full:  End of file Numbers only
LED_1_on;
{int n=0;\
while ((next_data_address +n) < (EEPROM)){\
temp_char = Read_write_mem('I', (next_data_address + n),EEPROM_buffer[n]); 
n++;\
if (n==(*ptr_EEP_buffer_pointer))break;}}
(*ptr_EEP_buffer_pointer) = 0;
return 1;

case 4: //RAM buffer completely full:  Numbers only
LED_1_on;
{int n=0;\
while ((next_data_address +n) < (EEPROM)){\
temp_char = Read_write_mem('I', (next_data_address + n),EEPROM_buffer[n]);  
n++;\
if (n==(*ptr_EEP_buffer_pointer))break;}
if((next_data_address +n) == (EEPROM)){   
return 1;}}  //EEPROM overflow occurs: Save text and exit
(*ptr_EEP_buffer_pointer) = 0; *ptr_Down_Load_Flag = 0; *ptr_Saved_UART_counter_value = UART_counter;
return 0;}
return 0;}


int decimal_conversion (char data_text, int *ptr_UART_counter){
int char_counter=0;
char data_string[7], num_char;
data_string[char_counter++] = data_text;
while(1){
num_char = Rx_data(); (*ptr_UART_counter)++;
if (num_char == ' ') break;
if (num_char == '\r') break;
if (num_char == '\n') break;
if (num_char == '\0') break;
if (num_char == '\t') break;
else data_string[char_counter++] = num_char;}
data_string[char_counter++] = '\0';
return my_atoi_with_sign (data_string);}

int Hex_conversion (char data_text, int *ptr_UART_counter){
int char_counter=0;
char data_string[7], num_char;
while(1){
num_char = Rx_data(); (*ptr_UART_counter)++;
if (num_char == ' ') break;
if (num_char == '\r') break;
if (num_char == '\n') break;
if (num_char == '\0') break;
if (num_char == '\t') break;
else data_string[char_counter++] = num_char;}
data_string[char_counter++] = '\0';
return askiX4_to_hex (data_string);}


void Upload_text(int EEP_address_counter){ 
char string_char;
int EEP_mem_counter = 0;
EEP_mem_counter = text_start;
while (EEP_mem_counter < EEP_address_counter){newline();sendHex (16, EEP_mem_counter); sendString ("    ");
while(1){string_char = Read_write_mem('O',(EEP_mem_counter++),0);
if ((string_char == '\0') || (EEP_mem_counter == EEP_address_counter))break;
sendChar (string_char);}}}

void Upload_data(int address_first_data_item, int EEP_data_counter){
char output_counter=0;
int data_item = 0 ;
if(!(EEP_data_counter))return;
newline();sendHex (16, (address_first_data_item) );
{int n=0; while((address_first_data_item+n+1) < EEPROM){
data_item = (  (Read_write_mem('O', address_first_data_item+n, 0x0) << 8) + Read_write_mem('O',(address_first_data_item+n+1), 0x0)  );
if  (output_counter == 4){newline();output_counter  = 0;sendHex (16, (address_first_data_item+n) );}
sendChar('\t'); sendHex (16, data_item ); sendChar ('\t'); sendsignedHex (data_item );output_counter++; n+=2; if(n>2*(EEP_data_counter-1)) break; }}}

void Upload_data_1(int address_first_data_item, int EEP_data_counter){
char output_counter=0;
if(!(EEP_data_counter))return;
int data_item = 0 ;
newline();sendHex (16, (address_first_data_item) );
{int n=0; while((address_first_data_item+n+1) < EEPROM){
data_item = (  (Read_write_mem('O', address_first_data_item+n, 0x0) << 8) + Read_write_mem('O',(address_first_data_item+n+1), 0x0)  );
if  (output_counter == 8){newline();output_counter  = 0;sendHex (16, (address_first_data_item+n) );}
sendChar('\t');  sendsignedHex (data_item );output_counter++; n+=2; if(n>2*(EEP_data_counter-1)) break; }}}

void Upload_data_2(int address_first_data_item, int EEP_data_counter){
char output_counter=0;
int data_item = 0 ;
if(!(EEP_data_counter))return;
newline();sendHex (16, (address_first_data_item) );
{int n=0; while((address_first_data_item+n+1) < EEPROM){
data_item = (  (Read_write_mem('O', address_first_data_item+n, 0x0) << 8) + Read_write_mem('O',(address_first_data_item+n+1), 0x0)  );
if  (output_counter == 8){newline();output_counter  = 0;sendHex (16, (address_first_data_item+n) );}
sendChar('\t'); sendHex (16, data_item ); output_counter++; n+=2; if(n>2*(EEP_data_counter-1)) break; }}}




