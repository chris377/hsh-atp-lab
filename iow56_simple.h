 /*!
 ***********************************************************************
 *   @file                   iow56_simple.h
 *   @par Project:     Wrapper Library for the IOW56
 *   @par Module:     Select Operation System (Linux / Windows)
 *
 *   @brief  Defines, types and function prototypes of module iow_simple.c
 *              This module can handle only ONE IOW56 in the system - if
 *              there are more, use the original routines!
 *
 *
 *   @par Author:     Ernst Forgber
 *   @par Department: E14
 *   @par Company:    Fachhochschule Hannover - University of Applied Sciences and Arts
 *
 ***********************************************************************
 *
 *   @par History       :
 *   @verbatim
 * Ver  Date        Author            Change Description
 * 0.0  22.10.2007 E. Forgber     - First Version
 * 0.1  05.02.2009 E.Forgber      - Iow56Ports_t Iow56ReadImmediateGPIO(IOWKIT_HANDLE devHandle);  
 *
 *   @endverbatim
 *
 ***********************************************************************/

#ifndef _iow56_simple_h_
#define _iow56_simple_h_

#include "iow56_simple_codecontrol.h"


#if (PLATFORM==OS_LINUX)
        ToDo!!
        #include <stdio.h>
        #include <stdlib.h>
        #include <unistd.h>
        #include "iowwintypes.h"
        #include "iowkit.h"
#endif

#if (PLATFORM==OS_MS_WINDOWS)
  #define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers #include <windows.h>
  #include <windows.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <math.h>
  #include <winbase.h>
  #include "iowkit.h"
#endif

/* if not already done, define boolean constants: */
#ifndef FALSE   
    #define FALSE 0
#endif    
#ifndef TRUE
    #define TRUE (!FALSE)
#endif    




/*!
 *****************************************************************
  @par Description:
IOW56 has 50 IO-Pins, organized as 7 8-Bit Ports.
******************************************************************/
typedef struct {
    BYTE P0; /*!<@arg 8-Bit port P0 */
    BYTE P1; /*!<@arg 8-Bit port P1 */
    BYTE P2; /*!<@arg 8-Bit port P2*/
    BYTE P3; /*!<@arg 8-Bit port P3 */
    BYTE P4; /*!<@arg 8-Bit port P4*/
    BYTE P5; /*!<@arg 8-Bit port P5 */
    BYTE P6; /*!<@arg 8-Bit port P6. Has only 2 pins! Only Bits P6.0 and P6.7 may be used! */
} Iow56Ports_t; 


/***********************************************
 * exported functions
 ***********************************************/
IOWKIT_HANDLE Iow56OpenDevice(void); 
void Iow56CloseDevice(IOWKIT_HANDLE devHandle);

BOOLEAN Iow56WriteGPIOBit(IOWKIT_HANDLE devHandle, int port, int bitNr, int val);
BOOLEAN Iow56WriteGPIOByte(IOWKIT_HANDLE devHandle, int port, int val);
BOOLEAN Iow56WriteGPIO(IOWKIT_HANDLE devHandle, Iow56Ports_t allPorts);

Iow56Ports_t Iow56ReadGPIO(IOWKIT_HANDLE devHandle);
Iow56Ports_t Iow56ReadBlockingGPIO(IOWKIT_HANDLE devHandle);
Iow56Ports_t Iow56ReadImmediateGPIO(IOWKIT_HANDLE devHandle);

int Iow56ReadGPIOBit(IOWKIT_HANDLE devHandle, int port, int bitNr);
int Iow56ReadGPIOByte(IOWKIT_HANDLE devHandle, int port);

#endif

