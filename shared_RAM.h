#ifndef SHARED_RAM_H_INCLUDED
#define SHARED_RAM_H_INCLUDED

#include <stdio.h>
#include <string.h>sRAM_setTerminateFlag(int x);
#include "ptl_lib.h"

void sRAM_init(void);
void sRAM_destroy(void);
void sRAM_waitTerminateSema(void);
void sRAM_signalTerminateSema(void);

int  sRAM_getTerminateFlag(void);
void sRAM_setTerminateFlag(int x);


int  sRAM_get_Endschalterportalhinten(void);
void sRAM_set_Endschalterportalhinten(int x);
int  sRAM_get_Endschalterportalvorne(void);
void sRAM_set_Endschalterportalvorne(int x);
int  sRAM_get_EndschalterhorBuersteunten(void);
void sRAM_set_EndschalterhorBuersteunten(int x);
int  sRAM_get_EndschalterhorBuersteoben(void);
void sRAM_set_EndschalterhorBuersteoben(int x);
int  sRAM_get_Abstandsensorhorbuersthinten(void);
void sRAM_set_Abstandsensorhorbuersthinten(int x);
int  sRAM_get_Abstandsensorhorbuerstvorne(void);
void sRAM_set_Abstandsensorhorbuerstvorne(int x);

int  sRAM_get_Portalzuruekfahren(void);
void sRAM_set_Portalzuruekfahren(int x);

int  sRAM_get_Portalvorwaertsfahren(void);
void sRAM_set_Portalvorwaertsfahren(int x);
int  sRAM_get_horBuerstenachobenfahren(void);
void sRAM_set_horBuerstenachobenfahren(int x);
int  sRAM_get_horBuerstenachuntenfahren(void);
void sRAM_set_horBuerstenachuntenfahren(int x);
int  sRAM_get_horbuerst(void);
void sRAM_set_horbuerst(int x);
int  sRAM_get_vertbuerst(void);
void sRAM_set_vertbuerst(int x);
int  sRAM_get_Aktorenfreigeben(void);
void sRAM_set_Aktorenfreigeben(int x);

int  sRAM_get_BewegungsNummer(void);
void sRAM_set_BewegungsNummer(int x);
int sRAM_get_CheckNummer(void);
void sRAM_set_CheckNummer(int x);

#endif // SHARED_RAM_H_INCLUDED
