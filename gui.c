#include <stdio.h>
#include <string.h>
#include "ptl_lib.h"

#include "iow_thread.h"
#include "safety_thread.h"
#include "shared_RAM.h"
#include "PKWWaschen.h"
#include "gui.h"


int i,j,k,l,m,n;
static App *app;
static Window *w;
Control *hinten, *vorne, *unten, *oben, *hinten2, *vorne2, *nachhinten, *nachvorne, *nachoben, *nachunten,*horbuerste, *vertbuerste,*nachhinten2,*nachvorne2;


void ist_checked_Portal_nach_hinten(void); /* fuer checkbox: Portal nach hinten fahren, check:fahren uncheck:fahren stop*/
void ist_checked_Portal_nach_vorne(void); /* fuer checkbox: Portal nach vorne fahren, check:fahren uncheck:fahren stop*/
void ist_checked_Hoerbuerste_nach_oben(void);/* fuer checkbox: Hor. Buerste nach oben fahren, check:fahren uncheck:fahren stop*/
void ist_checked_Hoerbuerste_nach_unten(void);/* fuer checkbox: Hor. Buerste nach unten fahren, check:fahren uncheck:fahren stop*/
void ist_checked_hoerbueste(void);/* fuer checkbox: Hor.Buerste anschalten, check:fahren uncheck:fahren stop*/
void ist_checked_vertbueste(void);/* fuer checkbox: vert. Buerste anschalten, check:fahren uncheck:fahren stop*/

void check_nummer_Abstandssensor_hinten_1(Control *a); /* fuer checkbox: Abstandssensor hinten, sensor empfinden: check, sensor nicht empfinden: uncheck */
void check_nummer_Abstandssensor_hinten_2(Timer *b);/* fuer Timer: Abstandssensor hinten, um ob der Abstandssensor etwas empfinden zu testen */
void check_nummer_Abstandssensor_vorne_1(Control *a);/* fuer checkbox: Abstandssensor vorne, sensor empfinden: check, sensor nicht empfinden: uncheck */
void check_nummer_Abstandssensor_vorne_2(Timer*b);/* fuer Timer: Abstandssensor vorne, sensor empfinden: um ob der Abstandssensor etwas empfinden zu testen*/
void check_nummer_endschalter_Buerste_oben_1(Control*a);/* fuer checkbox: Endschalter hor. Buerste oben, sensor empfinden: check, sensor nicht empfinden: uncheck */
void check_nummer_endschalter_Buerste_oben_2(Timer *b);/* fuer Timer: Endschalter hor. Buerste oben, sensor empfinden: um ob die hor. Buerste die Ende erreicht zu testen*/
void check_nummer_endschalter_Buerste_unten_1(Control *a);/* fuer checkbox: Endschalter hor. Buerste unten, sensor empfinden: check, sensor nicht empfinden: uncheck */
void check_nummer_endschalter_Buerste_unten_2(Timer *b);/* fuer Timer: Endschalter hor. Buerste unten, sensor empfinden: um ob die hor. Buerste die Ende erreicht zu testen*/
void check_nummer_endschalter_portal_hinten_1(Control *a);/* fuer checkbox: Endschalter Portal hinten, sensor empfinden: check, sensor nicht empfinden: uncheck */
void check_nummer_endschalter_portal_hinten_2(Timer *b);/* fuer Timer: Endschalter Portal hinten, sensor empfinden: um ob den Portal die Ende erreicht zu testen*/
void check_nummer_endschalter_portal_vorne_1(Control *a);/* fuer checkbox: Endschalter Portal vorne, sensor empfinden: check, sensor nicht empfinden: uncheck */
void check_nummer_endschalter_portal_vorne_2(Timer *b);/* fuer Timer: Endschalter Portal vorne, sensor empfinden: um ob den Portal die Ende erreicht zu testen*/

void check_nummer_nachhinten(Control *a);/*fuer checkbox: check: Portal nach hinten fahren, uncheck: Portal nach hinten fahren stop*/
void check_nummer_nachvorne(Control *a);/*fuer checkbox: check: Portal nach vorne fahren, uncheck: Portal nach vorne fahren stop*/
void check_nummer_nachoben(Control *a);/*fuer checkbox: check: hor. Buerste nach oben fahren, uncheck: hor. Buerste nach oben fahren stop*/
void check_nummer_nachunten(Control *a);/*fuer checkbox: check: hor. Buerste nach unten fahren, uncheck: hor. Buerste nach unten fahren stop*/
void check_nummer_hoerbueste(Control *a);/*fuer checkbox: check: hor. Buerste anschalten, uncheck: hor. Buerste ausschalten*/
void check_nummer_vertbueste(Control *a);/*fuer checkbox: check: vert. Buerste anschalten, uncheck: vert. Buerste ausschalten*/

void Notaus_Taster(Control *a);/*fuer Taster, drueken: NOTAUS!*/
void Refposition(Control *a);/*fuer Taster, drueken: referenzposition anfahren*/
void einmalwaschen(Control *a);/*fuer Taster, drueken: einmal waschen*/
void mehrmalswaschen (Control *a);/*fuer Taster, drueken: mehrmals waschen*/
void check_nummer_not_notaus(Control *a);/*fuer Taster, drueken nach Notaus: wieder anfangen zu waschen*/

void check_nummer_Aktoren_nachhinten(Timer *b);/*fuer Timer, wenn Portal nach hinten fahren, check checkbox:"Portal nach hinten fahren"*/
void check_nummer_Aktoren_nachvorne(Timer *b);/*fuer Timer, wenn Portal nach vorne fahren, check checkbox:"Portal nach vorne fahren"*/
void check_nummer_Aktoren_nachoben(Timer *b);/*fuer Timer, wenn hor. Buerste nach oben fahren, check checkbox:"hor. Buerste nach oben fahren"*/
void check_nummer_Aktoren_nacunten(Timer *b);/*fuer Timer, wenn hor. Buerste nach unten fahren, check checkbox:"hor. Buerste nach unten fahren"*/
void check_nummer_Aktoren_hor_an(Timer *b);/*fuer Timer, wenn hor. Buerste anschalten, check checkbox:"hor. Buerste an*/
void check_nummer_Aktoren_vert_an(Timer *b);/*fuer Timer, wenn vert. Buerste anschalten, check checkbox:"vert. Buerste an*/

/*!
**********************************************************************************************************
 @par exportierte Funktion:
 gui

 @par Beschreibung:
 Die Funktion fuer Oberflaeche.

**********************************************************************************************************/

void gui(int argc, char *argv[])
{
  Rect r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16,r17,r18,r19,r20,r21;
  Timer *t1,*t2,*t3,*t4,*t5,*t6,*t7,*t8,*t9,*t11,*t12,*t14;

  app = new_app(argc, argv);
  w = new_window(app, rect(0,0,600,550),"PKWWaschen", STANDARD_WINDOW);


  r1 = rect( 50,50,60,30);
  new_label(w,r1,"Seneoren:", ALIGN_LEFT);


  r2 = rect (50,120,150,30);
  new_label(w, r2, "Endschalter Portal:", ALIGN_LEFT);
  r3 = rect (50,140,60,30);
  t1 = new_timer(app,check_nummer_endschalter_portal_hinten_2,750);
  hinten = new_check_box (w, r3, "hinten", check_nummer_endschalter_portal_hinten_1);
  check_nummer_endschalter_portal_hinten_2(t1);
  r4 = rect (150,140,60,30);
  t2 = new_timer(app,check_nummer_endschalter_portal_vorne_2,750);
  vorne =new_check_box  (w, r4, "vorne", check_nummer_endschalter_portal_vorne_1);
  check_nummer_endschalter_portal_vorne_2(t2);/*fuer Endschalter Portal vorne und hinten*/



  r5 = rect (50,190,150,30);
  new_label(w,r5, "Endschalter Buerste:", ALIGN_LEFT);
  r6 = rect (50,210,60,30);
  t3 = new_timer(app,check_nummer_endschalter_Buerste_unten_2,750);/*Timer*/
  unten = new_check_box (w, r6, "unten", check_nummer_endschalter_Buerste_unten_1);
  check_nummer_endschalter_Buerste_unten_2(t3);/*Timer*/
  r7 = rect (150,210,60,30);
  t4 = new_timer(app,check_nummer_endschalter_Buerste_oben_2,750);/*Timer*/
  oben = new_check_box (w, r7, "oben", check_nummer_endschalter_Buerste_oben_1);
  check_nummer_endschalter_Buerste_oben_2(t4);/*Timer*/
  /*fuer Endschalter hor. Buerste oben und unten*/



  r8 = rect (50,260,150,30);
  new_label(w,r8,"Abstandssensor:",  ALIGN_LEFT);
  r9 = rect (50,280,60,30);
  t5 = new_timer(app,check_nummer_Abstandssensor_hinten_2,750);/*Timer*/
  hinten2 = new_check_box (w, r9, "hinten", check_nummer_Abstandssensor_hinten_1);
  check_nummer_Abstandssensor_hinten_2(t5);/*Timer*/
  r10 = rect (150,280,60,30);
  t6 = new_timer(app,check_nummer_Abstandssensor_vorne_2,750);/*Timer*/
  vorne2 = new_check_box (w, r10, "vorne", check_nummer_Abstandssensor_vorne_1);
  check_nummer_Abstandssensor_vorne_2(t6);/*Timer*/
  /*fuer Abstandssensor vorne und hinten*/




 r11 = rect (350,50,60,30);
  new_label(w,r11, "Aktoren:", ALIGN_LEFT);


  r12 = rect (350,120,250,30);
  t7 = new_timer(app,check_nummer_Aktoren_nachhinten,750);/*Timer*/
  nachhinten = new_check_box(w,r12, "Portal nach hinten bewegen", check_nummer_nachhinten);/*Checkbox*/
  check_nummer_Aktoren_nachhinten(t7);/*Timer*/
  r13 = rect (350,140,250,30);
  t8 = new_timer(app,check_nummer_Aktoren_nachvorne,750);/*Timer*/
  nachvorne = new_check_box(w,r13, "Portal nach vorne bewegen", check_nummer_nachvorne);/*Checkbox*/
  check_nummer_Aktoren_nachvorne(t8);/*Timer*/
  /*Timer und checkbox fuer Portal nach vorne und hinten fahren*/


   r14 = rect (350,190,250,30);
   t9 = new_timer(app,check_nummer_Aktoren_nachoben,750);/*Timer*/
  nachoben = new_check_box(w,r14, "hor.Buerste nach oben bewegen", check_nummer_nachoben);/*Checkbox*/
  check_nummer_Aktoren_nachoben(t9);/*Timer*/
  r15 = rect (350,210,250,30);
    t11 = new_timer(app,check_nummer_Aktoren_nacunten,750);/*Timer*/
  nachunten = new_check_box(w, r15, "hor.Buerste nach unten bewegen", check_nummer_nachunten);/*Checkbox*/
  check_nummer_Aktoren_nacunten(t11);/*Timer*/
  /*Timer und checkbox fuer hor. Buerste nach oben und unten fahren*/

  r16 = rect (350,260,250,30);
    t12 = new_timer(app,check_nummer_Aktoren_hor_an,750);/*Timer*/
  horbuerste = new_check_box(w,r16, "hor.Buerste an",  check_nummer_hoerbueste);/*Checkbox*/
  check_nummer_Aktoren_hor_an(t12);/*Timer*/
  r17 = rect (350,280,250,30);
    t14 = new_timer(app,check_nummer_Aktoren_vert_an,750);/*Timer*/
  vertbuerste = new_check_box(w, r17, "vert.Buerste an", check_nummer_vertbueste);/*Checkbox*/
  check_nummer_Aktoren_vert_an(t14);/*Timer*/
  /*Timer und checkbox fuer vert. und hor. Buerste anschalten*/


  r18 = rect (250,330,120,50);
  new_button(w,r18,"Notaus",Notaus_Taster);
  r19 = rect (100,400,120,50);
  new_button(w, r19, "referenz Position anfahren", Refposition);
  r20 = rect (250,400,120,50);
  new_button(w, r20, "einmal waschen", einmalwaschen);
  r20 = rect (400,400,120,50);
  new_button(w, r20, "mehrmals waschen", mehrmalswaschen);
  r20 = rect (250,470,120,50);
  new_button(w, r20, "starten", check_nummer_not_notaus);




  show_window(w);
  main_loop(app);
  return;
}
/*!
**********************************************************************************************************
 @par exportierte Funktion:
 check_nummer_endschalter_portal_hinten_1
 check_nummer_endschalter_portal_vorne_1
 check_nummer_endschalter_Buerste_oben_1
 check_nummer_endschalter_Buerste_unten_1
 check_nummer_Abstandssensor_vorne_1
 check_nummer_Abstandssensor_hinten_1

 @par Beschreibung:
 Diese sind Funktionen von Checkbox fuer Sensoren. Sie sind leer, weil das Checkbox nur fuer die Anzeige
 dient, ob der Sensor etwas empfindet. Wenn etwas empfindet wird, wird das checkbox angekreuzt, sonst nicht
 angekreuzt.

**********************************************************************************************************/

/*!
**********************************************************************************************************
 @par exportierte Funktion:
 check_nummer_endschalter_portal_hinten_2
 check_nummer_endschalter_portal_vorne_2
 check_nummer_endschalter_Buerste_oben_2
 check_nummer_endschalter_Buerste_unten_2
 check_nummer_Abstandssensor_vorne_2
 check_nummer_Abstandssensor_hinten_2

 @par Beschreibung:
 Diese sind Funktionen von Timer fuer Sensoren. Sie fragen jede 750 ms den Sensoren an, ob sie jetzt etwas
 empfenden. Wenn sie etwas empfinden, wird das zugehoerige Checkbox gekreuzt.

**********************************************************************************************************/






void check_nummer_endschalter_portal_hinten_1(Control *a)
{

}



void check_nummer_endschalter_portal_hinten_2(Timer *b)
{
      switch(sRAM_get_Endschalterportalhinten())
    {
        case 0: uncheck(hinten);
        break;

        case 1: check(hinten);
        break;

    }


}

void check_nummer_endschalter_portal_vorne_1(Control *a)
{

}

void check_nummer_endschalter_portal_vorne_2(Timer *b)
{

       switch(sRAM_get_Endschalterportalvorne())
    {
        case 0: uncheck(vorne);
        break;

        case 1: check(vorne);
        break;

    }
}

void check_nummer_endschalter_Buerste_oben_1(Control *a)
{


}

void check_nummer_endschalter_Buerste_oben_2(Timer *b)
{

       switch(sRAM_get_EndschalterhorBuersteoben())
    {
        case 0: uncheck(oben);
        break;

        case 1: check(oben);
        break;

    }
}

void check_nummer_endschalter_Buerste_unten_1(Control *a)
{

}

void check_nummer_endschalter_Buerste_unten_2(Timer *b)
{

       switch(sRAM_get_EndschalterhorBuersteunten())
    {
        case 0: uncheck(unten);
        break;

        case 1: check(unten);
        break;

    }
}

void check_nummer_Abstandssensor_vorne_1(Control *a)
{


}
void check_nummer_Abstandssensor_vorne_2(Timer *b)
{
        switch(sRAM_get_Abstandsensorhorbuerstvorne())
    {
        case 0: uncheck(vorne2);
        break;

        case 1: check(vorne2);
        break;

    }

}

void check_nummer_Abstandssensor_hinten_1(Control *a)
{

}
void check_nummer_Abstandssensor_hinten_2(Timer *b)
{

       switch(sRAM_get_Abstandsensorhorbuersthinten())
    {
        case 0: uncheck(hinten2);
        break;

        case 1: check(hinten2);
        break;

    }
}

/*!
**********************************************************************************************************
 @par exportierte Funktion:
 check_nummer_nachhinten
 check_nummer_nachvorne
 check_nummer_nachunten
 check_nummer_nachoben
 check_nummer_vertbueste
 check_nummer_hoerbueste

 @par Beschreibung:
 Diese sind Funktionen von Checkbox fuer Aktoren. Sie kooperieren mit Steuerungsthread und geben den Befehle
 fuer Bewegung(also zu Aktoren).

 z.B "check_nummer_nachhinten", wenn angekreuzt wird, set die Bewegungsnummer(Steuerungssthread) zu
 NACHHINTENFAHREN(in PKWWaschen.c), wenn nicht angekreuzt wird, set die Bewegungsnummer zu NACHHINTENFAHRENSTOP.
 Die andere sind die gleiche Geschichte wie "check_nummer_nachhinten".

 "check_nummer_nachvorne": Portal nach vorne fahren

 "check_nummer_nachunten": hor. Buerste nach unten fahren

 "check_nummer_nachoben" : hor. Buerste nach oben fahren

 "check_nummer_vertbueste" : vert. Buerste anschalten

 "check_nummer_hoerbueste" : hor. Buerste anschalten

**********************************************************************************************************/


void check_nummer_nachhinten(Control *a)
{


    printf("check_nummer_nachhinten gestartet\n");

    if(is_checked(nachhinten))
    {
    uncheck(nachvorne);
    sRAM_set_BewegungsNummer(NACHHINTENFAHREN);
    }
    else
    {
    sRAM_set_BewegungsNummer(NACHHINTENFAHRENSTOP);
    }


    }



void check_nummer_nachvorne(Control *a)
{

   if(is_checked(nachvorne))
   {

       uncheck(nachhinten);
       sRAM_set_BewegungsNummer(NACHVORNEFAHREN);
   }
   else
   {
       sRAM_set_BewegungsNummer(NACHVORNEFAHRENSTOP);
   }


}


  void check_nummer_nachunten(Control *a)
{
       if(is_checked(nachunten))
   {
       uncheck(nachvorne);
       sRAM_set_BewegungsNummer(NACHUNTENFAHREN);
   }
   else
   {
       sRAM_set_BewegungsNummer(NACHUNTENFAHRENSTOP);
   }

}


  void check_nummer_nachoben(Control *a)
{
       if(is_checked(nachoben))
   {
       sRAM_set_BewegungsNummer(NACHOBENFAHREN);
   }
   else
   {
       sRAM_set_BewegungsNummer(NACHOBENFAHRENSTOP);
   }

}

void check_nummer_vertbueste(Control *a)
{
          if(is_checked(vertbuerste))
   {
       sRAM_set_BewegungsNummer(VERTBUERSTEAN);
   }
   else
   {
       sRAM_set_BewegungsNummer(VERTBUERSTEAUS);
   }
    }

    void check_nummer_hoerbueste(Control *a)
{
             if(is_checked(horbuerste))
   {
       sRAM_set_BewegungsNummer(HORBUERSTEAN);
   }
   else
   {
       sRAM_set_BewegungsNummer(HORBUERSTEAUS);
   }
    }

void Notaus_Taster(Control *a)
{
    sRAM_set_Aktorenfreigeben(1);
}

void Refposition(Control *a)
{
    sRAM_set_BewegungsNummer(REFERENZPOSITION_ANFAHREN);
}

void einmalwaschen(Control *a)
{
    sRAM_set_BewegungsNummer(EINMAL_WASCHEN);
}

void mehrmalswaschen(Control *a)
{
    sRAM_set_BewegungsNummer(MEHRMALS_WASCHEN);
}

void check_nummer_not_notaus(Control *a)
{
    sRAM_set_Aktorenfreigeben(0);
    sRAM_set_BewegungsNummer(NOT_NOTAUS);
    }

/*!
**********************************************************************************************************
 @par exportierte Funktion:
 check_nummer_Aktoren_nachhinten
 check_nummer_Aktoren_nachvorne
 check_nummer_Aktoren_nachoben
 check_nummer_Aktoren_nacunten
 check_nummer_Aktoren_hor_an
 check_nummer_Aktoren_vert_an

 @par Beschreibung:
 Diese sind Funktionen von Timer fuer Aktoren. Sie empfinden jede 750 ms. Wenn die Bewegung durchgefuehrt wird,
 wird das Checkbox gekreuzt.


**********************************************************************************************************/


void check_nummer_Aktoren_nachhinten(Timer *b)
{

    if(sRAM_get_Portalzuruekfahren())
    {
        check(nachhinten);
    }
    else{
        uncheck(nachhinten);
    }




}
void check_nummer_Aktoren_nachvorne(Timer *b)
{
    if(sRAM_get_Portalvorwaertsfahren())
    {
        check(nachvorne);
    }
    else{
        uncheck(nachvorne);
    }
}

void check_nummer_Aktoren_nachoben(Timer *b)
{
        if(sRAM_get_horBuerstenachobenfahren())
    {
        check(nachoben);
    }
    else{
        uncheck(nachoben);
    }
}

void check_nummer_Aktoren_nacunten(Timer *b)
{
        if(sRAM_get_horBuerstenachuntenfahren())
    {
        check(nachunten);
    }
    else{
        uncheck(nachunten);
    }
}

void check_nummer_Aktoren_hor_an(Timer *b)
{
        if(sRAM_get_horbuerst())
    {
        check(horbuerste);
    }
    else{
        uncheck(horbuerste);
    }
}

void check_nummer_Aktoren_vert_an(Timer *b)
{
        if(sRAM_get_vertbuerst())
    {
        check(vertbuerste);
    }
    else{
        uncheck(vertbuerste);
    }
}
