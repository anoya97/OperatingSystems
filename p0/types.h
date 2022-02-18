//
// Created by brais on 21/9/21.
//

#ifndef P0_TYPES_H
#define P0_TYPES_H

#define CMD_LENGTH_LIMIT 4096


typedef char tCmd[CMD_LENGTH_LIMIT];
typedef int tOrden;
typedef struct tItemL {
    tCmd cmd;
    tOrden orden;
} tItemL;


#endif //P0_TYPES_H


