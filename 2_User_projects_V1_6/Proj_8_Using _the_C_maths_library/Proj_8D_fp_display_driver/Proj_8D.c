
/*Proj_8D_fp_display_driver
************************************************************************/



/*IT INTRODUCES

1.	Library subroutine "sprintf" which is like "printf" but sends the resulting string to an array 
	rather than to the display.

2.	Project subroutine "NumericString_to_display_R" which sends the characters saved in "array" to 
	the display.
*/



#include "Proj_8D_header_file.h"
#include <math.h>


void NumericString_to_display_R_local(char*);

FILE  mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE mystdin = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);
FILE uart_input_Double = FDEV_SETUP_STREAM(NULL, uart_getDouble, _FDEV_SETUP_READ);



int main (void){
char op;
char result_str[32];			//[24] needed to scroll 16 char array through 8 char display
double result, x1, x2;			

setup_HW;
stdout = &mystdout;


stdin = &uart_input_Double;
printf("Enter fpn (cr) -op- fpn (cr)\r\
Press sw1 untill display stops moving before repeating\r\
with new op and new data\r");
scanf("%lf", &x1);

while(1){

stdin = &mystdin;
op = getchar();
if(op == 'x'){ SW_reset;}

if(op != '~'){
if((op=='+')||(op=='-')||(op=='*')||(op=='/')||(op=='^')||(op=='%'))
{stdin = &uart_input_Double;
scanf("%lf", &x2);printf("%g %c %g", x1, op, x2);}}

switch(op){
case '+':result = x1+x2; break;
case '-':result = x1-x2; break;
case '*':result = x1*x2; break;
case '/':result = x1/x2; break;
case '^':result = pow(x1,x2); break;
case '%':result = pow(x1,(1/x2)); break;
case '~':result = 1/x1; break;
default: result = x1+0; printf("!!?"); break;}
printf(" = %g  ?",result);

x1 = result; printf("\r");

for(int m = 0; m<=31; m++)result_str[m] = 0;

sprintf(result_str, "%g", result);	
NumericString_to_display_R_local(result_str);}
return 0;}



/*****************************************************************************/
void NumericString_to_display_R_local(char array[]){
char display[8];
int m_max;

m_max = 0; while(array[m_max]){if(array[m_max]=='e'){array[m_max] = 'X';} 
m_max +=1;}m_max -= 1;

if(m_max <= 7){for(int m = 0; m <= m_max; m++)display[m_max - m] = array[m];
I2C_Tx_8_byte_array(display);}	

else{

while (1){
for(int m = 0; m <= 7; m++){display[7-m] = array[m];
I2C_Tx_8_byte_array(display);}

Timer_T0_10mS_delay_x_m(50);

for(int n = 1; n <= m_max+1; n++){while(switch_2_down);
for(int m = 0; m <= 7; m++){display[7-m] = array[m + n];
I2C_Tx_8_byte_array(display);}
Timer_T0_10mS_delay_x_m(25);
if(switch_1_down)break;	}if(switch_1_down)break;}}}


