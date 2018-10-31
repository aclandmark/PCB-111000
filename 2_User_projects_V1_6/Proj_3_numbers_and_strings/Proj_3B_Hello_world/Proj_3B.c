
/*Proj_3B_Hello_world
***********************************************************/
/*DISPLAYS MESSAGES ON THE PC SCREEN


INTRODUCES


1.	Use of pointers with text strings

The four text strings "message_1 to 4" defined in the program are placed in program memory by the compiler.

When the compiler encounters the names message_1, message_2, message_3 and message_4 it substitutes the 
address of the first character in the relevant string. Each string is terminated by the null character '\0' = 0.

The four addresses are also stored in the array "message_ptr" for use by the program.

2.	Project subroutine "String_to_PC()".

This subroutine requires the address of the first symbol of the string.  It then sends the symbols one at 
a time until a null character ('\0' i.e. zero) is detected when it quits.

Note: Command "String_to_PC("text")" also places the text in program memory and sends it to the PC using the 
subroutine "String_to_PC()".*/




#include "Proj_3B_header_file.h"

	
int main (void){
char * message_ptr[4];										//Define an array that will be used to hold the addresses of the data 
															//to be processed (i.e. printed out in this case)


															//Four text strings saved to program memory
char *message_1 = "Hello world";							//Note the alternative ways of defining these text strings		
char *message_2 = "Sending text to a PC";					//The '*' tells the compiler that the name "message_1" is a pointer 
															//It gives the address of the first letter in the string
															
char message_3[] = "Writing C programs";					//Defines array called "message_3" and loads it with the string
char message_4[] = "Downloading them to an ATMEGA device";	//The name "message_3" is also a pointer. It gives the address of the 'W'.


setup_HW;


message_ptr[0] = message_1;									//Saving the pointers may simplify the process 								
message_ptr[1] = message_2;									//of printing the messages out.
message_ptr[2] = message_3;									//It may also make better use of program memory space
message_ptr[3] = message_4;									//if there are a lot of message that share bits of text

String_to_PC("Defining and using text strings"); 			//Most basic method of sending text messages to the PC
newline();													//The string is paced in program memory 
															//The address of the 'D' is sent to subroutine "String_to_PC()"
															//Command must be repeated if message is to be sent again latter on


															//Print the messages out
for(int n = 0; n <=3; n++){
String_to_PC_alt (message_ptr[n]);							//Subroutine "String_to_PC_local()" requires the start address of "message_n"				
Char_to_PC('\t');	Char_to_PC('\t');						//Send a tabs
String_to_PC_local (*(message_ptr +n));					//Note alternative form: Terms "alt" and "local" can be swapped over
newline();}
waitforkeypress(); SW_reset;}




/**************************************************************************************************/
	

void String_to_PC_local(char s[]){  						//Local function only, normally use "String_to_PC()"						
int i=0;													//Note notation which is base on array.
while(1){
if(s[i] == '\0') break;								
Char_to_PC(s[i]); 										 
Char_to_PC(' ');											//Send a space
i++;
Timer_T0_10mS_delay_x_m(1);} }


/******************************************************************************************************/

void String_to_PC_alt(char* s){  							//Note alternative notations based on pointers						
int i=0;													
while(1){
if(*(s + i) == '\0') break;								
Char_to_PC(*(s + i)); 										 
Char_to_PC(' ');											
i++;
Timer_T0_10mS_delay_x_m(1);} }



