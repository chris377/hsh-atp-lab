

#include "PKWWaschen.h"
#include "shared_RAM.h"
#include "ptl_lib.h"




#if 0



#endif // 0

#define SAMPLE_PERIOD_IN_SEC 0.05


void Referenzposition_anfahren(void);/*referenzposition anfahren*/
void Alles_ausmachen(void);/*alles ausschalten*/
void Buerste_ganz_nach_oben_fahren(void);/*hor. Buerste fahren bis zu Ende*/
void Hoehe_der_Buerste_regulieren(void);/*die Hoehe von hor. Buerste regulieren, um die PKW richtig zu waschen*/
void Portal_ganz_nach_hinten_fahren(void);/*Portal fahren bis zu Ende*/
void PKWWaschen(void);/*mit dieser Funktion kann das PKW direkt waschen*/
void safer_nach_vorne(void);/*fuer die Bewegung von NACHVORNEFAHREN, Portal soll stoppen, wenn der Ende erreicht.*/
void safer_nach_hinten(void);/*fuer die Bewegung von NACHHINTENFAHREN, Portal soll stoppen, wenn der Ende erreicht.*/
void safer_nach_oben(void);/*fuer die Bewegung von NACHOBENFAHREN, hor. Buerste soll stoppen, wenn der Ende erreicht.*/
void safer_nach_unten(void);/*fuer die Bewegung von NACHUNTENFAHREN, hor. Buerste soll stoppen, wenn der Ende erreicht.*/



/****************** Threadfunktion und Funktionen, die der Thread nutzt *****/

/*!
**********************************************************************************************************
 @par exportierte Funktion:
 PTL_THREAD_RET_TYPE Steuerung_ThreadFunc

 @par Beschreibung:
 Es ist eine ThreadFunktion(Thread Steuerung). Mit bedingte Bewegungsnummer wird bedingte Bewegung durchgefuehrt.

 @param Bewegungsnummer

 @retval mit Bewegungsnummer die dazugehoerige Bewegung durchfuhren

**********************************************************************************************************/

PTL_THREAD_RET_TYPE Steuerung_ThreadFunc(void* pt)
{

    printf("Steuerung_ThreadFunc Thread ist gestartet...");


    while(sRAM_getTerminateFlag()==0)
    {
        PTL_Sleep(SAMPLE_PERIOD_IN_SEC);

        switch(sRAM_get_BewegungsNummer())
        {
        case BEWEGUNG_ALLES_AUS:
            Alles_ausmachen();
            break;
        case EINMAL_WASCHEN:
            PKWWaschen();
            printf("einmalwaschen!");
            sRAM_set_BewegungsNummer(BEWEGUNG_ALLES_AUS);
            break;
         case REFERENZPOSITION_ANFAHREN:
            Referenzposition_anfahren();
            printf("referenzposition anfahren!");
            sRAM_set_BewegungsNummer(BEWEGUNG_ALLES_AUS);
            break;
         case MEHRMALS_WASCHEN:
            PKWWaschen();
            printf("mehrmals waschen!");
            break;
        case NACHHINTENFAHREN:
            safer_nach_hinten();
            break;
        case NACHVORNEFAHREN:
            safer_nach_vorne();

            break;
        case NACHHINTENFAHRENSTOP:
            sRAM_set_Portalzuruekfahren(0);
            break;
        case NACHVORNEFAHRENSTOP:
            sRAM_set_Portalvorwaertsfahren(0);
            break;
        case NACHOBENFAHREN:
            safer_nach_oben();
            break;
        case NACHOBENFAHRENSTOP:
            sRAM_set_horBuerstenachobenfahren(0);
            break;
        case NACHUNTENFAHREN:
            safer_nach_unten();
            break;
        case NACHUNTENFAHRENSTOP:
            sRAM_set_horBuerstenachuntenfahren(0);
            break;
        case HORBUERSTEAN:
            sRAM_set_horbuerst(1);
            break;
        case HORBUERSTEAUS:
            sRAM_set_horbuerst(0);
            break;
        case VERTBUERSTEAN:
            sRAM_set_vertbuerst(1);
            break;
        case VERTBUERSTEAUS:
            sRAM_set_vertbuerst(0);
            break;
        case NOT_NOTAUS:
            PKWWaschen();
            printf("waschen!");
            break;
        default:
            puts("ungültiger Wert für Beweungsnummer?!");
            break;

        }


    }
    printf("Steuerung_ThreadFunc Thread terminiert...");
    sRAM_signalTerminateSema();

    return 0;
}


/*---------------------------------------------*/
/*!
**********************************************************************************************************
 @par exportierte Funktion:
 Alles_ausmachen

 @par Beschreibung:
 In diesem Funktion wird alles ausschalten. Es wird auch benutzt als die Inialisierung von Bewegungsnummer.

**********************************************************************************************************/



void Alles_ausmachen(void)
{
    sRAM_set_vertbuerst(0);
    sRAM_set_horbuerst(0);
    sRAM_set_Portalzuruekfahren(0);
    sRAM_set_Portalvorwaertsfahren(0);
    sRAM_set_horBuerstenachuntenfahren(0);
    sRAM_set_horBuerstenachobenfahren(0);
}


/*!
**********************************************************************************************************
 @par exportierte Funktion:
 Referenzposition_anfahren

 @par Beschreibung:
 Durch dieser Funktion faehrt die Anlage zu Referenzposition. Bei Referenzposition soll die hor. Buerste ganz an
 oben und das Portal ganz an hinten sein. Bevor das Anfahren von Referenzpositon wird alles ausgeschaltet, weil
 ansonst gibt es Konflikt, z.B faehrt das Portal jetzt nach unten, danach drueckt "Referenzposition anfahren".
 Jetzt werden die Aktoren von "hor. Buerste nach oben fahren" und "hor. Buerste nach unten fahren" gleichzeitig
 zu 1 gesetzt. Bewegt dann die Anlage nicht mehr.


**********************************************************************************************************/


void Referenzposition_anfahren(void)
{
    Alles_ausmachen();
    Buerste_ganz_nach_oben_fahren();
    // Bürste nach oben fahren
    Portal_ganz_nach_hinten_fahren();
    // Portal nach vorne fahren
}

/*!
**********************************************************************************************************
 @par exportierte Funktion:
 Buerste_ganz_nach_oben_fahren
 Portal_ganz_nach_hinten_fahren

 @par Beschreibung:
die erste 2 Zeile sind nur fuer Versicherung. Diese Funktion wird nur in "Referenzposition_anfahren" benutzt.
Wenn es allein benutzt wird, ist die erste 2 Zeile sehr wichtig, weil damit die hor. Buerste angehaltet werden.

Wenn die hor. Buerste nicht ganz an oben ist, faehrt die hor. Buerste dann nach oben.

Es ist die gleiche Geschischte bei "Portal_ganz_nach_hinten_fahren"


**********************************************************************************************************/



void Buerste_ganz_nach_oben_fahren(void)
{
    // Burste anhalten
    sRAM_set_horBuerstenachobenfahren(0);
    sRAM_set_horBuerstenachuntenfahren(0);
    while(sRAM_get_EndschalterhorBuersteoben() == 0)
    {
        // hoch fahren
        sRAM_set_horBuerstenachobenfahren(1);
        PTL_Sleep(0.1);
    }
    sRAM_set_horBuerstenachobenfahren(0);
}

void Portal_ganz_nach_hinten_fahren(void)
{
    // Burste anhalten
    sRAM_set_Portalzuruekfahren(0);
    sRAM_set_Portalvorwaertsfahren(0);
    while(sRAM_get_Endschalterportalhinten() == 0)
    {
        // hoch fahren
        sRAM_set_Portalzuruekfahren(1);
        PTL_Sleep(0.1);
    }
    sRAM_set_Portalzuruekfahren(0);
}

/*!
**********************************************************************************************************
 @par exportierte Funktion:
 PKWWaschen

 @par Beschreibung:
Bei PKW waschen soll die Anlage zuerst bei die Referenzposition sein. Bevor das Waschen von PKW soll die
vert. und hor. Buerste anschalten. Danach faengt das Waschen an. Das Portal geht nach vorne. Die Voraussetzung fuer
alle Bewegung ist, dass der Notaus Taster nicht gedrueck wird.

Bewegung fuer Portal: Das Portal soll weiter nach vorne fahren, bis es die Ende(Endschalter portal vorne) erreicht.
Wenn das Portal die Ende erreicht(das Waschen fertig), geht es zurueck zu Referenzposition, bei Portal ist es
Endschalter portal hinten.

Bewegung fuer hor. Buerste:Hor. Buerste soll zuerst nach unten fahren. Mit den Sensoren wird die Hoehe von hor.
Buerste reguliert. Dadurch kann das PKW waschen. Wenn die Sensoren nichts empfinden, bleibt(oder geht) die Buerste
ganz an unten. Wenn das Waschen fertig ist(damals ist das Portal am Endschalter Portal vorne). Geht die Buerste
nach oben bis Endschalter hor. Buerste oben, also zurueck zu Referenzposition anfahren.


**********************************************************************************************************/

void PKWWaschen(void)
{
    Referenzposition_anfahren();
    sRAM_set_horbuerst(1);
    sRAM_set_vertbuerst(1);
    while((sRAM_get_Aktorenfreigeben()== 0)&&(sRAM_get_Endschalterportalvorne()==0))
    {
        Hoehe_der_Buerste_regulieren();
        sRAM_set_Portalvorwaertsfahren(1);
        PTL_Sleep(0.1);
    }
    Referenzposition_anfahren();
    Alles_ausmachen();
}


/*!
**********************************************************************************************************
 @par exportierte Funktion:
 Hoehe_der_Buerste_regulieren

 @par Beschreibung:
 Mit Hilfe von Sensoren wird die Hoehe von hor. Buerste reguliert. Wenn nichts empfindet wird, bleibt die hor. Buerste
 ganz an unten.


**********************************************************************************************************/

void Hoehe_der_Buerste_regulieren(void)
{
   sRAM_set_horBuerstenachobenfahren(0);
   sRAM_set_horBuerstenachuntenfahren(0);
   while((sRAM_get_Abstandsensorhorbuersthinten()==1)&&(sRAM_get_Abstandsensorhorbuerstvorne()==1)&&(sRAM_get_EndschalterhorBuersteunten()==0)&&(sRAM_get_Aktorenfreigeben()== 0))
   {
       sRAM_set_horBuerstenachuntenfahren(1);
   }
    sRAM_set_horBuerstenachobenfahren(0);
    sRAM_set_horBuerstenachuntenfahren(0);
   while(((sRAM_get_Abstandsensorhorbuersthinten()==0)||(sRAM_get_Abstandsensorhorbuerstvorne()==0))&&(sRAM_get_EndschalterhorBuersteoben()== 0)&& (sRAM_get_Aktorenfreigeben()== 0))
   {
       sRAM_set_horBuerstenachobenfahren(1);
   }
    sRAM_set_horBuerstenachobenfahren(0);
    sRAM_set_horBuerstenachuntenfahren(0);
}

/*!
**********************************************************************************************************
 @par exportierte Funktion:
 safer_nach_vorne
 safer_nach_hinten
 safer_nach_unten
 safer_nach_oben

 @par Beschreibung:
 Diese Funktionen sind fuer die einfache Bewegungen.

 z.B "safer_nach_vorne": Portal bewegt nach vorne bis  "Endschalter Portal vorne" erreicht. Wenn "Endschalter Portal vorne" erreicht wird,
 muss die Bewegung gestoppt werden.

 Es ist die gleiche Geschichte bei die andere 3 Funktionen.

**********************************************************************************************************/

void safer_nach_vorne(void)
{
    if(sRAM_get_Endschalterportalvorne()==0)
    {
        sRAM_set_Portalzuruekfahren(0);
        sRAM_set_Portalvorwaertsfahren(1);
    }
    else
    {
        sRAM_set_Portalvorwaertsfahren(0);
    }
}

void safer_nach_hinten(void)
{
     if(sRAM_get_Endschalterportalhinten()==0)
    {
        sRAM_set_Portalvorwaertsfahren(0);
        sRAM_set_Portalzuruekfahren(1);
    }
    else
    {
        sRAM_set_Portalzuruekfahren(0);
    }
}

void safer_nach_unten(void)
{
        if(sRAM_get_EndschalterhorBuersteunten()==0)
    {
        sRAM_set_horBuerstenachobenfahren(0);
        sRAM_set_horBuerstenachuntenfahren(1);
    }
    else
    {
        sRAM_set_horBuerstenachuntenfahren(0);
    }
}

void safer_nach_oben(void)
{
        if(sRAM_get_EndschalterhorBuersteoben()==0)
    {
        sRAM_set_horBuerstenachuntenfahren(0);
        sRAM_set_horBuerstenachobenfahren(1);
    }
    else
    {
        sRAM_set_horBuerstenachobenfahren(0);
    }
}
