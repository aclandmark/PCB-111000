

/*
In this project the "setup_328_HW_Arduino_IO" code segment has been replaced by "setup_328_HW_Basic_IO".

Arduino provides a comprehensive library for sending data to and from a PC.
However simple DIY (do it yourself) functions are used here to illustate
the steps involved and introduce the Atmega328 data sheet.
 
ASKII coding is introduced.  Every character on the PC keyboard is assigned a number from 32 to 127.
For example 63 is used to send a question mark '?' to the PC.
Decimal 125 is sent as 3 eight bit numbers 49, 50 and 53. 
48 is used to represent zero.
digits 1, 2 and 5 can therfore by extracted by subtracting 48 from 49, 50 and 53.  
5 is then added to 2 times 10 and 1 times 100 to obtain 125 in binary form. 
*/



#include "Receiver_Transmitter_header.h"

int main (void)                          //Example 1
  { setup_HW;
  Char_to_PC_Basic('?');
  newline_Basic();
  while (1)
  { Char_to_PC_Local
    (waitforkeypress_Basic());
  }
  return 1;
  }

  


/************************************************************************************************************

  Use this area for saving the examples when they have been got working and finished with
  Use the space above for the active program (the one being worked on)



********Example 1: Echoes keypresses*************************************************************************
  int main (void)                          //Example 1
  { setup_HW;
  Char_to_PC_Basic('?');
  newline_Basic();
  while (1)
  { Char_to_PC_Local
    (waitforkeypress_Basic());
  }
  return 1;
  }




**********Example 2: Prints out ASKII characters**************************************************************
  int main (void)                          //Example 2
  { char symbol;
  setup_HW;
  newline_Basic();
  symbol = '!';
  while (symbol <= '~')
  { Char_to_PC_Local(symbol);
    _delay_ms(10);
    symbol++;
    wdr();
  }
  waitforkeypress_Basic();
  SW_reset;
  return 1;
  }




********Example 3: Echo character string or prints file*******************************************************
int main (void)                          //Example 3
  { setup_HW;
  while (!(isCharavailable_Basic(65)))
    Char_to_PC_Basic('?');
  newline_Basic();
  Char_to_PC_Basic(Char_from_PC_Basic());
  while (1)
  { if (isCharavailable_Basic(10))
      Char_to_PC_Basic(Char_from_PC_Basic());
    else break;
  }
  String_to_PC_Basic("Done\r\n");
  SW_reset;
  return 1;}
 




********Example 4: Send strings to the PC********************************************************************
int main (void)                          //Example 4
  { setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_3_down)wdr();
  
  String_to_PC_Basic("\r\nDefining and using text strings\r\n");
  const char *message_1 = "Hello world\r\n";
  const char *message_2 = "Sending text to a PC\r\n";
  const char message_3[] = "Writing C programs and\r\n";
  const char message_4[] = "Uploading them to a device\r\n";
  String_to_PC_Basic(message_1);
  String_to_PC_Basic(message_2);
  String_to_PC_Basic(message_3);
  String_to_PC_Basic(message_4);
  while(switch_3_up)wdr();
  while(switch_3_down);
  return 1;
  }  




************Example 5: Generate an ASKII table****************************************************************
int main (void)                          //Example 5
  { char symbol = '!';
  setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_3_down)wdr();
  
  newline_Basic();
  while (symbol <= '~')
  { Num_to_PC_Local(symbol);
    Char_to_PC_Local(symbol++);
    wdr();_delay_ms(50);
    if (!((symbol - '!') % 8))newline_Basic();
    else Char_to_PC_Local('\t');
  }
  while(switch_3_up)wdr();
  while(switch_3_down);
  }
  





*****************Example 6: Sending numbers to the PC*********************************************************
 int main (void)                          //Example 6
  { int i = 0, number = 12345;
  char s[12];
  setup_HW;
  wdt_enable(WDTO_120MS);
  while(switch_3_down)wdr();
  
  do {
    s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)
    Char_to_PC_Basic(s[m - 1]);
 
 newline_Basic();
 while(switch_3_up)wdr();
  while(switch_3_down);
  return 1;
  } 




**************Example 7: Entering numbers at the keyboard*****************************************************
int main (void)                          //Example 7
  { long num = 0;
  char keypress;
  setup_HW;
  num = 0; Char_to_PC_Basic('?');
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { num = num * 10 + keypress  - '0';
    I2C_Tx_long(num);
  }
  Num_to_PC_Local(num * 2);
    I2C_Tx_long(num * 2);
    while(switch_3_up);
    newline_Basic();
  SW_reset;
  return 1;
  }  



*******Example 8: Simple arithmetic Data to & from PC*********************************************************
int main (void)                          //Example 8
{ long num = 0;
  char numLength;
  char Num_string[12];
  
  setup_HW;
  String_to_PC_Basic("?");
  Num_string_from_KBD_Basic(Num_string);

  { int m = 0; while (Num_string[m])
      Char_to_PC_Basic(Num_string[m++]);
  }

  num = Askii_to_binary(Num_string);

  num = num * 2;
  String_to_PC_Basic("\r\n");

  numLength = Binary_to_Askii(num, Num_string);

  for (int m = numLength; m > 0; m--)
    Char_to_PC_Basic(Num_string[m - 1]);

  I2C_Tx_long(num);
while(switch_3_up);
    newline_Basic();
  SW_reset;
  return 1;
}



*************Example 9: Arithmetic: More on pointers********************************************************
int main (void)                          //Example 9
  { char keyboard_input;
  char Num_string[12];
  long Num, A = 55; long B = 7; long Div; long mod;
  int no_decimal_places;

    setup_HW;
  String_to_PC_Basic("?\r\n");
  
  for(int m = 0; m <=2; m++)
  {Num_string_from_KBD_Basic(Num_string);
  Num = Askii_to_binary(Num_string);
  switch (m)
  { case 0: A = Num; Char_to_PC_Basic('?');break;
  case 1: B = Num; String_to_PC_Basic("?\r\n");break;
  case 2: no_decimal_places = Num; break;}}

  divide(A, B, &Div, &mod, no_decimal_places);
  Num_to_PC_Local(Div);
  String_to_PC_Basic(". ");
    while (no_decimal_places) {
  no_decimal_places =    divide(mod*10, B, &Div, &mod, no_decimal_places);
  Num_to_PC_Local(Div);}
  String_to_PC_Basic("\r\n");
   SW_reset;
  return 1; }

  long divide(long A, long B, long *Div, long *mod, int no_decimal_places)
  {*Div = A/B;
  *mod = A%B;
  no_decimal_places -= 1;
  return no_decimal_places; } 



************This area is for project subroutines*************************************************************/
void Num_string_from_KBD_Basic(char * array_ptr)
{ char keypress;
  while ((keypress = waitforkeypress_Basic()) != '\r')
  { *array_ptr = keypress;
    array_ptr += 1;
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
int Binary_to_Askii (long number, char * array_ptr)
{ int i = 0;
  do {
    array_ptr[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  array_ptr[i] = '\0';
  return i;
}


/********************************************************************************************************/
void Char_to_PC_Local(char data)
{ while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;
}




/********************************************************************************************************/
void Num_to_PC_Local (long number)
{ int i = 0;
  char s[12];
  
  do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');
}



/********************************************************************************************************/
