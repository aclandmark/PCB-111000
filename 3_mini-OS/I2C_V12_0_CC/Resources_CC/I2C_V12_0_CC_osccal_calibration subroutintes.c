
void initialise_timers_for_cal_error(void);
void start_timers_for_cal_error(void);
long compute_error(char);	
long calibrate_quick_cal(char);
void start_T2_for_ATMEGA_168_cal(char);
long emergency_restore_operation(void);	

		/************************************************************************************/
		void initialise_timers_for_cal_error(void){
		TCNT1=0;TCCR1B = 0;								//Reset and halt T1
		TCCR2B =  0x0;	while(ASSR & (1 << TCR2BUB));	//Halt T2
		TCCR2A = 0; while(ASSR & (1 << TCR2AUB));		//Reset T2 
		TCNT2=0; while(ASSR & (1 << TCN2UB));	}		//Reset TCNT2

		/************************************************************************************/
		void start_timers_for_cal_error(void)
		{TCCR2B = 1; 	
		while(ASSR & (1 << TCR2BUB));
		TCCR1B = 1;}
		
		/************************************************************************************/
		long compute_error(char error_mode)//char OSCCAL_test_value)		
		{long error;
		EA_counter = 0;									//Compute error for each value of OSCCAL 10 times
		error_SUM = 0;
		while(EA_counter < 15);EA_counter = 0;
		error = error_SUM;
		if (error < 0) error *= (-1);
		if (error_mode){buffer[EA_buff_ptr] = error/10;EA_buff_ptr++;}
		return error/10;}
		
		/************************************************************************************/
		long calibrate_quick_cal(char OSCCAL_test)
		{long cal_error, error_mem;	
		T1_OVF=0;
		OSCCAL = OSCCAL_test;		
		cal_error = compute_error(0);						//compute error for OSCCAL_test
		error_mem = cal_error;
		OSCCAL++;											//increment OSCALL
		while(1){
		cal_error = compute_error(0);						//Calculate new error
		if (cal_error > error_mem) { 										//If it gets worse
		OSCCAL--; break;}								//decrement OSCCAL and exit							
		error_mem = cal_error;							
		OSCCAL++;}
		if (OSCCAL != OSCCAL_test)	return error_mem;		//REPLACE with OSCCAL_DV when OSCCAL_test is discarded
		else{																//OSCCAL is still the default value
		OSCCAL--;
		while(1){
		cal_error = compute_error(0);
		if (cal_error > error_mem){ 										//If it gets worse
		OSCCAL++;
		break;}								//increment OSCCAL and exit
		error_mem = cal_error;							
		OSCCAL--;}}
		return error_mem;}
		
		/************************************************************************************/
		void start_T2_for_ATMEGA_168_cal(char cal_mode){
		TCCR2B =  0x0;	while(ASSR & (1 << TCR2BUB));	//Halt T2
		TCCR2A = 0; while(ASSR & (1 << TCR2AUB));		//Reset T2 
		TCNT2=0; while(ASSR & (1 << TCN2UB));
		if(cal_mode){
		TIMSK2 |= (1 << TOIE2);							//Set Timer 2: interrupt on overflow
		TCCR2B = 1; 									//Start T2 with a clock of 32.768Hz
		while(ASSR & (1 << TCR2BUB));}
		else{TIMSK2 &= (~(1 << TOIE2));mode = 0;}}
		
		/************************************************************************************/
		long emergency_restore_operation(void)//char OSCCAL_test_value)		
		{long error;
		EA_counter = 0;									//Compute error for each value of OSCCAL 10 times
		error_SUM = 0;
		while(EA_counter < 2);EA_counter = 0;
		error = error_SUM;
		return error;}
		
		
		