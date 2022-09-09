/* safety_thread.c */


#include "safety_thread.h"
#include "shared_RAM.h"
#include "PKWWaschen.h"



#define DEBUG_MODULE

#ifdef DEBUG_MODULE
  #define DebugCode( code_fragment ) { code_fragment }
#else
  #define DebugCode( code_fragment )
#endif


#define SAMPLE_PERIOD_IN_SEC 0.05
// ------------------------------------------

#ifndef FALSE
    #define FALSE 0
#endif // FALSE
#ifndef TRUE
    #define TRUE  (!FALSE)
#endif // TRUE





/****************** Threadfunktion und Funktionen, die der Thread nutzt *****/
PTL_THREAD_RET_TYPE safety_ThreadFunc(void* pt)
{

    printf("safety Thread ist gestartet...");
    // init


    while(sRAM_getTerminateFlag()==0)
    {
        PTL_Sleep(SAMPLE_PERIOD_IN_SEC);

       if((sRAM_get_Endschalterportalhinten()== 1)&&(sRAM_get_Portalzuruekfahren()== 1))
       {
           sRAM_set_Portalzuruekfahren(0);
           puts("safety: ACHTUNG...");
       }
        /*Das Portal soll nicht weiter nach hinten fahren, wenn es der Endschalter erreicht.*/

       if((sRAM_get_EndschalterhorBuersteunten()==1)&&(sRAM_get_horBuerstenachuntenfahren()==1))
       {
           sRAM_set_horBuerstenachuntenfahren(0);
       }
       /*Die hor. Buerste soll nicht weiter nach unten fahren, wenn die der Endschalter erreicht.*/

       if((sRAM_get_Endschalterportalvorne()==1)&&(sRAM_get_Portalvorwaertsfahren()==1))
       {
           sRAM_set_Portalvorwaertsfahren(0);
       }
       /*Das Portal soll nicht weiter nach vorne fahren, wenn es der Endschalter erreicht.*/

       if((sRAM_get_EndschalterhorBuersteoben()==1)&&(sRAM_get_horBuerstenachobenfahren()==1))
       {
           sRAM_set_horBuerstenachobenfahren(0);
       }
       /*Die hor. Buerste soll nicht  weiter nach oben fahren, wenn die der Endschalter erreicht.*/



       sRAM_signalTerminateSema();
    }
    printf("safety Thread terminiert...");
    sRAM_signalTerminateSema();

    return 0;
}


