
/*
Three principal subroutines are provided:
	Cal_at_Power_on_Reset()
	manual_cal_mini-OS_device
	cal_plot_328()
	
The remaining subroutines are called by one or more of these three.
Operation also depends on the operation of the T1 and T2 ISR's

Cal_at_Power_on_Reset() should ensure that calibration is always adequate to enable
hex files to be downloaded to the user device.
The others two are really just developmemts tools.

Operation of Cal_at_Power_on_Reset():
Switch the DPDT to the left and then power cycle.
The mini-OS runs, detects the POR flag and runs the Cal_at_Power_on_Reset() subroutine. 

To stop it running always power cycle with the DPDT switched to the right, which will
make the bootloader run. 


Note:
Subroutine start_T2_for_ATMEGA_168_cal() is also included.
This sends an I2C interrupt to the user device every 7_8125mS 
during which time an 8MHz clock counts to 62,500
*/








void initialise_timers_for_cal_error(void);
void start_timers_for_cal_error(void);
long compute_error(char, char);

long fine_tune_cal(char);
long minimise_error (char, char*);
void start_T2_for_ATMEGA_168_cal(char);
long quick_cal(void);	
void Cal_at_Power_on_Reset (void);
void manual_cal_PCB_A_device(void);
void cal_plot_328(void);
void cal_adjust(void);
void cal_spot_check(void);	

void I2C_master_transmit(char);
char I2C_master_receive(char);


#define Get_ready_to_calibrate; \
TIMSK2 |= (1 << TOIE2);\
TIMSK1 |= (1 << TOIE1);\
initialise_timers_for_cal_error();\
start_timers_for_cal_error();

#define close_calibration; \
initialise_timers_for_cal_error();\
TIMSK2 &= (~(1 << TOIE2));\
TIMSK1 &= (~(1 << TOIE1));


#define calibrate_without_sign_plus_warm_up_time; \
cal_error = compute_error(0,0);\
cal_error = compute_error(0,0);\
cal_error = compute_error(0,0);




/*****************************************************************************/
void Cal_at_Power_on_Reset (){
long cal_error,error_mem;
int m;

if((MCUSR & (1 << PORF)));
else return;
MCUSR &= (~(1 << PORF));


Timer_T1_sub(T1_delay_1sec);

mode = 'T';
Get_ready_to_calibrate;
m = 0xF0;
do{OSCCAL = m;
cal_error = compute_error(0,0);			//low accurracy but quick to scan up to 160 cal factors
m -= 1;}
while(cal_error > 750);

OSCCAL -=2;
error_mem = compute_error(0,0);
if (cal_error < error_mem)OSCCAL +=2;
else  cal_error = error_mem;

eeprom_write_byte((uint8_t*)0x3FE, OSCCAL); 
eeprom_write_byte((uint8_t*)0x3FF, OSCCAL);
close_calibration;

Initialise_I2C_master_write;
I2C_master_transmit(OSCCAL);
I2C_master_transmit(cal_error >> 8);
I2C_master_transmit(cal_error);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}
	



/************************************************************************************************************/
void manual_cal_PCB_A_device(void){
long cal_error;
char OSCCAL_mem, OSCCAL_UV;

EA_buff_ptr = 0;
OSCCAL_mem = OSCCAL;
		
Get_ready_to_calibrate;
OSCCAL -=20;											//Compute cal error for 41 values of OSCCAL
for(int m = 0; m <= 40; m++)
{cal_error = compute_error(1,1);OSCCAL++;}
OSCCAL = OSCCAL_mem;
close_calibration;
		
Initialise_I2C_master_write;							//Transmit error values to user
I2C_master_transmit(OSCCAL_DV);
I2C_master_transmit(OSCCAL_mem - 20);						
for(int m = 0; m <= 40; m++){							
I2C_master_transmit(buffer[m] >> 8);
I2C_master_transmit(buffer[m]);}
I2C_master_transmit	(OSCCAL_mem);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	
	
Initialise_I2C_master_read;								//Read OSCCAL_UV (user value)
OSCCAL_UV = I2C_master_receive(0);							
TWCR = (1 << TWINT ) | (1 << TWEN ) | (1 << TWSTO );
	
/*********************************************/
Get_ready_to_calibrate;									//Test value of OSCCAL entered by user
if(OSCCAL_UV == 0xFF)OSCCAL = OSCCAL_DV;				//If 0xFF reinstate default value
else OSCCAL = OSCCAL_UV;								//OSCCAL test value
calibrate_without_sign_plus_warm_up_time;								
close_calibration;
	
Initialise_I2C_master_write;							//Report results to user
if(cal_error > 5550)			//1750					//Error resulting from User OSCCAL exceeds 1750
{I2C_master_transmit('X');								//Reject result
I2C_master_transmit(cal_error >> 8);						
I2C_master_transmit(cal_error);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
eeprom_write_byte((uint8_t*)0x3FE, 0xFF); 				//Reset OSCCAL values storred in EEPROM
eeprom_write_byte((uint8_t*)0x3FF, 0xFF);
OSCCAL = OSCCAL_DV;										//Reinstate default value
return;}			
	
/*************************************************/
else{I2C_master_transmit('Y');							//Error resulting from User OSCCAL is less than 1750
	
eeprom_write_byte((uint8_t*)0x3FE, OSCCAL_UV); 		//save user OSCCAL to EEPROM
eeprom_write_byte((uint8_t*)0x3FF, OSCCAL_UV); 
if(OSCCAL_UV == 0xFF) OSCCAL = OSCCAL_DV;				//Reinstate default value
else {OSCCAL = OSCCAL_UV;}	
	
TWDR = eeprom_read_byte((uint8_t*)0x3FE);				//Echo values saved to EEPROM to user
TWCR = (1 << TWINT) | (1 << TWEN);
while (!(TWCR & (1 << TWINT)));
TWDR = eeprom_read_byte((uint8_t*)0x3FF);
TWCR = (1 << TWINT) | (1 << TWEN);
while (!(TWCR & (1 << TWINT)));
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}}



/************************************************************************************/
long fine_tune_cal(char OSCCAL_init){					//Full accuracy but only tests several cal factors
long cal_error, error_mem;
char direction, OSCCAL_temp;
	
error_mem = minimise_error(OSCCAL_init, &direction);
OSCCAL_temp = OSCCAL;									//Save initial value
	
switch (direction){
case 'U':
cal_error = minimise_error(OSCCAL_temp + 6, &direction);
if (cal_error < error_mem)return cal_error;				//Improved result
else {OSCCAL = OSCCAL_temp; return error_mem;}			//Revert to initial value
break;
	
case 'D':
cal_error = minimise_error(OSCCAL_temp - 6, &direction);
if (cal_error < error_mem)return cal_error;				//Improved result
else {OSCCAL = OSCCAL_temp; return error_mem;}			//Revert to initial value
break;}
return 0;}



/************************************************************************************/
long minimise_error (char OSCCAL_init, char *direction)		
{long cal_error, error_mem;	
T1_OVF=0;
OSCCAL = OSCCAL_init;		
cal_error = compute_error(0,0);								//compute error for OSCCAL_test
error_mem = cal_error;

while(1){
OSCCAL += 1;
cal_error = compute_error(0,0);
if ((cal_error - error_mem) > 2500)continue;
	
if (cal_error > (error_mem + 50)) {						//If it gets worse
OSCCAL -=1; break;}											//decrement OSCCAL and exit							
error_mem = cal_error;}

if (OSCCAL != OSCCAL_init)	
{*direction = 'U'; 
return error_mem;	}										//REPLACE with OSCCAL_DV when OSCCAL_test is discarded

else{														//OSCCAL is still the default value

while(1){
OSCCAL -=1;
cal_error = compute_error(0,0);
if ((cal_error - error_mem) > 2500)continue;

if (cal_error > (error_mem + 50)){							//If it gets worse
OSCCAL += 1; break;}											//increment OSCCAL and exit
error_mem = cal_error;}
{*direction = 'D'; return error_mem;	}}}



/************************************************************************************/
long compute_error(char error_mode, char sign)	
{long error;
EA_counter = 0;										//Compute error for each value of OSCCAL 10 times
error_SUM = 0;
while(EA_counter < 15);EA_counter = 0;
error = error_SUM;
if ((error < 0) && (!(sign))) error *= (-1);
if (error_mode)
{buffer[EA_buff_ptr] = error/10;EA_buff_ptr++;}
return error/10;}



/************************************************************************************/
long quick_cal(void)	
{long error;
EA_counter = 0;										//Compute error for each value of OSCCAL 2 times
error_SUM = 0;
while(EA_counter < 2);EA_counter = 0;
error = error_SUM;
if (error < 0)error *= (-1);
return error;}



/*******************************************************************************************/
void cal_plot_328(void){							//Called by Proj_9F (mode M)
long cal_error;
OSCCAL_DV = eeprom_read_byte((uint8_t*)0x3FD);
for(int m = 0x10; m <= 0xF0; m++){
		
Get_ready_to_calibrate;
OSCCAL = m;
cal_error = compute_error(0,1);
OSCCAL = OSCCAL_DV;
close_calibration;

Initialise_I2C_master_write;

I2C_master_transmit(cal_error >> 8);
I2C_master_transmit(cal_error);
	
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}}



/************************************************************************************/
void initialise_timers_for_cal_error(void){
TCNT1=0;TCCR1B = 0;									//Reset and halt T1
TCCR2B =  0x0;	while(ASSR & (1 << TCR2BUB));		//Halt T2
TCCR2A = 0; while(ASSR & (1 << TCR2AUB));			//Reset T2 
TCNT2=0; while(ASSR & (1 << TCN2UB));	}			//Reset TCNT2



/************************************************************************************/
void start_timers_for_cal_error(void)
{TCCR2B = 1; 	
while(ASSR & (1 << TCR2BUB));
TCCR1B = 1;}
	


/************************************************************************************/
void start_T2_for_ATMEGA_168_cal(char cal_mode){
TCCR2B =  0x0;	while(ASSR & (1 << TCR2BUB));		//Halt T2
TCCR2A = 0; while(ASSR & (1 << TCR2AUB));			//Reset T2 
TCNT2=0; while(ASSR & (1 << TCN2UB));
if(cal_mode){
TIMSK2 |= (1 << TOIE2);							//Set Timer 2: interrupt on overflow
TCCR2B = 1; 										//Start T2 with a clock of 32.768Hz
while(ASSR & (1 << TCR2BUB));}
else{TIMSK2 &= (~(1 << TOIE2));mode = 0;}}


/*************************************************/	
void cal_spot_check(void)								//Called by diagnostic mode

{long cal_error;	
								
Get_ready_to_calibrate;
calibrate_without_sign_plus_warm_up_time;							
close_calibration;

Initialise_I2C_master_write;
I2C_master_transmit(OSCCAL);
I2C_master_transmit(cal_error >> 8);
I2C_master_transmit(cal_error);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	}


/****************************************************************************************************/
void cal_adjust(void){							//Called by diagnostic mode			
long cal_error;
char OSCCAL_init, OSCCAL_mem;

OSCCAL_init = OSCCAL;
Get_ready_to_calibrate;
cal_error = fine_tune_cal(OSCCAL_init);			//Adjusts OSCCAL
OSCCAL_mem = OSCCAL;
close_calibration;
Initialise_I2C_master_write;
I2C_master_transmit(OSCCAL_mem);
I2C_master_transmit(cal_error >> 8);
I2C_master_transmit(cal_error);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}
	

