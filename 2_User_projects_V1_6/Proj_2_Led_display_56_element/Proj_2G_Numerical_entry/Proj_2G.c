
/*Proj_2G_Numerical_entry
******************************************************************************/


/*A BETTER WAY OF ENTERING DIGITS WHICH INTRODUCES



1.	More on the #define statement. 
This is used here to define each digit in terms of its segment letters. Each set of segment letters is known 
as a string and is stored in program memory in an array terminated in the null character '\0' or (0). When 
the compiler sees a digit name (i.e zero) it substitutes the address at which the first segment is stored.

2.	The pointer symbol "*".  
In the statement 	“char* string_ptr = 0;” 
The memory location "string_ptr" is preceded by a "*".
This tells the compiler that it will be used to hold the address of data to be operated on rather than the 
data itself (in this case the address of the first segment used to define a digit).

3.	The "continue" statement.
Note that in a loop such as "for" or "while" a "break" statement causes program execution to jump to the 
statement following the end of the loop.
The "continue" statement causes program execution to jump to the bottom of the loop from where it repeats 
the loop again. Note that "break" is also used in the "case" construct.

Note:  
"waitforkeypress()" is used here to keep things simple.  There is no WDT.
If the number 45 is entered the number 54 gets displayed.  This is a common problem with displaying and printing numbers.*/




#include "Proj_2G_header_file.h"

#define zero "abcdef"										//chars a,b,c,d,e and f are stored in an array named "zero"
#define one "bc"											//chars b and c are stored in an array named "one"
#define two "abged"											//Note: the compiler terminates each string in zero
#define three "abcdg"
#define four "fgbc"
#define five "afgcd"
#define six "fegdc"
#define seven "abc"
#define eight "abcdefg"
#define nine "abcfg"

void display_num_string (char*, int);



int main (void){

char   digit;
int digit_num=0;											//defines number of next digit on display						
char* string_ptr = 0;										//pointer: will be loaded with the address of a segment string 
setup_HW;													//(i.e. the address of string "zero", "one", "two" etc....) 

String_to_PC("Send digits?");
while(1){

digit_num = 0;												//First digit on display

do{															//start of "do{}while();" loop
digit = waitforkeypress();									//user enters digit (0 to 9) at the PC keyboard


switch(digit){												//The appropriate address is loaded into location "string_pointer"
case '0': string_ptr = zero; break;							//The address of array zero is loaded into location "string_ptr"
case '1': string_ptr = one; break;
case '2': string_ptr = two; break;
case '3': string_ptr = three; break;
case '4': string_ptr = four; break;
case '5': string_ptr = five; break;
case '6': string_ptr = six; break;
case '7': string_ptr = seven; break;
case '8': string_ptr = eight; break;
case '9': string_ptr = nine; break;
default: continue; break;}									//Illegal key press: Go immediately to the start of the do loop

															//Send the address of the required string to subroutine "display_num_string();"
display_num_string(string_ptr, digit_num);digit_num++;} 
while (digit_num < 8);										//return to the top of the "do" loop until all digits have been illuminated

waitforkeypress();I2C_Tx_any_segment_clear_all();}}		//clear display and repeat


void display_num_string (char* s, int digit_num){			//Subroutine requires a pointer to the string		
int char_ptr=0;												//containing segments used to define a digit
char letter;

while(1){
letter = *(s + char_ptr);// (s[char_ptr]);					//Note these two expressions are equivalent
switch(letter){												//Work through the segments contained in the 
case 'a':													//string until '\0' is encountered
case 'b': 
case 'c': 
case 'd': 
case 'e': 
case 'f': 
case 'g': I2C_Tx_any_segment(letter, digit_num);break;		//update display one segment at a time
case 0:  return; break;										//zero indicates the end of the string
default: break;}char_ptr++;}}								//incrementing "char_ptr" steps through the string
															//Selecting segment letters in turn