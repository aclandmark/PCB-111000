
/*Proj_3C_numeric_strings
***********************************************************/
/*DISPLAYS NUMBERS ON THE PC SCREEN


INTRODUCES

1.	Project subroutine "I2C_Tx_8_byte_array(array)".

This sends an array of digits to the display. It is the sensible way of displaying numbers promised in 
"Proj_2I_Moving_display", but note the order of digits may still need to be reversed (see "Proj_2G_Numerical_entry").
 
2.	Project subroutine "NumericString_to_PC_local(numeric string)".

This subroutine scans through the numeric string until the null character is found. It then reads and sends the 
symbols to the PC one at a time in the reverse order until the start of the string is encountered.

3. 	Project subroutine "NumericString_to_display_R_local()".

This is similar to "NumericString_to_PC_local(numeric string)" however it writes the numeric string to another 
array in reverse order. It then sends a pointer to this array to "I2C_Tx_8_byte_array(array)".


4. 	Project subroutines "AtoB_local(num_string)" and "BtoA_local(number, num_string_2);" 
(Askii_to_binary and Binary_to_askii conversions).

We enter a number in the form of a string and must convert it to binary before we can do any arithmetic on it.
Similarly the answer must be converted back to a string before it can be displayed.
Note that the BtoA routine produces the string in the reverse order for the PC.


5.	long numbers:  The display has 8 digits. The compiler reserves 4 bytes for an 8 digit number and refers 
to it as "long". (Note 4 bytes can actually hold numbers up to 2^32 – 1 = 4,294,967,295 which has 10 digits.*/





#include "Proj_3C_header_file.h"

#define text "\r\nProcessing and displaying numeric strings. AK to continue"



	
int main (void){
long number;

setup_HW;

char *num_string_1 = "67108864";											//"num_string_1" is a pointer to an array containing chars 6,7,1,0,8,8,6,4
char num_string_2[9];														//"num_string_2" will be used to store the result of some arithmetic

 
String_to_PC(text); newline();

String_to_PC(num_string_1);													//The characters are in the correct order for display on the PC
NumericString_to_display_R_local(num_string_1);								//but must be reversed before being sent to the display

number = AtoB_local (num_string_1);											//Converts the string to binary and save in "number"


while(number>1){
waitforkeypress();
newline();

number /= 2;																//Do some arithmetic
BtoA_local(number,num_string_2);											//Convert the result to a string and store result in "num_string_2"

NumericString_to_PC_local(num_string_2);									//Sends the string to the PC (note it must be reversed first)

I2C_Tx_8_byte_array(num_string_2);}											//Display the string (note: there is no need to reverse it).


SW_reset;}																	//Reset when result reaches zero.



/**************************************************************************************************/
long AtoB_local(char array[]){												//Local function only
long num=0; int m=0;
while (array[m]){ num = num*10 + array[m] - '0'; m++; }					//Test this with paper and pencil
return num;}



/**************************************************************************************************/
void BtoA_local(long num,char digit_array[]){								//Local function only
int m=0;
do  {digit_array[m++] = (num%10) + '0';}									//Test this with paper and pencil as well
while((num = num/10) > 0);													//Note: The digits are generated in the reverse order
digit_array[m] = 0;}



/**************************************************************************************************/
void NumericString_to_PC_local(char* s){									//Local function only
int n=0;
while (s[n] != '\0')n++;													//scan through the string to find the null character
for(int m = n; m; m--)Char_to_PC(*(s + m-1));								//print symbols starting from the null rather than ending with it
Timer_T0_10mS_delay_x_m(4);}




/**************************************************************************************************/																//We just use I2C_Tx_8_byte_array(array);	
void NumericString_to_display_R_local(char array[]){
char reverse_array[9];
int n=0;
for(int m = 0; m<=8; m++)reverse_array[m]=0;								//Once again scan through the string to find the null character
while (array[n] != '\0')n++;
for(int m = n; m; m--)reverse_array[n-m] = *(array + m-1);				//Copy symbols to a new array starting with the final symbol and ending with the first
I2C_Tx_8_byte_array(reverse_array);}	


