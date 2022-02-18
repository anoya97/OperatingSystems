
#ifndef P0_HIST_LIST_H
#define P0_HIST_LIST_H


#include "types.h"
#include <stdbool.h>

/* Definicion de tipos */

#define LNULL NULL


typedef struct tNode* tPosL;

struct tNode {
    tItemL data;
    tPosL next;

};
typedef tPosL tList;

/* Prototipos de funciones */

void createEmptyList(tList* L);
bool insertItem(tItemL d, tList* L);
void updateItem(tItemL d , tPosL p, tList* L);
void deleteAtPosition(tPosL p, tList* L);
void deleteAtPositionM(tPosL p, tList* L);
tPosL findItem(tItemL d, tList L);
bool isEmptyList(tList L);
tItemL getItem(tPosL p, tList L);
tPosL first(tList L) ;
tPosL last(tList L);
tPosL previous(tPosL p, tList L);
tPosL next(tPosL p, tList L);



#endif




