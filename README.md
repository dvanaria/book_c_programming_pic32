# book_c_programming_pic32
Programming 32-bit Microcontrollers in C (Exploring the PIC32)
Lucio Di Jasio
2008

------------------------------------------------------------------------------
Overview:

    The book is set up as 16 "Days" worth of material:

        Each chapter explores one basic hardware peripheral of the PIC32MX
        family of microcontrollers, and one aspect of the C language per day.

    Recommended: 

        Microcontroller: PIC32MX360F512L (100 pin Plug In Module)
        MCU Core: MIPS M4K
        Development Board: Explorer 16 Dev Board
        IDE Software: MPLAB IDE (version 8)
        Hardware Programmer: PICkit3 

    All of the above hardware and software is made by Microchip Technology
    Incorporated (headquartered in Chandler, AZ), with the exception of the
    microcontroller core design, which is made by MIPS (San Jose, CA).

------------------------------------------------------------------------------
Specs on the Hardware:

  The MCU:

    Microchip Technology Inc. 
    PIC32MX360F512L (on the Explorer 16 Dev Board)
    80 MHz (configured to 72 MHz)
    32-bit MIPS M4K Core
    512K Flash ROM (plus 12K Boot Flash) - stores program
    32K RAM - stores data and variables
    Internal 8 MHz (FRC) and 32 kHz (LPRC) oscillators

  The Dev Board:

    Microchip Explorer 16 Development Board DM240001
    Part Number: DM240001
    Supports processor PIMs for both PIC24 and dsPIC families
    Alpha-numeric 16 x 2 LCD display
    Interfaces to MPLAB ICD 3, MPLAB REAL ICE, and PICkit3 programmers
       * needed to program the 512K Flash memory with new code/program
    Includes Microchip's TC1047A analog output temperature sensor
    Expansion connector for full device pin-out and bread board prototyping
    PICTail Plus connector for expansion boards
    Full documentation in download section below: user's guide, schematics
    External to PIM: 8 MHz (POSC) and 32768 Hz (SOSC) oscillators

------------------------------------------------------------------------------
Hardware Setup:

    1. Windows host computer running MPLAB IDE 8.91

    2. PICkit3 programmer plugged into Explorer 16 board (LEDs face the
        PIC32 PIM module) and the other end (USB) plugged into the host
        computer.

    3. Plug power into Explorer 16 board (optional - you can also set
                                          the PICkit3 to provide power
                                          to the board directly, via the IDE
                                          settings)

------------------------------------------------------------------------------
The Programmer (PICkit3):

    In order to "program" the PIC32, you need a physical device in between
    your dev board and your computer. There are many devices made for this
    (all proprietary Microchip products):

        REAL ICE          (In-Circuit Emulator device)
        ICD 2             (In-Circuit Debugger device)
        PIC32 Starter Kit (poorly-named hardware device / programmer)
        PICkit3           (In-Circuit Debugger/Programmer)

    How do these connect to the Dev Board?

        ICSP/ICD port (for the REAL ICE and ICD 2)
        JTAG port     (for the PIC32 Starter Kit device)
        PICkit2 port  (for the PICkit3)

    The PICkit3 supercedes the other devices and it's all you need for every
    project in the book.

    How to use:

          By default, the programmer does not provide power to the dev
          board. Use the MPLAB IDE to set the programmer so the PICkit3 
          will provide power to the Explorer 16 board, otherwise the MPLAB 
          IDE will not be able to detect the Explorer board.

          In MPLAB, go to:

              Programmer -> Settings -> PICkit3 Settings -> Power

                     "Power target circuit from PICkit3"

          Also note: The orientation of the PICkit3 on the Explorer 16 Dev
          Board is not at all intuitive or well labeled. With the Dev Board
          facing you (all words on the silkscreen are correctly oriented to
          read), the programmer's LED's should face in toward the center of
          the board.

------------------------------------------------------------------------------
Using MPLAB IDE:

    Menu options:

        Project (Build All)
        Debugger (Select Tool, Animate, Run, Halt, Step Into, etc)
        Programmer (Select Tool, Program, Verify, Erase, Settings, etc)

    The concept: You LOAD a specific tool into the IDE, either the Debugger
                 or the Programmer (not both, it has to be one or the other).

                 The Debugger allows you to step-through your program line
                 by line (either each C instruction or each assembly 
                 instruction) and view the contents of Special Function
                 Registers within the PIC32.

                 You can run the Debugger either on real hardware or on
                 the built-in simulator (MPLAB SIM). The simulator has the
                 added benefit of displaying a signal analyzer.

                 The Programmer allows you to just program the PIC32 Flash
                 memory and run the code on the hardware itself. The MPLAB
                 SIM is not an option here.

    Here are the options for the Debugger (under Debugger -> Select Tool)

           MPLAB REAL ICE
           PIC32 Starter Kit
           MPLAB ICD 2
           PICkit3
           MPLAB SIM

        The first half of the book can just use the simulator (MPLAB SIM)
        Debugger, not actually programming the board with the PICkit3. The 
        second half of the book requires programming the hardware itself.

------------------------------------------------------------------------------
Setting up the Primary Oscillator Clock Chain

    This book really should have started with Day 7, which talks about all
    the configuration options that are set each time a new program is flashed
    to the PIC32MX chip.

    These configuration options select which oscillator feeds into the Primary
    Oscillator Clock Chain (which then defines the System Clock and the 
    Peripheral Bus Clock, both used extensively in Chapters 1-6!!)

    This configuration can be set to be programmed whenever you flash new code
    to the PIC32MX, or can be programmed dynamically in source code. For this
    book (at least for the early chapters), we'll be setting them directly.

    The high-level idea:

        1. Select the external fast (8 MHz) oscillator
        2. Hook this up to a PLL (Phased Locked Loop) in order to scale
           the incoming signal.
        3. An input divider is set to 1:2 (scales 8 MHz to 4 MHz)
        4. The PLL scales this to x18 (producing 72 MHz)
        5. An output divider is set to 1:1 (no effect)
        6. The SYSTEM_CLOCK is then set to 72 MHZ
        7. This signal is divided by 2 to define the PERIPHERAL BUS CLOCK
           (now resulting in 36 MHz)

    All this is configured by setting just 3 special function registers
    (because these are each 32-bits wide). Within these SFRs are individual
    Fields which are set accordingly: 

        In the IDE, go to Configure -> Configuration Bits, and set the 
        following:

        PLL Input Divider                    2x Divider
        PLL Multiplier                       18x Multiplier
        System PLL Output Clock Divider      PLL Divide by 1

        Oscillator Selection Bits            Primary Osc w/PLL 
        Secondary Oscillator Enable          Disabled
        Internal/External Switch Over        Disabled
        Primary Oscillator Configuration     XT osc mode
        CLKO Output Signal Active on OSCO    Enabled
        Peripheral Clock Divisor             Pb_Clk is Sys_Clk/2
        Clock Switching and Monitor Select   Clock Switch Disabled
        Watchdog Timer Postscaler            1:1048576
        Watchdog Timer Enable                WDT Disabled

        Background Debugger Enable           Debugger is disabled
        ICE/ICD Comm Channel Select          ICE EMUC2/EMUD2 pins shared
        Program Flash Write Protect          Disable
        Boot Flash Write Protect Bit         Protection Disabled
        Code Protect                         Protection Disabled


    System Clock = 72 MHz
    Peripheral Bus Clock = 36 MHz


------------------------------------------------------------------------------
------------------------------------------------------------------------------
Introduction:

    Written in 2008, author has background in 8-bit microcontrollers and
    assembly language. Now migrating to Microchip's 16-bit and 32-bit MCUs.

    Microchip Inc. has a horrible way of naming their chips:

        PIC16   (actually an 8-bit MCU)
        PIC24   (actually a 16-bit MCU)
        PIC32   (got it right!)

    "The PIC32 is a powerful machine based on a well established 32-bit
     core (MIPS)"

        MIPS (Microprocessor without Interlocked Pipelined Stages) is a 
        family of reduced instruction set computer (RISC) instruction set 
        architectures (ISA) developed by MIPS Computer Systems, now MIPS 
        Technologies, based in the United States.

        There are multiple versions of MIPS: including MIPS I, II, III, 
        IV, and V; as well as five releases of MIPS32/64 (for 32- and 64-bit 
        implementations, respectively). The early MIPS architectures were 
        32-bit; 64-bit versions were developed later. As of April 2017, the 
        current version of MIPS is MIPS32/64 Release 6. MIPS32/64 primarily 
        differs from MIPS I–V by defining the privileged kernel mode System 
        Control Coprocessor in addition to the user mode architecture. 


    Documentation needed for the PIC32MX360F512L (all published by Microchip
    Technology Inc):

        PIC32 DataSheet (214 pages, "PIC32MX3xx4xx_datasheet.pdf") 
        PIC32 Family Reference Manual (1138 pages) 

        MIPS M4K DataSheet (47 pages)
        MIPS M4K Software User's Manual  (235 pages)

        Explorer 16 Development Board Users Guide (50 pages)

    --------------------------------------------------------------------------
    Microchip Explorer 16 Development Board DM240001

    (From User Guide)

    1. The Explorer 16 board has been designed to accommodate both permanently 
       mounted (i.e., soldered on) and detachable PIM processors. Slider 
       switch, S2, allows the user to choose which processor to use. 

           Besides the big PIM module, there is a PIC18LF4550 8-bit micro-
           controller on the board. It runs at 48 MHz and provides 32 K of
           program storage. It's unclear if this is the chip that would be
           selected if sliding S2 to PIC instead of PIM, or if this MCU is
           just being used as a controller for the USB-B port.

    2. An unregulated DC supply of 9V to 15V (preferably 9V) supplied to J12.
       For default functionality, a power supply with a current capability of 
       250 mA is sufficient. The Explorer 16 kit does not include a power 
       supply.


------------------------------------------------------------------------------
Day 1: The Adventure Begins (PortA)

    This chapter focuses on the embedded programming version of "hello world",
    light up some LEDs. A peripheral is examined: PortA. This is a 12-pin
    input/output peripheral routed to 12 pins on the PIC32 itself. It is 
    controlled primarilty by two SFRs:

        PORTA   (set 1 for +3.3V output, set 0 for 0.0V output)
        TRISA   (set each pin as an Input or an Output)

    8 of these pins are connected to LEDs on the Explorer 16 board. These LEDs
    live right below the PIM module and are in order: bits 7 6 5 4 3 2 1 0.

        PORTA is 32-bits, so: 0x000000E1
              will turn on LEDs in this fashion: 11100001

    The "Hello World" program introduces the MPLAB IDE and it's built-in MPLAB
    SIM simulator.

    **************************************************************************
    Using MPLAB IDE:

        1. New Project
        2. Select device PIC32MX360F512L
        3. Select PIC32 C-Compiler Tool Suite
        4. Create new file, then Add to Project
        5. Set Configuration Bits for 72 MHz System Clock 
                                  and 36 MHz Peripheral Bus Clock
        6. Include needed header file:

             #include <p32xxxx.h>

           Will include file p32mx360f512l.h (this is why we have to select
                                              a device)

           Where is this file? Project -> Set Language Tool Locations ->
                 -> Microchip PIC32 C-Compiler Toolsuite
                 -> Default Search Paths & Directories
                 -> Library Search Path

         C:\Program Files (x86)\Microchip\MPLAB C32 Suite\pic32mx\include
                   -> p32xxxx.h

         C:\Program Files (x86)\Microchip\MPLAB C32 Suite\pic32mx\include\proc
                   -> p32mx360f512l.h

              This file is 444 KB in size and is about 10,000 lines of text.
 
    The header file contains a long list of definitions for all the internal
    special-function registers (SFRs).

        * Found this on a separate video: "The peripherals of the PIC32 can
          be thought of as dozens of separate coprocessors - they are set
          to do work without any CPU support. You set and control these 
          peripherals through a large set of registers in memory space called
          Special Function Registers."

    Running a program:

        1. The MCU will first execute a short initialization code segment
           auto-inserted by the MPLAB C32 linker, known as Startup Code,
           or "crt0 code".

        2. The main() function is executed
 
    I/O pins on the PIC32:

        - Arranged in modules or "ports" of 16 pins each, named A, B, C, D, 
          E, F, G, H (8 ports total).

              Some have fewer. PortA has 12 pins for example.

        - Each port has several SFRs assigned to control its operation.

        - Naming:

              TRISA (SFR, 32-bits) Set each pins direction: Input or Output
              PORTA (SFR, 32-bits) Set each pins value: 0 or 1

    Build All (Project):

        1. Compiler is invoked, generating object code (.o)
           All the addresses of functions and variables are yet undefined.

        2. Linker is invoked, proper memory position established. Produces
           executable (.hex)

        3. All Source Files will be compiled. All Object Files will be linked
           together. All Library Files will be used also.

        4. A Linker Script can be used for advanced config: change the order
           and priority of data and code sections (book uses default).

    The Linker Script:

        - Informs the linker about the SFRs predefined position in memory 
           (according to the selected device datasheet). Also lists the 
           amount of RAM available, amount of Flash memory available, etc.

        - Open procdefs.ld
 
                C:\Program Files (x86)\Microchip\MPLAB C32 Suite
                  \pic32mx\lib\proc\32MX360F512L\procdefs.ld

        - Here are some critical entry points:

            /***********************************************************
             * Memory Address Equates
             * _RESET_ADDR      -- Reset Vector
             * _BEV_EXCPT_ADDR  -- Boot exception Vector
             * _DBG_EXCPT_ADDR  -- In-circuit Debugging Exception Vector
             * _DBG_CODE_ADDR   -- In-circuit Debug Executive address
             * _DBG_CODE_SIZE   -- In-circuit Debug Executive size
             * _GEN_EXCPT_ADDR  -- General Exception Vector
             ***********************************************************/          

    Building the First Project:

        1. Build All

        2. Debugger -> Select Tool -> MPLAB SIM

        3. Open Configure -> Settings -> Debugger (tab)

               Automatically save files before running
               Remove breakpoints upon importing a file
               Reset device to the beginning of main function

        4. Open View -> Watch, add the PORTA SFR

        5. Press the Simulator Reset button

        6. Step through program, see value of PORTA change.

        ******************************************************************
        * Important note when using the simulator: That "Simulator Reset
          Button" is the key - it sets the green arrow / breakpoint to
          the main() function, and allows you to step through the program
          from there, using the step-into and step-over buttons.
        ******************************************************************

    A closer look at PortA:

        - each pin (there are 12 of them) is driven by some
          somewhat complex logic, mainly 3 signals that reach
          the output pin (each output pin). Essential in 
          configuring each pin to be Input or Output.

        - default is Input pin for each pin in Port A

        - Another SFR, called TRISA, allows you to configure
          the direction of each pin in PortA. 

              TRISA = 0;      /* sets each pin to Output */

    JTAG Port (testing on real hardware, one more step):

        - PortA covers 16 pins of the PIC32 chip (the PIC32MX360F512L
          has 100 pins total). These pins are multiplexed with another
          function however, the in-circuit programmer and/or debugger,
          known as ICSP/ICD (historically) and the JTAG interface (the
          newer method of communicating with the programmer/debugger.)

        - By default, some of the pins (RA0, RA1, RA4, RA5) are set as
          JTAG port pins, and not I/O pins (in the case of the PIC32
          Starter Kit board) and the Explorer 16 board is the same.

        - You need to disable the JTAG port in order for those pins
          to be used as I/O pins, and get to the LEDs they are attached
          to:

              DDPCONbits.JTAGEN = 0;

        CLARIFICATION:

            I'm not using any debugger or programmer except the PICkit3,
            which has it's own port on the Dev Board. I'll need to disable
            the JTAG port (4 pins) so that I can use them as regular I/O
            pins, since I'm not using any programmer that needs those pins
            to communicate with the PIC32.

    Testing PortB:
    
        This port is multiplexed with the ADC peripheral, and by default this
        port is used as input pins to this peripheral. In order to turn off
        the ADC input functionality (and thus set these pins as digital output
        pins) we need to write to another SFR:

            TRISB = 0;           // all PORTB as output
            AD1PCFG = 0xffff;    // all PORTB as digital

    Final project code for Day 1:

        /*
        **Hello Embedded World!
        */
        #include <p32xxxx.h>

        main() {
          DDPCONbits.JTAGEN = 0;
          TRISA = 0;
          PORTA = 0xE1;
        }

      This will light up the following LEDs:  1110 0001


    Another solution (using PortB):

        /*
        **Hello Embedded World!
        */
        #include <p32xxxx.h>

        main() {
          TRISB = 0;
          AD1PCFG = 0xffff;    // all PORTB as digital
          PORTB = 0xff;
        }

    Memory Usage Gauge:

        The startup code uses about 490 words of memory

    Output pins can be configured to yield 5.0V (as opposed to 3.3V) by
    setting the ODCx control registers. Input pins can tolerate 3.3V or
    5.0V so no need to configure.
 
    Note on MPLAB Projects:

        Hello (project folder) 1.14 MB

            HelloWorld.elf                           161 KB
            HelloWorld.hex
            HelloWorld.map                            46 KB
            HelloWorld.mcp   (MPLAB Project File)
            HelloWorld.mcw   (MPLAB Workspace)       968 KB
            HelloWorld.c
            HelloWorld.d
            HelloWorld.o

    Disassembly Listing View:

        Very cool disassembly view, showing the assembly code interwoven
        within the C code.

    View -> View Disassembly:
    **********************************************************************
    1:                   /*
    2:                   **Hello Embedded World!
    3:                   */
    4:                   #include <p32xxxx.h>
    5:                   
    6:                   main() {
    9D0000D8  27BDFFF8   addiu       sp,sp,-8
    9D0000DC  AFBE0004   sw          s8,4(sp)
    9D0000E0  03A0F021   addu        s8,sp,zero
    7:                     DDPCONbits.JTAGEN = 0;
    9D0000E4  3C03BF81   lui         v1,0xbf81
    9D0000E8  8C62F200   lw          v0,-3584(v1)
    9D0000EC  7C0218C4   ins         v0,zero,3,1
    9D0000F0  AC62F200   sw          v0,-3584(v1)
    8:                     TRISA = 0x00;
    9D0000F4  3C02BF88   lui         v0,0xbf88
    9D0000F8  AC406000   sw          zero,24576(v0)
    9:                     PORTA = 0xe1;   // 1110 0001
    9D0000FC  3C02BF88   lui         v0,0xbf88
    9D000100  240300E1   addiu       v1,zero,225
    9D000104  AC436010   sw          v1,24592(v0)
    10:                  }
    9D000108  03C0E821   addu        sp,s8,zero
    9D00010C  8FBE0004   lw          s8,4(sp)
    9D000110  27BD0008   addiu       sp,sp,8
    9D000114  03E00008   jr          ra
    9D000118  00000000   nop         
    **********************************************************************


------------------------------------------------------------------------------
Day 2: Walking in Circles (Timer1)

    This project expands on Hello World by using another peripheral, Timer1,
    to systematically turn LEDs on and off. Also covered:

    1. Creating a main function that loops forever, instead of the flow of
       code going straight to exit()

    2. Peripheral in this module: 16-bit Timer1, driven by Peripheral Bus
       Clock with selectable prescaler.

    3. Features of the MPLAB SIM tool: Animate mode, and Logic Analyzer view

    Project -> Project Wizard
    Device: PIC32MX360F512L
    Active Toolsuite: Microchip PIC32 C-Compiler Toolsuite
    Configuration Bits: Set SC = 72 MHz and PBC = 36 MHz

    *************************************************************************
    Key concept: "When the main() function terminates and returns back to the
                  startup code crt0, a new function _exit() is called and the
                  PIC32 remains stuck there in a tight loop from which it can
                  escape only if a processor reset is performed"

    A proper application main loop is needed so it doesn't go to this _exit()

    While loops in C:

        while(a > 1) {
          a--;
        }

        * remember that in C, anything which evaluates to numeric 0 will be
          treated as "false", and any other value is "true"

    The structure of every embedded control program written in C:
 
        Two parts: initialization, and main loop

        #include <p32xxxx.h>
        main() {

            // initialization
            DDPCONbits.JTAGEN = 0;
            TRISA = 0xff00;  // 1=in 0=out

            // main loop
            while(1) {
                PORTA = 0xff;  // turn pins 0-7 on
                PORTA = 0;     // turn all pins off
            }
        }
    
    To run this:

        Debugger -> Select Tool -> MPLAB SIM
        Debugger -> Animate
        View -> Watch -> "Add SFR" button (PORTA selected)
        Step through code to see PORTA value toggle between 00 and FF
    
    To control runtime speed:

        Debugger -> Settings -> Animation Real Time Updates

        Then run the program using the >> button (Animate)

        The program will run and show you each line it's executing.

    Timer1:  

    To run on real hardware, you'll need to utilize the Timer1 peripheral
    to introduce a delay between toggling the PORTA value (otherwise the
    LED will toggle several million times a second).

    There are 3 SFRs that control most of Timer1's functions:

        TMR1    16-bit counter value (0 to 65,535)

        T1CON   32-bit SFR, enable Timer1, set Prescaler value
 
        PR1     rollover value for TMR1 (default is 65,535)

    The T1CON register is 32-bits, and you have to look at the datasheet
    for it's definition, but we'll set it like this:

        1000 0000 0011 0000       0x8030
 
        TCS   = 0  (bit 1)   0 = use main MCU clock as source (36 MHz)
        TSYNC = 0  (bit 2)   0 = synchronization not needed
        TCKPS = 11 (bit 4,5) 3 = to set prescaler to maximum 1:256
        TGATE = 0  (bit 7)   0 = input gating not needed
        SIDL  = 0  (bit 13)  0 = don't worry about behavior in IDLE mode
        TON   = 1  (bit 15)  1 = activate timer

        4 prescalers available:

            00 = 1
            01 = 8
            10 = 64 
            11 = 256

    CALCULATE DELAY:
    **************************************************************************

               36 MHz PBC = (1/36000000=0.00000002777) = 27.7ns

               Each tick (wth 256 prescaler) takes = 27.2 * 256 = 7.11us 

               We want to blink the LED twice per second, on=256ms, off=256ms

               How many times does 7.11us go into 256 ms?
                   256000 / 7.11 = 36,006 

               DELAY = 36006

    **************************************************************************

    Running the code in the simulator using the Logic Analyzer:

        Debugger -> Select Tool -> MPLAB SIM

              Make sure Trace All is configured:
              Debugger -> Settings -> Osc/Trace: Trace All
 
        View -> View Logic Analyzer

        1. The Animate feature is good for stepping through the code 
           automatically, and viewing internal registers.

        2. The Logic Analyzer is good for viewing electrical signals
           on the 100 pins coming out of (and going into) the PIC32.

        The simulator is WAY slower than real hardware. For instance, to see
        the signal toggle in the Logic Analyzer, you'll have to change the
        DELAY from 36006 to 36!

    A few notes:

        1. Binary OR is different than Logical OR:

                Binary OR will go bit by bit and compare each bit

                       1011000
                       1110000
                       -------
                       1111000

                Logical OR will take all the bits together as a value True or
                False, and the result is either True (1) or False (0)

                       1011000   ->  1
                       1110000   ->  1
                       -------   
                       0000001   ->  True

        2. In most C programs, you expect to return to the operating system,
           but there is none in the PIC32 (well, there are several RTOS 
           available for the PIC32, but often are overkill for projects).

        3. The MPLAB C32 tool suite comes with a complete set of standard
           C libraries and an additional set of peripherals libraries to
           simplify and standardize the use of all the internal resources
           of the PIC32. 

               WriteTimer1(0);
               OpenTimer1( T1_ON | T1_PS_1_256, 0xFFFF);

           Is this really easier? Plus, what side-effects do these library
           calls make?

           But for complex peripherals, like the DMA logic, the library offers
           a useful interface and is worth it.

    **************************************************************************
    T1CON = 0x8030;

        0000 0000 0000 0000 1000 0000 0011 0000

              bit 15 = 1 (Timer is enabled)

              bit 5 = 1
              bit 4 = 1  (1:256 prescale value)

                  Register 14-1: T1CON: Type A Timer Control Register
                     bit 31-16 Unimplemented: Read as ‘0’
                     bit 15 ON: Timer On bit(1)
                        1 = Timer is enabled
                        0 = Timer is disabled
                     bit 14 Unimplemented: Read as ‘0’
                     bit 13 SIDL: Stop in Idle Mode bit
                        1 = Discontinue operation when device enters Idle mode
                        0 = Continue operation when device enters Idle mode
                     bit 12 TWDIS: Asynchronous Timer Write Disable bit
                        1 = Writes to TMR1 are ignored until pending write 
                            operation completes
                        0 = Back-to-back writes are enabled (Legacy 
                            Asynchronous Timer functionality)
                     bit 11 TWIP: Asynchronous Timer Write in Progress bit
                        In Asynchronous Timer mode:
                          1 = Asynchronous write to TMR1 register in progress
                          0 = Asynchronous write to TMR1 register complete
                        In Synchronous Timer mode:
                          This bit is read as ‘0’.
                     bit 10-8 Unimplemented: Read as ‘0’
                     bit 7 TGATE: Timer Gated Time Accumulation Enable bit
                        When TCS = 1:
                            This bit is ignored.
                        When TCS = 0:
                            1 = Gated time accumulation is enabled
                            0 = Gated time accumulation is disabled
                     bit 6 Unimplemented: Read as ‘0’
                     bit 5-4 TCKPS<1:0>: Timer Input Clock Prescale Select bits
                           11 = 1:256 prescale value
                           10 = 1:64 prescale value
                           01 = 1:8 prescale value
                           00 = 1:1 prescale value
                     bit 3 Unimplemented: Read as ‘0
                     bit 2 TSYNC: Timer External Clock Input Synchronization 
                                  Selection bit
                           When TCS = 1:
                              1 = External clock input is synchronized
                              0 = External clock input is not synchronized
                           When TCS = 0:
                              This bit is ignored.
                     bit 1 TCS: Timer Clock Source Select bit
                          1 = External clock from TxCKI pin
                          0 = Internal peripheral clock
                     bit 0 Unimplemented: Read as ‘0’
    **************************************************************************

------------------------------------------------------------------------------
Day 3: Message in a Bottle

    Do-While Loops (one full iteration BEFORE logic test performed):

        do {

            i = i + 1;

        } while(TIME < DELAY);

    Variable Declarations (and Data Types / Sizes):

        The MPLAB C32 compiler defaults immediately to 32-bit for the basic
        integer type 'int' and makes a 'long' a synonym for it. In 16-bit
        compilers, int would be 16-bit and long would be 32-bit.


        long   32-bit    signed    -2,147,483,648  to  2,147,483,647
        int    32-bit    signed    -2,147,483,648  to  2,147,483,647
        short  16-bit    signed           -32,768  to         32,767
        char    8-bit    signed              -128  to            127

        unsigned char    ranges from 0............255
        unsigned short   ranges from 0.........65,535
        unsigned int     ranges from 0..4,294,967,295
        unsigned long    ranges from 0..4,294,967,295

        long long     64-bit integer

        float         32-bit floating point
        double        64-bit floating point

    For Loops:

        for(i=0; i<DELAY; i++) {
    
            //work

        }

    Arrays:

        char c[10];    10 x 8-bit integers
        short s[10];   10 x 16-bit integers
        int i[10];     10 x 32-bit integers

    Day 3's Project: lighting up the LED set attached to PortA in such a way
    that when the Explorer 16 board is moved quickly back and forth, a message
    will appear in big letters.

        Use Logic Analyzer: 

            1. Go to Debugger -> Settings -> Osc/Trace tab

                   check Trace All

            2. Then go to View -> Simulator Logic Analyzer

            3. Click on the Channels button, add output device pins such as 
               RA0 for PortA. For this project (Day 3, Message) select all the
               pins from RA0 to RA7. 

            4. Remember to reduce delay values by a factor of about 100 in 
               order for the scale of the signal view to be usable/meaningful.

            5. Neat feature: put the cursor (in the code editor) where you 
               want the simulator to run to and pause. For Day 3 project, go
               to the section of the LONG_DELAY so you can see the HELLO 
               signals on the output pins. Put the cursor there, then r-click
               and select Run to Cursor. 

    About the ++ and -- operators in the C Language:

        At the start, if a=0 and b=1:

            a = b++;    (now a=1 and b=2)

        Or we can do it this way:

            a = ++b;    (now a=2 and b=2)

    The role of the startup (crt0) code:

        1. An array declared in your code will be moved from Flash memory 
           (where all the program code resides before running) and allocated
           in RAM, and this happens BEFORE the main() function gets called.
        2. All globally declared variables will be initialized to 0 by the
           crt0 code. This could cause a delay if you have a big global
           array.
        3. You can define a special function:

              void _on_reset(void)

           if there is time-critical code that needs to run immediately upon
           power reset (to avoid that potential problem with large allocations
           before main() runs).
        

------------------------------------------------------------------------------
Day 4: Numbers

    The PIC32 has 32 registers (32-bits wide), and a 32-bit ALU. That's a LOT
    of registers!

    So the MPLAB C32 compiler assigns the following bit-sizes per datatype:

        char          8-bits
        short        16-bits
        int          32-bits
        long         32-bits
        long long    64-bits
        float        32-bits
        double       64-bits

    As far as PERFORMANCE, the 32-bit CPU can work as effienctly on 8, 16,
    or 32 bit values (one cycle each for math operations, thanks to the MMU
    and MDU units in the PIC32). 64 bit values will take a slight performance
    hit though.

    As far as RESOURCES (limited RAM), datatype choices have direct 
    consequences. For example, 8-bit values (char) only take up 1 byte of 
    memory, not 4.


    View -> Dissassembly Listing

        Using the Dissassembly Listing we can see exactly how the compiler
        implements the higher-level C code at a lower level (PIC32 assembly).

            main() {

                int i,j,k;

                i = 1234;
                j = 5678;
                k = i * j;
            }

        And the Dissassembly Listing:

            (Notice how each instruction takes up 4 bytes of memory)

            1:                   main() {
            9D0000D8  27BDFFE8   addiu       sp,sp,-24        (E8 = -24)
            9D0000DC  AFBE0014   sw          s8,20(sp)
            9D0000E0  03A0F021   addu        s8,sp,zero
            2:                   	int i,j,k;
            3:                   	i = 1234;
            9D0000E4  240204D2   addiu       v0,zero,1234
            9D0000E8  AFC20000   sw          v0,0(s8)
            4:                   	j = 5678;
            9D0000EC  2402162E   addiu       v0,zero,5678
            9D0000F0  AFC20004   sw          v0,4(s8)
            5:                   	k = i * j;
            9D0000F4  8FC30000   lw          v1,0(s8)
            9D0000F8  8FC20004   lw          v0,4(s8)
            9D0000FC  70621002   mul         v0,v1,v0
            9D000100  AFC20008   sw          v0,8(s8)
            6:                   }
            9D000104  03C0E821   addu        sp,s8,zero
            9D000108  8FBE0014   lw          s8,20(sp)
            9D00010C  27BD0018   addiu       sp,sp,24
            9D000110  03E00008   jr          ra
            9D000114  00000000   nop   


    MIPS M4K Core ISA

        To make sense of the disassembled code above, knowledge of the MIPS
        M4K ISA is needed. Like all other ISAs, this defines the number of 
        registers, their definitions/names, the instruction types and 
        instruction encoding, as well as the overall architecture such as the
        LOAD-STORE architecture MIPS uses. 

        The LOAD-STORE architecture is also known as "register-to-register"
        architecture. Basically, it means all operations (math/logic) are
        performed only on REGISTER contents, and so to work with RAM, values
        must be brought in from RAM (LOAD) and results must later be stored
        back into RAM (STORE).

    Here's a very simplified view of the MIPS M4K Core ISA in order to under-
    stand the disassembly above:

        register v0    (work register)
        register v1    (work register)
        register s8    (stores RAM address of variable:  s8   -> i
                                                         s8+4 -> j
                                                         s8+8 -> k
        register sp    (stack pointer)
        register zero  (holds hard-wired value of 0x00000000)

    Here's some pseudocode and corresponding instructions:

        1. RAM memory is allocated for the three variables, and s8 retains
           the 32-bit RAM address of the first one, i. The others are offset
           by 4 bytes each:

                 s8   -> i
                 s8+4 -> j
                 s8+8 -> k

        2. An immedate value of 1234 is loaded directly into the v0 register.
           The contents of v0 are stored at RAM[s8].  i --> 1234

        3. An immedate value of 5678 is loaded directly into the v0 register.
           The contents of v0 are stored at RAM[s8+4].  j --> 5678

        4. LOAD v1 with contents of RAM[s8]      (this is i)
           LOAD v0 with contents of RAM[s8+4]    (this is j)
           Multiply v1 and v0, save result in v0
           STORE v0 at location RAM[s8+8]        (this is k)
           

    Some notes about the RAM addresses and instruction encoding found in the
    listing:

        1. The first column shows the actual RAM address the program is loaded
           into. This is a 32-bit address (8 hex digits x 4 bits each = 32).

           Notice that at each location, a 32-bit encoded instruction is being
           stored, but each memory location is only 8-bits wide (*), so the 
           next listed RAM address will be 4 bytes ahead of the last:

               9D0000D8   D8,D9,DA,DB
               9D0000DC   DC,DD,DE,DF
               9D0000E0   E0,E1,E2,E3

        2. The value 1234 is represented as 4D2 in hex, but an 'int' is a 
           32-bit value, so really it's 000004D2.

           But the addiu PIC32 instruction limits an immediate value to 
           16-bits:

               0x 2402 04D2

                          opcode   rs    rt
                 0x2402:  001001 00000 00010

               6 bits opcode:     001001    (addiu)  
               5 bits rs           00000    (source register)  
               5 bits rt           00010    (target register)

               + 16 bit immediate value:   04D2

        3. There are 32 registers in the CPU, labeled r0 to r31, but each 
           register has an additional "symbolic" name that we see in the 
           Dissassembly Listing:

               sp  stack pointer
               s8  Saved Temporary - caller must prserve contents
               v0  function return value
               v1  function return value
 

    MIPS M4K Assembly (aka PIC32MX Assembly) so far:

        RESOURCE: Chapter 10 "M4K Processor Core Instructions" of the 235 page
                  "MIPS M4K Processor Core Manual":

            addiu       v0,zero,1234    ADDIU "Add Immediate UnsignedInteger" 
                                        Rt = Rs +U Immed
                                        v0 = 0 + 1234

            sw          v0,4(s8)        SW "Store Word" 
                                        Mem[Rs+offset] = Rt
                                        Mem[s8+4]      = v0

            lw          v1,0(s8)        LW "Load Word" 
                                        Rt = Mem[Rs+offset]
                                        v1 = Mem[s8+0]

            mul         v0,v1,v0        MUL "Multiply with register write" 
                                        HI | LO =Unpredictable
                                        Rd = LO

            addu        sp,s8,zero      ADDU "Add Unsigned Integer"  
                                        Rd = Rs +U Rt
                                        sp = s8 + 0

            jr          ra              JR "Jump Register" 
                                        PC = Rs

            nop                         NOP "No Operation"
                                          (SLL r0, r0, r0)  
                                          SLL "Shift Left Logical" 
                                          Rd = Rt << sa


    Review of Instruction Format Fields:

        opcode    6-bit primary operation code

        rd        5-bit specifier for the destination register

        rs        5-bit specifier for the source register

        rt        5-bit specifier for the target (source/destination) 
                  register or used to specify functions within
                  the primary opcode REGIMM

     immed        16-bit signed immediate used for logical operands, 
                  arithmetic signed operands, load/store
                  address byte offsets, and PC-relative branch signed 
                  instruction displacement 


    Review of RAM inside the PIC32:

        32-bit addressing = 2^32 = 4,294,967,296 locations!
                                   4,294,967,296 bytes
                                       4,194,304 KB (compare to 64 KB)
                                           4,096 MB (compare to 8 MB or 128 MB)
                                               4 GB

            AAAAFFFF (like 65,536 Commodore 64s)


        But wait, the PIC32MX360F512L (on the Explorer 16 Dev Board) only has
        512K Flash Memory (the ROM where the program is stored):

            FFFF =     65,536  bytes     (64 KB)     16-bits
           FFFFF =  1,048,575  bytes  (1,024 KB)     20-bits

        So the address range for 512K Flash Memory is:

            00000          0    
            7FFFF    524,287

            Because 80000 = 524,288 (this is exactly 512 K, 512x1024)

        Historically visualized with 00000 at the "bottom" and the Stack 
        Pointer moving "up"

            7FFFF
             ...
            00000

        Think of the PIC32MX360F512L has having 8 Commodore 64s worth of
        memory STORED INTERNALLY IN THE CHIP ITSELF.

        But wait, there's more. 

            PIC32MX instructions are 32-bit, so really that chunk of
            512 KB of memory really can't hold 524,287 instructions, it can
            only hold:

                524,288 / 4 = 131,072 instructions
 
                     (compare that to the Commodore 64s variable instruction
                      length, lets say 2 bytes per instruction, on average,
                      only leaves about 32,768 instructions, about 25% of what
                      the PIC32 can store).
 
            So let's say the PIC32MX360F512L has about 4 Commodore 64s worth
            of RAM.



    Optimizations:

        Disable them while learning, they will result in code that doesn't
        make sense to you at this level. Optimizations obscure the underlying
        fundamentals.

    Stepping through a Program:

        You can single step through the assembly code if you are in the Listing
        window, or you can single step through the C code if you're in the 
        editor window.

        In both cases, you can view several things:

            Watch (shows you internal CPU registers)

            Local Variables (shows you variables in the C code, their values)

    A note about Reset:

        When you want to reset the program:

            Debugger -> Reset -> Processor Reset

        Note that this doesn't clear RAM or all RAM contents, so variables
        will retain their previous values!

    64-bit Multiplication

        The 'long long' datatype reserves 64-bits of space, but CPU registers
        are only 32-bits wide. So how does the assembler handle this?

        By performing exactly as you would when multiplying two 2-digit numbers
        together. The only difference is the CPU is working on 32-bit wide 
        binary values instead of 1-bit wide decimal values:

              19
            x 24
            ----
              
                 4 x 9 =  36
                4 x 10 =  40
                20 x 9 = 180
               20 x 10 = 200
               -------------
                         456

        Four multiplications
        Four additions

        BUT, the assembler will automatically optimize this by dropping the
        last multiplication, since it will be garunteed to overflow.

    Integer Division (32-bit vs. 64-bit)

        32-bit division can be handled directly, with a single div instruction.
 
        64-bit division will rely on a subroutine:

            libgcc2.c (found in MPLAB C32 directory)


    Floating Point Numbers:

        Regardless of 32-bit (float) or 64-bit (double, long double), the
        MPLAB C32 compiler will use library calls (and expensive ones at that)
        to perform math operations on floating point numbers. 


    Measuring Performance:

        Debugger -> Stop Watch

        Do this after compiling and linking a project. Then, single-step
        through the program and the Stop Watch will measure the time!

        Time is in cycles, in microseconds:

            Debugger -> Settings -> Osc/Trace


    Compiler Defines Data Size:

        The older microcontrollers (16-bit PIC and dsPIC) used an older
        compiler, MPLAB C30. 

        The PIC32's compiler, MPLAB C32 assigns different widths for each
        datatype. For example, 'int' is now 32-bits, another name for a 'long'.

        In the older compiler (C30) an 'int' was 16-bits wide.

        This makes porting code between compilers difficult, even though the
        code is written in strict C.

        There is a library inttypes.h that define exact widths regardless of
        what compiler, for example:

            uint16_t  will always be a 16-bit unsigned integer type

            int32_t   will always be a 32-bit signed integer type


        Another useful library file, stddef.h, defines a datatype (size_t) that
        will hold #bytes of another object. And there are many functions in the
        string.h library that make use of that datatype, like the sizeof()
        function.

        MPLAB C32 supports other ANSI C libraries:

            limits.h  (holds size limits of all datatypes)
            float.h   (more size limits)
            math.h    (things liek M_PI)


------------------------------------------------------------------------------
Day 5: Interrupts

    Often microcontrollers can't afford the luxury of a multi-tasking 
    operating system. Interrupts are used to divide attention to different
    tasks.

    C doesn't directly support interrupts, so you have to define special
    functions to implement them. The MPLAB C32 compiler provides support
    to take advantage of the PIC32's interrupt mechanisms.

    PIC32 = up to 64 distinct interrupt sources
            Each can have an ISR (Interrupt Service Routine) defined



    The C32 compiler has libraries and language extensions to help.

    MIPS core

        The MIPS M4K core handles "exceptions" by maintaining an ISR
        vector table (a table of function pointers). Interrupts are
        a type of exception:

            divide by zero
            reset command
            access to memory that isn't there

        These ISR vectors can live in Data RAM or Program ROM, or
        both, and the most important interrupts have default vectors
        declared/defined at startup. 

        Example Exception Sources:

            Reset
            On-Chip Debug  (used by EJTAG devices)
            Cache Error
            General Exception
            Interrupt

        Notice just one vector is assigned to "Interrupt". That function
        uses a special register called "cause" to determine what triggered
        the interrupt.

        The ISR will first save the execution process of the CPU, called
        saving the "prologue", and be able to restore it, called the
        "epilogue".


    ISR for Interrupts:

        1. PROLOGUE (save processor state)
        2. query CAUSE 
        3. EPILOGUE (restore previous state)

    This central Interrupt vector can call any special ISR we define, using
    the following rules for ISR function definitions:

       ISR function return type void
       No arguments allowed - parameter type void
       These functions can't be called by other functions
       These functions shouldn't call other functions


    An example: One of the UART interfaces (there are 2), can generate any of
                the following 3 interrupts:
 
        1. New data received and is available in the receive buffer for 
           processing.
        2. When data in the transmit buffer has been sent and the buffer
           is empty, ready and available to transmit more.
        3. When an error condition has been generated and action might be
           required to reestablish communication.

    Up to 96 independent events can be managed by the PIC32 interrupt 
    control module. 
 
        So 96 sources, handled by 64 vectors


    7 Control Bits per Interrupt

        These are dispersed across several SFRs (of course they are).

            1. enable bit (all interrupts off by default) "Interrupt Enable"
            2. trigger bit (set when interrupt occurs) "Interrupt Flag"
            3. "Group Priority Level" ipl1, ipl2, ... , ipl7
               The higher the priority level, handled first. The PIC32 itself
               has a priority level kept in the MIPS core, and so the CPU will
               ignore any interrupts lower than it's value (3-bits):

                   Core priority level = ipl4
                   Ignores any interrupts of level ipl1, ipl2, ipl3 
            4. "Subpriority Level" 2 more bits



    Several different ways to define an interrupt handler called intHand()
    with vector #0 and ipl1 level:

        void __attribute__ (( interrupt(ipl1), vector(0) )) intHand(void) {
            // code here
        }

        #pragma interrupt intHand ipl1 vector 0 void intHand(void) {
            // code here
        }

        void __ISR(0,ipl1) intHand(void) {
            // code here
        }

    That last one is a macro defined in sys/attribs.h

    
    The int.h library from the Standard PIC32 Toolset:

       Two ways to invoke (the second includes a larger set)

           #include <int.h>
 
           #include <plib.h>


    What's included?

        INTEnableSystemSingleVectoredInt(); 
        This is all the startup code to enable the basic interrupt
        management mode of the PIC32.

        mXXSetIntPriority(x);
        Assigns a given priority level to the chosen interrupt source.

        mXXClearIntFlag();
        A macro allowing us to clear the interrupt flag of a source.
      
