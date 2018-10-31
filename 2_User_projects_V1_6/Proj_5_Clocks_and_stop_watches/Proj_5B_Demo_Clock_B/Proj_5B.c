
/*Proj_5B_Demo_Clock_B
***********************************************************/



/*INTRODUCES


Simple clock in which the display is kept up to date by incrementing a counter every second
The counter is converted to Hours, Minutes, Seconds each of which is then converted to askii 
characters and sent to the display.*/





#include "Proj_5B_header_file.h"


void set_time(void);
void timer_utoa(char);
void Display_time(void);

char digits[8], charH, charL, Hours, Minutes, Seconds;
unsigned int sec_counter;


int main (void){
char User_response;
setup_HW;
String_to_PC("Press 'R' to enter time or 'r' to start at time zero  ");
User_prompt;
if(User_response == 'R')set_time();
else {reset_clock_1; msecsH = 0; msecsL = 0;	}
I2C_Tx_8_byte_array(digits);

String_to_PC("AK to start\r\n");
waitforkeypress();binUnwantedChars ();

while(1){Timer_T0_10mS_delay_x_m(100);sec_counter++; Display_time();}}




/**********************************************************************************************************************/
void Display_time(void){
Hours = sec_counter/3600;
Minutes = (sec_counter%3600)/60;
Seconds = (sec_counter%3600)%60;
timer_utoa(Hours); HoursH = charH; HoursL = charL; 
timer_utoa(Minutes); MinsH = charH; MinsL = charL; 
timer_utoa(Seconds); SecsH = charH; SecsL = charL; 
I2C_Tx_8_byte_array(digits);}



/**********************************************************************************************************************/
void set_time(void){
String_to_PC("\r\nEnter start time Hours, Minutes and Seconds (12 hour clock, no spaces)\r\n");
while(isCharavailable(50) == 0){String_to_PC("T?  ");}		
		HoursH = receiveChar();
		while(isCharavailable(5) == 0); HoursL = receiveChar();
		while(isCharavailable(5) == 0); MinsH = receiveChar();
		while(isCharavailable(5) == 0); MinsL = receiveChar();
		while(isCharavailable(5) == 0); SecsH = receiveChar();
		while(isCharavailable(5) == 0); SecsL= receiveChar();	
		msecsH = 0; msecsL = 0;	
		sec_counter = ((((HoursH -'0') * 10) + HoursL-'0') * 3600) + ((((MinsH-'0') * 10) + MinsL-'0') * 60) +(SecsH-'0') * 10 + SecsL - '0'; }
		


/**********************************************************************************************************************/
void timer_utoa(char n){								//converts an unsigned number to a string of digits
if (n>=10)
{charL = n%10;
charH = (n-charL)/10;
charL = charL + '0';
charH = charH + '0';}
else
{charH = '0'; charL = n + '0';}}

