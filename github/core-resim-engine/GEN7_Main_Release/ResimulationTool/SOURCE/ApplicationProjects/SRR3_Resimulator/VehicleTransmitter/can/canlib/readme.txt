--------------------------------------------------------------------------------

                     CAN Driver Programming Library

          for CANcardXL/CANboardXL/CANcaseXL/CANcardX/CAN-AC2-PCI


                    Vector Informatik GmbH, Stuttgart

--------------------------------------------------------------------------------

                            Date:     22.01.2004
                            Version:  4.3

--------------------------------------------------------------------------------

Vector Informatik GmbH
Ingersheimer Straﬂe 24
D-70499 Stuttgart

Tel.:  0711-80670-200
Fax.:  0711-80670-555
EMAIL: support@vector-informatik.de
WEB:   www.vector-informatik.de

--------------------------------------------------------------------------------

Files:
------

VCAND.DOC      CAN Driver Library User Interface Description

DLL\
 VCAND.H       Header file for the driver library interface
 VCAND32.DLL   DLL for Borland-C, Microsoft-C and Visual Basic
 VCAND32.LIB   IMPORT library for Borland-C
 VCAND32.DEF   DEF file for VCAND32.DLL
 VCANDM32.LIB  IMPORT library for Microsoft-C
 VCANDM32.DEF  DEF file for VCAND32M.DLL
 VBCANIF.DLL   DLL for Visual Basic
 CANTRACD.PAS  Unit for Delphi
 VLabLib.llb   LabView VI Libraray

CANGEN\
 CANGEN.C      ANSI-C Source
 CANGEN.TXT    Description (Cyclic transmision of CAN messages)
 CANGEN.DSP    Microsoft Projektfile
 VCAND.H       Header file for the driver library interface

CANLOG\
 CANLOG.C      ANSI-C Source
 CANLOG.TXT    Description (Logging of CAN messages)
 CANLOG.IDE    Borland Projektfile
 VCAND.H       Header file for the driver library interface


CANDEMO\
 CANDEMO.C     ANSI-C Source
 CANDEMO.TXT   Description (Demonstrates the usage of the most important API functions)

CANCOUNT\
 CANCOUNT.C    ANSI-C Source
 CANCOUNT.TXT  Description (Count CAN messages)

CANTRACE\
 CANTRACE.C    ANSI-C Source
 CANTRACE.TXT  Description (Trace CAN messages)

CANSING\
 CANSING.C     ANSI-C Source
 CANSING.H     To port your application from CANcard to
               CANcardX, CANpari and CAN-AC2.
 CANSING.TXT   Description

CANVBAS\
 CANVBAS.DOC   CAN Driver Library User Interface Description for Visual Basic
 CANVBAS.BAS   Visual Basic Source
 CANVBAS.VBP   Visual Basic Projektfile
 CANVBASD.BAS   
 MAIN.FRM
 SETTINGS.FRM
 VBCANIF.CPP   ANSI-C Source to generate VBCANIF.DLL with VCANDM32.LIB

CANLATW\
 CANLATW.TXT   Description
 CANLATW.RES   Get the time delay TxRq and Tx 
 CANLATW.CPP   (generated with Borland C++ Builder)
 CANLATW.BPR   Borland C++ Builder Projektfile
 UNIT1.CPP
 UNIT1.H
 UNIT1.DFM

CANDELPH\
 CANTRACD.TXT Description 
              Trace CAN messages (generated with Delphi)
 CANTRACD.DPR Delphi Projektfile

CANLOGDYN\
 CANlogDyn.*      Source
 CANlogDyn.TXT    Description (Logging of CAN messages) (load vcand32 dynamically)
 CANlogDyn.DSP    Microsoft Projektfile
 CANlogDynDlg.c*  Source
 StdAfx.*         Source
 resource.h       Header
 VCAND.H          Header file for the driver library interface

LabView\
 LabView.TXT Description 
 VLabLib.llb utilizes all important functions from the driver library.
 VLabSample.llb includes three samples:
   OpenCAN.vi: open CAN
   CloseCAN.vi: close CAN
   Send&read.vi: send and read messages from the CAN bus
 VCAND32.DLL     DLL for Borland-C, Microsoft-C and Visual Basic, Delphi, LabView

EXEC\
 README.TXT      Descripton for Sample Applications
 CANGEN.EXE      Cyclic transmision of CAN messages
 CANLOG.EXE      Logging of CAN messages 
 CANDEMO.EXE     Demonstrates the usage of the most important API functions
 CANCOUNT.EXE    Count CAN messages
 CANTRACE.EXE    Trace CAN messages (generated with Borland)
 CANVBAS.EXE     Demonstrates the usage of the most important API functions for
                 Visual Basic
 CANTRACD.EXE    Trace CAN messages (generated with Delphi)
 CANLATW.EXE     Get the time delay TxRq and Tx (generated with Borland C++ Builder)
 VCAND32.DLL     DLL for Borland-C, Microsoft-C and Visual Basic, Delphi, LabView
 VBCANIF.DLL     DLL for Visual Basic
 CANlogDyn.exe   Logging of CAN messages 
 VLabSample.llb  LabView Samples

------------------------------------------------------------------------------
		
The DLLs are generated with Microsoft VC++ 6.0. 
The module definition files are created with impdef.exe from Borland. 

If you are working with another compiler version, use the module definition
files vcand32.def or vcandms32.def.
You can create an new import library with lib.exe from VC++ or implib.exe
from Borland.

Example :
	1.	lib /def:vcandm32.def 
	2.	Result: new vcandm32.lib 
	3.	Link the new import-library to your project

To run the sample programs on Windows98/ME/2000/NT/XP you will need a Vector CANcardXL, 
CANcardX or CAN-AC2-PCI with the appropriate drivers.

For more informations read the vcand.pdf

------------------------------------------------------------------------------

Version:
--------		

   Date:     22.01.2004
   Version:  4.3
   Changes:  - support CANboardXL
             - support CANcaseXL

   Date:     03.12.2002
   Version:  3.4
   Changes:  - support CANcardXL
             - library for WinXP
             - improved documentation vcand.doc
             - improved samples
             - support Delphi 7.0

   Date:     09.11.2001
   Version:  3.2
   Changes:  - 3 new function: 
               - ncdAddAcceptanceRange() : You can set a range for the acceptance filter several times
               - ncdRemoveAcceptanceRange() : You can remove a range of the acceptance filter several times
               - ncdResetAcceptance() : You can reset the acceptance filter and the acceptance filter is closed
             -New Sample programms:
               -CANlogDyn: Log CAN messages (load vcand32 dynamically)
                          New MS Visual 6.0 sample 
               -LabView: New LabView sample and library
                         VLabLib.llb utilizes all important functions from the driver library.
                         VLabSample.llb includes three samples:
                           OpenCAN.vi: open CAN
                           CloseCAN.vi: close CAN
                           Send&read.vi: send and read messages from the CAN bus
             - load vcand32 dynamically with loadlib.cpp
                 implemented in the sample CANlogDyn                     
             - improved documentation vcand.doc
             - improved Visual Basic sample canvbas with new functions

   Date:     18.05.2001
   Version:  3.1
   Changes:  - improved documentation vcand.doc
             - improved samples
             - library for Win2000

   Date:     19.01.2000
   Version:  2.6
   Changes:  - improved documentation vcand.doc
             - improved CANlibD.pas
             - improved Visual Basic sample canvbas
             - CAN-AC2-PCI support
             - 1 new function: 
               -ncdSetChannelTransceiver() : This function is used to set the transceiver modes

   Date:     27.05.1999
   Version:  2.05
   Changes:  - 4 new functions: 
               -ncdGetApplConfig() : Gets the application configuration
               -ncdSetApplConfig() : Sets the application configuration
               -ncdGetChannelVersion() : Get version information of a channel
               -ncdSetReceiveMode() : Suppress error frames and chipstate events
             -New Sample programms:
               -CANtracD: Trace CAN messages
                          New Delphi sample (32 Bit)
               -CANlatw:  Get the time delay between Tx and TxRq 
                          New Borland C++ Builder sample
             - Projektfiles for:
               -Microsoft : cangen.dsp
               -Borland   : canlog.ide
               -Delphi    : cantracd.dpr
               -Visual Basic: canvbas.vbp
               -C++ Builder Borland : canlatw.bpr

   Date:     30.10.1998
   Version:  2.0b
   Changes:  -New dll vbcanif.dll for Visual Basic sample
              Note: To start your Visual Basic application you will need the
              vcand32.dll, vbcanif.dll and the Visual Basic system dll msvbvm50.
             -Instruction cansing.txt to facilitate the porting of your
              applications(CANcard) to use CANcardX, CANpari and CAN-AC2.
             -ncdResetClock() needs init access (bug in vcand.doc)

   Date:     7.9.1998
   Version:  2.0a
   Changes:  -User Interface Description for Visual Basic with samples
             -New Sample programms:
               -CANcount: Count CAN messages
               -CANTRACE: Trace CAN messages
               -CANsing:  To port your application from CANcard to
                          CANcardX, CANpari and CAN-AC2.  

   Date:     7.7.1998
   Version:  2.00
   Note:     First Release

