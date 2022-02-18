#include "hist_list.h"
#include <stdlib.h>
#include <string.h>


bool createNode(tPosL *p){
    *p = malloc(sizeof(struct tNode)); //Reserva espacio en memoria
    return *p != LNULL;
}

void createEmptyList(tList* L){
    *L = LNULL;
}


bool insertItem(tItemL d, tList* L){
    tPosL q, r; //Punteros a un Nodo

    if (!createNode(&q)){
        return false;
    }else{
        q -> data = d; //Le asigna el item d a q
        q -> next = LNULL;

        if (*L == LNULL){ //Lista vacía
            q -> data.orden = 0;
            *L = q;
        }else{
            for (r = *L; r->next != LNULL ; r = r->next);
            q -> data.orden = r->data.orden + 1;
            r -> next = q;

        }
        return true;
    }

}


void deleteAtPosition(tPosL p, tList* L){
    tPosL q;

    if (p == *L){ //Borrar primer elemento
        *L = (*L) -> next; //Ahora la lista apunta al segundo elemento
    }else{
        if (p -> next == LNULL){ //Eliminar ultima posición

            for (q = *L; q -> next != p; q = q -> next); //Busca el anterior de p
            q -> next = LNULL;
        }else{ //eliminar posición intermedia
            q = p -> next;
            p -> data = q -> data;
            p -> next = q -> next;
            p = q;
        }
    }
    free(p); //Libera el espacio de memoria de p

}



tPosL findItem(tItemL d, tList L){
    tPosL p;

    for ( p = L; (p != LNULL) && (p -> data.orden != d.orden) ; p = p -> next); //Busca la posición del nickname introducido

    return p;
}



bool isEmptyList(tList L){
    return L == LNULL;
}


tItemL getItem(tPosL p, tList L){
    return p -> data;
}


tPosL first(tList L){
    return L;
}



tPosL last(tList L){
    tPosL p;

    for(p = L; p -> next != LNULL; p = p -> next);
    return p;

}


tPosL previous(tPosL p, tList L){
    tPosL q;

    if (p == L){
        return LNULL;
    }else{
        for(q = L; q -> next != p; q = q -> next);
        return q;
    }
}


tPosL next(tPosL p, tList L){
    return p -> next;
}
