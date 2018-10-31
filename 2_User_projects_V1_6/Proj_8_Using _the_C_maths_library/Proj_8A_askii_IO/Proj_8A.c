
/*Proj_8A_askii_IO
********************************************/


/*Previous projects have used project (i.e. D.I.Y.) subroutines to transfer data to and from the PC.   
In some instances this is considered a sensible approach by many programmers.  These subroutines are 
not necessarily particularly complex and the d.i.y. approach can offer greater flexibility.  
However there are C libraries which provide a comprehensive range of input output subroutines.  
They also provide many other functions which programmers should be aware of.
In particular they provide a “maths” library which should normally be used as a first preference.  



IT INTRODUCES



1.	Use of the standard C-libraries to format input/output to and from the PC.

2.	Library subroutine "scanf" which converts keyboard input to a variety of different formats 
	including string and numeric.

3.	Library subroutine "printf" which converts binary data to a variety of different numeric formats 
	and combines it with text before sending it to the PC screen.

4.	Project subroutines uart_getHex(); uart_getDecimal(); and uart_getDouble(); all used to acquire 
	data and all designed to ignore any illegal key presses.

6.	Project subroutine "uart_putchar" used to send characters to the PC for display.

5.	The "stdin" and "stdout" functions that determine which of the "uart" functions will be active.

6.	Data streams into which characters are received from the PC and from which they are sent to the PC.

7.	Input & output of strings, hex numbers, decimal numbers, long numbers and floating point numbers.

8.	A new version of "makefile" designed to cater for floating point numbers which must be set up as follows:	
	Click on or press the windows icon and scroll down the program listing to "WinAVR-20100110\MFILE[WinAVR]" 
	Click on it then select the Makefile command. 
	For both "printf()" and "scanf" options select "floating point".


Note: 
This program is based completely on the results of internet searches.

See for example:
https://www.tutorialspoint.com/c_standard_library/c_function_scanf.htm
 and
https://appelsiini.net/2011/simple-usart-with-avr-lib*/



#include "Proj_8A_header_file.h"



/*******Define data streams*****************/
FILE  uart_output = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

FILE uart_input_any_char = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);
FILE uart_input_Hex = FDEV_SETUP_STREAM(NULL, uart_getHex, _FDEV_SETUP_READ);
FILE uart_input_Integer = FDEV_SETUP_STREAM(NULL, uart_getDecimal, _FDEV_SETUP_READ);
FILE uart_input_Long = FDEV_SETUP_STREAM(NULL, uart_getDecimal, _FDEV_SETUP_READ);
FILE uart_input_Double = FDEV_SETUP_STREAM(NULL, uart_getDouble, _FDEV_SETUP_READ);

int main(void)
{	char input;
	int integer_number;
	long long_number;
	double Floating_point_num;

	setup_HW;
	stdout = &uart_output;
	stdin  = &uart_input_any_char;

if ((MCUSR & (1 << PORF)) || MCUSR & (1 << EXTRF)){MCUSR = 0;	

puts("Type c, s, h, d, l, e\n");SW_reset;}				//char, string, hex, decimal, long floating (note: Long float is not supported)

do{printf("?    ");}while((isCharavailable(100) == 0));
input = getchar();
	
printf("\n");	
	
	switch (input){
	case 'c':
	{printf("Enter chars. Return to escape\nEcho:\n"); }
	while(1){input = getchar();
	printf("%c  ", input);
	if((input == '\r') || (input == '\n'))break;}break;
	
	case 's':
	printf("Enter string (space to escape)\nEcho:\n");
	scanf("%s", &input);
	printf("%s  ", &input);
	break;
	
	case 'h':
	stdin  = &uart_input_Hex;
	printf("Enter hex number (zero to escape)\nEcho:");
	while(1){scanf("%x", &integer_number);
	if(!(integer_number))break;
	printf("\n%x  ", integer_number);
	printf("%o  ", integer_number);
	printf("%d  ", integer_number);
	printf("%u  ", integer_number);}
	break;
	
	case 'd':
	stdin  = &uart_input_Integer;
	printf("Enter decimal number <0 or >0 (zero to escape)\nEcho:");
	while(1){scanf("\n%d", &integer_number);
	if(!(integer_number))break;
	printf("\n%d  ", integer_number);
	printf("%x  ", integer_number);}
	break;
	
	
	case 'l':
	stdin  = &uart_input_Long;
	printf("Enter long number <0 or >0 (zero to escape)\nEcho:");
	while(1){scanf("\n%ld", &long_number);
	if(!(long_number))break;
	printf("\n%ld  ", long_number);
	printf("%lx  ", long_number);}
	break;
	
		
	case 'e':																//4.5E3 for example
	stdin  = &uart_input_Double;
	printf("Enter scientific number <0 or >0 (zero to escape)\nEcho:");
	while(1){scanf("%lf", &Floating_point_num);
	if(!(Floating_point_num))break;
	printf("\n%g  ", Floating_point_num);}
	break;
	
	default: break;}
	
	SW_reset;
	return 0;}
	

/*************************************************************************************/
int uart_putchar_local(char c, FILE *mystr_output)
	{if (c == '\n')
	uart_putchar_local('\r', mystr_output);
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
	return 0;}
	
	
/*************************************************************************************/
int uart_getchar_local(FILE *mystr_input)
{while (!(UCSR0A & (1 << RXC0)));
return UDR0;}


/*************************************************************************************/
int uart_getDecimal_local(FILE *mystr_input)
{char keypress;
{while (!(UCSR0A & (1 << RXC0)));
keypress = UDR0;
while(!(decimal_digit (keypress)) && (keypress != '\r') && (keypress != '\n')&& (keypress != '-'))
{while((isCharavailable(100) == 0));keypress = UDR0;}
return keypress;}}

/*************************************************************************************/
