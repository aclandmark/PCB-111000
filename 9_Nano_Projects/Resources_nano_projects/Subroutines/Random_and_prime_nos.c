
unsigned char PRN_8bit_GEN(void);
unsigned int PRN_16bit_GEN(unsigned int, unsigned char*);
void prime_no_generator(int,int, int*);
void prime_no_generator_plus(int,int,  int*);
int Product_search (int);
int Prime_search (int, int);




/*****************************************************************/
unsigned char PRN_8bit_GEN(void){
unsigned int bit;
unsigned char lfsr;

lfsr = eeprom_read_byte((uint8_t*)(0x1FC));
bit = (( lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 4)) & 1;
lfsr = (lfsr >> 1) | (bit << 7);
eeprom_write_byte((uint8_t*)(0x1FC),lfsr);
return lfsr;}


/*****************************************************************/
unsigned int PRN_16bit_GEN(unsigned int start, unsigned char *PRN_counter){				//Pseudo random number generation 
unsigned int bit, lfsr, eep_address;													//The calling routine provides memory space for PRN_counter
unsigned char eep_offset = 0;																//The subroutine provides it for every thing else


if((!(*PRN_counter)) && (!(start)))														//Only read the EEPROM the first time a program calls this subroutine
{lfsr = (eeprom_read_byte((uint8_t*)0x1FC) << 8) + 
eeprom_read_byte((uint8_t*)0x1FB);}										//Data saved to EEPROM survives resets including POR (power on reset)

else lfsr = start;																		//Use previous PRN value to generate the next one

bit = (( lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
lfsr = (lfsr >> 1) | (bit << 15);

*PRN_counter += 1;	 
if(*PRN_counter == 255)

{eeprom_write_byte((uint8_t*)0x1FC,(lfsr>>8));							//Save every 16th PRN number to EEPROM
eeprom_write_byte((uint8_t*)0x1FB, lfsr);									//This prevents the display from endlessly repeating
Toggle_LED_1;															//Note: Saving every one burns out the EEPROM location too quickly
*PRN_counter = 0;}

return lfsr;}









/*****************************************************************/
void prime_no_generator(int l,int n,  int reg[]){
int i, m;
i=2; 
while (i*i < n*(l+1)){
m=0;
while(i*(i+m) <= n*l)m++; 
while(i*(i+m) <= n*(1+l)){reg[i*(i+m) -1 - n*l] = 0; m++; }	
i++;
wdr();}}


/*****************************************************************/
void prime_no_generator_plus( int l,int n,  int reg[]){
int i, m;
i=2; 
while (i*i < n*(l+1)){
m=0;
while(i*(i+m) <= n*l)m++; 
if(reg[i*(i+m) -1 - n*l]){
while(i*(i+m) <= n*(1+l)){reg[i*(i+m) -1 - n*l] = 0; m++; }	}
i++;}}


/*****************************************************************/
int Product_search	(int number)	{
int search_array[10];	
int j=0;
int n=10;
int l=0;
while(1){
for(int k =0; k < n; k++) {search_array[k] = k+1+n*l;}
prime_no_generator(l,n,search_array);
j=0; while(search_array[j] == 0)j++;
while (j<=10){if ((search_array[j] >= 2) &&  (number%search_array[j] == 0)){return search_array[j];}j++;}
if(l*10 > number)return 0;
l++;}}



/*****************************************************************/
int Prime_search ( int number, int array_size)	{
int search_array[array_size];	
int j=0;
int l=0;
while (1){
for(int k =0; k < array_size; k++) {search_array[k] = k+1+array_size*l;}
prime_no_generator(l,array_size,search_array);
j=0;
while (j<array_size){if ((search_array[j] >= 2) &&  (number%search_array[j] == 0)){
if (search_array[j]== number) {return 1;} else return 0;}j++;}
if(l*array_size > number)return 0;
l++;}	}
