
/*!
 ***********************************************************************
 *   @file                    iow56_simple.c
 *   @par Project:     Wrapper Library for the IOW56
 *   @par Module:     Select Operation System (Linux / Windows)
 *
 *   @brief  This file contains simplified function-calls for the usage of the
 *              IO-Warrior56 (and only for this Version!).
 *              If you need more functions, you have to use the standard
 *              I/O-Functions directly
 *              from "iowkit.h".
 *              Define the PLATFORM symbol in iow_simple_codecontrol.h
 *              to either
 *              OS_LINUX or OS_MS_WINDOWS, re-compile and re-link the module.
 *
 *
 *   @par Author:     Ernst Forgber
 *   @par Department: E14
 *   @par Company: Fachhochschule Hannover - University of Applied
 *        Sciences and Arts
 *
 ***********************************************************************
 *
 *   @par History       :
 *   @verbatim
 * Ver  Date        Author         Change Description
 * 0.0  22.10.2007 E. Forgber     - First Version
 * 0.1  05.02.2009 E.Forgber      - Iow56Ports_t Iow56ReadImmediateGPIO(IOWKIT_HANDLE devHandle);  
 *
 *   @endverbatim
 *
 ***********************************************************************/

/*! \mainpage IOW-Simple : A simplified Library for the usage of the IO-Warrior56, Version 0.1 
 *
 * \section intro Introduction
 *

 * This module was developed for the simple use of the IO-Warrior 56 in course
 * of the "PIA-Labor" by the students.
*  The operation system has to be selected in file iow56_simple_codecontrol.h,
 * the module will have to be re-complied and linked to the application.
 *
 * \section install Usage


In order to use the module, the headers iow56_simple.h, iow56_simple_codecontrol.h
and the iow56_simple.c are needed. The application has to include iow56_simple.h,
iow56_simple.c has to be compiled and linked to the application.
Additional there is also the original IO-Warrior Library needed:
The Header iowkit.h is included by iow56_simple.h
and has to bee located in the same directory as iow56_simple.
The static library iowkit_bcc.lib (special version for Borland's bcc32 compiler)
is needed, which calls the Functions in the dynamic library
iowkit.dll.  Both files must be in the Working-Directory with your sourcecode !
NOTE:  The original iowkit.lib does NOT work with the Borland-Compiler.  If there is no
copy of iowkit_bcc.lib available, you have to
build a new static library from the dynamic library  by using the Borland-Tool implib.exe
(in "BCC55\Bin\") .
@verbatim
             CMD#:  implib  iowkit_bcc.lib iowkit.dll
@endverbatim
  For the usage in an own (Windows) project,  following files have to be used:
  @verbatim
       iow56_simple.c  as source file
       iowkit_bcc.lib as library
       iowkit.dll as dynamic link library at runtime
@endverbatim

Exported functions are documented in file iow56_simple.c, see its documentation for
details.

 **********************************************************************/

#include <stdio.h>
#include "iow56_simple.h"


// #define DEBUG_MODULE 

#ifdef DEBUG_MODULE
  #define DebugCode( code_fragment ) { code_fragment }
#else
  #define DebugCode( code_fragment )
#endif

/* speichert den zuletzt ausgegebenen 50Bit-Wert.
  P0,P1,P2,P3,P4,P5, P6.0, P6.1 Die oberen Bits von P6 sollen
  immer mit 1 beschrieben werden. */
static BYTE port_out_g[7];      /*!< GLOBAL Variable with the last written 50-Bit-Value to the IOW */

/* speichert den zuletzt eingelesenen 50Bit-Wert.
  P0,P1,P2,P3,P4,P5, P6.0, P6.1 Die oberen Bits von P6 sollen
  immer mit 1 beschrieben werden. */
static Iow56Ports_t port_in_g={0x00,0x00,0x00,0x00,0x00,0x00,0x00}; /*!< GLOBAL Variable with the last  50-Bit-Value read from the IOW */


/*!
 **********************************************************************
 @par Exported Function:
    Iow56OpenDevice

  @par Description:
    Opens the IOW56 device. Only one device may be present in the system,
    multiple IOWs cannot be handled by this module.
    All 50 IO-Pins are initialized with logical 1 in order to enable read mode
    of open drain inputs.
  @param  none

  @retval device handle of NULL no error
   @par Example :
  @verbatim
    IOWKIT_HANDLE devHandle;
    ...
    devHandle = Iow56OpenDevice();
    if(NULL==devHandle) {   puts("cannot open iow56 device..."); }
  @endverbatim
 ************************************************************************/
IOWKIT_HANDLE Iow56OpenDevice(void)
{   IOWKIT_HANDLE devHandle;
    Iow56Ports_t p1={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    Iow56Ports_t p0={0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    DebugCode( printf("Iow56OpenDevice: enter\n"); );
    devHandle = IowKitOpenDevice();
    if (devHandle == NULL)
    {   fprintf(stderr,"Failed to open IOW56 device\n");
        return NULL;
    }
    if(1 != IowKitGetNumDevs())
    {   fprintf(stderr,"Sorry, this module can handle only ONE IOW56\n");
        Iow56CloseDevice(devHandle);
        return NULL;
    }
    /* set all pins to 0, perform dummy read */
    Iow56WriteGPIO(devHandle, p0);
    Iow56ReadGPIO(devHandle);
    /* set all pins to '1', enable input for all open drain pins */
    Iow56WriteGPIO(devHandle, p1);
    /* after the second dummy read there should have been at least one read */
    /* with new data in order to init the global static Iow56Ports_t port_in_g; */
    /* which holds and gives back the last input vales in order, there is no */
    /* signal change between to read accesses */
    Iow56ReadGPIO(devHandle);
    DebugCode( printf("Iow56OpenDevice: return\n"); );
    return devHandle;
}

/*!
 **********************************************************************
 @par Exported Function:
    Iow56CloseDevice

  @par Description:
    Closes the IOW56 device. Only one device may be present in the system,
    multiple IOWs cannot be handled by this module.
  @param  none

  @retval void
   @par Example :
  @verbatim
    IOWKIT_HANDLE devHandle;
    ...
    devHandle = Iow56OpenDevice();
    if(NULL==devHandle) {   puts("cannot open iow56 device..."); }
    ...
    Iow56CloseDevice(devHandle);
  @endverbatim
 ************************************************************************/
void Iow56CloseDevice(IOWKIT_HANDLE devHandle)
{
    DebugCode( printf("Iow56OpenDevice: enter\n"); );
    IowKitCloseDevice(devHandle);
    DebugCode( printf("Iow56OpenDevice: return\n"); );
}

/*!
 **********************************************************************
 * @par Exported Function:
 *   Iow56WriteGPIOBit
 *
 * @par Description:
 Write single bit value to first iow56 in system. Internally, always a
 complete set of 50 bits has to be sent to the device. When writing to
 the device, all 50 bits are stored in a private module variable. This routine
 will change only the specified bit, all other 49 remain unchanged, i.e. their
 old values are sent again.  Iow56 Pins are open collector
 with internal pull up. Writing a 1 to a pin enables "read" operation
 for that pin - without external driver the pin will read 1 due to
 pull up. Note: port 6 has only 2 usable bits: p6.6 and p6.7, see iow56
 documentation for details.
 * @see
 * @arg
 *
 * @param  devHandle  Type IOWKIT_HANDLE (get this Device handle from the function Iow56OpenDevice( ) )
 * @param  port Type INT (Portnumber of the IOW, allowed Value 0...6)
 * @param  bitNr Type INT (Number oft the Bit to be written, allowed  Value 0...7)
 * @param  val Type INT (Value == 1: set Bit, 0: reset  Bit)
 *
 * @retval TRUE         - no error
 * @retval FALSE       - an error occured
 *
 * @par Example :
 *
 * @verbatim
    {   int val=0;
        puts("Port P0.0, Eingabe ein:1 aus:0");
        scanf("%d", &val);
        if(!Iow56WriteGPIOBit(devHandle, 0, 0, val))
        {   puts("error...");
        }
    }
  @endverbatim
 ************************************************************************/
BOOLEAN Iow56WriteGPIOBit(IOWKIT_HANDLE devHandle, int port, int bitNr, int val)
{   BYTE bitmask;
    IOWKIT56_IO_REPORT rep;
    int i;

    DebugCode( printf("Iow56WriteGPIOBit: enter\n"); );
    if((port<0)||(port>6))    return FALSE;
    if((bitNr<0)||(bitNr>7)) return FALSE;
    bitmask = (BYTE) (1 << (bitNr));
    /* keep all 50 bits in global modul variable */
    if(val==0) port_out_g[port] &= (~bitmask); /* reset bit */
    else       port_out_g[port] |= bitmask;  /* set bit */
    rep.ReportID = 0;
    for(i=0;i<7;i++)
    {   rep.Bytes[i]=port_out_g[i];
    }
    DebugCode(printf("ports 0..6 out: %X %X %X %X %X %X %X\n",
              port_out_g[0],port_out_g[1],port_out_g[2],
              port_out_g[3],port_out_g[4],port_out_g[5],port_out_g[6]););
    DebugCode( printf("Iow56WriteGPIOBit: exit\n"); );
    return IowKitWrite(devHandle, IOW_PIPE_IO_PINS, (PCHAR) &rep,
           IOWKIT56_IO_REPORT_SIZE) == IOWKIT56_IO_REPORT_SIZE;
}


/*!
 **********************************************************************
 * @par Exported Function:
 *  Iow56WriteGPIOByte
 *
 * @par Description:
 Write single byte value to first iow56 in system. Internally, always a
 complete set of 50 bits has to be sent to the device. When writing to
 the device, all 50 bits are stored in a private module variable. This routine
 will change only the specified byte, all other bits remain unchanged, i.e. their
 old values are sent again.  Iow56 Pins are open collector
 with internal pull up. Writing a 1 to a pin enables "read" operation
 for that pin - without external driver the pin will read 1 due to
 pull up. Note: port 6 has only 2 usable bits: p6.6 and p6.7, see iow56
 documentation for details.
 *
 * @see
 * @arg
 *
 * @param  devHandle  Type IOWKIT_HANDLE (get this Device handle from the function Iow56OpenDevice( ) )
 * @param  port Type INT (Portnumber of the IOW, allowed Value 0...6)
 * @param  val Type INT (Value of Byte to be written,  0 to xFF)
 *
 *
 * @retval TRUE         - no error
 * @retval FALSE       - an error occured
 *
 * @par Example :
 *
 * @verbatim
    {   int val=0;
        puts("Port P0, Eingabe 0..255");
        scanf("%d", &val);
        if(!Iow56WriteGPIOByte(devHandle, 0, val))
        { puts("error...");
        }
    }
  @endverbatim
 ************************************************************************/
BOOLEAN Iow56WriteGPIOByte(IOWKIT_HANDLE devHandle, int port, int val)
{   ULONG bitmask;
    IOWKIT56_IO_REPORT rep;
    int i;

    if((port<0)||(port>6))      return FALSE;
    /* keep all 50 bits in global modul variable */
    port_out_g[port] = (BYTE) val;
    rep.ReportID = 0;
    for(i=0;i<7;i++)
    {   rep.Bytes[i]=port_out_g[i];
    }
    DebugCode(printf("ports 0..6 out: %X %X %X %X %X %X %X\n",
              port_out_g[0],port_out_g[1],port_out_g[2],
              port_out_g[3],port_out_g[4],port_out_g[5],port_out_g[6]););
    return IowKitWrite(devHandle, IOW_PIPE_IO_PINS, (PCHAR) &rep,
           IOWKIT56_IO_REPORT_SIZE) == IOWKIT56_IO_REPORT_SIZE;
}



/*!
 **********************************************************************
 * @par Exported Function:
 *  Iow56WriteGPIO
 *
 * @par Description:
 * Write all 50 Bit  to first IOW56 in system. Pins are open collector
 *  with internal pull up. Writing a 1 to a pin enables "read" operation
 *  for that pin - without external driver the pin will read 1 due to
 *  pull up. Note: port 6 has only 2 usable bits: p6.6 and p6.7, see iow56
 documentation for details.
 *
 * @see
 * @arg
 *
 * @param  devHandle  Type IOWKIT_HANDLE (get this Device handle from the function Iow56OpenDevice( ) )
 * @param  value Type Iow56Ports_t (7 Bytes to be written to the iow56)
 *
 * @retval TRUE         - no error
 * @retval FALSE       - an error occured
 *
 * @par Example :
 *
 * @verbatim
    {   int i;
        Iow56Ports_t p={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

        if(!Iow56WriteGPIO(devHandle, p))
        { puts(error...);
        }
    }
  @endverbatim
 ************************************************************************/
BOOLEAN Iow56WriteGPIO(IOWKIT_HANDLE devHandle, Iow56Ports_t allPorts)
{
    IOWKIT56_IO_REPORT rep;
    int i;

    /* keep all 50 bits in global modul variable */
    port_out_g[0] = (BYTE) allPorts.P0;
    port_out_g[1] = (BYTE) allPorts.P1;
    port_out_g[2] = (BYTE) allPorts.P2;
    port_out_g[3] = (BYTE) allPorts.P3;
    port_out_g[4] = (BYTE) allPorts.P4;
    port_out_g[5] = (BYTE) allPorts.P5;
    port_out_g[6] = (BYTE) allPorts.P6;
    for(i=0;i<7;i++)
    {   rep.Bytes[i]=port_out_g[i];
    }
    rep.ReportID  = 0;  /* pipe0 ist GPIO */
    DebugCode(printf("ports 0..6 out: %X %X %X %X %X %X %X\n",
              port_out_g[0],port_out_g[1],port_out_g[2],
              port_out_g[3],port_out_g[4],port_out_g[5],port_out_g[6]););
    return IowKitWrite(devHandle, IOW_PIPE_IO_PINS, (PCHAR) &rep,
           IOWKIT56_IO_REPORT_SIZE) == IOWKIT56_IO_REPORT_SIZE;
}




/*!
 **********************************************************************
 * @par Exported Function:
 *  Iow56ReadGPIO
 *
 * @par Description:
 *Read 50Bits  from first iow56 in system. Pins are open collector
 * with internal pull up. Writing a 1 to a pin enables "read" operation
 * for that pin - without external driver the pin will read 1 due to
 *  pull up. This function returns immediately, i.e. does not wait for any
 *  change on pin logic level.
 Note: port 6 has only 2 usable bits: p6.6 and p6.7, see iow56
 documentation for details.
 *  There is an internal queue holding the history of GPIO pins. If pin level
 *  changes occur that are not read, they are queued for later read accesses.
 *  Use Iow56ReadImmediateGPIO() in order to read current input pin level.
 *
 * @see
 * @arg
 *
 * @param  devHandle  Type IOWKIT_HANDLE (get this Device handle from the function Iow56OpenDevice( ) )
 *
 * @retval              - Iow56Ports_t with 50 Bit from the IOW
 *
 * @par Example :
 *
 * @verbatim
        Iow56Ports_t p;
        ...
        p = Iow56ReadGPIO(devHandle);
  @endverbatim
 ************************************************************************/
Iow56Ports_t Iow56ReadGPIO(IOWKIT_HANDLE devHandle)
{   IOWKIT56_IO_REPORT rep;
    ULONG res;

    memset(&rep, 0, IOWKIT56_IO_REPORT_SIZE);
    res = IowKitReadNonBlocking(devHandle, IOW_PIPE_IO_PINS,
          (PCHAR) &rep, IOWKIT56_IO_REPORT_SIZE);
    if(res != IOWKIT56_IO_REPORT_SIZE)
    {  /* keine neuen Daten verfuegbar, gib die alten zurueck */
       DebugCode(printf("Iow56ReadGPIO: IowKitReadNonBlocking res:%d\n",res););
       DebugCode(printf("keine neuen Daten!\n"););
    }
    else
    {   /* neue Daten verfuegbar, kopieren und zurueck geben. */
        port_in_g.P0 = rep.Bytes[0];
        port_in_g.P1 = rep.Bytes[1];
        port_in_g.P2 = rep.Bytes[2];
        port_in_g.P3 = rep.Bytes[3];
        port_in_g.P4 = rep.Bytes[4];
        port_in_g.P5 = rep.Bytes[5];
        port_in_g.P6 = rep.Bytes[6];
    }
    DebugCode(printf("ports 0..6: %X %X %X %X %X %X %X\n",
              port_in_g.P0,port_in_g.P1,port_in_g.P2,
              port_in_g.P3,port_in_g.P4,port_in_g.P5,port_in_g.P6););
    return port_in_g;
}




/*!
 **********************************************************************
 * @par Exported Function:
 *  Iow56ReadBlockingGPIO
 *
 * @par Description:
 Read 50Bits  from first iow56 in system. Pins are open collector
 * with internal pull up. Writing a 1 to a pin enables "read" operation
 * for that pin - without external driver the pin will read 1 due to
 *  pull up. This function will block until a signal change on at least one pin
 is registered.
 Note: port 6 has only 2 usable bits: p6.6 and p6.7, see iow56
 documentation for details.
 *  There is an internal queue holding the history of GPIO pins. If pin level
 *  changes occur that are not read, they are queued for later read accesses.
 *  Use Iow56ReadImmediateGPIO() in order to read current input pin level.

 * @see Iow56ReadGPIO(), Iow56ReadImmediateGPIO()
 * @arg
 *
 *
 * @param  devHandle  Type IOWKIT_HANDLE (get this Device handle from the function Iow56OpenDevice( ) )
 *
 *
 * @retval               - Iow56Ports_t with 50 Bit from the IOW
 *
 * @par Example :
 *
 * @verbatim
        Iow56Ports_t p;
        ...
        p = Iow56ReadBlockingGPIO(devHandle);
  @endverbatim
 ************************************************************************/
Iow56Ports_t Iow56ReadBlockingGPIO(IOWKIT_HANDLE devHandle)
{
    IOWKIT56_IO_REPORT rep;
    ULONG res;

    rep.ReportID = 0;  // pipe0 ist GPIO
    res = IowKitRead(devHandle, IOW_PIPE_IO_PINS,
           (PCHAR) &rep, IOWKIT56_IO_REPORT_SIZE);
    if(res != IOWKIT56_IO_REPORT_SIZE)
    {   printf("error in Iow56ReadBlockingGPIO: IowKitRead\n");
        return port_in_g;
    }
    else
    {   /* neue Daten verfuegbar, kopieren und zurueck geben. */
        port_in_g.P0 = rep.Bytes[0];
        port_in_g.P1 = rep.Bytes[1];
        port_in_g.P2 = rep.Bytes[2];
        port_in_g.P3 = rep.Bytes[3];
        port_in_g.P4 = rep.Bytes[4];
        port_in_g.P5 = rep.Bytes[5];
        port_in_g.P6 = rep.Bytes[6];
    }
    DebugCode(printf("ports 0..6: %X %X %X %X %X %X %X\n",
              port_in_g.P0,port_in_g.P1,port_in_g.P2,
              port_in_g.P3,port_in_g.P4,port_in_g.P5,port_in_g.P6);)
    return port_in_g;
}


/*!
 **********************************************************************
 * @par Exported Function:
 *  Iow56ReadImmediateGPIO
 *
 * @par Description:
 *Read current value of 50Bits  from first iow56 in system. Pins are open collector
 * with internal pull up. Writing a 1 to a pin enables "read" operation
 * for that pin - without external driver the pin will read 1 due to
 *  pull up. This function returns immediately, i.e. does not wait for any
 *  change on pin logic level.
 Note: port 6 has only 2 usable bits: p6.6 and p6.7, see iow56
 documentation for details.
 *  This routine reads the current pin values, no queue is involved. 
 *
 * @see  
 * @arg   Iow56ReadGPIO
 *
 * @param  devHandle  Type IOWKIT_HANDLE (get this Device handle from the function Iow56OpenDevice( ) )
 *
 * @retval              - Iow56Ports_t with 50 Bit from the IOW
 *
 * @par Example :
 *
 * @verbatim
        Iow56Ports_t p;
        ...
        p = Iow56ReadImmediateGPIO(devHandle);
  @endverbatim
 ************************************************************************/
Iow56Ports_t Iow56ReadImmediateGPIO(IOWKIT_HANDLE devHandle)
{   IOWKIT56_SPECIAL_REPORT special_rep;
    IOWKIT56_IO_REPORT rep;
    ULONG res;
    
    /* To get the port status just send a report with ID
               $FF to interface 1:
           */    
    memset(&special_rep, 0, IOWKIT56_SPECIAL_REPORT_SIZE);
    special_rep.ReportID = 0xFF;
    res = IowKitWrite(devHandle, IOW_PIPE_SPECIAL_MODE,
          (PCHAR) &special_rep, IOWKIT56_SPECIAL_REPORT_SIZE);
    if (res != IOWKIT56_SPECIAL_REPORT_SIZE)
    {  /* error */
       puts("Iow56ReadImmediateGPIO: IowKitWrite() has crashed");
       DebugCode(printf("Iow56ReadImmediateGPIO: IowKitWrite res:%d\n",res););
       DebugCode(printf("keine neuen Daten!\n"););
       
    }
    /* This will result in the current pin status to be
    returned immediately in an input report with ID
    $FF with the following format:
    */
    res = IowKitRead(devHandle, IOW_PIPE_SPECIAL_MODE,
          (PCHAR) &special_rep, IOWKIT56_SPECIAL_REPORT_SIZE);
    if (res != IOWKIT56_SPECIAL_REPORT_SIZE)
    {  puts("Iow56ReadImmediateGPIO: IowKitRead() has crashed");
       /* keine neuen Daten verfuegbar, gib die alten zurueck */
       DebugCode(printf("Iow56ReadImmediateGPIO: IowKitRead res:%d\n",res););
       DebugCode(printf("keine neuen Daten!\n"););
    }
    else
    {   /* neue Daten verfuegbar, kopieren und zurueck geben. */
        port_in_g.P0 = special_rep.Bytes[0];
        port_in_g.P1 = special_rep.Bytes[1];
        port_in_g.P2 = special_rep.Bytes[2];
        port_in_g.P3 = special_rep.Bytes[3];
        port_in_g.P4 = special_rep.Bytes[4];
        port_in_g.P5 = special_rep.Bytes[5];
        port_in_g.P6 = special_rep.Bytes[6];
    }
    DebugCode(printf("repID: %X  ports 0..6: %X %X %X %X %X %X %X\n", 
              special_rep.ReportID, special_rep.Bytes[0], special_rep.Bytes[1],
              special_rep.Bytes[2], special_rep.Bytes[3], special_rep.Bytes[4],
              special_rep.Bytes[5], special_rep.Bytes[6]););
              
    return port_in_g;
}







/*!
 **********************************************************************
 * @par Exported Function:
 *  Iow56ReadGPIOByte
 *
 * @par Description:
 Read a single byte value from first iow56 in system. Internally, always a
 complete set of 50 bits has to be read from the device, but this function will
 return only one port value. Iow56 Pins are open collector
 with internal pull up. Writing a 1 to a pin enables "read" operation
 for that pin - without external driver the pin will read 1 due to
 pull up. Note: port 6 has only 2 usable bits: p6.6 and p6.7, see iow56
 documentation for details.
 *
 * @see
 * @arg Iow56ReadGPIOBit(), Iow56ReadImmediateGPIO(), Iow56ReadBlockingGPIO(), Iow56ReadGPIO()
 *
 * @param  devHandle  Type IOWKIT_HANDLE (get this Device handle from the function Iow56OpenDevice( ) )
 * @param  port Type INT (Portnumber of the IOW, allowed Value 0...6)
 *
 * @retval Type INT, value of selected port byte,  0 to xFF
 *
 * @par Example :
 *
 * @verbatim
    {   int val=0;

        printf("reading port P0: %X\n" Iow56ReadGPIOByte(devHandle, 0));

    }
  @endverbatim
 ************************************************************************/
int Iow56ReadGPIOByte(IOWKIT_HANDLE devHandle, int port)
{   int val = 0;
    port_in_g = Iow56ReadImmediateGPIO(devHandle);
    switch(port)
    {   case 0 : val = port_in_g.P0; break;
        case 1 : val = port_in_g.P1; break;
        case 2 : val = port_in_g.P2; break;
        case 3 : val = port_in_g.P3; break;
        case 4 : val = port_in_g.P4; break;
        case 5 : val = port_in_g.P5; break;
        case 6 : val = port_in_g.P6; break;
        default: puts("Iow56ReadGPIOByte: illegal port value...");
    }
    return val;
}




/*!
 **********************************************************************
 * @par Exported Function:
 *  Iow56ReadGPIOBit
 *
 * @par Description:
 Read a single bit value from first iow56 in system. Internally, always a
 complete set of 50 bits has to be read from the device, but this function will
 return only one bit value. (That is, if you want to read all 50 bits, use 
 Iow56ReadImmediateGPIO() - that will be 50 times faster...). Iow56 Pins are open collector
 with internal pull up. Writing a 1 to a pin enables "read" operation
 for that pin - without external driver the pin will read 1 due to
 pull up. Note: port 6 has only 2 usable bits: p6.6 and p6.7, see iow56
 documentation for details.
 *
 * @see
 * @arg Iow56ReadGPIOBit(), Iow56ReadImmediateGPIO(), Iow56ReadBlockingGPIO(), Iow56ReadGPIO()
 *
 * @param  devHandle  Type IOWKIT_HANDLE (get this Device handle from the function Iow56OpenDevice( ) )
 * @param  port Type INT (Portnumber of the IOW, allowed Value 0...6)
 * @param  bitNr Type INT (number of selected bit of port, allowed Value 0...7)
 *
 * @retval Value of selected bit,  0 or 1
 *
 * @par Example :
 *
 * @verbatim
    {   int val=0;

        printf("reading port P0.3: %X\n" Iow56ReadGPIOBit(devHandle, 0, 3));

    }
  @endverbatim
 ************************************************************************/
int Iow56ReadGPIOBit(IOWKIT_HANDLE devHandle, int port, int bitNr)
{   int val;
    
    val = Iow56ReadGPIOByte(devHandle, port);
    if(val & (1<<bitNr)) val = 1;
    else                 val = 0;
    return val;
}


