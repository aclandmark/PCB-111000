/*********************
Projects version 9
I2C program version 4

V9.9 Prevents users applying impractical user-cal values to the ATMEGA 328
Performs scan of the ATMEGA 328 clock cal factor
Following POR check cal factor and perform auto clock recovery if necassary

*EEPROM reservations
0x3FF	user cal if set
0x3FE	user cal if set
0x3FD	Default cal supplied by Atmel
0x3FC	If 1: press 'x' diagnostic mode else press 'r' normal mode
0x3FB	If 0 use multiplexter (T0) period of 4ms else use period of 2mS (std)


***********************/


# include "I2C_V12_0_Header_file_CC.h"
void I2C_master_transmit(char);
char I2C_master_receive(char);

int main (void){
char clock_flag=0, stop_watch_flag, PIC_cmd, Ten_mS_tick_counter = 0, stop_watch_mode;
char timer_store [95], overflow=0;
signed char digit_num=0;
int timer_record, timer_pointer, record_counter, letter=0, product;
unsigned long RHSofDP = 0;
char Op = 0;
char result[4];
signed char expnt_result=0;
int accumlator=0, acc_input,interim_result;
char payload_size, sign;
char I2C_data[10], display_backup[9];		
long L_number;
signed char expnt;
signed char exponent_BKP[2];
char keypres_counter_old=0, keypres_counter, SW3_keypress, sw2_keypress, Return;
int RN=0;
int text_max, eeprom_ptr;
char string_counter, string_no, sign_bit;
long cal_error, error_mag;
char OSCCAL_DV, OSCCAL_UC, OSCCAL_WV;
char test_num, test_digit;

char *SW_Version = "OS: I2C_V12_0_CC\r\n";
char *SW_info = "SW information: Operating system I2C_V12_0_CC\
  Projects V1_4 and Bootloader V4_22_CC. (zero_SUT & auto clock recovery) \
  External programmer V2_29A\r\n";
	

/****Watchdog initiated for mode G only (user clock/stop watch with
external 10mS crystal interrupt).*********/

/***********Brown-out:  This is set (using config bits only) for 2.9V*************/

if(MCUSR & (1 << BORF)){				//Detect brown-out
MCUSR &= (~(1 << BORF));}				//Reset brown-out flag 

ADMUX |= (1 << REFS0);					//select internal ADC ref and remove external supply on AREF pin
setup_watchdog;	
set_digit_drivers;
clear_digits;
clear_display;
exponent_BKP[0]=0; exponent_BKP[1]=0;
expnt=0;
sei();					
OSCCAL_WV = OSCCAL;		
OSCCAL_DV = eeprom_read_byte((uint8_t*)0x3FD);		
TWBR = 32;										//gives 100KHz I2C clock for TWSR 

/****Start Real time clock with 1 second tick****/
ASSR = (1 << AS2); 								//initialise T2 for crystal
timer_2_counter=0;								//Initialsise timer_2_counter to zero

/*************If POR check OSC Calibration and recal if necessary*****************************/

if((MCUSR & (1 << PORF))){									//Only run clock check following a POR
		Timer_T1_sub(T1_delay_500ms);
		MCUSR &= (~(1 << PORF));
		mode = 'T';											//Required by ISR(TIMER1_OVF_vect) & (TIMER2_OVF_vect)
		TIMSK2 |= (1 << TOIE2);								//Set Timer 2: interrupt on overflow
		TIMSK1 |= (1 << TOIE1);
		initialise_timers_for_cal_error();
		start_timers_for_cal_error();
		cal_error = compute_error(0);
		cal_error = compute_error(0);						//Xtal warm up time
		cal_error = compute_error(0);
	if(cal_error > 750){
	mode = 'U';
		initialise_timers_for_cal_error();
		start_timers_for_cal_error();
		for(int m = 0x10; m <= 0xF0; m++){OSCCAL = m ;cal_error = emergency_restore_operation();	//low accurracy but quick to scan up to 160 cal factors
		if (cal_error >= 0) error_mag = cal_error; else error_mag = cal_error * (-1);
		if ((error_mag < 750) && (m > 0x14))break;}			//Warmup time needed for T1/T2 synchronisation???
		mode = 'O';
		OSCCAL_test = OSCCAL;
	initialise_timers_for_cal_error();  				
	start_timers_for_cal_error();
	cal_error = calibrate_quick_cal(OSCCAL_test);				//Full accuracy but only tests several cal factors
	if(cal_error > 500)cal_error = calibrate_quick_cal(OSCCAL + 4);
	if(cal_error > 500)cal_error = calibrate_quick_cal(OSCCAL - 2);	
	if(cal_error > 500) OSCCAL = OSCCAL_test;		
	OSCCAL_WV = OSCCAL;
	eeprom_write_byte((uint8_t*)0x3FE, OSCCAL_WV); 
	eeprom_write_byte((uint8_t*)0x3FF, OSCCAL_WV);}
	initialise_timers_for_cal_error();  						//dissable timers
	TIMSK2 &= (~(1 << TOIE2));									//Dissable Timer 2: interrupt on overflow
	TIMSK1 &= (~(1 << TOIE1));}
			
/********Start multiplexer**************************/		
		
	T0_interupt_cnt = 0;	
	TIMSK0 |= (1 << TOIE0);										//T0 interrupt enabled
	MUX_cntl = 0;
	if(eeprom_read_byte((uint8_t*)0x3FB) == 0xFF)
	{timer_T0_sub_with_interrupt(T0_delay_2ms);}				//Normal Display brightness
	else {timer_T0_sub_with_interrupt(T0_delay_500us);}

/***************Repetitively poll the PIC until it responds*******************/
while(1){

while((mode == 'G')||(mode == 'g')\
||(mode == 'I')||(mode == 'i')\
 || (mode == 'P')||(mode == 'p'));	
/********Code parks here following WDT, POR and Brown-out***************/
while(1){
TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);			//send a start condition  (this identical to a "repeated start")
while (!(TWCR & (1 << TWINT)));							//Wait for TWINT flag 
TWDR = 0x03;												//Address of slave (master read operation)  SLA + R
TWCR = (1 << TWINT) | (1 << TWEN);							//Clear TWINT bit to start transmission of address
while (!(TWCR & (1 << TWINT)));							//Wait for TWINT flag 
if (TWSR == 0x40)break;										//SLA + R successfully transmitted ACK recveived 
if(clock_flag==1){refresh_clock_display;}}

clock_flag=0; 

payload_size = I2C_master_receive(1);

if(payload_size)mode = I2C_master_receive(1);
else mode = I2C_master_receive(0);

if (mode != 'B') display_mask=0;	//dissable flashing digits

if(payload_size){															//payload is zero for mode 'G'

for (int m = 0; m < payload_size; m++){								//payload_size = Num digits to be downloaded
	if (m == (payload_size-1))I2C_data[m] = I2C_master_receive(0);
	else I2C_data[m] = I2C_master_receive(1);}
	TWCR =  (1 << TWINT ) | (1 << TWEN ) | (1 << TWSTO );}				//Send stop bit	

switch(mode){
case 'a': case 'b': case 'B':case 'd': case 'D': case 'g': 
case 'G': case 'j': case 'J': case 'w': case 'W': break;
default: {for(int m = 0; m < 8; m++)display_buf[m] = 0;}break;}			//case B added

switch (mode){
case 'A':														//Called by subroutines I2C_Tx_any_segment(char segment, char digit_num)
case 'a':														//and I2C_Tx_any_segment_clear_all(void)
	letter = I2C_data[0];
	digit_num = I2C_data[1];
	switch (letter){
	case 'A': case 'a': letter = 0; break;
	case 'B': case 'b': letter = 3; break;
	case 'C': case 'c': letter = 4; break;
	case 'D': case 'd': letter = 2; break;
	case 'E': case 'e': letter = 5; break;
	case 'F': case 'f': letter = 6; break;
	case 'G': case 'g': letter = 1; break;
	default: letter = 8; break;	}
	if(letter == 8)break;
	if((digit_num >= 8) || (digit_num < 0))break;
	
	if (mode == 'A'){clear_digits;clear_display;}
	if (mode == 'a'){if(display_buf[letter] & (1 << digit_num))display_buf[letter] &= (~(1 << digit_num));
						else display_buf[letter] |= (1 << digit_num);} //toggles a segment
	break;
	
		
case 'B':									//Called by subroutine I2C_Tx_2URNs_from_IO(char s[]). Uses I/O to enter two real numbers 
case 'b':									//Used mode C to do arithmetic on the numbers
											//Generates numbers of type 0.1234 With an exponent
//Test for new sum and initialise variables
if ((!(I2C_data[1]))&&(!(I2C_data[2]))&&(!(I2C_data[3]))&&(!(I2C_data[4]))\
&&(!(I2C_data[5]))&&(!(I2C_data[6]))&&(!(I2C_data[7]))){
if(I2C_data[0]=='A'){exponent[0]=0;exponent[1]=0; display_mask = 0; RN=0;keypres_counter_old=0;
exponent_BKP[0]=0; exponent_BKP[1]=0; overflow=0; break;}}

//Initialise exponent and display mask
if ((I2C_data[0] =='P') & (!(exponent[RN]))){exponent[RN]=-1;for (int m = 0; m < 8; m++)
	display_buf[m] = I2C_data[m];break;}

//Detect SW3 keypresses
{int m=0;while (I2C_data[7-m]==0)m++; keypres_counter=8-m;}  //count sw3 keypresses
if ((keypres_counter != keypres_counter_old) || ((keypres_counter == keypres_counter_old) && (I2C_data[0] =='P')))
SW3_keypress=1; else SW3_keypress=0;  
keypres_counter_old=keypres_counter;

//Detect SW2 keypresses
sw2_keypress=0;
for(int m=0; m<8; m++){if(display_buf[m] == I2C_data[m])sw2_keypress++;} 
if ((sw2_keypress == 8) && (I2C_data[0] !='P')) Return = 1; else Return = 0; 

//Check for overflow condition
if((display_buf[7]) && ((I2C_data[7] != display_buf[7])
|| (I2C_data[6] != display_buf[6])|| (I2C_data[5] != display_buf[5])
|| (I2C_data[4] != display_buf[4])|| (I2C_data[3] != display_buf[3])
|| (I2C_data[2] != display_buf[2]))){overflow++;} 
if(overflow)Return = 1;


//Update display and exponent during entry of a single number
if((!(Return)) &&(!(overflow))){
for (int m = 0; m < 8; m++){display_buf[m] = I2C_data[m];}
if(SW3_keypress){
if((!(RN)) && exponent[0]){display_mask = (display_mask <<1) + 1; exponent[0]--;
	for(int p = 0; p < 8; p++)strobe[p] = 0;}		//synchronise strobe
if(RN){
if(!(exponent[1])){display_mask = (display_mask <<1);for(int p = 0; p < 8; p++)strobe[p] = 0;}
if (exponent[1]<-1){display_mask =  (display_mask <<1) + 1; exponent[1]--; for(int p = 0; p < 8; p++)strobe[p] = 0;}
if(exponent[1]==-1){display_mask +=1;for(int p = 0; p < 8; p++)strobe[p] = 0;exponent[1]--;}}}}
	
//Enter second number when complete
if((Return)){				//sw2 keypress
if(RN){					//Second sw2 keypress
for(int m = 0; m<=7; m++){I2C_data[m]=display_buf[m];display_buf[m] = 0;}
TIMSK1 |= (1<<TOIE1);
timer_T1_sub_with_interrupt(T1_delay_500ms);while(!(T1_ovf_flag)); T1_ovf_flag=0;		//Flash display
TIMSK1 &= (~(1<<TOIE1));						///////////////////////////////////////! changed to ~
for(int m = 0; m<=7; m++)display_buf[m] = I2C_data[m];

if((!(overflow))||(overflow==1)){
if (exponent[0])exponent[0]++;
if (exponent[1])exponent[1]++;}

//calculate Number[0] and [1]
{int n,q;
n=0; while(display_buf[n])n++;	q=n;						//determine length of number string
number[1] = 0;
while(n){
number[1] = number[1] * 10 + display_buf[n-1] - '0'; n--;} 			//convert the numerical string to a long number (4 bytes)

n=q+1;while((display_buf[n]) && (n<=7))n++;
number[0] = 0;
while(n-q-1){
number[0] = number[0] * 10 + display_buf[n-1] - '0'; n--;}}

//backup the display, numbers and exponents
for (int n = 0; n<=7; n++)display_backup[n] = display_buf[n]; display_backup[8] = display_mask;
exponent_BKP[0] = exponent[0]; exponent_BKP[1] = exponent[1];number[4] = number[0]; number[5] = number[1];
break;}

//Enter first number when complete
if(!(RN)){		//First sw2 keypress
	display_mask = (display_mask << 2);	for(int p = 0; p < 8; p++)strobe[p] = 0;		//synchronise strobe
	{for (int n = 0; n < 7; n++){display_buf[7-n] = display_buf[6-n];}} 
	display_buf[0] = '\0';				//shift display left and blank digit 0
	{for (int n = 0; n < 7; n++){display_buf[7-n] = display_buf[6-n];}}
	display_buf[0] = '0';				//shift display left and initialise digit 0
	keypres_counter_old = keypres_counter+2;
	RN=1;}}break;
	
case 'C':																//Called by subroutine I2C_Tx_Uarithmetic_OP(char Op)												
case 'c':
																		//Used with mode 'B' which provuides two numbers in scientific form
Op = I2C_data[0];														//Op is either 'M' for multiply or 'D' for divide
exponent[0] = exponent_BKP[0];	exponent[1] = exponent_BKP[1];	
number[0] = number[4]; number[1] = number[5];		//restore exponents
	switch(Op){
	case 'M': RHSofDP = multiply_real(number, exponent); break;	
	case 'D': RHSofDP = divide_real (number, exponent); break;
	case 'R':  
	mode_C_ptr=0;TIMSK1 |= (1<<TOIE1);timer_T1_sub_with_interrupt(T1_delay_500ms);
	exponent[1] = number[1]; RHSofDP = any_root(number, exponent);
	TIMSK1 &= (~(1<<TOIE1)); TCCR1B = 0;				//////////////////////! changed to ~																														////	
	for(int m = 0; m<=7; m++)display_buf[m]=0;break;
	case 'P': exponent[1] = number[1]; RHSofDP = any_power(number,exponent);break;
	case 'A': for(int m = 0; m<=7; m++)display_buf[m] = display_backup[m]; 
	display_mask = display_backup[8];
	for(int p = 0; p < 8; p++)strobe[p] = 0; break;}
	if(Op == 'A')break;
	Disp_Long(RHSofDP,exponent[2]);expnt_result = exponent[2];
	for(int m = 0; m <= 3; m++){result[3-m] = RHSofDP; RHSofDP = RHSofDP >> 8;}
	
	
	/******Return result to AT168 or PIC**********/
	Initialise_I2C_master_write;									//SLA + W successfully transmitted ACK recveived

	for(int m = 0; m<=3; m++)
	{I2C_master_transmit(result[m]);}
	I2C_master_transmit(expnt_result);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);			//Transmit stop NOT actually needed
	break;

case 'D':														//Called by subroutines I2C_Tx_Initiate_tables(char s[])
case 'd':														//and I2C_Tx_Inc_tables(char s[])
																//and I2C_Tx_dec_tables(char s[])	
if (!(I2C_data[7])){											
for (int m = 0; m < 8; m++){									//8 digits to be downloaded
display_buf[m] = I2C_data[m];}
	
//Initiate tables display
for (int n = 0; n < 7; n++){display_buf[7-n] = display_buf[6-n];} display_buf[0] = '\0';//shift display left
for (int n = 0; n < 7; n++){display_buf[7-n] = display_buf[6-n];} display_buf[0] = '0';//shift display left
display_buf[0] = 1 + '0';										//Initialise digits[0] & [1]
display_buf[1]='0';

	for(int m = 0; m<= 3; m++){display_buf[7-m] = display_buf[3-m];display_buf[3-m]=0;}
	if(!(display_buf[7])) display_buf[7] = '0';
	if(!(display_buf[5])) display_buf[5] = '0';}
	
else{

switch (I2C_data[7]){
case '1':
if ((display_buf[4] == '9') && (display_buf[5] =='9'))
{display_buf[4]='0';display_buf[5]='0';}
if(display_buf[4]== '9')
{display_buf[4]='0'; display_buf[5]++;}
else display_buf[4]++; break;

case '2': 
	if ((display_buf[4]=='1') && (display_buf[5]=='0')) break; 
	if (display_buf[4]=='0'){display_buf[5]--; display_buf[4]='9';}
	else display_buf[4]--;break;	  }}
	
	product = (((display_buf[7]-'0') * 10) + display_buf[6]-'0') * (((display_buf[5]-'0') * 10) + display_buf[4]-'0');
	for(int m = 0; m<=3; m++)display_buf[m]=0;
	{int m=0;do(display_buf[m++] = product%10 + '0'); while ((product/=10) > 0);}
	display_mask = 0b00110000;
	for(int p = 0; p < 8; p++)strobe[p] = 0;					//synchronise strobe
	break;	
	
case 'E':														//Called by subroutine I2C_Tx_accumulator_op(char s[],char Op)
case 'e':														//for add, subtract and clear modes
	for (int m = 0; m < 9; m++){								//9 digits to be downloaded
	if (m == 8)Op = I2C_data[m];
	else display_buf[m] = I2C_data[m];}										//Store digits for display
	{int m = 0; while((display_buf[m]) && (m < 3))m++; 
	acc_input = 0;
	while(m){
	acc_input = acc_input*10 + (display_buf[m-1] - '0'); m--;}
	if (Op == 'A')accumlator += acc_input;
	if (Op == 'S')accumlator -= acc_input;
	if (Op == 'C')accumlator = 0;
	
	interim_result = accumlator;
		
	if((accumlator) < 0)accumlator = -accumlator;
	m = 0;
	do{display_buf[m+3] = accumlator%10 + '0';m++;} while((accumlator/=10) > 0);
	if(interim_result < 0)display_buf[m+3] = '-';
	display_mask = 0b00000111;
	for(int p = 0; p < 8; p++)strobe[p] = 0;
	accumlator = interim_result;}break;	

case 'F':														//Called by subroutine I2C_Tx_accumulator_op(char s[],char Op)
case 'f':														//for data input modes
for (int m = 0; m < 3; m++){									//8 digits to be downloaded
	display_buf[m] = I2C_data[m];}								//Store digits for display
	interim_result = accumlator;
	if((accumlator) < 0)accumlator = -accumlator;
	{int m = 0;
	do{display_buf[m+3] = accumlator%10 + '0';m++;} while((accumlator/=10) > 0);
	if(interim_result < 0)display_buf[m+3] = '-';
	display_mask = 0b00000111;
	for(int p = 0; p < 8; p++)strobe[p] = 0;
	accumlator = interim_result;}	break;
	
case 'G':															//Called by subroutine I2C_initiate_10mS_ref(void)
case 'g':
	TCCR2B = 2;//= 6;												//Set clock to 4096Hz and start it
	TIMSK2 |= ((1 << OCIE2A) | (1 << TOV2));						//Enable timer clock interrupt
	wdt_enable(WDTO_120MS); break;									//Start WDT on 15mS
	
case 'H':															//Called by subroutine I2C_Tx_long(long)
case 'h':															//Receives and displays a long number
L_number = 0;
for (int m = 0; m < 4; m++){
L_number += I2C_data[m];
if(m<3) L_number = L_number << 8;}
Display_num(L_number);break;

case 'I':															//Called by subroutine I2C_Tx_float_num(long, char)
case 'i':															//Recieves scientific number and then scrolls it across the display
L_number = 0;
for (int m = 0; m < 4; m++){
L_number += I2C_data[m];
if(m<3) L_number = L_number << 8;}
expnt = I2C_data[4];							
float_to_askii(L_number, expnt,Sc_Num_string);
TCCR2B = 2;//= 6;												//Set clock to 4096Hz and start it
TIMSK2 |= (1 << TOIE2);	
break;

case 'J':															//Called by subroutine I2C_displayToLong(void)
case 'j':															//Converts display to long and transmits it to the slave
L_number=0;
sign = '+';
{int m = 7; while (!(display_buf[m]))m--;
if (display_buf[m] == '-') {sign = '-'; m--;}
while(m>=0){L_number = L_number*10 + display_buf[m] - '0'; m--;}}
if (sign == '-')L_number = L_number * (-1);
for(int m = 0; m <= 3; m++){result[3-m] = L_number; L_number = L_number >> 8;}
long_to_slave_I2C;
break;

case 'k':															//Called by subroutine I2C_Tx_real_num(long L_number)
case 'K':															//Displays a real number
L_number = 0;
for (int m = 0; m < 4; m++){
L_number += I2C_data[m];
if(m<3) L_number = L_number << 8;}
Display_real_num(L_number);break;

case'M':															//Called by subroutine I2C_Rx_get_version(char str_type)
case'm':
switch (I2C_data[0]){
case '0': string_to_slave_I2C(SW_Version); break;
case '1': string_to_slave_I2C(SW_info); break;
default: break;}
break;

case'N':													//caled by I2C_Tx(1, 'N', &cal_mode);													
case'n': if (I2C_data[0] == 1){							//Manual calibration if cal_mode is 1
		EA_buff_ptr = 0;									//It is automatically read at reset
		TIMSK2 |= (1 << TOIE2);								//Set Timer 2: interrupt on overflow
		TIMSK1 |= (1 << TOIE1);
		initialise_timers_for_cal_error();
		start_timers_for_cal_error();
		OSCCAL -=20;											/////////////////
		for(int m = 0; m <= 40; m++){cal_error = compute_error(1);OSCCAL++;}/////////////////
		OSCCAL = OSCCAL_WV;
		initialise_timers_for_cal_error();  				//dissable timers
		TIMSK2 &= (~(1 << TOIE2));							//Dissable Timer 2: interrupt on overflow
		TIMSK1 &= (~(1 << TOIE1));
		
Initialise_I2C_master_write;
	I2C_master_transmit(OSCCAL_DV);
	I2C_master_transmit(OSCCAL_WV - 20);						////////////////////////////////
	for(int m = 0; m <= 40; m++){								////////////////////////////////
	I2C_master_transmit(buffer[m] >> 8);
	I2C_master_transmit(buffer[m]);}
	I2C_master_transmit	(OSCCAL_WV);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	
	
	Initialise_I2C_master_read;
	OSCCAL_UC = I2C_master_receive(0);					//No more digits to receive
	TWCR = (1 << TWINT ) | (1 << TWEN ) | (1 << TWSTO );
	
	/*********************************************/
	TIMSK2 |= (1 << TOIE2);								//Set Timer 2: interrupt on overflow
	TIMSK1 |= (1 << TOIE1);
	initialise_timers_for_cal_error();
	start_timers_for_cal_error();
	if(OSCCAL_UC == 0xFF)OSCCAL = OSCCAL_DV;
	else OSCCAL = OSCCAL_UC;							//OSCCAL test value
	cal_error = compute_error(0);						//Repeat 3 times to ovecome
	cal_error = compute_error(0);						//warm up error
	cal_error = compute_error(0);						//post exit programming mode
	initialise_timers_for_cal_error();  				//dissable timers
	TIMSK2 &= (~(1 << TOIE2));							//Dissable Timer 2: interrupt on overflow
	TIMSK1 &= (~(1 << TOIE1));
	
	
	Initialise_I2C_master_write;
	if(cal_error > 1000)
	{I2C_master_transmit('X');
	I2C_master_transmit(cal_error >> 8);
	I2C_master_transmit(cal_error);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	eeprom_write_byte((uint8_t*)0x3FE, 0xFF); 
	eeprom_write_byte((uint8_t*)0x3FF, 0xFF);
	OSCCAL = OSCCAL_DV;
	OSCCAL_WV = OSCCAL_DV;
	break;}												//Exit from calibration mode
	/*************************************************/
	else{I2C_master_transmit('Y');
	
	eeprom_write_byte((uint8_t*)0x3FE, OSCCAL_UC); 
	eeprom_write_byte((uint8_t*)0x3FF, OSCCAL_UC); 
	if(OSCCAL_UC == 0xFF) OSCCAL_WV = OSCCAL_DV;
	else {OSCCAL = OSCCAL_UC;  OSCCAL_WV = OSCCAL; }	
	
	TWDR = eeprom_read_byte((uint8_t*)0x3FE);
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	TWDR = eeprom_read_byte((uint8_t*)0x3FF);
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}}
															//caled by I2C_Tx(1, 'N', &cal_mode);
	if (I2C_data[0] == 0){									//check the calibration
		TIMSK2 |= (1 << TOIE2);								//Set Timer 2: interrupt on overflow
		TIMSK1 |= (1 << TOIE1);
		initialise_timers_for_cal_error();
		start_timers_for_cal_error();
		cal_error = compute_error(0);						//Repeat 3 times to ovecome
		cal_error = compute_error(0);						//warm up error
		cal_error = compute_error(0);						//post exit programming mode
		initialise_timers_for_cal_error();  				//dissable timers
		TIMSK2 &= (~(1 << TOIE2));							//Dissable Timer 2: interrupt on overflow
		TIMSK1 &= (~(1 << TOIE1));

Initialise_I2C_master_write;
	I2C_master_transmit(OSCCAL);
	I2C_master_transmit(cal_error >> 8);
	I2C_master_transmit(cal_error);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	}break;
	
case'O':								//called by I2C_Tx(1, 'O', &cal_mode)															
case'o':								//performs a quick cal. Note: cal_mode is not used
	
	OSCCAL_test = OSCCAL;
	//ASSR = (1 << AS2);					//Set T2 for use with crystal
	TIMSK2 |= (1 << TOIE2);				//Set Timer 2: interrupt on overflow
	TIMSK1 |= (1 << TOIE1);
	initialise_timers_for_cal_error();
	start_timers_for_cal_error();
	
		cal_error = compute_error(0);						//Repeat 3 times to ovecome
		cal_error = compute_error(0);						//warm up error
		cal_error = compute_error(0);						
	
	cal_error = calibrate_quick_cal(OSCCAL_test);				//Adjusts OSCCAL
	OSCCAL_WV = OSCCAL;
	initialise_timers_for_cal_error();  				//dissable timers
	TIMSK2 &= (~(1 << TOIE2));							//Dissable Timer 2: interrupt on overflow
	TIMSK1 &= (~(1 << TOIE1));
	Initialise_I2C_master_write;
	I2C_master_transmit(OSCCAL_WV);
	I2C_master_transmit(cal_error >> 8);
	I2C_master_transmit(cal_error);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	
	break;

case'P':																							
case'p':	start_T2_for_ATMEGA_168_cal(1);	break;			//Generates 7_8125mS TWI slave interrupt  

case 'Q':
case 'q':	MUX_cntl = I2C_data[0];							//Explore operation of multiplexer
			
			if(eeprom_read_byte((uint8_t*)0x3FB) == 0xFF){
			switch (MUX_cntl){
			case 0:	timer_T0_sub_with_interrupt(T0_delay_2ms);break;
			case 1: timer_T0_sub_with_interrupt(T0_delay_4ms);break;
			case 2:	timer_T0_sub_with_interrupt(T0_delay_6ms);break;
			case 3: timer_T0_sub_with_interrupt(T0_delay_8ms);break;
			case 4:	timer_T0_sub_with_interrupt(T0_delay_10ms);break;
			case 5: timer_T0_sub_with_interrupt(T0_delay_12ms);break;
			case 6: timer_T0_sub_with_interrupt(T0_delay_20ms);break;
			case 7: timer_T0_sub_with_interrupt(T0_delay_33ms);break;
			default: timer_T0_sub_with_interrupt(T0_delay_2ms);break;}}
			
			else{T0_interupt_cnt = 0;
			switch (MUX_cntl){
			case 0:	timer_T0_sub_with_interrupt(T0_delay_500us);break;
			case 1: timer_T0_sub_with_interrupt(T0_delay_1ms);break;
			case 2:	timer_T0_sub_with_interrupt(T0_delay_1500us);break;
			case 3: timer_T0_sub_with_interrupt(T0_delay_2ms);break;
			case 4:	timer_T0_sub_with_interrupt(T0_delay_2500us);break;
			case 5: timer_T0_sub_with_interrupt(T0_delay_3ms);break;
			case 6: timer_T0_sub_with_interrupt(T0_delay_5ms);break;
			case 7: timer_T0_sub_with_interrupt(T0_delay_8ms);break;
			default: timer_T0_sub_with_interrupt(T0_delay_500us);break;}}

			for(int p = 0; p <= 7; p++)display_buf[p] = p + '0';
			break;

case 'R':												//For manufacturint test
case 'r':
		TIMSK0 &= (~(1 << TOIE0));						//Disable Multiplexer
		clear_digits; clear_display;
		Initialise_I2C_master_read;
		test_num = I2C_master_receive(1);
		switch(test_num){
		case '0': zero; break;
		case '1': one; break;
		case '2': two; break;
		case '3': three; break;
		case '4': four; break;
		case '5': five; break;
		case '6': six; break;
		case '7': seven; break;
		case '8': eight; break;
		case '9': nine; break;}
		test_digit = I2C_master_receive(0);
		switch(test_digit){
		case '0': digit_0; break;
		case '1': digit_1; break;
		case '2': digit_2; break;
		case '3': digit_3; break;
		case '4': digit_4; break;
		case '5': digit_5; break;
		case '6': digit_6; break;
		case '7': digit_7; break;}
		TWCR =  (1 << TWINT ) | (1 << TWEN ) | (1 << TWSTO );		//Send stop bit	
		break;


case 'S': case 's':											//Scans 328 cal-factor fronmm 0x10 to 0xF0
		OSCCAL_DV = eeprom_read_byte((uint8_t*)0x3FD);
		for(int m = 0x10; m <= 0xF0; m++){
		TIMSK2 |= (1 << TOIE2);								//Set Timer 2: interrupt on overflow
		TIMSK1 |= (1 << TOIE1);
		initialise_timers_for_cal_error();
		start_timers_for_cal_error();
		OSCCAL = m;
		cal_error = compute_error(0);
		OSCCAL = OSCCAL_DV;
		initialise_timers_for_cal_error();  				//dissable timers
		TIMSK2 &= (~(1 << TOIE2));							//Dissable Timer 2: interrupt on overflow
		TIMSK1 &= (~(1 << TOIE1));
			 	
		Initialise_I2C_master_write;
		I2C_master_transmit(m);
		if(cal_error <= 1500){
		I2C_master_transmit(cal_error >> 8);
		I2C_master_transmit(cal_error);}
		else{
		I2C_master_transmit(10000 >> 8);
		I2C_master_transmit(0);	}
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}
		
		Initialise_I2C_master_write;
		I2C_master_transmit(0xFE);
		I2C_master_transmit(0xFF);
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);break;

/**********Modes 'T' and 't' are not available***************************/
/**********Modes 'U' and 'u' are not available***************************/

case 'W':																	
case 'w':

if(I2C_data[0] == 1)
	{if(eeprom_read_byte((uint8_t*)0x3FB) != 0x01)
	{eeprom_write_byte((uint8_t*)0x3FB, 0x01);}							//Reduces LED intensity
	else {eeprom_write_byte((uint8_t*)0x3FB, 0xFF);}}break;				//Full brightness


case 'x':																	//0x3FC is set to 1 when the user enters 'x' at the p/r prompt
case 'X':	Initialise_I2C_master_write;									//The means that diagnostic tests are to be carried out
			I2C_master_transmit(eeprom_read_byte((uint8_t*)(0x3FC)));		//Including version info and calibration checks
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
			eeprom_write_byte((uint8_t*)0x3FC, 0x0);break;
case 'y':																				//Called by "Proj_6H_message_from_the_OS"
case 'Y':																				//Reads messages stored in AT328 EEPROM up to address 0x200
text_max = (eeprom_read_byte((uint8_t*)(0)) <<8) + eeprom_read_byte((uint8_t*)(1));
string_counter = 0;
eeprom_ptr = 5;

while(eeprom_ptr <= text_max){

if (!(eeprom_read_byte((uint8_t*)(eeprom_ptr))))string_counter++;
eeprom_ptr++;}
	
	string_counter -=1;
	Initialise_I2C_master_write;
	I2C_master_transmit(string_counter);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);			//Transmit stop NOT actually needed
	Initialise_I2C_master_read;
	
	string_no = I2C_master_receive(0);					//One data item only										//Receive data from slave
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);			//Transmit stop NOT actually needed
	
if(!(string_no))string_no++;
string_counter=1;
eeprom_ptr = 5;

while(1){if(string_counter==string_no)break;
		else if (!(eeprom_read_byte((uint8_t*)(eeprom_ptr))))string_counter++;
		eeprom_ptr++;}

	Initialise_I2C_master_write;

	do{
	I2C_master_transmit(eeprom_read_byte((uint8_t*)eeprom_ptr));
	eeprom_ptr++;}while(eeprom_read_byte((uint8_t*)eeprom_ptr));
	I2C_master_transmit('\0');
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);			//Transmit stop NOT actually needed
break;

case 'z':														//Called by Read_AT328_EEPROM(char Op) starting at address 0x200
case 'Z':														//Used to restore AT168 EEPROM from backup version on AT328
																//So that it can be used as a programmer
	Initialise_I2C_master_write;
	
	{int m = 0x200;
	while (m <= 0x3F6)	
	{I2C_master_transmit(eeprom_read_byte((uint8_t*)m));
	m++;}	
	I2C_master_transmit(eeprom_read_byte((uint8_t*)0x3FE));
	I2C_master_transmit(eeprom_read_byte((uint8_t*)0x3FF));
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}				//Transmit stop NOT actually needed
break;


case 1:																//Called by subroutine I2C_Tx_8_byte_array(char s[])
case 3:																//Called by subroutine I2C_Tx_BWops(char*)
for (int m = 0; m < 8; m++){	
	display_buf[m] = I2C_data[m];}										//Store digits for display
	break;
case 4:																//Called by subroutine I2C_Tx_2_integers((unsigned int s1, unsigned int s2))
for (int m = 0; m < 4; m++){										//Drives the vertical display segments
	display_buf[m] = I2C_data[m];}							
	for (int m = 4; m < 8; m++)display_buf[m] = 0;
	break;
case 5:																//Called by subroutine I2C_Tx_display_char (char, char)															
	output_2 =  I2C_data[0];										//Demonstrates +/- char as symbol and binary
	sign_bit = I2C_data[1];
	for (int m = 0; m < 8; m++)display_buf[m] = 0;
	if (sign_bit == 's') {Disp_CharS (output_2);}
	if (sign_bit == 'u') {Disp_CharU (output_2);}
	break;
case 6:																		//1 signed integer													
	for (int m = 0; m < 2; m++){								
	display_buf[m] = I2C_data[m];}										//Store digits for display	
	display_buf[2] = display_buf[0];display_buf[3] = display_buf[1];
	for (int m = 4; m < 8; m++)display_buf[m] = 0;
	break;
case 7: 																//I2C_Tx_OS_timer(AT_clock_mode, start_time)
for (int m = 0; m < 8; m++){											//AT_clock_mode =7 receives 8 chars (hours mins secs)
	clock_buf[7-m] = I2C_data[m];}	
	refresh_clock_display;
	TCCR2B = 2;													//Set clock to 4096Hz and start it
	TIMSK2 |= (1 << TOIE2);										//Enable timer clock interrupt
	clock_flag=1;
	timer_mode = 1;
	break;
case 8: 														//Used with subroutine I2C_Tx_Clock_command(char timer_mode, char command)
	PIC_cmd =  I2C_data[0];										//Receive command from the PIC for the 1 second timer/clock
	switch(PIC_cmd){
	case '1': clock_flag=1; break;									//Display clock
	case '2': clock_flag=0; break;									//Hide clock Clock runs in background while display is used else where
	case '3': clock_flag=0; refresh_clock_display; break;			//Pause clock
	case '5': {increment_seconds;} clock_flag=1; break;
	case '6': {decrement_minutes;} clock_flag=1; break;
	case '7': {decrement_seconds;} clock_flag=1; break;
	case '8': {increment_minutes;} clock_flag=1; break;}			
	timer_mode = 1;
	break;
																//STOP WATCH Control
																//Used by I2C_Tx_Clock_command(one100ms_mode) and I2C_Tx_Clock_command(ten_ms_mode)
case 9:															//I2C_Tx_Clock_command(store_time)
	stop_watch_mode =  I2C_data[0];								//I2C_Tx_Clock_command(display_current_time); 
	initiate_stop_watch_display;								//I2C_Tx_Clock_command(display_stored_times);
	if (stop_watch_mode == '2') {timer_mode = 2;stop_watch_buf[2] = 0; stop_watch_buf[5] = 0;}
	else {timer_mode = 3;stop_watch_buf[1] = 0; stop_watch_buf[4] = 0;}
	TCNT2 = 0;														//Set clock to 4096Hz and start it												
	TCCR2B = 2;													
	TIMSK2 |= (1 << TOIE2);	
	stop_watch_flag=1;
	timer_record = 0;
	timer_pointer = 0;
	record_counter = 0;
	while(1){//A											//stop watch not aqble to run in background

while(1){//B												// in loop B update dislays and wait for command
TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);			//send a start condition
while (!(TWCR & (1 << TWINT)));								//Wait for TWINT flag 
TWDR = 0x03;												//Address of slave (master read operation)  SLA + R
TWCR = (1 << TWINT) | (1 << TWEN);							//Clear TWINT bit to start transmission of address
while (!(TWCR & (1 << TWINT)));								//Wait for TWINT flag 
if (TWSR == 0x40)break;

if (TCNT2 == Ten_mS_tick){//C								//Pole timer T2 for 10mSec interval
Ten_mS_tick = (Ten_mS_tick + 41)%256; 
switch (timer_mode){//D
case 3:														//100ms timer;
Ten_mS_tick_counter++;
if (Ten_mS_tick_counter == 10) {update_milli_sec_display(); Ten_mS_tick_counter = 0;} break;
case 2: update_milli_sec_display();							//10ms timer;
break;}//D

if (stop_watch_flag==1){//E
refresh_stop_watch_display; 
}//E    
}//C
}//BB		 													//Exit loop to receive mode and stop watch command

TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);				// Acquire payload_size and ignore it
while (!(TWCR & (1 << TWINT)));
payload_size = TWDR;

TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);				// Acquire mode either 9 or 10
while (!(TWCR & (1 << TWINT)));
mode = TWDR;

TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	PIC_cmd =  TWDR;
	TWCR =  (1 << TWINT ) | (1 << TWEN ) | (1 << TWSTO );
	
	if (mode == 10) {TCCR2B = 0; TIMSK2 &= (!(1 << TOIE2));{clear_display_buffer;}break;}      //I2C_Tx_Clock_command(AT_exit_stop_watch);
switch(PIC_cmd){
	case '1': stop_watch_flag=1; break;
	case '2': stop_watch_flag=0; refresh_stop_watch_display; 
				for(int m = 0; m < 8; m++) {timer_store [timer_record + m] = display_buf[m];}
				timer_record += 8; timer_record = timer_record%80; record_counter++; break;
	case '3': stop_watch_flag=1; initiate_stop_watch_display;  
			if (timer_mode == 2){stop_watch_buf[2] = 0; stop_watch_buf[5] = 0;}
			if (timer_mode == 3){stop_watch_buf[1] = 0; stop_watch_buf[4] = 0;}
			TCNT2 = 0; break;
	case '4': if (record_counter){stop_watch_flag=0; for(int m = 0; m < 8; m++) {display_buf[m] = timer_store[timer_pointer + m];}
	if(record_counter < 10){timer_pointer += 8;  timer_pointer = timer_pointer%timer_record;} 
	else{timer_pointer += 8;  timer_pointer = timer_pointer%80; }}
	
	break;
	}
}//A
break;
	}}}

	void I2C_master_transmit(char data){
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));}		

	char I2C_master_receive(char mode){
	char data;
	if(!(mode))TWCR = (1 << TWINT) | (1 << TWEN);				
	else TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);					
	while (!(TWCR & (1 << TWINT)));
	data =  TWDR;
	return data;}



