/* iow_thread.c */

/*----------------------------------
pin connections:
port pin    is connected to
0   0       Endschalterportalhinten;       Endschalterportalhinten, 1: aktiv
0   1       Endschalterportalvorne;       Endschalterportalvorne, 1: aktiv
0   2       EndschalterhorBuersteunten;       EndschalterhorBuersteunten, 1: aktiv
0   3       EndschalterhorBuersteoben;       EndschalterhorBuersteoben, 1: aktiv
0   4       Abstandsensorhorbuersthinten;       Abstandsensorhorbuersthinten, 0: aktiv
0   5       Abstandsensorhorbuerstvorne;       Abstandsensorhorbuerstvorne, 0: aktiv

------------------------------------------------
1   0       Portalzuruekfahren;    Portalzuruekfahren, 1: aktiv
1   1       Portalvorwaertsfahren;   Portalvorwaertsfahren, 1: aktiv
1   2       horBuerstenachobenfahren;    horBuerstenachobenfahren, 1: aktiv
1   3       horBuerstenachuntenfahren;    horBuerstenachuntenfahren, 1: aktiv
1   4       horbuerst;    horbuerst, 1: aktiv
1   5       vertbuerst;    vertbuerst, 1: aktiv

------------------------------------------------
2   0       Aktorenfreigeben;      Aktorenfreigeben, 1: aktiv
----------------------------------*/



#include "iow_thread.h"
#include "shared_RAM.h"
#include "iow56_simple.h"

#define SAMPLE_PERIOD_IN_SEC 0.01
#define USE_IOW_HARDWARE 1   /* 0: no hardware  1: use IOW hardware */

/*-------------------------------------*/
//#define DEBUG_MODULE

#ifdef DEBUG_MODULE
  #define DebugCode( code_fragment ) { code_fragment }
#else
  #define DebugCode( code_fragment )
#endif
/*-------------------------------------*/


static IOWKIT_HANDLE InitIOW56(void);
static int  getBit(int x, int bitNumber);
static void setBit(int *x, int bitNumber, int bitVal);



/****************** Threadfunktion und Funktionen, die der Thread nutzt *****/
PTL_THREAD_RET_TYPE iow_ThreadFunc(void* pt)
{   IOWKIT_HANDLE devHandle;
    int p0,p1,p2;

    DebugCode( puts("iow_ThreadFunc()..."); )
    printf("IOW Thread ist gestartet...");
    // iow initialisieren
#if USE_IOW_HARDWARE
    if(NULL==(devHandle = InitIOW56()))
    {       printf("cannot open IOW56 device...\n");
            return -1;
    }
#else
    printf("\n\n\nTESTVERSION ohne IOWarrior!!\n\n\n");
#endif

    while(sRAM_getTerminateFlag()==0)
    {
        // periodic thread action...

        PTL_Sleep(SAMPLE_PERIOD_IN_SEC);
        //DebugCode( printf("+"); )
        // read all sensors and write to shared ram
#if USE_IOW_HARDWARE
        p0 = Iow56ReadGPIOByte(devHandle, 0);
#else
        p0 = 0;
#endif
        DebugCode( printf("p0(r):%x  ",p0); )
        sRAM_set_Endschalterportalhinten( getBit(p0,0));
        sRAM_set_Endschalterportalvorne( getBit(p0,1));
        sRAM_set_EndschalterhorBuersteunten( getBit(p0,2));
        sRAM_set_EndschalterhorBuersteoben( getBit(p0,3));
        sRAM_set_Abstandsensorhorbuersthinten( getBit(p0,4));
        sRAM_set_Abstandsensorhorbuerstvorne( getBit(p0,5));


        p1 = 0;

        DebugCode( printf("p1(r):%x  ",p1); )
        setBit(&p1, 0, sRAM_get_Portalzuruekfahren());
        setBit(&p1, 1, sRAM_get_Portalvorwaertsfahren());
        setBit(&p1, 2, sRAM_get_horBuerstenachobenfahren());
        setBit(&p1, 3, sRAM_get_horBuerstenachuntenfahren());
        setBit(&p1, 4, sRAM_get_horbuerst());
        setBit(&p1, 5, sRAM_get_vertbuerst());


        Iow56WriteGPIOByte(devHandle, 1, p1);

        // read all actor setpoints and write to iow
        p2=0;
        setBit(&p2, 0, sRAM_get_Aktorenfreigeben());

        DebugCode( printf("p2(w):%x  ",p2); )
#if USE_IOW_HARDWARE
        Iow56WriteGPIOByte(devHandle, 2, p2);
#endif
        // Freigabesignal 0:aktiv

    }

    // iow schliessen
#if USE_IOW_HARDWARE
    Iow56CloseDevice(devHandle);
#endif
    printf("IOW Thread terminiert...");
    sRAM_signalTerminateSema();

    return 0;
}


/*---------------------------------------------*/



IOWKIT_HANDLE InitIOW56(void)
{       IOWKIT_HANDLE devHandle;

        devHandle = Iow56OpenDevice();
        if (devHandle == NULL)
        {       printf("Failed to open IOW56 device\n");
                return NULL;
        }
        /* alle Pins koennen als Ein- und Ausgang benutzt werden.
           Die Ausgaenge sind als open-collector Schaltung realisiert, wenn
           sie durchschalten, verbinden sie den IOW-Pin mit Masse.
           D.h. man kann einen Pin nur dann als Eingang benutzen, wenn man
           den open-collector Transistor abschaltet, d.h. eine logische EINS
           ausgibt. Daher gibt diese Routine zunaechst an allen Pins EINS aus. */
        {   Iow56Ports_t p={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
            Iow56WriteGPIO(devHandle, p);
        }
        puts("Init: alle IOW Pins auf '1' gesetzt.");
        return devHandle;
}


/*-----------------------------------*/
static int  getBit(int x, int bitNumber)
{
    if(x & (1<<bitNumber)) return 1;
    else                   return 0;
}
/*-----------------------------------*/
static void setBit(int *x, int bitNumber, int bitVal)
{
    if(bitVal==0)
    {
        *x = (*x) & (~(1<<bitNumber));  // Bit loeschen
    }
    else
    {
        *x = (*x) | (1<<bitNumber);     // Bit auf 1 setzen
    }
}
/*-----------------------------------*/
/*-----------------------------------*/
/*-----------------------------------*/


