
/*Proj_8B_data_arrays
***********************************************************************/

/*Puts the C maths library through its paces.

Data arrays are generated in a spreadsheet and then saved as .csv files.
As a file is downloaded the program keeps a running total of the sum and product of its numbers
At the end of the download the arithmetic and geometric means are calculated and sent to the PC.

Note: we slow the data rate down a bit because flow control has not been implemented.
*/



#include "Proj_8B_header_file.h"
#include <math.h>


int num_counter;

int main(void)
{	char input;
	double Floating_point_num, accumulator_A, accumulator_G;

	setup_HW;
	
	
/*************Display Table headings following POR or programming and then reset************/	

if ((MCUSR & (1 << PORF)) || MCUSR & (1 << EXTRF)){MCUSR = 0;	
	stdout = &uart_output;
	stdin  = &uart_input_any_char;
	printf("Select 9.6k baud rate, then AK to continue\r\n");
	USART_init(0,103);
	do{printf("?    ");}while((isCharavailable(100) == 0));
	input = getchar();
	printf("\rSend data file\rArithmetic mean\tGeometric mean\t data items");
	SW_reset;}



/***********Jump immediately to this point in the program following a SW_reset************/

USART_init(0,103);
	
	num_counter = 0;
	stdout = &uart_output;
	stdin  = &uart_input_Double_from_csv;
	accumulator_G = 0;
	accumulator_A = 0;
	while(1){scanf("%lf", &Floating_point_num);
	if(!(Floating_point_num))break;
	accumulator_A += Floating_point_num;
	accumulator_G += log10(Floating_point_num);}
	binUnwantedChars();
	printf("\n%g  ", (accumulator_A/(num_counter-1)));
	printf("\t\t%g  ", pow(10,(accumulator_G/(num_counter-1))));
	printf("\t\t%d  ", (num_counter-1));
	
	SW_reset;
	return 0;}
	

	
/*************************************************************************************/
int uart_getDouble_from_csv(FILE *mystr_input)						//Included here because of use of global variable num_counter
{char keypress;
{while (!(UCSR0A & (1 << RXC0)));
keypress = UDR0; 
while(!(decimal_digit (keypress)) && (keypress != '\r')
&& (keypress != '\n')&& (keypress != '-')&& (keypress != '.')&& (keypress != '\t')
&& (keypress != 'e') && (keypress != 'E')&& (keypress != ','))
{while((isCharavailable(100) == 0));keypress = UDR0;}
if ((keypress == ',') || (keypress == '\t')  || (keypress == '\r')){keypress = '\n';num_counter += 1;}
return keypress;}}