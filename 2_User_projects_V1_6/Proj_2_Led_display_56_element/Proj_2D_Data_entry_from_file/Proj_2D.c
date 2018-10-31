
/*Proj_2D_Data_entry_from_file
******************************************************************/


/*ONE MORE CHANCE TO CREATE PATTERNS ON THE DISPLAY
Data used to drive the display is saved in a .txt file.



IT INTRODUCES


1.	Project subroutine "String_to_PC()".
This will be discussed in more detail in "Proj_3_numbers_and_strings".

2.	Project subroutines "isCharavailable(m)” and “receiveChar()”.
"isCharavailable(m)” waits to receive a character from the PC and then returns 1. However if no character has been 
received after 15m mS it returns zero. If there is a character this is then retrieved using “receiveChar()”.
	
3.	The data array.
This is a set of memory locations all referenced with a single name plus an offset. In this program the array is given 
the name "file" and has 100 locations. When the compiler sees the name "file" it substitutes the address of the first 
memory location reserved for array "file". When the compiler sees "file[25]" it adds 25 to the address of the first 
location.

The name "file" is another example of a pointer.

4.	Downloading a file from the PC and saving it to an array.
The program uses "waitforkeypress()" to detect the first character in the file.  It then receives remaining characters 
using "isCharavailable()".  This subroutines times out at the end of the file download allowing program execution to 
move on to the next stage.

Note:
If the file size exceeds the 100 characters allowed for, the array bounds will be exceeded. The contents of other memory 
locations will be corrupted and the program will crash.

The array "file" has elements array[0], array[1], array[2]...... array[99].  (Note: array[100] is illegal.)   

Many systems use flow control to pause the download to prevent the target pcb from being overwhelmed with data but this 
has not been implemented on the project_pcb due to insufficient I/O.  Another option is to reduce the speed at which 
data is downloaded.



USER INSTRUCTIONS


If no file has been downloaded hold switch 2 down while pressing switch 1 and download a file as requested. 
Press switch_1 each time the display pauses.
Subsequently just press sw1 and avoid the file download.
When the display is fully populated holding sw3 down while pressing sw1 will depopulate it without introducing any timer 
delays. 

Note: 
	A sample file is provided.
	Switch location on PCB SW3 - SW2 - SW1*/
	


#include "Proj_2D_header_file.h"




int main (void){

char keypress;
char segment=0, digit_num=0, counter=0;									
							
char delay=6;									//delay determines the speed with which display is updated
char file[100];									//100 memory locations are reserved and used to store the contents of a file
int n;											//Used to hold the size of the file (must be <=100 characters)
setup_HW;

while(switch_1_up);								//press switch 1 to initiate
if(switch_2_down){								//download file if switch 2 is also pressed
while(switch_1_down);							//wait for sw1 to be released


/**************File download process*******************/


String_to_PC("segment, number file\r\n");		//To be discussed in Proj_3
file[0] = waitforkeypress();					//Wait for start of file download 
n=1;
while(1) 
{if(isCharavailable(2)) 						//waits several mS and returns 1 if a characer has 
{keypress = receiveChar(); 						//been downloaded or zero when the download is complete

switch(keypress){
case '0': case '1': case '2': 
case '3': case '4': case '5':
case '6': case '7': case 'a': 
case 'b': case 'c': case 'd': 
case 'e': case 'f':  case 'g':  
file[n] = keypress;								//save legitimate characters to the array 
n++;break;										//"file" which con hold up to 100 characters
default: break;}}								//skip unwanted characters such as return, line feed, space etc.
else break; }									//break when file download is complete



/************Save file size and characters to EEPROM********************/


Char_to_EEPROM(0,n);							//copy the number of characters to EEPROM
for(int p = 0; p < n; p++){					//copy file to EEPROM starting at location zero
Char_to_EEPROM(p+1,file[p]);}}



/************Read EEPROM one character at a time and use it to update the display
Jump straight to this point if file download is not required*********************/


while(1){										//infinite loop used to display file
counter=0;
n = Char_from_EEPROM(0);						//read file size from EEPROM
for(int p = 0; p < n; p++){					//Read characters from EEPROM
switch (p%2){									//p%2 only takes on the values 0 and 1
case 0: 										//if zero read segment (a-g)
segment = Char_from_EEPROM(p+1); 
break;
case 1: 										//if 1 read number (1-7)
digit_num = Char_from_EEPROM(p+1) - '0';
I2C_Tx_any_segment (segment, digit_num);		//update the display once
Timer_T0_10mS_delay_x_m(delay);
if(counter == 7)								//if (delay) pause after the display 
{counter =0; if(delay)while(switch_1_up);} 	//has been updated 7 times until sw1 is pressed
else counter++;
break;}}										//End of "switch" statements




/**************Option to write to the display in one go with no delays***************************************/


												//End of file reached
while(switch_1_up);								//Delay can be reduced to zero to illustrate speed																			//Pulse switch_1 to increment the display
if (switch_3_down) delay = 0; 					//with which display can be changed
else delay = 6;
while(switch_1_down);}}





