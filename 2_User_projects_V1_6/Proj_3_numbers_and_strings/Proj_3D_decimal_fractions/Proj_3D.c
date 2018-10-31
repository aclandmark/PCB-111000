
/*Proj_3D_decimal_fractions
***********************************************************/

/*DISPLAYS FRACTIONS TO INFINITE PRECISION


INTRODUCES


1.	The project subroutine "I2C_displayToNum()".
This uses the Mini-OS to convert the display to a binary number and sending it to the Atmega 168.

2.	Project subroutine "fraction_to_decimal_string_local()".
This illustrates the process of converting a fraction to a decimal number.*/




void fraction_to_decimal_string_local(long, long, char*);	//Prototype for subroutine
															//Note "main" provides the digits array that holds the result
															//The subroutine requires a pointer to this array

#include "Proj_3D_header_file.h"

char * message_ptr[6];

char *num_string_1 = "537";									//define a number of messages
char *num_string_2 = "1379";
char text_1[] = " divided by ";
char text_2[] = " equals ";
char *num_string_3 = "58";
char *num_string_4 = "124";




int main (void){
long long_num[2];											//Denominator and numerator
char User_response, k;	
char digits[8];												//Number to be sent to display in string form

setup_HW;		

message_ptr[0] = num_string_1;
message_ptr[1] = num_string_2;
message_ptr[2] = text_1; 
message_ptr[3] = text_2;
message_ptr[4] = num_string_3; 
message_ptr[5] = num_string_4;

User_prompt;												//User responds by pressing either "R" or "r"

String_to_PC("AK?\r\n");

if(User_response == 'r')k=0; else k=4;

for(int m = 0; m<=1; m++){									//Convert denominator and numerator to binary
NumericString_to_display_R_local(message_ptr[m+k]);		//Send number string to display
String_to_PC(message_ptr[m+k]);							//Send numerical string to PC
String_to_PC	(message_ptr[m+2]);						//send text string to PC
long_num[m] = I2C_displayToNum();							//get display as binary number
waitforkeypress();}

fraction_to_decimal_string_local(long_num[0], long_num[1], digits);

SW_reset;}	




/**************************************************************************************************/
void fraction_to_decimal_string_local(long numerator, long denominator, char digits[]){
long digit;
int m=6;

clear_digits;

digits[7]= '0';
digits[6] = '_';
String_to_PC("0.");

while(switch_2_up){										//Press sw2 to terminate the process
digit = (numerator *10/denominator);					//Check with paper and pencil
numerator = ((numerator *10)%denominator);				//Check with paper and pencil

Char_to_PC(digit+'0');									//Send fist decimal place to pc

if(m){digits[m-1] = (digit+'0');						//Send first decimal place to the display
m--;I2C_Tx_8_byte_array(digits);						//Stop sending decimal places to the display after the first 6.
Timer_T0_10mS_delay_x_m(12);}							//Repeat for second decimal place
Timer_T0_10mS_delay_x_m(6);}}




/**************************************************************************************************/
void NumericString_to_display_R_local(char array[]){
char reverse_array[9];
int n=0;
for(int m = 0; m<=8; m++)reverse_array[m]=0;
while (array[n] != '\0')n++;
for(int m = n; m; m--)reverse_array[n-m] = *(array + m-1);	
I2C_Tx_8_byte_array(reverse_array);}

