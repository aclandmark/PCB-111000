# PCB-111000
56 Projects for the novice C programmer

Purpose: To introduce newcomers to C-programming for Atmega micro-controllers using the WinAVR compiler.  Programmers, target devices and a display are all provided on the pcb.  Sample user programs are provided to illustrate various applications such as transferring data to and from a PC, reading switches and driving a display.  Finished projects such as a stop watch, reaction time tester or calculator can be operated independently of a PC provided that a 5V USB supply is available.

PCB 111000 Hardware:
    1. An Atmega 328 that drives an 8 digit 7 segment display and downloads user code to an Atmega 168.
    
    2. An Atmega 168 for which the user writes simple C-programs.
    
    3. Three switches that can be used to provide inputs for the Atmega 168.
    4. An FTDI 230X USB to UART bridge that enables the PCB to be connected to a PC.
    5. A programming interface:  This is normally used to program the Atmega 168 but can also be used to update the Atmega 328 code.
    6. An I2C interface via which user code controls the display.

PCB 111000 Software:
    1. A basic Atmega programmer that resides in the boot partition of the Atmega 328.
    2. An application that drives the display, hosts an I2C interface and includes other functions including a clock/stop watch and arithmetic (known as the mini-OS).
    3. An Atmega 328 programmer that can be downloaded onto the Atmega 168.
    4. Subroutine library to support user code.
