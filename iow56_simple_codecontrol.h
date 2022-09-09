
/*!
 ***********************************************************************
 *   @file                  iow56_simple_codecontrol.h
 *   @par Project:    IOW56-Simple Library
 *   @par Module:    Select Operation System (Linux / Windows)
 *
 *   @brief  This file is used to select the platform the simplified IOW-function library
 *              will be running on. Define the PLATFORM symbol to either
 *              OS_LINUX or OS_MS_WINDOWS and re-compile the module.
 *
 *
 *   @par Author:     Ernst Forgber
 *   @par Department: E14
 *   @par Company:    Fachhochschule Hannover - University of Applied Sciences and Arts
 *
 ***********************************************************************/

#ifndef _iow56_simple_codecontrol_h_
#define _iow56_simple_codecontrol_h_

/* don't edit this *******/
#ifndef OS_MS_WINDOWS
  #define OS_MS_WINDOWS 1
#endif
#ifndef OS_LINUX
  #define OS_LINUX      2
#endif


/***********************************************************************/
/******* select the platform PTL will be running on ********************/
/***********************************************************************/
#ifndef PLATFORM
  #define PLATFORM OS_MS_WINDOWS /*!< select OS_LINUX or OS_MS_WINDOWS */
#endif



#endif


