/* safety_thread.h */

#ifndef _steuerung_thread_h_
#define _steuerung_thread_h_

#include <stdio.h>
#include <string.h>
#include "ptl_lib.h"



#define BEWEGUNG_ALLES_AUS 0
#define BEWEGUNG_TEST 1
#define REFERENZPOSITION_ANFAHREN 2
#define MEHRMALS_WASCHEN 3
#define EINMAL_WASCHEN 4
#define NACHHINTENFAHREN 5
#define NACHHINTENFAHRENSTOP 6
#define NACHVORNEFAHREN 7
#define NACHVORNEFAHRENSTOP 8
#define NACHOBENFAHREN 9
#define NACHOBENFAHRENSTOP 10
#define NACHUNTENFAHREN 11
#define NACHUNTENFAHRENSTOP 12
#define HORBUERSTEAN 13
#define HORBUERSTEAUS 14
#define VERTBUERSTEAN 15
#define VERTBUERSTEAUS 16
#define NOT_NOTAUS 17




/* Prototyp der Threadfundktion */
PTL_THREAD_RET_TYPE Steuerung_ThreadFunc(void* pt);



#endif
