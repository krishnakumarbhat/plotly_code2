

                          README.TXT file
             README file for CANcard V4.05 installation
                    (C) Copyright Softing GmbH 1999


===========================================================================================
  CONTENTS
===========================================================================================

1 ABOUT CANcard V4.05	
  1.1 WHAT’S NEW
  1.2 SUPPLIED FILES	

2 INSTALLATION	
  2.1 SYSTEM REQUIREMENTS
  2.2 DOS
  2.3 Win3.1x
  2.4 Win9x
  2.5 WinNT

3 RELEASE NOTES
  3.1 UPDATE CANcard V4.02
  3.2 UPDATE CANcard V2.04 and beta V223-250
  3.3 COMPATIBILITY TO OTHER SOFTING CAN INTERFACES

4 PROGRAMMING NOTES
  4.1 API LINKING
  4.2 INTERRUPT PROCESSING
  4.3 CYCLIC TRANSMISSION

5 CANCARD-SJA SPECIFIC NOTES

6 CANCARD2 SPECIFIC NOTES
  6.1 PHYSICAL INTERFACE
  6.1 PC INTERFACE

7 SUPPORT HOTLINE


===========================================================================================
1 ABOUT CAN-AC2-PCI V4.05
===========================================================================================
  1.1 WHAT’S NEW
  ==============
By developing CANcard V4.05 the version V4.02 was extended/modified by following features:

- CANcard2 support (16bit device).
- WIN32 interrupt support.
- The handshake in object buffer mode of the WIN32 driver has been modified to provide faster
  object access than API V4.02.
- Ordinal numbers of the API functions in the export table are fully compatible to those of
  all other Softings CAN interfaces (V4.05 and higher). Thus, different interfaces may be 
  driven by the same application just by renaming the API DLL.
- CANcard-SJA timestamp bugfix


  1.2 SUPPLIED FILES
  ==================

	CANcard L2 V4.05 (Non NT pack):
	-------------------------------
	CANcard V4.05 Setup.EXE		Installation program for Win9x
	cancardd.inf			Win9x PnP Installation script for CANcard
	cansja.inf			Win9x PnP Installation script for CANcard-SJA
	cancard2.inf			Win9x PnP Installation script for CANcard2
	ednec95.inf			Win9x PnP Installation script for EDICcard-C
	edic295.inf			Win9x PnP Installation script for EDICcard2
	cancardd.vxd			Win9x Virtuel Device Driver 
	cancardd.dll			Device Driver DLL
	edic.vxd			Win9x Virtuel Device Driver 
	edicdp32.dll			Device Driver DLL
	Readme.txt			User notes

	CANcard L2 V4.05N (NT pack):
	----------------------------
	CANcard V4.05N Setup.EXE	Installation program for Win9x and NT
	cancardd.inf			Win9x PnP Installation script for CANcard
	cansja.inf			Win9x PnP Installation script for CANcard-SJA
	cancard2.inf			Win9x PnP Installation script for CANcard2
	ednec95.inf			Win9x PnP Installation script for EDICcard-C
	edic295.inf			Win9x PnP Installation script for EDICcard2
	cancardd.vxd			Win9x Virtuel Device Driver 
	cancardd.dll			Device Driver DLL
	edic.vxd			Win9x Virtuel Device Driver 
	edicdp32.dll			Device Driver DLL
	Readme.txt			User notes

	CANcard L2 16bit:
	-----------------
	DOSsetup.exe			Selfextracting installation file for DOS
	CrdSetup.exe			Win3.1x Installation program


Attention!
CrdSetup.exe runs only in Win3.1x. If you like to use the 16bit driver in Win9x please
contact Softing for related installation software.






===========================================================================================
2 INSTALLATION
===========================================================================================
  2.1  SYSTEM REQUIREMENTS
  ========================
- 100% IBM-compatible
- DOS, Win3.1x, Windows 9x or Windows NT running
- free PC Card slot
- 16bit Card&Socket services (only DOS and Win3.1x users)
- 8kbyte free upper memory
- free interrupt line
- at least 500kByte free on hard disk

  2.2 DOS
  =======
1. Install 16bit Card&Socket services supplied by your PC provider.
2. Copy DOSsetup.exe to your hard disk.
3. Run DOSsetup.exe
4. Plug in CANcard
5. Connect CAN channel 1 and CAN channel 2 with a  cable with bus termination 
   resistance (124 ohm).

         D-SUB9 CAN1                 D-SUB9 CAN2

   CAN_L     Pin 2 -------------------- Pin 2
   GND       Pin 3 -------------------- Pin 3
   CAN_H     Pin 7 -------------------- Pin 7

6. Start 'Can_test.exe' in directory 'Ms15dos' of the installed software and choose 'FIFO'
   and 'Polling' as operational mode. After successful initialization ('Chip is running')
   type 'h' for help and ‘t’ to transmit a test CAN message.
   If the frame is transmitted successfully the program prints three lines:
   
   'XMT STD CAN1 ...'   Transmission request on CAN 1
   'REC STD CAN2 ...'   Message received on CAN 2
   'ACK STD CAN1 ...'   Transmit acknowledge CAN 1


  2.3 Win3.1x
  ===========
1. Install 16bit Card&Socket services supplied by your PC provider
2. Run CrdSetup.exe
3. Plug in CANcard
4. Connect CAN channel 1 and CAN channel 2 with a  cable with bus termination 
   resistance (124 ohm).

         D-SUB9 CAN1                 D-SUB9 CAN2

   CAN_L     Pin 2 -------------------- Pin 2
   GND       Pin 3 -------------------- Pin 3
   CAN_H     Pin 7 -------------------- Pin 7

5. Start the test program ‘Dll_test.exe’ in ‘Win_311’.
6. Initialize (menu: File->Initialization) the board with DEFAULT (button) values. 
   If certain numbers of chip type, HW and SW type are displayed and no error message is 
   following the configuration and initialization of the board was successfully. 
7. Run the communication (menu: File->Communication) and press the SEND and the RECEIVE button. 
   If the transmitted frame can be seen in the receive display the configuration and 
   communication runs successfully.


  2.4 WINDOWS 9X
  ==============
1. Remove any former API driver version of CANcard.
2. Check device manager to ensure that enough ressources are available.
3. Run CANcard V4.05(N) Setup.EXE.
4. Plug in HW
5. Connect CAN channel 1 and CAN channel 2 with a  cable with bus termination 
   resistance (124 ohm).

         D-SUB9 CAN1                 D-SUB9 CAN2

   CAN_L     Pin 2 -------------------- Pin 2
   GND       Pin 3 -------------------- Pin 3
   CAN_H     Pin 7 -------------------- Pin 7

6. Start 'Can_test.exe' in directory 'Win32' of the installed software and choose 'FIFO'
   and 'Polling' as operational mode. After successful initialization ('Chip is running')
   type 'h' for help and ‘t’ to transmit a test CAN message.
   If the frame is transmitted successfully the program prints three lines:
   
   'XMT STD CAN1 ...'   Transmission request on CAN 1
   'REC STD CAN2 ...'   Message received on CAN 2
   'ACK STD CAN1 ...'   Transmit acknowledge CAN 1


  2.5 WINDOWS NT
  ==============
1. Remove any former API driver version of CANcard.
2. Check the NT diagnostic for available ressources.
3. Run CANcard V4.05(N) Setup.EXE defining the applied HW and required ressources.
4. Plug in HW and reboot the system.
5. Connect CAN channel 1 and CAN channel 2 with a  cable with bus termination 
   resistance (124 ohm).

         D-SUB9 CAN1                 D-SUB9 CAN2

   CAN_L     Pin 2 -------------------- Pin 2
   GND       Pin 3 -------------------- Pin 3
   CAN_H     Pin 7 -------------------- Pin 7

6. Start 'Can_test.exe' in directory 'Win32' of the installed software and choose 'FIFO'
   and 'Polling' as operational mode. After successful initialization ('Chip is running')
   type 'h' for help and ‘t’ to transmit a test CAN message.
   If the frame is transmitted successfully the program prints three lines:
   
   'XMT STD CAN1 ...'   Transmission request on CAN 1
   'REC STD CAN2 ...'   Message received on CAN 2
   'ACK STD CAN1 ...'   Transmit acknowledge CAN 1

ATTENTION:
Swapping between 8bit CANcard/CANcard-SJA (EDICcard-C) and 16bit CANcard2 (EDICcard2) in 
Windows NT requires change of certain registry entries. The sufficient changes are made 
by Registryfiles supplied in the sub directory 'Registry' of the installed software.
Please double click following Registry Files before swapping your hardware:
	CANcard/CANcard-SJA to CANcard2:	CANcard16bit.reg
	CANcard2 to CANcard/CANcard-SJA:	CANcard8bit.reg
	EDICcard-C to EDICcard2:		EDICcard16bit.reg
	EDICcard2 to EDICcard-C:		EDICcard8bit.reg
Please reboot the PC after the changes are done!



===========================================================================================
3   RELEASE NOTES
===========================================================================================
  3.1 UPDATE CANCARD V4.01/4.02
  ================================================
The V4.05 API is compatible to the V4.02. 
Programs written for CANcard V4.01 or V4.02 only need to be relinked.


  3.2 UPDATE CANCARD V2.04 AND BETA V223-250
  ================================================
Applications developed with CAN-AC2 V2.04 can be updated to the new API V4.0 by 
following steps:

1. Remove old CANcard driver software from PC.
2. Install CANcard V4.02.
3. Include 'Canlay2.h' and 'Can_def.h' instead of 'Cancard.h'
4. Change parameter of INIPC_initialize_board in the source code according to 
   the new parameter structure set (see manual):
5. Conclude CANcard access with 'INIPC_close_board' before program exit in 
   order to release system ressources.
6. If explicit linking method is used change function names according to standard 
   calling convention.
7. If implicit linking method is used link compiled program with new import library.


  3.3 COMPATIBILITY TO OTHER SOFTING CAN INTERFACES
  =================================================
Full compatibility to all CAN APIs V4.x of CAN-AC2-104, CAN-AC2 and CAN-AC2-PCI is provided 
paying attention to following points:

· The functional scope of versions  V4.0, V4.01, V4.02 and V4.03 are fully supported.
· Functional scope is 100% compatible to V4.05 of all CAN interfaces. 
  It is possible to operate an application with certain HW platforms just by renaming 
  the DLL appreciating that 

-> Initialization parameter are set to valid values for all platforms 
-> CAN Highspeed is used by default (CANPC_set_output_control(-1)).
-> Time stamp access with CAN-AC2 (ISA) must be enabled explicitly.

· Side grading an application from a different HW base the above point should be taken into account.
· API V4.05 will be 100% supported by future versions.





===========================================================================================
4 PROGRAMMING NOTES
===========================================================================================
  4.1 API LINKING
  ===============

* The DLL function are provided in STDCALL convention. 
   Therefore, the function names in declarations of explicit linking need to be extended by
   - an underscore in front
   - a '@' with succeeding number of parameter bytes at the end

   Example: _CANPC_initialize_chip@20
   (see IMPORT section of  'Cancard.def')

* The explicit linking method is applied in most visualization tools which can access 
   DLL functions. Furthermore, it is valid for Visual Basic, Delphi and LabVIEW. 
   (For Visual Basic Users a Definition Module is provided with the CANcard SW which includes 
   the VB declarations.)

* The import library supplied with the DLL is of Microsoft standard.  Hence, it is not
   applicable with Borland Compilers. Instead of the import library the Borland user may link
   the supplied Module Definition File 'Cancard.def' to the Borland project.


  4.2 INTERRUPT PROCESSING
  ========================

  INTERRUPT EVENTS
  ----------------
For many applications it is useful to be informed by interrupt about occurrence of CAN events. 
Otherwise, the hardware had to be polled for new events which requires more PC processor 
time. The firmware triggers a hardware interrupt to the PC on the following CAN events:

-  Reception of data, remote and error frames
-  Acknowledge on successful transmissions if enabled
-  Change of bus state


  WIN32 INTERRUPT PROGRAMMING
  ---------------------------
If the driver detects an interrupt it triggers a WIN32 event which can be 
evaluated by the application controlling a WIN32 process or thread. Thus, an application or 
thread can be created which is only processed in case of the interrupt.

As a prerequisite the interrupt event must be created by the application. The hardware driver 
must be supplied with the handle of this WIN32 event using the API function 
CANPC_set_interrupt_event. 

Furthermore, a thread must be created and started which gets into WAIT state until the 
interrupt event is triggered by the driver. Then, the thread is becoming activ and 
the necessary interrupt activities are processed.

Before termination of the WIN32 process the created resources should be released for proper 
operation. 

The application of the WIN32 interrupt is exemplary implemented in the test program 
‘Can_test.exe’. The interrupt relevant functions are sampled in ‘Intexmpl.c’ in ‘Source’ directory 
of the installed software. This C source code provides macro functions for initialization and 
termination of the interrupt handling as well as an interrupt service thread which may be linked 
to a customer application.

NOTE:
Interrupting an ongoing API functions in object buffer mode may result in deadlocks or false 
return codes. This must be avoided by a thread synchronisation applying a critical section as 
shown in the example C code. For further information about synchronisation and critical
sections refer to the related sections of your compiler manual.

 
  CANPC_SET_INTERRUPT_EVENT
  -------------------------

CANPC_set_interrupt_event(HANDLE InterruptEvent)

This function gives a HANDLE (pointer) of a WIN32 event to the driver. If a CAN interrupt 
occurs the driver sets this event which might be evaluated by the application.

The event must be created beforehand by the application with CreateEvent which is a function 
of the WIN32 API and returns the required HANDLE. The WIN32 event can be used to control 
the processing of a WIN32 process or thread.

Function return code:
 0: Function successful
-1: Function not successful


  4.3 CYCLIC TRANSMISSION
  =======================

Cyclic transmission is only supported in dynamic object buffer mode. It is programmed by
following procedure:

1. Define the transmit object of the identifier (CANPC_define_object)
   before starting the operation (CANPC_start_chip)
2. Define this object to be cyclic (CANPC_define_cyclic) after CANPC_start_chip adjusting
   the rate (ms) and the number of cycles (0 for unlimited).
3. Start transmission of the object with CANPC_write_object or CANPC_send_object.
   An online supply with new data is possible with CANPC_supply_object.
4. Stop cyclic transmission by calling CANPC_define_cyclic with a rate of 0.

Note:
Changing the cyclic definition (rate) during cyclic transmission is not possible and
resilts in object access errors. If you like to change the cyclic rate you have to stop
the cyclic transmission first.



 
===========================================================================================
5 CANCARD-SJA SPECIFIC NOTES
===========================================================================================

Since the driver detects automatically whether CANcard with NEC 72005 or CANcard-SJA/CANcard2 
with Philips SJA1000 is applied the CAN controller type doesn’t influence the API 
functionality. Nevertheless, a few additional comments to the CANcard manual V4.0 
rev.01 should be considered using CANcard-SJA:

- DOS applications for CANcard-SJA need access to following firmware files:
	cansja.sbn
	bcard.sbn
	ldcard.sbn

- The chip type parameter of CANPC_get_version (see manual section 5.2.28) returns 
 1000 to indicate the SJA1000 chip.

- The SJA1000 was developed to be conform to the 82C200. Hence, register settings of 
 bit timing and output control differ to the NEC 72005.

	- Bit timing (baud rate) of the CAN bus is set by the API with function 
	  CANPC_initialize_chip (see manual section 5.2.4). The parameter set of 
	  CANPC_initialize_chip is conform to the bit timing parameter set of the 82C200 
	  CAN controller to enable API conformity for all CAN interfaces provided by 
	  Softing. Hence, the description in the manual is valid regardless of the used 
	  CAN controller.
	- Output driver configuration in the output control register is set by the function 
	  CANPC_set_output_control (see manual section 5.2.5). If the CANcard-SJA is 
	  used with the CAN High Speed interface (default) the output control register 
	  must be set to a value of FBHex. This default output control register setting can 
	  also be chosen automatically by passing the default parameter -1 which 
	  assures compatibility with CAN-AC2 applications using CAN High Speed 
	  Standard.


===========================================================================================
6 CANCARD2 SPECIFIC NOTES
===========================================================================================

  6.1 PHYSICAL INTERFACE
  ======================
The physical interface for the CANcard2 is integrated in the CANcard Adapter Cable. Thus, the
user is enabled to drive different interface types with the same card just by plugging the
related cable. The interface cable is available in three types:

DHSC	Dual Highspeed Cable		Both channels with CAN Highspeed transceivers
HLSC	Highspeed-Lowspeed Cable	Channel 1: Highspeed; Channel 2: Lowspeed transceiver
DLSC	Dual Highspeed Cable		Both channels with CAN Lowspeed transceivers

The pinning of the D-SUB9 connectors for CAN High speed and CAN Lowspeed rea different only
at Pin 9. Thus, following pin description is valid for all connectors:

	Pin Number	Signal
	----------	------ 
	1		reserved
	2		CAN_L (dominant low)
	3		CAN GND
	4		reserved
	5		CAN shield
	6		CAN GND
	7		CAN_H (dominant high)
	8		reserved
	9		KL30 (vehicle volt. supply), only with Lowspeed transceivers


  6.2 PC INTERFACE
  ================
CANcard2 (EDICcard2) is a 16bit device. The CANcard API is independant to the fact whether
a 8bit CANcard or a 16bit CANcard2 is applied as hardware. Only in Windows NT the cards can 
not be exchanged without any adaption. If the hardware is changed form a 16bit to a 
8bit card or vice versa the registry settings have to be changed beforehand.

The sufficient changes are made by Registry files supplied in the sub directory 'Registry' 
of the installed software. Please double click following Registry Files before swapping 
your hardware:

	CANcard/CANcard-SJA to CANcard2:	CANcard16bit.reg
	CANcard2 to CANcard/CANcard-SJA:	CANcard8bit.reg
	EDICcard-C to EDICcard2:		EDICcard16bit.reg
	EDICcard2 to EDICcard-C:		EDICcard8bit.reg

Please reboot the PC after the changes are done!

ATTENTION!
CANcard2/EDICcard is applicanle only in Windows 9x or NT. If you like to run a PCMCIA card 
in Win3.1x or DOS you had to use a 8bit CANcard/CANcard-SJA. 


===========================================================================================
7   SUPPORT HOTLINE
===========================================================================================
If you face problems installing or applying the interface you can contact the
technical support hotline (Tel.: +49 89 456 56-0, E-mail: support.can@softing.com) with 
following informations:

1. Product name, version and serial number
2. SW name and version (see installation disc)
3. Operating system
4. Result of running test programm
5. Description of procedure until error
6. Error description

Further we would apreciate if you report detected printing errors of the manual or handling 
problems of the API.