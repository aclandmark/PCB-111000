char PRN_8bit_GEN(void);

void I2C_master_transmit(char);
char I2C_master_receive(char);


void Message_from_the_OS(void){

int text_max, eeprom_ptr;
char string_counter, string_no;


text_max = (eeprom_read_byte((uint8_t*)(0)) <<8) + eeprom_read_byte((uint8_t*)(1));
string_counter = 0;
eeprom_ptr = 5;

while(eeprom_ptr <= text_max){

if (!(eeprom_read_byte((uint8_t*)(eeprom_ptr))))string_counter++;
eeprom_ptr++;}
	
	string_counter -=1;
	Initialise_I2C_master_write;
	I2C_master_transmit(string_counter);
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);			//Transmit stop NOT actually needed
	Initialise_I2C_master_read;
	
	string_no = I2C_master_receive(0);					//One data item only										//Receive data from slave
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);			//Transmit stop NOT actually needed
	
if(!(string_no))string_no++;
string_counter=1;
eeprom_ptr = 5;

while(1){if(string_counter==string_no)break;
		else if (!(eeprom_read_byte((uint8_t*)(eeprom_ptr))))string_counter++;
		eeprom_ptr++;}

	Initialise_I2C_master_write;

	do{
	I2C_master_transmit(eeprom_read_byte((uint8_t*)eeprom_ptr));
	eeprom_ptr++;}while(eeprom_read_byte((uint8_t*)eeprom_ptr));
	I2C_master_transmit('\0');
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);			//Transmit stop NOT actually needed
}

/*int text_max, eeprom_ptr;
char string_counter, string_no;

text_max = (eeprom_read_byte((uint8_t*)(0)) <<8) +\
eeprom_read_byte((uint8_t*)(1));
string_counter = 0;
eeprom_ptr = 5;

while(eeprom_ptr <= text_max){

if (!(eeprom_read_byte((uint8_t*)(eeprom_ptr))))
string_counter++;
eeprom_ptr++;}
	
string_counter -=1;
string_no =  (PRN_8bit_GEN()%(string_counter)) + 1;			//Generate random string number

if(!(string_no))string_no++;
string_counter=1;
eeprom_ptr = 5;

while(1){if(string_counter==string_no)break;
	else if (!(eeprom_read_byte((uint8_t*)(eeprom_ptr))))
	string_counter++;
	eeprom_ptr++;}

Initialise_I2C_master_write;

do{
I2C_master_transmit(eeprom_read_byte((uint8_t*)eeprom_ptr));
eeprom_ptr++;}while(eeprom_read_byte((uint8_t*)eeprom_ptr));
	
I2C_master_transmit('\0');
I2C_master_transmit('\0');
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}*/		



/*****************************************************************/
char PRN_8bit_GEN(void){
unsigned int bit;
char lfsr;

lfsr = eeprom_read_byte((uint8_t*)(0x3F3));
bit = (( lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 4)) & 1;
lfsr = (lfsr >> 1) | (bit << 7);
eeprom_write_byte((uint8_t*)(0x3F3),lfsr);
return lfsr;}




