

void waitforchar(void);
char waitforkeypress(void);
char receiveChar(void);
char isCharavailable(char);
//char isCharWaiting(void);			//was isCharavailableNow
char wait_for_return_key(void);
void binUnwantedChars_dot (void);
void binUnwantedChars (void);
void Char_to_PC(char);
void String_to_PC(char*);
void newline(void);

void Num_to_PC(char, long);					//was sendLongNum()
void SBtoAL(char*, long, char);	
void NumericString_to_PC(char*);			//was sendNumericString

char decimal_digit (char);
char hex_digit (char);
void Exp_to_PC (signed char);

void Num_to_PC_U(char, unsigned long);		//was sendLongNumU()
void UBtoAL(char*, unsigned long, char);








/*********************************************************************/
void waitforchar(void){
while (!(UCSR0A & (1 << RXC0)));}



/*********************************************************************/
void binUnwantedChars_dot (void){char bin_char;
while(1){if (isCharavailable(5)==1){bin_char = receiveChar();Char_to_PC('.');}else break;}newline();}



/*********************************************************************/
void binUnwantedChars (void){char bin_char;
while(1){if (isCharavailable(5)==1)bin_char = receiveChar();else break;}}



/*********************************************************************/
void Num_to_PC(char radix, long long_num){
char array[12];							//Long has 10 chars + sign + null terminator	
SBtoAL(array, long_num, radix);			//calls the Binary to askii subroutine
NumericString_to_PC(array);}				//Prints characters in reverse order



/*********************************************************************/
void SBtoAL(char array[], long num, char radix){					//Signed Binary to Askii
int m=0;
long sign;

if (num == 0x80000000){
switch(radix){
case 10: array[0] = '8';array[1] = '4'; array[2] = '6';		//0x80000000 * -1 = 0
array[3] = '3';array[4] = '8';array[5] = '4'; array[6] = '7';
array[7] = '4';array[8] = '1';array[9] = '2';
array[10] = '-'; array[11] = '\0'; break;

case 16: array[0] = '0';array[1] = '0'; array[2] = '0';		//0x80000000 * -1 = 0
array[3] = '0';array[4] = '0';array[5] = '0'; array[6] = '0';
array[7] = '8';array[8] = '-';array[9] = '\0';
array[10] = '\0'; array[11] = '\0'; break; } return;}

for (int n = 0; n <=11; n++)array[n] = 0;	
if ((sign = num) < 0) num = num * (-1);

do {array[m] = num % radix;
if(array[m] < 10)array[m] += '0'; else array[m] += '7';
m++;} while ((num = num/radix) > 0);
if (sign < 0) {array[m] = '-';m++;}}



/*********************************************************************/
void NumericString_to_PC(char* s){					
int n=0;
while (s[n] != '\0')n++;							//scroll to end of string counting the number of characters
for(int m = n; m; m--)Char_to_PC(*(s + m-1));}		//print last character first



/*********************************************************************/
char hex_digit (char data){
switch(data){
case '0': case '1': case '2': case '3': case '4': case '5': case '6':
case '7': case '8': case '9': case 'A': case 'B': case 'C': case 'D':
case 'E': case 'F': return 1; break;
default: return 0; break;}}



/*********************************************************************/
void Exp_to_PC (signed char expt){
char sign;
if (expt < 0) {sign = '-'; String_to_PC (" E-");Num_to_PC(10,expt*(-1));}
else {String_to_PC (" E+");Num_to_PC(10,expt);}}



/*********************************************************************/
void Num_to_PC_U(char radix, unsigned long long_num){
char array[12];							//Long has 10 chars + sign + null terminator	
UBtoAL(array, long_num, radix);			//calls the Binary to askii subroutine
NumericString_to_PC(array);}	



/*********************************************************************/
void UBtoAL(char array[], unsigned long num, char radix){					//Signed Binary to Askii
int m=0;
for (int n = 0; n <=11; n++)array[n] = 0;	
do {array[m] = num % radix;
if(array[m] < 10)array[m] += '0'; else array[m] += '7';
m++;} while ((num = num/radix) > 0);}


