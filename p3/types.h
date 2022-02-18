//
// Created by brais on 21/9/21.
//

#ifndef P0_TYPES_H
#define P0_TYPES_H

#define CMD_LENGTH_LIMIT 4096
#define MAX_LETTERRS 4096
#define LEERCOMPLETO ((ssize_t)-1)


#define ERROR -1
#define ACTIVO 0
#define PARADO 1
#define TERMINADO 2
#define SENAL 3

typedef char tCmd[CMD_LENGTH_LIMIT];
typedef int tOrden;
typedef char *tMem;
typedef char tFecha[128];
typedef unsigned long int tSize;
typedef char tFichero[CMD_LENGTH_LIMIT];
typedef int tDescriptor;
typedef int tKey;
typedef int tPid;
typedef int tPriority;
typedef char tUser[400];
typedef char tComLine[MAX_LETTERRS];
typedef int tEstado;
typedef char *tSenal;

typedef struct tItemL {
    tCmd cmd;
    tOrden orden;
    tSize size;
    tMem mem;
    tFecha fecha;
    tFichero fichero;
    tDescriptor descriptor;
    tKey key;
    tPid pid;
    tPriority priority;
    tUser user;
    tComLine comLine;
    tEstado estado;
    tSenal senal;
} tItemL;


#endif //P0_TYPES_H


