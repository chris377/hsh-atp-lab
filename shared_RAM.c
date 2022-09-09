/* shared_RAM.c */

#include "shared_RAM.h"
#include "PKWWaschen.h"


typedef struct
{
    PTL_sem_t sema;
    PTL_sem_t terminate_sema;
    int emergencyStop;        /**< Freigabe von Port2 0: go, 1: stop */

    int terminateFlag;        /**< Flag zum Beenden der Threads 0:go, 1:terminate*/

    int Endschalterportalhinten;      /**< Endschalterportalhinten P 0.0, 1: aktiv */
    int Endschalterportalvorne;      /**< Endschalterportalvorne P 0.1, 1: aktiv */
    int EndschalterhorBuersteunten;      /**< EndschalterhorBuersteunten P 0.2, 1: aktiv */
    int EndschalterhorBuersteoben;      /**< EndschalterhorBuersteoben P 0.3, 1: aktiv */
    int Abstandsensorhorbuersthinten;      /**< Abstandsensorhorbuersthinten P 0.4, 0: aktiv */
    int Abstandsensorhorbuerstvorne;      /**< Abstandsensorhorbuerstvorne P 0.5, 0: aktiv */

    int Portalzuruekfahren;    /**< Portalzuruekfahren P 1.0,  1: aktiv */
    int Portalvorwaertsfahren;    /**< Portalvorwaertsfahren P 1.1, 1: aktiv */
    int horBuerstenachobenfahren;    /**< horBuerstenachobenfahren P 1.2, 1: aktiv */
    int horBuerstenachuntenfahren;    /**< horBuerstenachuntenfahren P 1.3, 1: aktiv */
    int horbuerst;    /**< horbuerstan P 1.4, 1: aktiv */
    int vertbuerst;    /**<  vertbuerst P 1.5, 1: aktiv */


    int Aktorenfreigeben;     /**< Aktorenfreigeben P2.0, 0: aktiv */

    int BewegungsNummer;


} sram_t;

static sram_t sRAM;








//#define DEBUG_MODULE

#ifdef DEBUG_MODULE
  #define DebugCode( code_fragment ) { code_fragment }
#else
  #define DebugCode( code_fragment )
#endif


void sRAM_init(void)
{   DebugCode( puts("sRAM_init()..."); )
    if(0!= PTL_SemCreate(&(sRAM.sema), 1))
    {   puts("error creating semaphore\n");
    }
    if(0!= PTL_SemCreate(&(sRAM.terminate_sema), 0))
    {   puts("error creating semaphore\n");
    }
    // set usefull defaults...

    sRAM.terminateFlag   = 0;  /*  0: go, 1: terminate threads */

    sRAM.Endschalterportalhinten = 0;      /* Endschalterportalhinten, 1: aktiv */
    sRAM.Endschalterportalvorne = 0;      /* Endschalterportalvorne, 1: aktiv */
    sRAM.EndschalterhorBuersteunten = 0;      /* EndschalterhorBuersteunten, 1: aktiv */
    sRAM.EndschalterhorBuersteoben = 0;      /* EndschalterhorBuersteoben, 1: aktiv */
    sRAM.Abstandsensorhorbuersthinten = 1;      /* Abstandsensorhorbuersthinten, 0: aktiv */
    sRAM.Abstandsensorhorbuerstvorne = 1;      /* Abstandsensorhorbuerstvorne, 0: aktiv */

    sRAM.Portalzuruekfahren = 0; /* Portalzuruekfahren, 1: (aktiv) */
    sRAM.Portalvorwaertsfahren = 0;    /* Portalvorwaertsfahren, 1: aktiv */
    sRAM.horBuerstenachobenfahren = 0;    /* horBuerstenachobenfahren, 1: aktiv */
    sRAM.horBuerstenachuntenfahren = 0;    /* horBuerstenachuntenfahren, 1: aktiv */
    sRAM.horbuerst = 0;    /*horbuerst, 1: aktiv */
    sRAM.vertbuerst = 0;    /* vertbuerst, 1: aktiv */

    sRAM.Aktorenfreigeben = 0;    /* Aktorenfreigeben, 0: aktiv */

    sRAM.BewegungsNummer = 0;



}

//----------------------------------
void sRAM_destroy(void)
{   DebugCode( puts("sRAM_destroy()..."); )
    PTL_SemDestroy(&(sRAM.sema));
    PTL_SemDestroy(&(sRAM.terminate_sema));
}
//----------------------------------
void sRAM_waitTerminateSema(void)
{
    PTL_SemWait(&(sRAM.terminate_sema));
}
//----------------------------------
void sRAM_signalTerminateSema(void)
{
    PTL_SemSignal(&(sRAM.terminate_sema));
}
//----------------------------------

int  sRAM_getTerminateFlag(void)
{   int x;
    DebugCode( puts("sRAM_getTerminateFlag..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM.terminateFlag;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_setTerminateFlag(int x)
{   DebugCode( puts("sRAM_setTerminateFlag..."); )
    PTL_SemWait(&(sRAM.sema));
         sRAM.terminateFlag = x;
    PTL_SemSignal(&(sRAM.sema));
}


//----------------------------------
int  sRAM_get_Endschalterportalhinten(void)
{   int x;
    DebugCode( puts("sRAM_get_Endschalterportalhinten"); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM.Endschalterportalhinten;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_Endschalterportalhinten(int x)
{   DebugCode( puts("sRAM_set_Endschalterportalhinten..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM.Endschalterportalhinten= x;
    PTL_SemSignal(&(sRAM.sema));
}

int  sRAM_get_Endschalterportalvorne(void)
{   int x;
    DebugCode( puts("sRAM_get_Endschalterportalvorne..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM.Endschalterportalvorne;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_Endschalterportalvorne(int x)
{   DebugCode( puts("sRAM_set_Endschalterportalvorne..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM.Endschalterportalvorne= x;
    PTL_SemSignal(&(sRAM.sema));
}

int  sRAM_get_EndschalterhorBuersteunten(void)
{   int x;
    DebugCode( puts("sRAM_get_EndschalterhorBuersteunten..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM.EndschalterhorBuersteunten;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_EndschalterhorBuersteunten(int x)
{   DebugCode( puts("sRAM_set_EndschalterhorBuersteunten..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM.EndschalterhorBuersteunten= x;
    PTL_SemSignal(&(sRAM.sema));
}

int  sRAM_get_EndschalterhorBuersteoben(void)
{   int x;
    DebugCode( puts("sRAM_get_EndschalterhorBuersteoben..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM.EndschalterhorBuersteoben;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_EndschalterhorBuersteoben(int x)
{   DebugCode( puts("sRAM_set_EndschalterhorBuersteoben..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM.EndschalterhorBuersteoben= x;
    PTL_SemSignal(&(sRAM.sema));
}


int  sRAM_get_Abstandsensorhorbuersthinten(void)
{   int x;
    DebugCode( puts("sRAM_get_Abstandsensorhorbuersthinten..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM.Abstandsensorhorbuersthinten;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_Abstandsensorhorbuersthinten(int x)
{   DebugCode( puts("sRAM_set_Abstandsensorhorbuersthinten..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM.Abstandsensorhorbuersthinten= x;
    PTL_SemSignal(&(sRAM.sema));
}



int  sRAM_get_Abstandsensorhorbuerstvorne(void)
{   int x;
    DebugCode( puts("sRAM_get_Abstandsensorhorbuerstvorne..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM.Abstandsensorhorbuerstvorne;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_Abstandsensorhorbuerstvorne(int x)
{   DebugCode( puts("sRAM_set_Abstandsensorhorbuerstvorne..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM.Abstandsensorhorbuerstvorne= x;
    PTL_SemSignal(&(sRAM.sema));
}

int  sRAM_get_Portalzuruekfahren(void)
{   int x;
    DebugCode( puts("sRAM_get_Portalzuruekfahren..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM.Portalzuruekfahren;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_Portalzuruekfahren(int x)
{   DebugCode( puts("sRAM_set_Portalzuruekfahren..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM.Portalzuruekfahren= x;
    PTL_SemSignal(&(sRAM.sema));
}


int  sRAM_get_Portalvorwaertsfahren(void)
{   int x;
    DebugCode( puts("sRAM_get_Portalvorwaertsfahren..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM.Portalvorwaertsfahren;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_Portalvorwaertsfahren(int x)
{   DebugCode( puts("sRAM_set_Portalvorwaertsfahren..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM.Portalvorwaertsfahren= x;
    PTL_SemSignal(&(sRAM.sema));
}

int  sRAM_get_horBuerstenachobenfahren(void)
{   int x;
    DebugCode( puts("sRAM_get_ horBuerstenachobenfahren..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM. horBuerstenachobenfahren;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_horBuerstenachobenfahren(int x)
{   DebugCode( puts("sRAM_set_ horBuerstenachobenfahren..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM. horBuerstenachobenfahren= x;
    PTL_SemSignal(&(sRAM.sema));
}

int  sRAM_get_horBuerstenachuntenfahren(void)
{   int x;
    DebugCode( puts("sRAM_get_ horBuerstenachuntenfahren..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM. horBuerstenachuntenfahren;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_horBuerstenachuntenfahren(int x)
{   DebugCode( puts("sRAM_set_ horBuerstenachuntenfahren..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM. horBuerstenachuntenfahren= x;
    PTL_SemSignal(&(sRAM.sema));
}

int  sRAM_get_horbuerst(void)
{   int x;
    DebugCode( puts("sRAM_get_ horbuerst..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM. horbuerst;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_horbuerst(int x)
{   DebugCode( puts("sRAM_set_ horbuerst..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM. horbuerst= x;
    PTL_SemSignal(&(sRAM.sema));
}

int  sRAM_get_vertbuerst(void)
{   int x;
    DebugCode( puts("sRAM_get_ vertbuerst..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM. vertbuerst;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_vertbuerst(int x)
{   DebugCode( puts("sRAM_set_ vertbuerst..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM. vertbuerst= x;
    PTL_SemSignal(&(sRAM.sema));
}


int  sRAM_get_Aktorenfreigeben(void)
{   int x;
    DebugCode( puts("sRAM_get_ Aktorenfreigeben..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM. Aktorenfreigeben;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_Aktorenfreigeben(int x)
{   DebugCode( puts("sRAM_set_ Aktorenfreigeben..."); )
    PTL_SemWait(&(sRAM.sema));
        sRAM. Aktorenfreigeben= x;
    PTL_SemSignal(&(sRAM.sema));
}

int  sRAM_get_BewegungsNummer(void)
{   int x;
    DebugCode( puts("sRAM_get_ BewegungsNummer..."); )
    PTL_SemWait(&(sRAM.sema));
        x = sRAM. BewegungsNummer;
    PTL_SemSignal(&(sRAM.sema));
    return x;
}
void sRAM_set_BewegungsNummer(int x)
{   DebugCode( puts("sRAM_set_ BewegungsNummer..."); )
    printf("sRAM_set_BewegungsNummer: %d\n", x);
    PTL_SemWait(&(sRAM.sema));
        sRAM. BewegungsNummer= x;
    PTL_SemSignal(&(sRAM.sema));
}




