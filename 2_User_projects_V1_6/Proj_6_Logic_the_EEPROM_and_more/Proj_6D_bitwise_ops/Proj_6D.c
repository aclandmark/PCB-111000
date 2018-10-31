
/*Proj_6D_bitwise_ops
*************************************************************/


/*EXPLORES some of the logic functions used in earlier programs including	
OR, Exclusive OR, AND and NOT

These are used to test bits, clear them, set them and reverse them.
They are also used to configure the HW and come in handy in many ways.


USER INSTRUCTIONS

Respond to User prompt with -r- to use random numbers or with -R- to enter numbers.
Select the logic operation when requested and enter two numbers (0-9) if requested.
Top two rows give the inputs to the logic function selected.
Bottom row gives the result.*/




#include "Proj_6D_header_file.h"


char Op(char, char);

char digits[8];
char BWOp, comp;																	//bit wise operation and complement (i.e. swap ones and zeros)

int main (void){
char User_response;

setup_HW;
User_prompt;

String_to_PC("Select OP: 	| 	^ 	& 	~| 	~^ 	or	~&");
BWOp = waitforkeypress();
if (BWOp == '~') {comp = 1; BWOp = waitforkeypress();}else comp = 0;				//detect complement operator
if ((BWOp != '|') && (BWOp != '^') && (BWOp != '&'))
{wdt_enable(WDTO_30MS); while(1);}													//reset if duff char was sent 

if (User_response == 'R'){															//Operates on user defined data

String_to_PC("\r\nSend 2 digits");
for (int m = 0;  m < 7; m++)digits[m]=0;											//clear array
for (int m = 0;  m < 2; m++){digits[m] = waitforkeypress() - '0';}				//Load first 2 locations with user data
digits[2] =  Op(digits[0] , digits[1]);											//Put result in third location
I2C_Tx_BWops(digits);																//sends full array of 8 char variables

String_to_PC("\r\nPress AK to increment digits or x to escape\r\n");
 
while(1){
if (waitforkeypress() =='x')break;
digits[0]++; digits[1] +=1;
digits[2] =  Op(digits[0] , digits[1]);
I2C_Tx_BWops(digits);}

wdt_enable(WDTO_30MS); while(1);}
	

String_to_PC("\r\nUse random numbers: Press AK or x to escape\r\n");				//Operates on random data
while (waitforkeypress() !='x'){
digits[0] = PRN_8bit_GEN();
digits[1] = PRN_8bit_GEN();
digits[2] =  Op(digits[0] , digits[1]);
I2C_Tx_BWops(digits);}									
wdt_enable(WDTO_30MS); while(1);}




/*************************************************************************************/
char Op(char A, char B)
{char result=0;
switch (BWOp){
case '|': result = A | B; break;
case '^': result = A ^ B; break;
case '&': result = A & B; break;}
if (comp == 1) result = ~result;
return result;}




