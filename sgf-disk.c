
/* ============================================================================
 _   _ _____   ____   _    ____    __  __  ___  ____ ___ _____ ___ _____ ____  
| \ | | ____| |  _ \ / \  / ___|  |  \/  |/ _ \|  _ \_ _|  ___|_ _| ____|  _ \ 
|  \| |  _|   | |_) / _ \ \___ \  | |\/| | | | | | | | || |_   | ||  _| | |_) |
| |\  | |___  |  __/ ___ \ ___) | | |  | | |_| | |_| | ||  _|  | || |___|  _ < 
|_| \_|_____| |_| /_/   \_\____/  |_|  |_|\___/|____/___|_|   |___|_____|_| \_\
                                                                                                                                   
============================================================================ */

/************************************************************
 *
 *  FONCTIONS DE BASE POUR LES E/S DE BLOCKS.
 *
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "sgf-header.h"


/************************************************************
 Informations sur le disque simulé
 ************************************************************/

int trace_sgf_disk = 0;


static struct HARD_DISK {
    FILE*   file;
    int     size;
    int     exist;
    char    name[32];
    }
    hd = {NULL, 0, 0, ""};


#define NU_BLOC_OK(n)           (((n) >= 0) && ((n) < hd.size))


/************************************************************
 Initialisation du module
 ************************************************************/

static void init_sgf_disk() {
    const char *name = "disk0";
    
    hd.file = NULL;
    hd.exist = 0;
    hd.size = 0;
    strcpy(hd.name, "");
    
    FILE *file = fopen(name, "r+b");
    sgf_assert_msg(file != NULL, "impossible de trouver 'disk0'");
    
    int result = fseek(file, 0, SEEK_END);
    sgf_assert_msg(result == 0, "impossible de trouver 'disk0'");
    
    long size = (ftell(file) / BLOCK_SIZE);
    sgf_assert_msg(size > 0, "impossible de trouver 'disk0'");
    sgf_assert_msg(size < 10000, "disque 'disk0' trop important");
    
    hd.file = file;
    hd.exist = 1;
    hd.size = size;
    strcpy(hd.name, name);
}


/*****************************************************************
 Lire un bloc directement sur le disque physique.
 ****************************************************************/

void read_block(int n, BLOCK* bloc) {
    if (!hd.exist) init_sgf_disk();
    
    sgf_assert_msg(NU_BLOC_OK(n), "n° de bloc incorrect.");
        
    if (fseek(hd.file, (n * BLOCK_SIZE), SEEK_SET) == 0)
        if (BLOCK_SIZE == fread(bloc, 1, BLOCK_SIZE, hd.file)) {
            if (trace_sgf_disk) {
                fprintf(stderr, "read block %d\n", n);
            }
            return ;
        }
    
    panic("sgf-disk: read_block: impossible de lire le bloc %d\n", n);
    exit(EXIT_FAILURE);
}


/************************************************************
 Ecrire un bloc directement sur le disque physique.
 ************************************************************/

void write_block(int n, BLOCK* b) {
    if (!hd.exist) init_sgf_disk();
    
    sgf_assert_msg(NU_BLOC_OK(n), "n° de bloc incorrect.");
        
    if (fseek(hd.file, (n * BLOCK_SIZE), SEEK_SET) == 0) {
        if (BLOCK_SIZE == fwrite(b, 1, BLOCK_SIZE, hd.file)) {
            fflush(hd.file);
            if (trace_sgf_disk) {
                fprintf(stderr, "write block %d\n", n);
            }
            return ;
        }
    }
    
    panic("sgf-disk: impossible d'écrire le bloc %d\n", n);
    exit(EXIT_FAILURE);
}


/************************************************************
 Récupérer la taille du disque (en blocs)
 ************************************************************/

int get_disk_size() {
    if (!hd.exist) init_sgf_disk();
    
    return (hd.size);
}


/************************************************************
 écrire/lire un INODE dans/depuis un bloc
 ************************************************************/

INODE read_inode(int n) {
    BLOCK b;
    INODE inode;

    assert(sizeof(INODE) <= sizeof(BLOCK));
    read_block(n, &b);
    memcpy(&inode, &b, sizeof(INODE));
    return inode;
}


void write_inode(int n, INODE inode) {
    BLOCK b;
    
    assert(sizeof(INODE) <= sizeof(BLOCK));
    memcpy(&b, &inode, sizeof(INODE));
    write_block(n, &b);
}


/************************************************************
 écrire/lire le répertoire dans/depuis un bloc
 ************************************************************/

void read_dir_block(DIRECTORY *dir) {
    BLOCK b;

    assert(sizeof(DIRECTORY) <= sizeof(BLOCK));
    read_block(ADR_DIR_BLOCK, &b);
    memcpy(dir, &b, sizeof(*dir));
}


void write_dir_block(DIRECTORY *dir) {
    BLOCK b;

    assert(sizeof(DIRECTORY) <= sizeof(BLOCK));
    memcpy(&b, dir, sizeof(*dir));
    write_block(ADR_DIR_BLOCK, &b);
}


/************************************************************
 Afficher le message d'erreur et stopper la simulation.
 ************************************************************/

void panic (const char *format, ...) {
    va_list ap;
    
    fprintf(stderr, "Panique: ");
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
    /* ARRET DE LA SIMULATION */
}


/************************************************************
 Afficher le message d'erreur et stopper la simulation.
 ************************************************************/

void panic_at (const char *file, int line, const char *format, ...) {
    va_list ap;
    
    fprintf(stderr, "Panique %s:%d ", file, line);
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
    /* ARRET DE LA SIMULATION */
}
