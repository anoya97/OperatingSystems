/*Nombres miembros del grupo:
 * Brais González Piñeiro
 * Raul Fernandez del Blanco
 * Armando Martínez Noya
 *
 * Grupo de prácticas 2.4
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <math.h>
#include "hist_list.h"
#include <ctype.h>
#include <errno.h>

#define MAX_LETTERRS 4096

int TrocearCadena(char  *cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void imprimirPrompt(){
    printf("->");
}

void leerEntrada(char *cadena){
    fgets(cadena,MAX_LETTERRS,stdin);
}

void borrarLista(tList *L){
    tPosL p,q;

    p = last(*L);

    while (p != LNULL){
        q = p;
        p = previous(p, *L);
        deleteAtPosition(q, L);
    }
}

void autores(int palabras, char *trozos[MAX_LETTERRS]){

    if(palabras==1){
        printf("Brais Gonzalez Piñeiro: brais.gonzalezp\nRaul Fernandez del Blanco: r.delblanco\nArmando Martínez Noya: a.mnoya\n");
    }else{
        if(strcmp(trozos[1], "-l") == 0){
            printf("brais.gonzalezp\nr.delblanco\na.mnoya\n");

        }else {
            if(strcmp(trozos[1], "-n") == 0){
                printf("Brais Gonzalez Piñeiro\nRaúl Fernández del Blanco\nArmando Martínez Noya\n");
            }
        }
    }

}

void pid(int palabras, char *trozos[MAX_LETTERRS]){
    if(palabras==1){
        printf("PID de shell: %d\n", getpid());
    }else{
        if(strcmp(trozos[1], "-p") == 0){
            printf("Pid del padre shell: %d\n", getppid());
        }
    }

}

void carpeta(int palabras, char *trozos[MAX_LETTERRS]){

    if (palabras == 1) {

        //fflush(stdout);
        if(getcwd(NULL, 0) != NULL){
            printf("Current working directory: %s\n", getcwd(NULL, 0));
        } else {
            printf("Error al leer el directorio actual: %s\n" , strerror(errno));
        }
    } else {
        if (chdir(trozos[1]) != 0) {
           printf("Error al moverse de directorio: %s\n" ,strerror(errno));
        }
    }

}

void fecha(int palabras, char *trozos[MAX_LETTERRS]){


    time_t tiempo = time(0);
    if(localtime(&tiempo)!=NULL) {
        struct tm *tlocal = localtime(&tiempo);
        char output[128];

        if (palabras == 1) {
            strftime(output, 128, "%H:%M:%S\n%d/%m/%y", tlocal);
            printf("%s\n", output);
        } else {
            if ((strcmp(trozos[1], "-d")) == 0) {
                strftime(output, 128, "%d/%m/%y", tlocal);
                printf("%s\n", output);
            } else {
                if ((strcmp(trozos[1], "-h")) == 0) {
                    strftime(output, 128, "%H:%M:%S", tlocal);
                    printf("%s\n", output);
                } else {

                }
            }
        }
    }
    else{
        printf("No se ha podido obtener el tiempo: %s\n" , strerror(errno));
    }

}

void hist(int palabras, char *trozos[MAX_LETTERRS], tList *L){

    tPosL p,q;
    tItemL com;

    if(palabras == 1){

        if(isEmptyList(*L)){
            puts("Error: Hist not possible");
        } else {
            p = first(*L);
            while (p != LNULL){
                com = getItem(p, *L);
                printf("%d -> %s", com.orden, com.cmd);
                p = next(p, *L);
            }
        }

    } else {
        if(strcmp(trozos[1], "-c") == 0){

            if(isEmptyList(*L)){
                puts("Error: Hist -c not possible");
            } else {
                borrarLista(L);
            }

        } else {
            p = last(*L);
            while(p != *L) {
                com = getItem(p, *L);
                if (fabs((double)strtol(trozos[1], NULL, 10)) == com.orden) {
                    q = first(*L);
                    while (q != p) {
                        com = getItem(q, *L);
                        printf("%d -> %s", com.orden, com.cmd);
                        q = next(q, *L);
                    }
                }
                p = previous(p, *L);
            }
        }
    }

}

void infosis(int palabras){

    if(palabras==1) {
        struct utsname uts;
        if (uname(&uts) == 0) {
            printf("%s (%s) OS:%s %s %s\n", uts.nodename, uts.machine, uts.sysname, uts.release, uts.version);
        }
        else{
            printf("Información del sistema no disponible: %s\n" , strerror(errno));
        }
    }

    else{
        printf("Opción no disponible\n");
    }

}

void ayuda(int palabras, char *trozos[MAX_LETTERRS]){

    if(palabras == 1){
        puts("'ayuda cmd' donde cmd es uno de los siguientes comandos:");
        puts("fin salir bye fecha pid autores hist comando carpeta infosis ayuda");
    } else {
        if(strcmp(trozos[1], "autores") == 0){
            puts("autores [-l|-n]\tMuestra los nombres y logins de los autores");
        } else {
            if(strcmp(trozos[1], "pid") == 0){
                puts("pid [-p]\t\tMuestra el pid del shell o de su proceso padre");
            } else{
                if(strcmp(trozos[1], "carpeta") == 0){
                    puts("carpeta [dir]\tCambia (o muestra) el directorio actual del shell");
                }else {
                    if(strcmp(trozos[1], "fecha") == 0){
                        puts("fecha [-d|.h]\tMuestra la fecha y o la hora actual");
                    } else {
                        if(strcmp(trozos[1], "hist") == 0){
                            puts("hist [-c|-N]\tMuestra el historico de comandos, con -c lo borra");
                        }else {
                            if(strcmp(trozos[1], "comando") == 0){
                                puts("comando [-N]\tRepite el comando N (del historico)");
                            } else {
                                if(strcmp(trozos[1], "ayuda") == 0){
                                    puts("ayuda [cmd]\t\tMuestra ayuda sobre los comandos");
                                } else {
                                    if(strcmp(trozos[1], "fin") == 0){
                                        puts("fin \t\t\tTermina la ejecucion del shell");
                                    } else {
                                        if(strcmp(trozos[1], "salir") == 0){
                                            puts("salir \t\t\tTermina la ejecucion del shell");
                                        } else {
                                            if(strcmp(trozos[1], "bye") == 0){
                                                puts("bye \t\t\tTermina la ejecucion del shell");
                                            } else {
                                                puts("Opción incorrecta");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}

bool procesarEntrada(char cadena[MAX_LETTERRS], tList *list, bool control){
    char *trozos[MAX_LETTERRS];
    int palabras;
    tItemL comand;

    *trozos = malloc(sizeof(char *));
    stpcpy(comand.cmd, cadena);
    palabras = TrocearCadena(cadena, trozos);
    if (control) {
        if (strcmp(cadena, "\n") != 0) {
            if (!insertItem(comand, list)) {
                puts("Error en la inserción\n");
            }
        }
    }

    if(trozos[0] == NULL){
        return false;
    } else {
        if ((strcmp(trozos[0], "fin") == 0) || (strcmp(trozos[0], "salir") == 0) || (strcmp(trozos[0], "bye") == 0)) {
            borrarLista(list);

            return true;
        } else {
            if (strcmp(trozos[0], "autores") == 0) {
                autores(palabras, trozos);
            } else {
                if (strcmp(trozos[0], "pid") == 0) {
                    pid(palabras, trozos);
                } else {
                    if (strcmp(trozos[0], "carpeta") == 0) {
                        carpeta(palabras, trozos);
                    } else {
                        if (strcmp(trozos[0], "fecha") == 0) {
                            fecha(palabras, trozos);
                        } else {
                            if (strcmp(trozos[0], "hist") == 0) {
                                hist(palabras, trozos, list);
                            } else {
                                if (strcmp(trozos[0], "comando") == 0) {
                                    tPosL p = *list;
                                    tPosL q;
                                    int a = 0;
                                    int b=0;
                                    tItemL j;
                                    if(isdigit(*trozos[1])) {
                                        long h = (strtol(trozos[1], NULL, 10));
                                        if (h >= 0) {
                                            for (int i = 0; i <= h; i++) {
                                                while (p->next != LNULL && a < i) {
                                                    a++;
                                                    p = p->next;
                                                }
                                            }
                                            for (q = *list; q->next != LNULL; q = q->next) {
                                                b++;
                                            }
                                            if (h < b) {
                                                strcpy(j.cmd, p->data.cmd);
                                                printf("Ejecutando hist (%d): %s", a, j.cmd);
                                                control = false;
                                                procesarEntrada(j.cmd, list, control);
                                            } else {
                                                printf("No hay elemento %ld en el historico\n", h);
                                            }
                                        }
                                    }
                                } else {
                                    if (strcmp(trozos[0], "infosis") == 0) {
                                        infosis(palabras);
                                    } else {
                                        if (strcmp(trozos[0], "ayuda") == 0) {
                                            ayuda(palabras, trozos);
                                        } else {
                                            printf("%s: no es un comando del shell\n", cadena);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return false;
        }
    }

}

int main() {
    bool terminado = false, control;
    char cadena[MAX_LETTERRS];
    tList list;
    createEmptyList(&list);
    while(!terminado){
        control = true;
        imprimirPrompt();
        leerEntrada((char *) &cadena);
        terminado = procesarEntrada(cadena, &list, control);
    }
    return 0;
}