
void I2C_Tx_2_integers(unsigned int, unsigned int);
void I2C_Tx_any_segment(char, char);
void I2C_Tx_any_segment_clear_all(void);
void I2C_Tx_8_byte_array(char*);
void I2C_Tx_8_byte_array_plus_mode(char, char*);
void I2C_Tx_BWops(char*);
void I2C_Tx_OS_timer(char, char*);
void I2C_Tx_Clock_command(char, char);
void I2C_Tx_display_char (char, char);
void I2C_Tx_2_chars(char, char, char);
void I2C_initiate_10mS_ref(void);
void I2C_Tx_display(void);
void I2C_Tx_LED_dimmer(void);
void I2C_Set_Tx_Rx_speed(char);

char waitforkeypress(void);
void String_to_PC(char*);
void newline(void);

void I2C_Tx(char, char, char*);
void send_byte_with_Ack(char);
void send_byte_with_Nack(char);
char receive_byte_with_Ack(void);
char receive_byte_with_Nack(void);

/************************************************************************/
void I2C_Tx_2_integers(unsigned int s1, unsigned int s2){			
char num_bytes=4; char mode=4; char s[4];
for (int m = 0;  m < 4; m++){
switch (m){
case 0: s[m] = s1; break; 											//Send S1 lower byte
case 1: s[m] = s1 >> 8; break; 									//Send S1 higher byte									
case 2: s[m] = s2; 	break;											//Send S1 lower byte
case 3: s[m] = s2 >> 8; break;}}									//Send S1 higher byte
I2C_Tx(num_bytes,mode, s);}


/************************************************************************/
void I2C_Tx_any_segment(char segment, char digit_num){
char s[2]; char num_bytes=2; char mode = 'a';
s[0] = segment;
s[1] = digit_num; 
I2C_Tx(num_bytes,mode, s);}


/************************************************************************/
void I2C_Tx_any_segment_clear_all(void){
char segment = 'a'; char digit_num = 0;
char s[2]; char num_bytes=2; char mode = 'A';
s[0] = segment;
s[1] = digit_num; 
I2C_Tx(num_bytes,mode, s);}



/************************************************************************/
void I2C_Tx_8_byte_array(char s[]){									//was I2C_Tx_1
char num_bytes=8; char mode=1;
I2C_Tx(num_bytes,mode, s);}

/************************************************************************/
void I2C_Tx_8_byte_array_plus_mode(char mode, char s[]){			//was I2C_Tx_1('F', digits)
char num_bytes=8;
I2C_Tx(num_bytes,mode, s);}


/************************************************************************/
void I2C_Tx_BWops(char s[]){										//Bit wise operations
char num_bytes=8;
char mode = 3;
I2C_Tx(num_bytes,mode, s);}


/************************************************************************/
void I2C_Tx_OS_timer(char mode, char s[]){								
char num_bytes=8;
I2C_Tx(num_bytes,mode, s);}

/************************************************************************/
void I2C_Tx_Clock_command(char timer_mode, char command){			
char s[2]; char num_bytes=1;
s[0] = command;
I2C_Tx(num_bytes, timer_mode, s);}

/************************************************************************/
void I2C_Tx_display_char (char single_char, char sign_bit){
char s[2]; char num_bytes=2; char mode = 5;
s[0] = single_char;
s[1] = sign_bit;
I2C_Tx(num_bytes, mode, s);}


/************************************************************************/
void I2C_Tx_2_chars(char mode, char letter, char digit_num){		//was I2C_Tx_4
char s[2]; char num_bytes=2;
for (int m = 0;  m < 2; m++){
switch(m){
case 0: s[m] = letter;break;
case 1: s[m] = digit_num; break;}}
I2C_Tx(num_bytes,mode, s);}



/************************************************************************/
void I2C_initiate_10mS_ref(void){
char num_bytes=0;
char mode = 'G';

waiting_for_I2C_master;
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);

TWCR = (1 << TWINT) | (1 << TWEN);		//clear interrupt and leave I2C slave active
TWCR |= (1 << TWEA) | (1 << TWIE);} 	//Slave mode with interrupt and Enable Acknowledge


/************************************************************************/
void I2C_Tx_display(void){
char keypress;
char s[2]; char num_bytes=1, mode = 'Q';
s[1] = 0;
String_to_PC("\r\nSend 0 to 7 to adjust interval x when done\r\n");
while(1){
keypress = waitforkeypress();
if (keypress == 'x')break;
else
{s[0] = keypress - '0';
I2C_Tx(num_bytes,mode, s);}
}

s[0] = 0;
I2C_Tx(num_bytes,mode, s);
String_to_PC("AK?");
waitforkeypress();
}


/************************************************************************/
void I2C_Tx_LED_dimmer(void){
char Dimmer_control;

if((PINB & 0x40)^0x40)Dimmer_control = 1;			//if Switch_2 down
else Dimmer_control = 0;							//Normal operation

I2C_Tx(1, 'W', &Dimmer_control);}	




