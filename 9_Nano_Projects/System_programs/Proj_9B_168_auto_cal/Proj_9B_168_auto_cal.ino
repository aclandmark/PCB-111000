

/*Proj_9B_168_auto_cal
************************************************************************/


/*IT INTRODUCES

1.  Project subroutine "emergency_cal()". 
  This is like subroutine "compute_error()" but quicker and less accurate.

2.  Project subroutine "Calibration_tuning()".
  This automatically attempts to find the best value for OSCCAL.

Note: Proj_9B can be used if the default value of OSCCAL is not good enough for PC comms. */


#include "Proj_9B_header_file.h"


volatile char T1_OVF;
volatile long error_SUM;
unsigned char OSCCAL_WV, OSCCAL_DV, New_UC_value;
volatile int EA_counter, EA_buff_ptr;
char cal_mode;

int main (void){
long error;

setup_HW;                             //"setup_HW_E;"initially saves default OSCCAL in 0x1F9 and

TIMSK1 |= (1 << TOIE1);                     //Enable T1 interrupt
I2C_initiate_7_8125mS_ref();                    //Request Mode P: 7.8125mS ref signal from master


/*****Waiting for first I2C tick***********/
waiting_for_I2C_master;                       //Energise slave I2C and wait for master
send_byte_with_Nack(1);                       //Master will respond by staying in mode P
clear_I2C_interrupt;                        //House keeping
TCNT1=0;TCCR1B = 1;                         //Start T1 with no pre-scaling (at 8MHz)
sei();  


cal_mode = 1;   
for(int m = 0; m < 15; m++)error = emergency_cal();       //warm-up time

for(int m = 0x10; m <= 0xF0; m++){                  //results are stored in array "buffer"
OSCCAL = m; if (!(m%5))Char_to_PC('.');
if ((error = emergency_cal()) < 1000)break;}
waiting_for_I2C_master;
send_byte_with_Nack(0);                       //Master responds by exiting mode P
clear_I2C_interrupt;
cli();
TIMSK1 &= (~(1 << TOIE1));                      //Disable T1 interrupt
newline(); 
String_to_PC("Approx. cal factor ");Num_to_PC(16, OSCCAL);  
String_to_PC("   Error "); Num_to_PC(10, error);
newline();


cal_mode = 5;
TIMSK1 |= (1 << TOIE1);                     //Enable T1 interrupt
I2C_initiate_7_8125mS_ref();    
waiting_for_I2C_master;
send_byte_with_Nack(1);                       //Master responds by staying in mode P
clear_I2C_interrupt;
TCNT1=0;TCCR1B = 1;                         //Start T1
sei();                                //Global interrupt enable
OSCCAL_WV = OSCCAL;
error =  Calibration_tuning(OSCCAL_WV);               //Fine tuning: First step may terminate in a subsidiary dip 
if (error > 500) error =  Calibration_tuning(OSCCAL + 4);     //Fine tuning: Second step to locate main dip 
if (error > 500) error =  Calibration_tuning(OSCCAL - 2);
if (error > 500) OSCCAL_WV = OSCCAL;



waiting_for_I2C_master;
send_byte_with_Nack(0);                       //Master responds by exiting mode P
clear_I2C_interrupt;
String_to_PC("Cal factor ");
Num_to_PC(16, OSCCAL);
String_to_PC("   Error ");
Num_to_PC(10, error);


String_to_PC("\r\nSave? y or AOK\r\n");
if(waitforkeypress() == 'y'){
New_UC_value = OSCCAL;


eeprom_write_byte((uint8_t*)0x1FE, New_UC_value);         //Save new user cal value to EEPROM addresses 0x1F7 and 8
Timer_T0_sub(T0_delay_10ms);
eeprom_write_byte((uint8_t*)0x1FF, New_UC_value);
Timer_T0_sub(T0_delay_10ms);

    
String_to_PC("Values saved to EEPROM   ");              //Echo values back from the EEPROM
Num_to_PC(16,eeprom_read_byte((uint8_t*)0x1FE));
String_to_PC ("    ");
Num_to_PC(16,eeprom_read_byte((uint8_t*)0x1FF));
newline();}
else String_to_PC("Not saved\r\n");

String_to_PC("\r\nAK to repeat\r\n");
  waitforkeypress();SW_reset;}




/**********************************************************************************************/
long emergency_cal(void)  
    {long error;
    waiting_for_I2C_master_with_ISR;              //TWI generates interrupt every 7.8125mS 
    EA_counter = 0;                       //Compute error for each value of OSCCAL 10 times
    error_SUM = 0;
    while(EA_counter < 2);EA_counter = 0;TWCR = 0;          //wait here for 2 TWI interrupts
    error = error_SUM;
    if (error < 0) error *= (-1);               //Only interested in the magnitude of the error
    return error;}                          //return average error values
  


/**********************************************************************************************/
long compute_error(void)  
    {long error;
    waiting_for_I2C_master_with_ISR;
    EA_counter = 0;                       //Compute error for each value of OSCCAL 10 times
    error_SUM = 0;
    while(EA_counter < 15);EA_counter = 0;TWCR = 0;     //wait here for 15 TWI interrupts
    error = error_SUM;
    if (error < 0) error *= (-1);               //Only interested in the magnitude of the error
    return error/10;}                     //return average error values





/**********************************************************************************************/
ISR(TIMER1_OVF_vect){T1_OVF++; }                  //T1 should count to 62500 in 7.8125ms.




/************************************************************************************************/
ISR(TWI_vect){                            //T2 (mini_OS) overflows: giving a calibrated tick rate
long TCNT1_BKP;
TWDR;                               //Read TWDR the data register
send_byte_with_Nack(1);                       //Master responds by remaining in mode P
clear_I2C_interrupt;

TCCR1B = 0;                             //Halt T1
TCNT1_BKP = TCNT1;                          //Copy the value of TCNT1
TCNT1 = 0;                              //Clear TCNT1
TCCR1B = 1;                             //Get T1 running again ASAP (Note T2 (mini_OS) has not stopped running)
if(EA_counter < cal_mode)T1_OVF = 0;                //Ignore first result for "emergency_cal()" or first 5 for "compute_error"
else
{switch(T1_OVF){
case 0: error_SUM = error_SUM + (TCNT1_BKP - 62500); break;
case 1: error_SUM = error_SUM + (TCNT1_BKP - 62500 + 65536); break;
case 2: error_SUM = error_SUM + (TCNT1_BKP - 62500 + 131072); break;}
T1_OVF = 0;}                            //Sum error results fo the next 20 TWI interrupts
EA_counter++;                           //counter increments up to 25.
waiting_for_I2C_master_with_ISR;}                 //Be ready for next TWI interrupt 



/**********************************************************************************************/
void I2C_initiate_7_8125mS_ref(void){
char num_bytes=0;
char mode = 'U';
waiting_for_I2C_master;
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
clear_I2C_interrupt;} 


/**********************************************************************************************/  
long Calibration_tuning(char OSCCAL_test)
    {long cal_error, error_mem; 
    OSCCAL = OSCCAL_test;   
    cal_error = compute_error();                  //compute error for OSCCAL_test
    error_mem = cal_error;
    OSCCAL++;                           //increment OSCALL
    while(1){
    cal_error = compute_error();                  //Calculate new error
    if (cal_error > error_mem) {                  //If it gets worse
    OSCCAL--; break;}                       //decrement OSCCAL and exit             
    error_mem = cal_error;              
    OSCCAL++;}
    if (OSCCAL != OSCCAL_test)  return error_mem;         //REPLACE with OSCCAL_DV when OSCCAL_test is discarded
    else{                             //OSCCAL is still the default value
    OSCCAL--;
    while(1){
    cal_error = compute_error();
    if (cal_error > error_mem){                   //If it gets worse
    OSCCAL++;
    break;}                             //increment OSCCAL and exit
    error_mem = cal_error;              
    OSCCAL--;}}
    return error_mem;}
