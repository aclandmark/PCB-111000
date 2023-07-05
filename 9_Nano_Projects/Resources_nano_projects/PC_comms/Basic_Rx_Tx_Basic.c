
void String_to_PC_Basic(const char*);
char wait_for_return_key_Basic(void);
void I2C_Tx_8_byte_array(char*);
long I2C_displayToNum(void);

void send_byte_with_Ack(char);
void send_byte_with_Nack(char);
char receive_byte_with_Ack(void);
char receive_byte_with_Nack(void);
void I2C_Tx_initiate_mode(char);



/*********************************************************************/
void setup_PC_comms_Basic (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
UCSR0B = 0;
UBRR0H = UBRROH_N;  									
UBRR0L = UBRR0L_N;  								
UCSR0A = (1 << U2X0);
UCSR0B = (1 << RXEN0) | (1<< TXEN0);
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);}



/*********************************************************************/
char isCharavailable_Basic (int m){int n = 0;
while (!(UCSR0A & (1 << RXC0)))											//Return 1 immediately that a character is received
{n++; wdr();															//No character yet: Increment counter											
if (n>8000) {m--;n = 0;}if (m == 0)return 0;}							//Counter overflows before a character has been received: return zero
return 1;}



/*********************************************************************/
char waitforkeypress_Basic (void){										//Wait here indefinitely but prevent WDTime-out
while (!(UCSR0A & (1 << RXC0)))wdr();	
return UDR0;}		



/*********************************************************************/
char Char_from_PC_Basic(void)											//Return character detected by "isCharavailable()"
{return UDR0;}



/**********************************************************************************************************************************************************************************/
void Char_to_PC_Basic(char data){														
while (!(UCSR0A & (1 << UDRE0)));										//Wait for transmit buffer to be ready to accept new data
UDR0 = data;}															//Load data register with "data" and it will immediately be transmitted



/**********************************************************************************************************************************************************************************/
void newline_Basic(void){String_to_PC_Basic ("\r\n");}



/**********************************************************************************************************************************************************************************/
void String_to_PC_Basic(const char s[]){								//Transmits a sequence (string) of characters and requires the address in program memory of the first one
int i = 0;																//"i" gives the relative address of the next character to be transmitted
while(i < 200){															//Transmits up to 200 characters using "Char_to_PC()" or untill the null (\0) character is detected
if(s[i] == '\0') break;
Char_to_PC_Basic(s[i++]);}}												//Transmit character and increment "i" so that it addresses (points to) the next one.



/**********************************************************************************************************************************************************************************/
char decimal_digit_Basic (char data){											//Returns 1 if data is a character of 0 to 9 inclusive
if (((data > '9') || (data < '0')) )return 0;							//Otherwise return zero
else return 1;}




/**********************************************************************************************************************************************************************************/
void Int_to_PC_Basic (int number)
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



/**********************************************************************************************************************************************************************************/
int Int_from_PC_Basic(char digits[]){
char keypress;
for(int n = 0; n<=7; n++) digits[n] = 0; 

do
{keypress =  waitforkeypress_Basic();} 
while (!(decimal_digit_Basic(keypress)));                                      //(non_decimal_char(keypress));  //Not -,0,1,2,3,4,5,6,7,8 or 9
digits[0] = keypress;
I2C_Tx_8_byte_array(digits);

while(1){
if ((keypress = wait_for_return_key_Basic())  =='\r')break;
if (decimal_digit_Basic (keypress))                                           //012345678or9  :Builds up the number one keypress at a time
{for(int n = 7; n>=1; n--)
digits[n] = digits[n-1];                                                //Shifts display left for each keypress
digits[0] = keypress;
I2C_Tx_8_byte_array(digits);}}
            
return I2C_displayToNum();}



/**********************************************************************************************************************************************************************************/
char wait_for_return_key_Basic(void){                  						//Returns key presses one at a time
char keypress,temp;
while(1){																//Remain in while loop until a character is received
if (isCharavailable_Basic(8)){												//Pauses but returns 1 immediately that a character is received
keypress = Char_from_PC_Basic();												//Skip if no character has been received 
break;}}																//Exit while loop when character has been read
if((keypress == '\r') || (keypress == '\n')){							//Detect \r\n, \r or \n and converts to \r
if (isCharavailable_Basic(1)){temp = Char_from_PC_Basic();}
keypress = '\r';}
return keypress;}



/**********************************************************************************************************************************************************************************/
void Read_Hello_world_string(void){
char receive_byte;

I2C_Tx_initiate_mode('H');
waiting_for_I2C_master;									//Initiate comuninations:master to send string
do{receive_byte = receive_byte_with_Ack();				//print out string as characters are received
if(receive_byte)Char_to_PC_Basic(receive_byte);}
while(receive_byte);									//Detect '\0' character used to terminate a string
receive_byte_with_Nack();								//Receve a second null char at the end of the string
clear_I2C_interrupt;}									//Complete transaction




