/* getting_started_main.c :
ATP-Lab: Autowaschanlage mit Sicherheits-Thread

*/

#include <stdio.h>
#include <string.h>
#include "ptl_lib.h"

#include "iow_thread.h"
#include "safety_thread.h"
#include "shared_RAM.h"
#include "PKWWaschen.h"
#include "gui.h"





/* Prototypen der Funktionen die main()  benutzt*/
void UserInterface(void);
int  PrintMenue(void);
void ExecuteMenue(int c);





/*---------------------------------------------*/

int main(int argc, char *argv[])
{   PTL_thread_t iow_ThreadID, safety_ThreadID, Steuerung_ThreadID;

    printf("ATP-Lab Hochregallager, getting started\n");

    /* shared RAM initialisieren, create semaphores */
    sRAM_init();
    /* threads starten */

    if(0!=PTL_CreateThread(&iow_ThreadID, iow_ThreadFunc, NULL))   /* thread von iow starten*/
    { puts("error starting thread");
      return -1;
    }
    if(0!=PTL_CreateThread(&safety_ThreadID, safety_ThreadFunc, NULL))   /* thread von safety starten*/
    { puts("error starting thread");
      return -1;
    }
    if(0!=PTL_CreateThread(&Steuerung_ThreadID, Steuerung_ThreadFunc, NULL))   /* thread von steuerung starten*/
    { puts("error starting thread");
      return -1;
    }




#if 0
    /* Textmenue */
    UserInterface();
#else
    /* hier später die grafische Oberflaeche */
    gui(argc, argv);
#endif

    /* Thread-Ende kommandieren: */
    sRAM_setTerminateFlag(1);
    puts("main() wartet auf das Ende der Threads...\n");
    sRAM_waitTerminateSema();
    sRAM_waitTerminateSema();
    sRAM_waitTerminateSema();
    puts("main() ist beendet...\n");
    sRAM_destroy();
    return 0;
}
/*---------------------------------------------*/
/*********** Funktionen die main()  benutzt ********************************/

/*---------------------------------------------*/
void UserInterface(void)
{   int c=0;
    while ((c!='q') && (c!='Q'))
    {   c=PrintMenue();
        ExecuteMenue(c);
    }
}
/*---------------------------------------------*/
int  PrintMenue(void)
{   int c;
    printf("a:stop\n");
    printf("b:anfangen\n");
    printf("c:portalvorwaertsfahren\n");
    printf("d:portalvorwaertsfahren stop\n");
    printf("q:quit\n");
    printf("-------------------\n");
    printf(">:");
    fflush(stdin);
    c=getchar();
    return c;
}
/*---------------------------------------------*/
void ExecuteMenue(int c)
{   int i;
    switch(c)
    {   case 'a':
        case 'A':sRAM_set_Aktorenfreigeben(1);
                  break;
        case 'b':
        case 'B':sRAM_set_Aktorenfreigeben(0);
                  break;
        case 'c':
        case 'C':sRAM_set_BewegungsNummer(NACHVORNEFAHREN);
                  break;
        case 'd':
        case 'D':sRAM_set_BewegungsNummer(NACHVORNEFAHRENSTOP);
                  break;

        case 'q':
        case 'Q': puts("Ende einleiten...");break;




        default:  printf("unbekanntes Kommando!");
                  break;
    }
}/*textmenue fuer testen Steuerung thread*/
/*---------------------------------------------*/

