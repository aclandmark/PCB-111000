
/*Proj_5A_Demo_Clock_A
***********************************************************/


/*IT INTRODUCES

Simple clock in which the display is kept up to date by manipulating the string sent to the display every 100 mS.

It uses a free running clock oscillator: accuracy is therefore poor.
Pencil and paper recommended to understand string manipulation.


USER INSTRUCTIONS

Respond to the user prompt by pressing "R" and entering the time when requested or by pressing "r" and letting 
the clock start at time zero.*/




#include "Proj_5A_header_file.h"


char digits[8];

int main (void){
char User_response;

setup_HW;
String_to_PC("Press 'R' to enter time or 'r' to start at time zero  ");
User_prompt;

if(User_response == 'R')set_time();
else {reset_clock_1;}
I2C_Tx_8_byte_array(digits);							//set display to "0000000_"

String_to_PC("AK to start\r\n");
waitforkeypress();

while(1){Timer_T0_10mS_delay_x_m(10);Inc_display();}}




/**********************************************************************************************************************************************************/
void Inc_display(void){
 if (msecsH < '9') msecsH++; //no need to update seconds display
	else {msecsH = '0'; if (SecsL < '9') SecsL++; 	//update msecs and seconds units
							else {SecsL = '0'; if (SecsH < '5') SecsH++; //update msecs and seconds units and tens
													else {SecsH = '0';  if (MinsL < '9') MinsL++; //update msecs, seconds units and tens and minutes units
																			else {MinsL = '0'; if (MinsH < '5') MinsH++; else {MinsH = '0';

 switch (HoursH){ case '0': if (HoursL < '9')HoursL++; else {HoursL='0'; HoursH++;} break;
 case '1': if(HoursL < '1')HoursL++; else {HoursL='0'; HoursH='0';} break;}
 
 }	//	update msecs, seconds units and tens, minutes units and tens and hours
 }	//	update msecs, seconds units and tens and minutes units and tens
 }	//	update msecs, seconds units and tens and minutes units
 }	//	update msecs and seconds units and tens
 }	//  update msecs and seconds units
I2C_Tx_8_byte_array(digits);}




/**********************************************************************************************************************************************************/
void set_time(void){
String_to_PC("Enter start time Hours, Minutes and Seconds (12 hour clock with no spaces)\r\n");
while(isCharavailable(50) == 0){String_to_PC("T?  ");}		
		HoursH = receiveChar();
		while(isCharavailable(5) == 0); HoursL = receiveChar();
		while(isCharavailable(5) == 0); MinsH = receiveChar();
		while(isCharavailable(5) == 0); MinsL = receiveChar();
		while(isCharavailable(5) == 0); SecsH = receiveChar();
		while(isCharavailable(5) == 0); SecsL= receiveChar();	
		msecsH = '0'; msecsL = 0;		 }
