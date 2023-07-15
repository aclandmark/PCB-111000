

/*
Explores division when carried out using firmware rather than hardware.
 */


#include "Division_subroutine_header.h"


int main (void){
long Num_1, Num_2;
char digits[8];

 setup_HW;
 _delay_ms(10);
 sei();
   
while(1){
String_to_PC_Basic("\r\nNum_1? (>0)\t");

Num_string_from_KBD_Basic_with_Echo(digits);
Num_1 = Askii_to_binary(digits);

String_to_PC_Basic("\r\nNum_2? (>Num_1)\t");
 Num_string_from_KBD_Basic_with_Echo(digits);
Num_2 = Askii_to_binary(digits);

//clear_display;
I2C_Tx_any_segment_clear_all();
_delay_ms(100);
for(int m = 0; m <= 7; m++)digits[m] = 0;

String_to_PC_Basic("\r\nAnswer =   ");

if (Num_1 > Num_2)fraction_to_decimal_string_Local(Num_2, Num_1, digits); 
else fraction_to_decimal_string_Local(Num_1, Num_2, digits); 
String_to_PC_Basic("\r\n");}SW_reset;}



/**************************************************************************************************/
void fraction_to_decimal_string_Local(long remainder, long denominator, char digits[]){
long digit;
int m=6;
//clear_display;
I2C_Tx_any_segment_clear_all();
_delay_ms(100);
//digits[7]= '0' | 0x80;
digits[7]= '0';
digits[6]= '.';

String_to_PC_Basic("0.");

while(switch_2_up){
digit = divide_A_by_B(product_AB(remainder,10),denominator);    //uses local function for divide function
remainder = A_modulo_B(product_AB(remainder,10),denominator);   //uses local function to provide the remainder
Char_to_PC_Basic(digit+'0');
if(m){digits[m-1] = (digit+'0');m--;
//Int_num_string_to_display;
I2C_Tx_8_byte_array(digits);

Timer_T0_10mS_delay_x_m(12);}
Timer_T0_10mS_delay_x_m(6);}
}



/**************************************************************************************************/
long product_AB(long A, long B){                                //Product is simply multiple addition
long product = 0;
while(B){product += A; B--;}                                    //Check with paper and pencil   
return product;}



/**************************************************************************************************/
long divide_A_by_B(long A, long B){                           //Division is multiple subtraction while result exceeds zero
long result = 0;
while(A>0){A-=B;result++;}
if(!(A))return result;
else return result-1;}



/**************************************************************************************************/
long A_modulo_B(long A, long B){                          //Check with paper and pencil 
return (A - product_AB(divide_A_by_B(A,B),B));}



/*******************************************************************************************************/
void Num_string_from_KBD_Basic_with_Echo(char * array_ptr)
{ char keypress;
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { *array_ptr = keypress;
    array_ptr += 1;
    Char_to_PC_Basic(keypress);
  }
  *array_ptr = '\0';
}



/********************************************************************************************************/
long Askii_to_binary(char * array_ptr) {
  long num = 0;
{ int m = 0; while (*(array_ptr + m)) {
      num = num * 10 + (*(array_ptr + m++))  - '0';
    }
  }
  return num;
}



/********************************************************************************************************/
