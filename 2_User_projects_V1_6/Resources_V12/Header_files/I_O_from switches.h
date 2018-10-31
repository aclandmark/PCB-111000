/*
For pcbs V2 & 3 
sw1 is PIND7	This switch is at the corner of the pcb
sw2 is PINB6	This switch is between switches 1 and 3
sw3 is PIND2	This switch is next to the DPDT switch
Counting from the DPDT switch the push button switches
are placed in order 3, 2 then 1.

Switches sw1 and sw3 generate a call to ISR(PCINT2_vect) 
Switch sw2 generates a call to ISR(PCINT0_vect)

If PCI is required: Set_up and enable PCI
PCI interrupt call can be delayed. Execute "hold_PCI" then "restore PCI" later on.
To temporarily prevent PCI from generating interrupts, execute "dissable pci".

Commands "clear_PCI_on_sw1_and_sw3" and "clear_PCI_on_sw2" are provided to clear
spurious interrupt calls generated by switch bounce.
*/

volatile char PCMSK0_mem, PCMSK2_mem;

#define Init_display_for_pci_data_entry \
clear_digits;\
digits[0] = '0';\
I2C_Tx_8_byte_array(digits);

/******************************************************************/
#define initialise_OS_chip_for_arithmetic \
clear_digits;\
I2C_Tx_8_byte_array(digits);\
digits[0] = 'A';\
I2C_Tx_2URNs_from_IO(digits);\
Timer_T0_10mS_delay_x_m(5);\
digits[0] = '0';\
I2C_Tx_2URNs_from_IO(digits);


/******************************************************************/
#define switch_1_down	((PIND & 0x80)^0x80)
#define switch_1_up		(PIND & 0x80)
#define switch_3_down	((PIND & 0x04)^0x04)
#define switch_3_up		(PIND & 0x04)
#define switch_2_down	((PINB & 0x40)^0x40)
#define switch_2_up		(PINB & 0x40)

/******************************************************************/
#define restore_PCI_on_sw2  set_up_PCI_on_sw2;
#define restore_PCI_on_sw1_and_sw3  set_up_PCI_on_sw1_and_sw3;
#define restore_PCI  set_up_PCI;

/******************************************************************/
#define setup_and_enable_PCI  config_sw2_for_PCI; config_sw1_and_sw3_for_PCI; 

/******************************************************************/
#define config_sw2_for_PCI  set_up_PCI_on_sw2; enable_pci_on_sw2;
#define config_sw1_and_sw3_for_PCI  set_up_PCI_on_sw1_and_sw3; enable_pci_on_sw1_and_sw3;

/******************************************************************/
#define set_up_PCI_on_sw2  			PCICR |= (1 << PCIE0);
#define set_up_PCI_on_sw1_and_sw3  	PCICR |= (1 << PCIE2);
#define hold_PCI_on_sw2 			PCICR &= (~(1 << PCIE0));
#define hold_PCI_on_sw1_and_sw3  	PCICR &= (~(1 << PCIE2));
#define PCI_on_sw2_on_hold			(!(PCICR & (1 << PCIE0)))
#define PCI_on_sw1_and_sw3_on_hold	(!(PCICR & (1 << PCIE2)))

/******************************************************************/
#define set_up_PCI \
PCICR |= ((1 << PCIE0) | (1 << PCIE2));

/******************************************************************/
#define hold_PCI \
PCICR &= (~((1 << PCIE0) | (1 << PCIE2)));

/******************************************************************/
#define PCIset_up ((PCICR & (1 << PCIE0))  || (PCICR & (1 << PCIE2)))
#define PCIenabled ((pci_on_sw1_and_sw3_enabled) || (pci_on_sw2_enabled))


/******************************************************************/
#define enable_pci_on_sw1_and_sw3  PCMSK2 |= (1 << PCINT18) | (1 << PCINT23);
#define disable_pci_on_sw1_and_sw3  PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));
#define pci_on_sw1_and_sw3_enabled (PCMSK2 & 0x84) == 0x84
#define pci_on_sw1_and_sw3_disabled (PCMSK2 & 0x84) == 0x00

#define enable_pci_on_sw1  	PCMSK2 |= (1 << PCINT23);
#define disable_pci_on_sw1  PCMSK2 &= (~(1 << PCINT23));
#define enable_pci_on_sw3  	PCMSK2 |= (1 << PCINT18);
#define disable_pci_on_sw3  PCMSK2 &= (~(1 << PCINT18));

/******************************************************************/
#define enable_pci_on_sw2  PCMSK0 |= (1 << PCINT6);
#define disable_pci_on_sw2  PCMSK0 &= (~(1 << PCINT6));
#define pci_on_sw2_enabled (PCMSK0 & 0x40) == 0x40
#define pci_on_sw2_disabled (PCMSK0 & 0x40) == 0x00

/******************************************************************/
#define enable_pci  PCMSK0 |= (1 << PCINT6);    PCMSK2 |= (1 << PCINT18) | (1 << PCINT23);
#define disable_pci PCMSK0 &= (~(1 << PCINT6)); PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));

/******************************************************************/
#define save_pci_enabled_state   PCMSK0_mem = PCMSK0; PCMSK2_mem = PCMSK2;
#define restore_pci_enabled_state   PCMSK0 = PCMSK0_mem; PCMSK2 = PCMSK2_mem;


/******************************************************************/
#define clear_PCI_on_sw1_and_sw3  	PCIFR |= (1<< PCIF2);
#define clear_PCI_on_sw2 		 	PCIFR |= (1<< PCIF0);
