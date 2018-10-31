
/*Proj_9C_ATMEGA328_manual_cal
******************************************************************/


/*Like Proj_9A this project generates a print out of calibration error as a function of the calibration
factor OSCCAL and enables the user to choose a value for OSCCAL.


IT INTRODUCES 


Project Subroutine "I2C_Tx(1, 'N', &cal_mode)".
This causes the mini-OS to carry out manual calibration of the ATMEGA 328.

Note:  
Calibration subroutines are not visible to the user but are similar to those used for the ATMEGA 168 
however T2 running under crystal control provides the 7.8125 mS interrupt.

There is no auto_cal_project for the Atmega 328 because this runs automatically under the control 
of the mini-OS following a POR.
*/



#include "Proj_9C_header_file.h"



int main (void){
long percentage_error;
int Results[41], error;
char osccal_MIN;
char cal_UC, OSCCAL_DV;
char  New_UC_value;  
char cal_mode=1, Test_res;


setup_HW_E;

String_to_PC("\r\nATMEGA 328 manual calibration (please wait 10 seconds)\r\n\
Cal factor user value   \t");


I2C_Tx(1, 'N', &cal_mode);										//Initiate Atmega 328 calibration mode
waiting_for_I2C_master;											//Atmega 328 accepts request to calibrate							
OSCCAL_DV = receive_byte_with_Ack();							//Receive 328 OSCCAL_default Value
osccal_MIN = receive_byte_with_Ack(); 							//Working value + 20	

for(int m = 0; m <= 40; m++){									//Receive table giving 40 calibration error results
Results[m] = receive_byte_with_Ack();
Results[m] = (Results[m] << 8) + receive_byte_with_Ack();}	
cal_UC = receive_byte_with_Nack(); 								//Receive 328 OSCCAL_User Value
clear_I2C_interrupt;

Num_to_PC(16, cal_UC ); newline();								//Print User cal factor
String_to_PC("Cal factor default value   \t");					//Print OSCCAL_default_Value
Num_to_PC(16, OSCCAL_DV ); newline();

for(int m = 0; m <= 40; m++)									//Print table of values
{Num_to_PC(16,osccal_MIN); String_to_PC("   \t");
Num_to_PC(10,Results[m]); String_to_PC("   \t");
percentage_error = Results[m];
Num_to_PC(10,percentage_error*100/62500);Char_to_PC('%');
osccal_MIN++;newline();}

String_to_PC("Enter new user cal value\r\n\
or enter FF to delete the user cal");							//Request new OSCCAL_User_Value

New_UC_value = Hex_from_KBD();
String_to_PC ("\r\nPress y if OK or AOK and repeat");

while(1){
if(waitforkeypress() == 'y')break; 
else {newline();Char_to_PC('?');New_UC_value = Hex_from_KBD();String_to_PC(" y?");}}

waiting_for_I2C_master;											//Send OSCCAL_User_Value to AT328 device
send_byte_with_Nack(New_UC_value);								//mini-OS will test that User cal is suitable
clear_I2C_interrupt;

/**************************************/
waiting_for_I2C_master;
Test_res =  receive_byte_with_Ack();


if(Test_res == 'X'){											//min-OS rejects user-cal value just entered
error = receive_byte_with_Ack();
error = (error << 8) + receive_byte_with_Nack();clear_I2C_interrupt;
newline(); Num_to_PC(10, error); String_to_PC("  Error too great! Try again."); 
newline();}

if(Test_res == 'Y'){											//min-OS accepts user-cal value just entered
String_to_PC("\r\nValues saved to EEPROM   \t");

New_UC_value = receive_byte_with_Ack();
Num_to_PC(16, New_UC_value); String_to_PC("   \t");
New_UC_value = receive_byte_with_Nack();
clear_I2C_interrupt;
Num_to_PC(16, New_UC_value); newline();}
String_to_PC("AK to repeat\r\n");
waitforkeypress();SW_reset;}




