
void waitforchar(void);
char waitforkeypress(void);
char receiveChar(void);
char isCharavailable(char);
char isCharavailableNow(void);
char wait_for_return_key(void);
void binUnwantedChars_dot (void);
void binUnwantedChars (void);
void sendChar(char);
void sendString(char*);
void newline(void);

void sendLongNum(char, long);
void SBtoAL(char*, long, char);
void sendNumericString(char*);

char decimal_digit (char);
char hex_digit (char);
void sendExp (signed char);

void sendLongNumU(char, unsigned long);
void UBtoAL(char*, unsigned long, char);



/*********************************************************************/
void waitforchar(void){
while (!(UCSR0A & (1 << RXC0)));}


/*********************************************************************/
char waitforkeypress(void){
while (!(UCSR0A & (1 << RXC0)));
return UDR0;}


/*********************************************************************/
char receiveChar(void)
{return UDR0;}


/*********************************************************************/
char isCharavailable (char m){int n = 0;
while (!(UCSR0A & (1 << RXC0))){n++;
if (n>8000) {m--;n = 0;}if (m == 0)return 0;}
return 1;}


/*********************************************************************/
char isCharavailableNow(){
if(UCSR0A & (1 << RXC0)) return 1;
else return 0;}


/*********************************************************************/

char wait_for_return_key(void){	
char keypress,temp;
keypress = waitforkeypress();
if((keypress == '\r') || (keypress == '\n')){
if (isCharavailable(1)){temp = receiveChar();}keypress = '\r';}
return keypress;}



/*********************************************************************/
void binUnwantedChars_dot (void){char bin_char;
while(1){if (isCharavailable(5)==1){bin_char = receiveChar();sendChar('.');}else break;}newline();}


/*********************************************************************/
void binUnwantedChars (void){char bin_char;
while(1){if (isCharavailable(5)==1)bin_char = receiveChar();else break;}}


/*********************************************************************/
void sendChar(char data){
while (!(UCSR0A & (1 << UDRE0)));
UDR0 = data;}

/*********************************************************************/
void sendString(char s[]){
int i = 0;
while(i < 200){
if(s[i] == '\0') break;
sendChar(s[i++]);} }

/*********************************************************************/
void newline(void){sendString ("\r\n");}


/*********************************************************************/
void sendLongNum(char radix, long long_num){
char array[12];							//Long has 10 chars + sign + null terminator	
SBtoAL(array, long_num, radix);			//calls the Binary to askii subroutine
sendNumericString(array);}				//Prints characters in reverse order

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
void sendNumericString(char* s){					
int n=0;
while (s[n] != '\0')n++;							//scroll to end of string counting the number of characters
for(int m = n; m; m--)sendChar(*(s + m-1));}		//print last character first


/*********************************************************************/
char decimal_digit (char data){
if (((data > '9') || (data < '0')) )return 0;
else return 1;}


/*********************************************************************/
char hex_digit (char data){
switch(data){
case '0': case '1': case '2': case '3': case '4': case '5': case '6':
case '7': case '8': case '9': case 'A': case 'B': case 'C': case 'D':
case 'E': case 'F': return 1; break;
default: return 0; break;}}


/*********************************************************************/
void sendExp (signed char expt){
char sign;
if (expt < 0) {sign = '-'; sendString (" E-");sendLongNum(10,expt*(-1));}
else {sendString (" E+");sendLongNum(10,expt);}}


/*********************************************************************/
void sendLongNumU(char radix, unsigned long long_num){
char array[12];							//Long has 10 chars + sign + null terminator	
UBtoAL(array, long_num, radix);			//calls the Binary to askii subroutine
sendNumericString(array);}	


/*********************************************************************/
void UBtoAL(char array[], unsigned long num, char radix){					//Signed Binary to Askii
int m=0;
for (int n = 0; n <=11; n++)array[n] = 0;	
do {array[m] = num % radix;
if(array[m] < 10)array[m] += '0'; else array[m] += '7';
m++;} while ((num = num/radix) > 0);}


