//
// Created by brais on 21/9/21.
//

#ifndef P0_TYPES_H
#define P0_TYPES_H

#define CMD_LENGTH_LIMIT 4096

#define LEERCOMPLETO ((ssize_t)-1)

typedef char tCmd[CMD_LENGTH_LIMIT];
typedef int tOrden;
typedef char *tMem;
typedef char tFecha[128];
typedef unsigned long int tSize;
typedef char tFichero[CMD_LENGTH_LIMIT];
typedef int tDescriptor;
typedef int tKey;

typedef struct tItemL {
    tCmd cmd;
    tOrden orden;
    tSize size;
    tMem mem;
    tFecha fecha;
    tFichero fichero;
    tDescriptor descriptor;
    tKey key;
} tItemL;


#endif //P0_TYPES_H


