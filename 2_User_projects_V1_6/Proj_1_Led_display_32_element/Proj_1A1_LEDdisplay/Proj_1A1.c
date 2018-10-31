	
/*Proj_1A1_LEDdisplay
******************************************************/

/*VARIATION ON THE FIRST PROGRAM: 

WHAT’S DIFFERENT ABOUT IT?	 

MORE ABOUT THE C PROGRAMMING LANGUAGE

1.	The "while(1)" loop:
Code following the "while(1) statement and contained within the {} brackets is repeated endlessly.

Note: The result of replacing the “1” between the “()” brackets with a variable is that program execution 
only remains within the “{}” brackets while the variable is other than zero. It is normal for the variable 
to be adjusted every time that the loop is executed until of course it reaches zero when program execution 
exits the “while-loop”.

2.	The presence of a “for-loop” within the “while-loop”.  
The design of C allows programs to be readable despite a considerable degree of complexity. 

3.	The use of the tabs key to indent the code and make it easier to follow.

4. 	The idea that the "{}" brackets always occur in pairs, click on one bracket 
and its companion will also be highlighted.  Note the symmetry of the brackets.

5.	The delay introduced by the "SW_reset" statement. As a result of the “while(1)” statement there is no need 
to perform a SW_reset or put up with the delay that it introduces. However “PORT_1" must be reset to 1 at the 
beginning of each "while-loop".


MORE ON DRIVING THE DEVELOPMENT TOOLS

Use the "Tools/Options/Styles/Brace Match/Text Colour" dialogue to set the colours.  Make sure that the colour 
is very clear, in which case use of the tabs key is no longer really necessary and there is no need for code to 
take up so much space.  

*/


//THE PROGRAM STARTS HERE




#include "Proj_1A1_header_file.h"


int main (void)
{												//start of main routine
unsigned int PORT_1;							
setup_HW;

while(1){										//While loop starts here
	PORT_1=1;									//In the absence of a "SW_reset", "PORT_1" mast be re-initialised every time the loop is repeated
	for(int m = 1; m < 17; m++){				//for loop starts here
		I2C_Tx_2_integers(PORT_1, PORT_1);
		Timer_T0_10mS_delay_x_m(6);
		PORT_1 = PORT_1 << 1;}					//for loop ends here
		}										//while loop ends here
}												//end of main loop
