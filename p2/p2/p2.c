/*Nombres miembros del grupo:
 * Brais González Piñeiro
 * Raul Fernandez del Blanco
 * Armando Martínez Noya
 *
 * Grupo de prácticas 2.4
*/

#include "hist_list.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define MAX_LETTERRS 4096
int global1 = 10, global2 = 420;
char global3[MAX_LETTERRS] = "Viva Armando Martinez";

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

void borrarListaM(tList *L){
    tPosL p,q;
    p = last(*L);

    while (p != LNULL){
        q = p;
        p = previous(p, *L);
        deleteAtPositionM(q, L);
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
        char *dir = getcwd(NULL, 0);
        if(dir != NULL){
            printf("Current working directory: %s\n", dir);
        } else {
            printf("Error al leer el directorio actual: %s\n" , strerror(errno));
        }
        free(dir);
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
        puts("fin salir bye fecha pid autores hist comando carpeta infosis ayuda crear borrar borrarrec listfich listdir recursiva e-s volcarmem llenarmem dealloc malloc mmap shared memoria");
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
                        puts("fecha [-d|-h]\tMuestra la fecha y o la hora actual");
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
                                                if(strcmp(trozos[1], "crear") == 0){
                                                    puts("crear [-f] [name]\t Crea un fichero o directorio");
                                                } else {
                                                    if(strcmp(trozos[1], "borrar") == 0){
                                                        puts("borrar [name1 name2 ..]\t Borra ficheros o directorios vacios");
                                                    } else {
                                                        if(strcmp(trozos[1], "borrarrec") == 0){
                                                            puts("borrarrec [name1 name2 ..]\t Borra ficheros o directorios no vacios");
                                                        } else {
                                                            if(strcmp(trozos[1], "listfich") == 0){
                                                                puts("listfich [-long][-link][-acc] n1 n2 ..\tlista ficheros");
                                                            } else {
                                                                if(strcmp(trozos[1], "listdir") == 0){
                                                                    puts("listdir [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ..\tlista ficheros contenidos de directorios");
                                                                } else {
                                                                    if (strcmp(trozos[1], "malloc") == 0) {
                                                                        puts("malloc [-free] tam\t asigna(o desasigna) memoria en el programa");
                                                                    } else {
                                                                        if (strcmp(trozos[1], "mmap") == 0) {
                                                                            puts("mmap [-free] fich prms\t mapea(o desmapea) ficheros en el espacio de direcciones del proceso");
                                                                        } else {
                                                                            if(strcmp(trozos[1], "shared") == 0){
                                                                                puts("shared [-free|-create|-delkey] cl tam\t asigna(o desasigna) memoria compartida en el programa");
                                                                            } else{
                                                                                if(strcmp(trozos[1], "dealloc") == 0){
                                                                                    puts("dealloc -malloc|-shared|-mmap....\tDesasigna un bloque de memoria asignado con malloc, shared o mmap");
                                                                                } else{
                                                                                    if(strcmp(trozos[1], "memoria") == 0){
                                                                                        puts("memoria [-blocks|-funcs|-vars|-all|-pmap] ..\tMuestra muestra detalles de la memoria del proceso");
                                                                                    } else{
                                                                                        if(strcmp(trozos[1], "volcarmem") == 0){
                                                                                            puts("volcarmem addr cont \tVuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr");
                                                                                        } else{
                                                                                            if(strcmp(trozos[1], "llenarmem") == 0){
                                                                                                puts("llenarmem addr cont byte \t Llena la memoria a partir de addr con byte");
                                                                                            } else{
                                                                                                if(strcmp(trozos[1], "recursiva") == 0){
                                                                                                    puts("recursiva [n] \tInvoca a la funcion recursiva n veces");
                                                                                                } else {
                                                                                                    if(strcmp(trozos[0], "e-s") == 0){
                                                                                                        puts("e-s [read|write] [-o] fiche addr cont \tInvoca a la funcion recursiva n veces");
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
    }
}

void crear(int palabras, char *trozos[MAX_LETTERRS]){

    if(palabras == 1){
        carpeta(palabras, trozos);
    } else {
        if(strcmp(trozos[1], "-f") == 0){
            FILE *fichero;
            fichero = fopen(trozos[2], "a+");
            fclose(fichero);
        } else {
            if (mkdir(trozos[1], 0755) != 0) {
                printf("No se ha podido crear carpeta: %s\n" , strerror(errno));
            }
        }
    }

}

void borrar(int palabras, char *trozos[MAX_LETTERRS]){

    if(palabras == 1){
        carpeta(palabras, trozos);
    } else {
        for (int i = 1; i < palabras; i++) {
            if (rmdir(trozos[i]) != 0 && remove(trozos[i])!=0) {
                printf("No se ha podido eliminar %s: %s\n",trozos[i], strerror(errno));
            }
        }
    }
}

int remove_directory(const char *path) {

    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d) {
        struct dirent *p;

        r = 0;

        while (!r && (p=readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf) {
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode)) {
                        r2 = remove_directory(buf);
                    } else {
                        r2 = unlink(buf);
                    }
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }
    if (!r) {
        r = rmdir(path);
    }

    return r;
}

void borrarrec(int palabras, char *trozos[MAX_LETTERRS]){
    if (palabras == 1) {
        carpeta(palabras,trozos);
    }
    else {
        for (int i = 1; i < palabras; ++i) {
            if (remove_directory(trozos[i]) != 0) {
                printf("No se ha podido eliminar: %s\n", strerror(errno));
            }
        }
    }
}

void Fichnamesize(char *trozos[MAX_LETTERRS]){
    struct stat estadistica;
    if (stat(*trozos, &estadistica) != 0) {
        printf("Error al leer fichero %s: %s\n",*trozos,strerror(errno));
    } else {
        printf("%ld %s\n", estadistica.st_size,*trozos);
    }

}

char LetraTF (mode_t m)
{
    switch (m&S_IFMT) {
        case S_IFSOCK: return 's'; /*socket*/
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /*fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio*/
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no debería aparecer*/
    }
}

char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy (permisos,"----------");
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='x';
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    return permisos;
}

void Fich(char *trozos[MAX_LETTERRS],bool lg,bool link, bool acc){
    struct stat stats;
    time_t tiempo;
    char permisos[10];
    char name [10000];
    char output[128];

    if(lstat(*trozos,&stats)!=0){
        printf("Error al leer fichero %s: %s\n",*trozos,strerror(errno));
    }
    else {

        if(acc){
            tiempo = stats.st_atime;
        } else {
            tiempo = stats.st_mtime;
        }

        strftime(output, 128, "%y/%m/%d-%H:%M", localtime(&tiempo));
        struct passwd *userf = getpwuid(stats.st_uid);
        struct group *groupf = getgrgid(stats.st_gid);
        char tipo;
        tipo = LetraTF(stats.st_mode);

        if(lg){
            if(link && tipo == 'l'){
                char buffer[4096]="";
                if (readlink(*trozos, buffer, sizeof(buffer)) <= 0) {
                    sprintf(name, "%s -> No encontrado", *trozos);
                    printf("%s \n", buffer);
                } else {
                    sprintf(name, "%s -> %s", *trozos, buffer);
                }
            } else {
                strcpy(name, *trozos);
            }

            printf("%16s%4ld (%8ld)%10s%10s%11s%8ld %s\n", output, stats.st_nlink, stats.st_ino, userf->pw_name, groupf->gr_name, ConvierteModo(stats.st_mode, permisos), stats.st_size, name);
        } else {
            Fichnamesize(trozos);
        }

    }
}

void listfich(int palabras, char *trozos[MAX_LETTERRS]){

    int cont = 1;
    bool link = false, lg = false, acc = false;

    if(palabras == 1){
        carpeta(palabras, trozos);
    } else {

        for (int i = 1; i < palabras && (strcmp(trozos[i], "-long") == 0 || strcmp(trozos[i], "-link") == 0 || strcmp(trozos[i], "-acc") == 0); ++i) {
            if(strcmp(trozos[i], "-link") == 0){
                link = true;
            }
            if(strcmp(trozos[i], "-acc") == 0){
                acc = true;
            }
            if(strcmp(trozos[i], "-long") == 0){
                lg = true;
            }
            cont++;
        }

        if (cont == palabras) {
            carpeta(1, trozos);
        } else {
            for (int i = cont; i < palabras; ++i) {
                Fich(&trozos[i], lg, link, acc);
            }
        }

    }
}

void Dir(char *trozos[MAX_LETTERRS], bool lg, bool link, bool acc, bool hid){
    DIR *folder;
    struct dirent *fich;
    struct stat stats;
    time_t tiempo;
    char permisos[12];
    char name [10000];
    char path[1000];
    char buffer[4096] = "";
    char output[128];
    folder=opendir(*trozos);


    if(folder==NULL){
        Fich(trozos,lg, link, acc);
    } else {
        while ((fich = readdir(folder)) != NULL) {

            strcpy(path, *trozos);
            strcat(path, "/");
            strcat(path, fich->d_name);

            if(lstat(path, &stats) != 0){
                printf("Error al leer fichero %s: %s\n", fich->d_name, strerror(errno));
            } else {

                if(acc){
                    tiempo = stats.st_atime;
                } else {
                    tiempo = stats.st_mtime;
                }
                strftime(output, 128, "%y/%m/%d-%H:%M", localtime(&tiempo));
                struct passwd *userf = getpwuid(stats.st_uid);
                struct group *groupf = getgrgid(stats.st_gid);
                char tipo;
                tipo = LetraTF(stats.st_mode);

                if(lg){
                    if(link && tipo == 'l'){

                        if (readlink(fich->d_name, buffer, sizeof(buffer)) <= 0) {
                            sprintf(name, "%s -> No encontrado", fich->d_name);
                            printf("%s", buffer);
                        } else {
                            sprintf(name, "%s -> %s", fich->d_name, buffer);
                        }
                    } else {
                        strcpy(name, fich->d_name);
                    }

                    if(hid){
                        printf("%16s%4ld (%8ld)%10s%10s%11s%8ld %s\n", output, stats.st_nlink, stats.st_ino, userf->pw_name, groupf->gr_name, ConvierteModo(stats.st_mode, permisos), stats.st_size, name);
                    } else {
                        if(fich->d_name[0] != '.'){
                            printf("%16s%4ld (%8ld)%10s%10s%11s%8ld %s\n", output, stats.st_nlink, stats.st_ino, userf->pw_name, groupf->gr_name, ConvierteModo(stats.st_mode, permisos), stats.st_size, name);
                        }
                    }

                } else {

                    if(hid){
                        printf("%8ld %s\n", stats.st_size, fich->d_name);
                    } else {
                        if(fich->d_name[0] != '.'){
                            printf("%8ld %s\n", stats.st_size, fich->d_name);
                        }
                    }
                }
            }
        }
    }
    closedir(folder);
}

void DirA(char *path[MAX_LETTERRS],bool lg, bool link, bool acc, bool hid){

    DIR *d = opendir(*path);
    size_t path_len = strlen(*path);

    if (d) {
        struct dirent *p;

        printf("************%s\n", *path);
        Dir(path, lg, link, acc, hid);

        while ((p=readdir(d))) {

            char *buf;

            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }
            len = path_len + strlen(p->d_name) + 2;
            buf= malloc(len);
            struct stat statbuf;
            snprintf(buf, len, "%s/%s", *path, p->d_name);

            if (!stat(buf, &statbuf)) {
                if (LetraTF(statbuf.st_mode) == 'd') {
                    DirA(&buf, lg, link, acc, hid);
                }
            }
            free(buf);
        }

        closedir(d);
    } else {
        Fich(path, lg, link, acc);
    }
}

void DirB(char *path[MAX_LETTERRS],bool lg, bool link, bool acc, bool hid) {

    DIR *d = opendir(*path);
    size_t path_len = strlen(*path);

    if (d) {
        struct dirent *p;

        while ((p=readdir(d))) {

            char *buf;

            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }
            len = path_len + strlen(p->d_name) + 2;
            buf= malloc(len);
            struct stat statbuf;
            snprintf(buf, len, "%s/%s", *path, p->d_name);

            if (!stat(buf, &statbuf)) {
                if (LetraTF(statbuf.st_mode) == 'd') {
                    DirB(&buf, lg, link, acc, hid);
                }
            }
            free(buf);
        }
        printf("************%s\n", *path);
        Dir(path, lg, link, acc, hid);
        closedir(d);
    } else {
        Fich(path, lg, link, acc);
    }
}

void listdir(int palabras, char *trozos[MAX_LETTERRS]){

    int cont = 1;
    bool link = false,hid = false,reca=false,recb=false, acc = false, lg = false;

    if(palabras == 1){
        carpeta(palabras, trozos);
    } else {

        for (int i = 1; i < palabras && (strcmp(trozos[i], "-long") == 0 || strcmp(trozos[i], "-link") == 0 || strcmp(trozos[i], "-acc") == 0 || strcmp(trozos[i], "-hid") == 0 || strcmp(trozos[i], "-reca") == 0 || strcmp(trozos[i], "-recb") == 0); ++i) {

            if(strcmp(trozos[i], "-link") == 0){
                link = true;
            }
            if(strcmp(trozos[i], "-acc") == 0){
                acc = true;
            }
            if(strcmp(trozos[i], "-long") == 0){
                lg = true;
            }
            if(strcmp(trozos[i], "-hid") == 0){
                hid = true;
            }
            if(strcmp(trozos[i], "-reca") == 0){
                reca = true;
            }
            if(strcmp(trozos[i], "-recb") == 0){
                recb = true;
            }
            cont++;
        }

        if (cont == palabras) {
            carpeta(1, trozos);
        } else {

            for (int i = cont; i < palabras; ++i) {

                if(reca){
                    DirA(&trozos[i], lg, link, acc, hid);
                } else {
                    if(recb){
                        DirB(&trozos[i], lg, link, acc, hid);
                    } else {
                        printf("************%s\n", trozos[i]);
                        Dir(&trozos[i], lg, link, acc, hid);
                    }
                }
            }
        }
    }
}

void * MmapFichero (char * fichero, int protection, tList *mallocs){

    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;
    tItemL t;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);


    if (protection&PROT_WRITE) {
        modo = O_RDWR;
    }

    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1) {
        return NULL;
    }

    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED) {
        return NULL;
    }

    t.mem = p;
    t.size = s.st_size;
    if(localtime(&tiempo)!=NULL) {
        strftime(t.fecha, 128, "%d %H:%M", tlocal);
    }
    strcpy(t.fichero, fichero);
    t.descriptor = df;
    strcpy(t.cmd, "Mmap");
    t.key = -1;

    if(!insertItem(t, mallocs)){
        puts("Error durante la inserción");
    }

    return p;
}

ssize_t LeerFichero (char *fich, void *p, ssize_t n) {
    /* le n bytes del fichero fich en p */
    ssize_t nleidos,tam=n; /*si n==-1 lee el fichero completo*/
    int df, aux;
    struct stat s;

    if (stat (fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1) {
        return ((ssize_t) -1);
    }

    if (n==LEERCOMPLETO) {
        tam = (ssize_t) s.st_size;
    }

    if ((nleidos=read(df,p, tam))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return ((ssize_t)-1);
    }

    close (df);
    return (nleidos);
}

void dopmap (void){ /*no arguments necessary*/
    pid_t pid;
    /*ejecuta el comando externo pmap para */
    char elpid[32];
    /*pasandole el pid del proceso actual */
    char *argv[3]={"pmap",elpid,NULL};
    sprintf (elpid,"%d", (int) getpid());

    if ((pid=fork())==-1){
        printf("Imposible crear proceso: %s\n", strerror(errno));
        return;
    }

    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            printf("cannot execute pmap: %s\n", strerror(errno));
        exit(1);
    }

    waitpid (pid,NULL,0);
}

void mallocF (int palabras, char *trozos[MAX_LETTERRS], tList *mallocs){

    tItemL t;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    int mes=tlocal->tm_mon;
    char month[128];
    tPosL pos;

    if (palabras==1 || (strcmp(trozos[1], "-free") == 0 && palabras == 2)){
        printf("******Lista de bloques asignados malloc para el proceso: %d\n", getpid());
        if(!isEmptyList(*mallocs)){
            for (tPosL p = first(*mallocs); p != NULL ; p = next(p, *mallocs)) {
                t = getItem(p, *mallocs);
                switch (mes) {
                    case 0:
                        strcpy(month,"Ene");
                        break;
                    case 1:
                        strcpy(month,"Feb");
                        break;
                    case 2:
                        strcpy(month,"Mar");
                        break;
                    case 3:
                        strcpy(month,"Abr");
                        break;
                    case 4:
                        strcpy(month,"May");
                        break;
                    case 5:
                        strcpy(month,"Jun");
                        break;
                    case 6:
                        strcpy(month,"Jul");
                        break;
                    case 7:
                        strcpy(month,"Ago");
                        break;
                    case 8:
                        strcpy(month,"Sep");
                        break;
                    case 9:
                        strcpy(month,"Oct");
                        break;
                    case 10:
                        strcpy(month,"Nov");
                        break;
                    case 11:
                        strcpy(month,"Dic");
                        break;
                    default: puts("Error");
                }
                if(strcmp(t.cmd,"malloc")==0) {
                    printf("%21p%17ld %s %s %s\n", t.mem, t.size, month, t.fecha, t.cmd);
                }
            }
        }
    } else {
        if(strcmp(trozos[1], "-free") == 0){
            if(isdigit(*trozos[2])){
                long h = (strtol(trozos[2], NULL, 10));
                t.size = h;
                pos = findItem(t, *mallocs);

                if(pos != LNULL){
                    t = getItem(pos, *mallocs);
                    if(strcmp(t.cmd, "malloc")==0){
                        deleteAtPositionM(pos, mallocs);

                    }
                } else {
                    printf("No hay bloque de ese tamaño asignado con malloc\n");
                }

            } else {
                puts("No se asignan bloques de 0 bytes");
            }
        } else {
            if(isdigit(*trozos[1])){
                long h = (strtol(trozos[1], NULL, 10));
                t.mem = malloc(h);
                t.size = h;
                strcpy(t.fichero, " ");
                t.key = -1;
                strcpy(t.cmd, "malloc");
                if(localtime(&tiempo)!=NULL) {
                    strftime(t.fecha, 128, "%d %H:%M", tlocal);
                }

                if (t.mem != NULL) {
                    if (!insertItem(t, mallocs)) {
                        puts("Error en la inserción\n");
                    } else {
                        printf("Asignados %ld bytes en %p\n", t.size, t.mem);
                    }
                } else {
                    puts("Error durante la reserva de memoria");
                }

            } else {
                puts("No se asignan bloques de 0 bytes");
            }
        }
    }
}

void Mmap (char *arg[], tList *mallocs) {/*arg[0] is the file name and arg[1] is the permissions*/

    char *perm;
    void *p;
    int protection = 0;
    if (arg[0] == NULL) {/*Listar Direcciones de Memoria mmap;*/ return; }
    if ((perm = arg[1]) != NULL && strlen(perm) < 4) {
        if (strchr(perm, 'r') != NULL) protection |= PROT_READ;
        if (strchr(perm, 'w') != NULL) protection |= PROT_WRITE;
        if (strchr(perm, 'x') != NULL) protection |= PROT_EXEC;
    }
    if ((p = MmapFichero(arg[0], protection, mallocs)) == NULL) {
        printf("Imposible mapear fichero: %s\n", strerror(errno));

    } else {
        printf("fichero %s mapeado en %p\n", arg[0], p);
    }
}

void mmapf(int palabras, char *trozos[MAX_LETTERRS], tList *mallocs){

    tItemL t;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    int mes=tlocal->tm_mon;
    char month[128];
    char *aux[MAX_LETTERRS];
    tPosL p;

    if (palabras==1 || (strcmp(trozos[1], "-free") == 0 && palabras == 2)){
        printf("******Lista de bloques asignados mmap para el proceso: %d\n", getpid());
        if(!isEmptyList(*mallocs)){
            for (p = first(*mallocs); p != NULL ; p = next(p, *mallocs)) {
                t = getItem(p, *mallocs);
                switch (mes) {
                    case 0:
                        strcpy(month, "Ene");
                        break;
                    case 1:
                        strcpy(month, "Feb");
                        break;
                    case 2:
                        strcpy(month, "Mar");
                        break;
                    case 3:
                        strcpy(month, "Abr");
                        break;
                    case 4:
                        strcpy(month, "May");
                        break;
                    case 5:
                        strcpy(month, "Jun");
                        break;
                    case 6:
                        strcpy(month, "Jul");
                        break;
                    case 7:
                        strcpy(month, "Ago");
                        break;
                    case 8:
                        strcpy(month, "Sep");
                        break;
                    case 9:
                        strcpy(month, "Oct");
                        break;
                    case 10:
                        strcpy(month, "Nov");
                        break;
                    case 11:
                        strcpy(month, "Dic");
                        break;
                    default:
                        puts("Error");
                }
                if (strcmp(t.cmd, "Mmap")==0){
                    printf("%21p%17ld %s %s %s (descriptor %d)\n", t.mem, t.size, month, t.fecha, t.fichero, t.descriptor);
                }
            }
        }
    } else {

        if(palabras == 2){
            aux[0] = trozos[1];
            aux[1] = "rwx";
            Mmap(aux, mallocs);

        } else {
            if(strcmp(trozos[1], "-free") == 0){

                for (p = first(*mallocs); (p != LNULL) && (strcmp(getItem(p,*mallocs).fichero, trozos[2]) != 0) ; p = next(p, *mallocs)); //Obtiene la posición del primer elemento que queremos eliminar

                if (p != NULL && strcmp(getItem(p, *mallocs).cmd, "Mmap")==0) {
                    t = getItem(p, *mallocs);

                    if (munmap(t.mem, t.size) == -1 && close(t.descriptor) == -1) {
                        printf("Imposible desmapear fichero: %s\n", strerror(errno));
                    }
                    deleteAtPositionM(p, mallocs);

                }
            } else{
                aux[0]=trozos[1];
                aux[1]=trozos[2];
                Mmap(aux,mallocs);
            }
        }
    }
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam, tList *mallocs) {
    /*Obtienen un puntero a una zaona de memoria compartida*/
    /*si tam >0 intenta crearla y si tam==0 asume que existe*/
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    tItemL t,r;
    tPosL q;

    if (tam) {
        /*si tam no es 0 la crea en modo exclusivo esta funcion vale para shared y shared -create*/
        flags = flags | IPC_CREAT | IPC_EXCL;
        /*si tam es 0 intenta acceder a una ya creada*/
    }

    if (clave==IPC_PRIVATE){
        /*no nos vale*/
        errno=EINVAL; return NULL;
    }

    if ((id=shmget(clave, tam, flags))==-1) {
        return (NULL);
    }

    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        /*si se ha creado y no se puede mapear*/
        if (tam) {
            /*se borra */
            shmctl(id, IPC_RMID, NULL);
        }

        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);

    for (q = first(*mallocs); (q != NULL) && (clave != getItem(q, *mallocs).key) ; q = next(q, *mallocs));

    strcpy(t.cmd, "shared");
    t.mem = p;
    if(q == NULL){
        t.size = tam;
    }else {
        r = getItem(q, *mallocs);
        t.size = r.size;
    }
    strcpy(t.fichero," ");
    t.key=clave;

    if(localtime(&tiempo)!=NULL) {
        strftime(t.fecha, 128, "%d %H:%M", tlocal);
    }

    if(!insertItem(t, mallocs)){
        puts("Error durante la inserción");
    }

    /* Guardar En Direcciones de Memoria Shared (p, s.shm_segsz, clave.....);*/
    return (p);
}

void ListarMallocs(tList *mallocs) {
    tItemL t;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    int mes = tlocal->tm_mon;
    char month[128];
    tPosL p;
    if (!isEmptyList(*mallocs)) {
        for (p = first(*mallocs); p != NULL; p = next(p, *mallocs)) {
            t = getItem(p, *mallocs);
            switch (mes) {
                case 0:
                    strcpy(month, "Ene");
                    break;
                case 1:
                    strcpy(month, "Feb");
                    break;
                case 2:
                    strcpy(month, "Mar");
                    break;
                case 3:
                    strcpy(month, "Abr");
                    break;
                case 4:
                    strcpy(month, "May");
                    break;
                case 5:
                    strcpy(month, "Jun");
                    break;
                case 6:
                    strcpy(month, "Jul");
                    break;
                case 7:
                    strcpy(month, "Ago");
                    break;
                case 8:
                    strcpy(month, "Sep");
                    break;
                case 9:
                    strcpy(month, "Oct");
                    break;
                case 10:
                    strcpy(month, "Nov");
                    break;
                case 11:
                    strcpy(month, "Dic");
                    break;
                default:
                    puts("Error");
            }
            if (strcmp(t.cmd, "malloc") == 0) {
                printf("%14p:  size:%5ld. %s %s %s %s \n", t.mem, t.size, t.cmd, month, t.fecha, t.fichero);
            } else if (strcmp(t.cmd, "Mmap") == 0) {
                printf("%14p:  size:%5ld. mmap  %s (fd:%d) %s %s\n", t.mem, t.size, t.fichero, t.descriptor, month,
                       t.fecha);
            } else {
                printf("%14p:  size:%5ld. %s memory (key %d) %s %s \n", t.mem, t.size, t.cmd, t.key, month, t.fecha);
            }
        }
    }
}

void SharedDelkey (char *args[]) /*arg[0] points to a str containing the key*/
{
    key_t clave;
    int id;
    char *key=args[0];
    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("shared -delkey clave_valida\n");
        return;
    }

    if ((id=shmget(clave,0,0666))==-1){
        printf("Cannot remove key %d: %s\n", clave, strerror(errno));
        return;
    }

    if (shmctl(id,IPC_RMID,NULL)==-1) {
        printf("shctl: imposible obtener memoria compartida: %s\n", strerror(errno));
    } else {
        printf("Key %d removed from the system\n", clave);
    }
}

void SharedCreate (char *arg[], tList *mallocs){ /*arg[0] is the key and arg[1] is the size*/

    key_t k;
    size_t tam=0;
    void *p;

    if (arg[0]==NULL || arg[1]==NULL){
        /*Listar Direcciones de Memoria Shared */
        return;
    }

    k=(key_t) strtol(arg[0], NULL, 10);

    if (arg[1]!=NULL) {
        tam = (size_t) atoll(arg[1]);
    }

    if ((p=ObtenerMemoriaShmget(k,tam, mallocs))==NULL) {
        printf("Imposible obtener memoria shmget: %s\n", strerror(errno));
    } else {
        printf("Memoria de shmget de clave %d asignada en %p\n", k, p);
    }
}

void shared (int palabras, char *trozos[MAX_LETTERRS], tList *mallocs){
    tItemL t;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    int mes=tlocal->tm_mon;
    char month[128];
    char *aux[MAX_LETTERRS];
    tPosL p;
    void *q;
    key_t k;

    if (palabras==1 || ( palabras == 2 && (strcmp(trozos[1], "-free") == 0 || strcmp(trozos[1], "-create") == 0 || strcmp(trozos[1], "-delkey") == 0)) || (palabras == 3 && strcmp(trozos[1], "-create") == 0 && (strcmp(trozos[2], "-free") == 0 || strcmp(trozos[2], "-delkey") == 0 ||
                                                                                                                                                                                                                  isdigit(*trozos[2])))){
        printf("******Lista de bloques asignados shared para el proceso: %d\n", getpid());
        if(!isEmptyList(*mallocs)){
            for (p = first(*mallocs); p != NULL ; p = next(p, *mallocs)) {
                t = getItem(p, *mallocs);
                switch (mes) {
                    case 0:
                        strcpy(month,"Ene");
                        break;
                    case 1:
                        strcpy(month,"Feb");
                        break;
                    case 2:
                        strcpy(month,"Mar");
                        break;
                    case 3:
                        strcpy(month,"Abr");
                        break;
                    case 4:
                        strcpy(month,"May");
                        break;
                    case 5:
                        strcpy(month,"Jun");
                        break;
                    case 6:
                        strcpy(month,"Jul");
                        break;
                    case 7:
                        strcpy(month,"Ago");
                        break;
                    case 8:
                        strcpy(month,"Sep");
                        break;
                    case 9:
                        strcpy(month,"Oct");
                        break;
                    case 10:
                        strcpy(month,"Nov");
                        break;
                    case 11:
                        strcpy(month,"Dic");
                        break;
                    default: puts("Error");
                }
                if(strcmp(t.cmd,"shared")==0) {
                    printf("%21p%17ld %s %s %s (key %d)\n", t.mem, t.size, month, t.fecha, t.cmd, t.key);
                }
            }
        }
    } else {

        if(palabras == 2 && isdigit(*trozos[1])){
            k = (key_t)strtol(trozos[1],NULL,10);
            if((q=ObtenerMemoriaShmget(k,0,mallocs))==NULL){
                printf("Cannot allocate: %s\n", strerror(errno));
            } else {
                printf("Memoria de shmget de clave %d asignada en %p\n", k, q);
            }

        }else{
            if(palabras == 3){
                if (isdigit(*trozos[2])){
                    for ( p = first(*mallocs); (p != LNULL) && strtol(trozos[2], NULL, 10) != getItem(p,*mallocs).key ; p = next(p, *mallocs));

                    if(p==NULL){
                        puts("Introduzca un key valido");
                    } else{
                        if(strcmp(trozos[1], "-free") == 0 && strcmp(getItem(p, *mallocs).cmd, "shared")==0){
                            t=getItem(p,*mallocs);
                            if(shmdt(t.mem)==0){
                                deleteAtPositionM(p,mallocs);
                                printf("Shared memory block at %p (key %d) has been dealocated\n", t.mem, t.key);
                            }else{
                                perror("error");
                            }
                        }
                        if(strcmp(trozos[1], "-delkey") == 0){
                            aux[0] = trozos[2];
                            SharedDelkey(aux);
                        }

                    }
                }
            } else {
                if(strcmp(trozos[1], "-create") == 0 && isdigit(*trozos[2]) && isdigit(*trozos[3])){
                    aux[0] = trozos[2];
                    aux[1] = trozos[3];
                    SharedCreate(aux, mallocs);
                }

            }
        }

    }
}

void doRecursiva (int n)
{
    char automatico[MAX_LETTERRS];
    static char estatico[MAX_LETTERRS];
    printf ("parametro n:%d en %p\n",n,&n);
    printf ("array estatico en:%p \n",estatico);
    printf ("array automatico en %p\n",automatico);
    n--;
    if (n>0){
        doRecursiva(n);
    }
}

void dealloc (int palabras, char *trozos[MAX_LETTERRS], tList *mallocs){

    char *aux[MAX_LETTERRS];
    char *p;
    char text[MAX_LETTERRS];

    if(palabras==1) {
        ListarMallocs(mallocs);
    }
    else if(palabras == 2){
        tItemL t;
        tPosL pos;

        sscanf(trozos[1], "%p", &p);

        for(pos = first(*mallocs); pos != NULL && p != getItem(pos, *mallocs).mem; pos = next(pos,*mallocs));

        t = getItem(pos, *mallocs);

        if(strcmp(t.cmd,"malloc")==0){
            sprintf(text, "%lu", t.size);
            aux[0] = "malloc";
            aux[1] = "-free";
            aux[2] = text;
            palabras = 3;
            mallocF(palabras, aux, mallocs);
        }
        else if(strcmp(t.cmd,"Mmap")==0){
            aux[0] = "mmap";
            aux[1] = "-free";
            aux[2] = t.fichero;
            palabras = 3;
            mmapf(palabras, aux, mallocs);

        }
        else if(strcmp(t.cmd,"shared")==0){
            sprintf(text, "%d", t.key);
            aux[0] = "shared";
            aux[1] = "-free";
            aux[2] = text;
            palabras = 3;
            shared(palabras, aux, mallocs);
        }

    } else if(palabras==3){

        if(strcmp(trozos[1],"-malloc")==0){

            aux[0] = "malloc";
            aux[1] = "-free";
            aux[2] = trozos[2];
            mallocF(palabras, aux, mallocs);
        }
        else if(strcmp(trozos[1],"-mmap")==0){
            aux[0] = "mmap";
            aux[1] = "-free";
            aux[2] = trozos[2];
            mmapf(palabras, aux, mallocs);

        }
        else if(strcmp(trozos[1],"-shared")==0){//Ta to mal
            aux[0] = "shared";
            aux[1] = "-free";
            aux[2] = trozos[2];
            shared(palabras, aux, mallocs);
        }

    }
}

void vars(){
    int local1=1,local2=2,local3=3;
    int static static1=1,static2=2,static3=3;
    printf("Variables locales:  \t %p, \t %p, \t %p \n", &local1, &local2, &local3 );
    printf("Variables globales: \t %p, \t %p, \t %p \n", &global1, &global2, &global3 );
    printf("Variables estáticas: \t %p, \t %p, \t %p \n", &static1, &static2, &static3 );

}

void funcs(){
    printf("Funciones programa: \t %p, \t %p, \t %p \n", &TrocearCadena, &pid, &mmapf );
    printf("Funciones libreria: \t %p, \t %p, \t %p \n", &printf , &fgets, &malloc );
}

void memoria (int palabras,char *trozos[MAX_LETTERRS],tList *mallocs){
    if(palabras==1 || (palabras==2 && (strcmp(trozos[1],"-all")==0 ||strcmp(trozos[1],"-blocks")==0))){
        ListarMallocs(mallocs);
        if(palabras==1 || strcmp(trozos[1],"-all")==0 ){
            vars();
            funcs();
        }
    }
    else if(palabras==2){
        if(strcmp(trozos[1],"-vars")==0){
            vars();
        }
        else if(strcmp(trozos[1],"-funcs")==0){
            funcs();
        }
        else if(strcmp(trozos[1],"-pmap")==0){
            dopmap();
        }
    }
}

void volcarmem (int palabras, char *trozos[MAX_LETTERRS]) {
    unsigned char *p;
    int bytes;
    if (trozos[1] == NULL) {
        return;
    } else {
        sscanf(trozos[1], "%p", &p);
    }
    if(trozos[2]==NULL){
        bytes=25;
    }else{
        bytes=atoi(trozos[2]);
    }
    for(int i=0; i<bytes; i=i+25){
        for(int j=i; (j<(i+25)) && (j<bytes); j++){
            if(isprint(p[j])){
                printf(" %c ", p[j]);
            } else {
                printf("   ");
            }
        }
        printf("\n");
        for(int j=i; (j<(i+25))&&(j<bytes); j++){
            printf("%02x ",p[j]);
        }
        printf("\n");
    }
}


void llenarMem (int palabras,char *trozos[MAX_LETTERRS]) {
    int bytes;
    unsigned char chardeci;
    unsigned char *p;
    if (trozos[1] == NULL) {
        return;
    }
    if (trozos[2] == NULL) {
        bytes = 128;
    } else {
        bytes = atoi(trozos[2]);
    }
    if (trozos[3] == NULL) {
        chardeci = 65;
    } else {
        chardeci = strtoul(trozos[3], NULL, 0);
    }
    sscanf(trozos[1], "%p", &p);
    for (int i = 0; i < bytes; i++) {
        p[i] = chardeci;
    }
}

void recursiva (int palabras, char *trozos[MAX_LETTERRS]){
    int n;
    if(palabras==2 && isdigit(*trozos[1])) {
        n = (strtol(trozos[1], NULL, 10));
        doRecursiva(n);
    }
}

void EscribirFichero (char *fich, void *p, ssize_t n,bool o) {
    FILE * fichero;
    if(o==false){
        remove(fich);
    }
    fichero = fopen(fich,"a+");
    if(fichero==NULL){
        printf("Error al abrir el fichero\n");
    }
    fwrite(p,n,sizeof(char),fichero);
    fclose(fichero);
}

void esr(int palabras, char *trozos[MAX_LETTERRS]){
    if (palabras == 5) {
        unsigned char *p;
        sscanf(trozos[3], "%p", &p);
        LeerFichero(trozos[2], p, atoi(trozos[4]));
    }
}

void esw(int palabras,char *trozos[MAX_LETTERRS]){
    unsigned char *p;
    if(palabras==6){
        sscanf(trozos[4],"%p",&p);
        EscribirFichero(trozos[3],p,atoi(trozos[5]),false);
    }
    else {
        if (palabras == 5) {
            sscanf(trozos[3], "%p", &p);
            EscribirFichero(trozos[2], p, atoi(trozos[4]), true);
        }
    }
}


void priority(int palabras,char *trozos[MAX_LETTERRS]){
    if(palabras==1){
        printf("La priodridad del proceso del shell que se esta ejecutando es : %d\n" ,getpriority(PRIO_PROCESS,getpid()));
    }
    else if(palabras==2){
        printf("La priodridad del proceso pid introducido es : %d\n" ,getpriority(PRIO_PROCESS,atoi(trozos[1])));
    }
    else if(palabras==3){
        setpriority(PRIO_PROCESS,atoi(trozos[1]),atoi(trozos[2]));
    }
}

void redder(int palabras,char *trozos[MAX_LETTERRS]){//Inacabada
    if(palabras==1){

    }
    else if(palabras==2){
        if(strcmp(trozos[1],"-reset")==0){

        }else{
            int fd;
            fd = open(trozos[1],O_RDWR|O_CREAT|O_APPEND,777);
            dup2(fd, fileno(stderr));

        }
    }
}

void entorno(int palabras, char *trozos[MAX_LETTERRS],char *envp[]){//faltan cositas
    extern char **environ;
    if(palabras==1) {
        while(*envp)
            printf("%s\n",*envp++);
    }
    else if(palabras==2){
        if(strcmp(trozos[1],"-environ")==0){
            for (char **env = environ; *env; ++env)
                printf("%s\n", *env);
        }
        else if(strcmp(trozos[1],"-addr")==0){
            printf("Value as pointers:\nenviron:%p  third argument main:%p \nAdresses:\nenviron:%p  third argument main:%p\n" ,*environ,*envp,&environ,&envp);
        }
    }
}

void mostrarvar(int palabras,char *trozos[MAX_LETTERRS],char *envp[]){
    if(palabras==1){
        char *aux[MAX_LETTERRS];
        aux[0]="entorno";
        aux[1]="-environ";
        entorno(2,aux,NULL);
    }
    else if(palabras==2){
        extern char **environ;
        char *variable;
        char variableenv[MAX_LETTERRS],variablemain[MAX_LETTERRS];
        variable = getenv(trozos[1]);
        for (char **env = environ; *env; ++env) {
            if (strcmp(variable,*env)==0){
                strcpy(variableenv,*env);
            }
        }
        while(*envp){
            if(strcmp(variable,*envp)==0){
                strcpy(variablemain,*envp);
            }
        envp++;
        }
        if(variable==NULL){
            printf("No se encontro la variable\n");
        }
        else {
        printf("Con arg3 main: %s(%p) p\nCon environ %s(%p) p\nCon getenv %s(%p)\n" ,variablemain,&variablemain,variableenv,&variableenv,variable,&variable);
        }
    }
}

void cambiarvar(int palabras,char *trozos[MAX_LETTERRS]) {
}

void uid(int palabras,char *trozos[MAX_LETTERRS]) {
}

void ffork(int palabras,char *trozos[MAX_LETTERRS]) {
}

void ejec(int palabras,char *trozos[MAX_LETTERRS]) {
}

void ejecpri(int palabras,char *trozos[MAX_LETTERRS]) {
}

void fg(int palabras,char *trozos[MAX_LETTERRS]) {
}

void fgpri(int palabras,char *trozos[MAX_LETTERRS]) {
}

void back(int palabras,char *trozos[MAX_LETTERRS]) {
}

void backpri(int palabras,char *trozos[MAX_LETTERRS]) {
}

void ejecas(int palabras,char *trozos[MAX_LETTERRS]) {
}

void fgas(int palabras,char *trozos[MAX_LETTERRS]) {
}

void bgas(int palabras,char *trozos[MAX_LETTERRS]) {
}

void listjobs(int palabras,char *trozos[MAX_LETTERRS]) {
}

void jobs(int palabras,char *trozos[MAX_LETTERRS]) {
}

void borrarjobs(int palabras,char *trozos[MAX_LETTERRS]) {
}


bool procesarEntrada(char cadena[MAX_LETTERRS], tList *list, tList *mallocs, char *trozos[MAX_LETTERRS], char *envp[]){

    int palabras;
    tItemL comand;

    stpcpy(comand.cmd, cadena);
    palabras = TrocearCadena(cadena, trozos);

    if (strcmp(cadena, "\n") != 0 && strcmp(trozos[0], "comando") != 0 && !insertItem(comand, list)) {
        puts("Error en la inserción\n");
    }

    if(trozos[0] == NULL){

        return false;
    } else {
        if ((strcmp(trozos[0], "fin") == 0) || (strcmp(trozos[0], "salir") == 0) || (strcmp(trozos[0], "bye") == 0)) {
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
                                        if (!isEmptyList(*list)) {
                                            if (h >= 0) {
                                                for (int i = 0; i <= h; i++) {
                                                    while (next(p, *list) != LNULL && a < i) {
                                                        a++;
                                                        p = next(p, *list);
                                                    }
                                                }

                                                for (q = *list; next(q, *list) != LNULL; q = next(q, *list)) {
                                                    b++;
                                                }

                                                if (h <= b) {
                                                    strcpy(j.cmd, getItem(p, *list).cmd);
                                                    printf("Ejecutando hist (%d): %s", a, j.cmd);
                                                    procesarEntrada(j.cmd, list, mallocs, trozos,envp);
                                                } else {
                                                    printf("No hay elemento %ld en el historico\n", h);
                                                }
                                            }
                                        } else {
                                            printf("No hay elemento %ld en el historico\n", h);
                                        }
                                    }
                                } else {
                                    if (strcmp(trozos[0], "infosis") == 0) {
                                        infosis(palabras);
                                    } else {
                                        if (strcmp(trozos[0], "ayuda") == 0) {
                                            ayuda(palabras, trozos);
                                        } else {
                                            if(strcmp(trozos[0], "crear") == 0){
                                                crear(palabras, trozos);
                                            } else{
                                                if(strcmp(trozos[0], "borrar") == 0){
                                                    borrar(palabras, trozos);
                                                } else{
                                                    if(strcmp(trozos[0], "borrarrec") == 0){
                                                        borrarrec(palabras, trozos);
                                                    } else{
                                                        if(strcmp(trozos[0], "listfich") == 0){
                                                            listfich(palabras, trozos);
                                                        } else{
                                                            if(strcmp(trozos[0], "listdir") == 0){
                                                                listdir(palabras, trozos);
                                                            } else{
                                                                if (strcmp(trozos[0], "malloc") == 0) {
                                                                    mallocF(palabras, trozos, mallocs);
                                                                } else {
                                                                    if (strcmp(trozos[0], "mmap") == 0) {
                                                                        mmapf(palabras, trozos, mallocs);
                                                                    } else {
                                                                        if(strcmp(trozos[0], "shared") == 0){
                                                                            shared(palabras,trozos,mallocs);
                                                                        } else{
                                                                            if(strcmp(trozos[0], "dealloc") == 0){
                                                                                dealloc(palabras,trozos,mallocs);
                                                                            } else{
                                                                                if(strcmp(trozos[0], "memoria") == 0){
                                                                                    memoria(palabras,trozos,mallocs);
                                                                                } else{
                                                                                    if(strcmp(trozos[0], "volcarmem") == 0){
                                                                                        volcarmem(palabras, trozos);
                                                                                    } else{
                                                                                        if(strcmp(trozos[0], "llenarmem") == 0){
                                                                                            llenarMem(palabras, trozos);
                                                                                        } else{
                                                                                            if(strcmp(trozos[0], "recursiva") == 0){
                                                                                                recursiva(palabras,trozos);
                                                                                            }else{
                                                                                                if(strcmp(trozos[0], "e-s") == 0 && palabras == 1){
                                                                                                    puts("uso: e-s [read|write] ......");
                                                                                                } else {
                                                                                                    if(strcmp(trozos[0], "e-s") == 0 && strcmp(trozos[1], "read") == 0){
                                                                                                        esr(palabras, trozos);
                                                                                                    } else {
                                                                                                        if (strcmp(trozos[0], "e-s") == 0 && strcmp(trozos[1], "write") == 0){
                                                                                                            esw(palabras, trozos);
                                                                                                        } else {
                                                                                                            if(strcmp(trozos[0],"priority") == 0){
                                                                                                                 priority(palabras,trozos);
                                                                                                            }else{
                                                                                                                 if(strcmp(trozos[0],"rederr") == 0){
                                                                                                                     redder(palabras,trozos);
                                                                                                                 }else{
                                                                                                                      if(strcmp(trozos[0],"entorno") == 0) {
                                                                                                                          entorno(palabras,trozos,envp);
                                                                                                                      }else{
                                                                                                                           if(strcmp(trozos[0],"mostrarvar") == 0){
                                                                                                                              mostrarvar(palabras,trozos,envp);
                                                                                                                           }else{
                                                                                                                              if(strcmp(trozos[0],"cambiarvar") == 0){
                                                                                                                                  cambiarvar(palabras,trozos);
                                                                                                                              }else{
                                                                                                                                  if(strcmp(trozos[0],"uid") == 0){
                                                                                                                                      uid(palabras,trozos);
                                                                                                                                  }else{
                                                                                                                                      if(strcmp(trozos[0],"fork") == 0){
                                                                                                                                          ffork(palabras,trozos);
                                                                                                                                      }else{
                                                                                                                                          if(strcmp(trozos[0],"ejec") == 0){
                                                                                                                                              ejec(palabras,trozos);
                                                                                                                                          }else{
                                                                                                                                              if(strcmp(trozos[0],"ejecpri") == 0){
                                                                                                                                                 ejecpri(palabras,trozos);
                                                                                                                                              }else{
                                                                                                                                                  if(strcmp(trozos[0],"fg") == 0){
                                                                                                                                                      fg(palabras,trozos);
                                                                                                                                                  }
                                                                                                                                                  else{
                                                                                                                                                      if(strcmp(trozos[0],"fgpri") == 0){
                                                                                                                                                          fgpri(palabras,trozos);
                                                                                                                                                      }else{
                                                                                                                                                          if(strcmp(trozos[0],"back") == 0){
                                                                                                                                                              back(palabras,trozos);
                                                                                                                                                          }
                                                                                                                                                          else{
                                                                                                                                                              if(strcmp(trozos[0],"backpri") == 0){
                                                                                                                                                                  backpri(palabras,trozos);
                                                                                                                                                              }else{
                                                                                                                                                                  if(strcmp(trozos[0],"ejecas") == 0){
                                                                                                                                                                      ejecas(palabras,trozos);
                                                                                                                                                                  }else{
                                                                                                                                                                      if(strcmp(trozos[0],"fgas") == 0){
                                                                                                                                                                          fgas(palabras,trozos);
                                                                                                                                                                      }else{
                                                                                                                                                                          if(strcmp(trozos[0],"bgas") == 0){
                                                                                                                                                                              bgas(palabras,trozos);

                                                                                                                                                                          }else{
                                                                                                                                                                              if(strcmp(trozos[0],"listjobs") == 0){
                                                                                                                                                                                  listjobs(palabras,trozos);
                                                                                                                                                                              }else{
                                                                                                                                                                                  if(strcmp(trozos[0],"job") == 0){
                                                                                                                                                                                      jobs(palabras,trozos);
                                                                                                                                                                                  }else{
                                                                                                                                                                                      if(strcmp(trozos[0],"borrarjobs") == 0){
                                                                                                                                                                                          borrarjobs(palabras,trozos);
                                                                                                                                                                                      }
                                                                                                                                                                                      else{
                                                                                                                                                                                          printf("%s: no es un comando del shell\n", cadena);
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

int main(int argc, char *argv[], char *envp[]) {

    bool terminado = false;
    char cadena[MAX_LETTERRS];
    char *trozos[MAX_LETTERRS];
    tList list;
    tList mallocs;

    createEmptyList(&list);
    createEmptyList(&mallocs);

    while(!terminado){

        imprimirPrompt();
        leerEntrada((char *) &cadena);
        terminado = procesarEntrada(cadena, &list, &mallocs, trozos,envp);
    }

    borrarLista(&list);
    if(!isEmptyList(mallocs)){
        borrarListaM(&mallocs);
    }

    return 0;
