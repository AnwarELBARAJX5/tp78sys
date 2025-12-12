
/************************************************************
 Utilisation du SGF
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "sgf-header.h"


int main() {
    init_sgf();
    
    printf("\nLISTE DES FICHIERS\n\n");
    list_directory();
    
    printf("\nCONTENU DE essai.txt\n\n");
    OFILE *file = sgf_open("essai.txt", READ_MODE);
    for (int c; ((c = sgf_getc(file)) > 0); ) {
        putchar(c);
    }

    //TEST
    printf("\nTEST \n");
        if (file == NULL) {
            printf("Erreur ouverture fichier\n");
            return EXIT_FAILURE;
        }
        for (int i = 0; i < file->inode.size; i += 2) {
            if (sgf_seek(file, i) == -1) {
                printf("Erreur seek a la position %d\n", i);
                break;
            }
            int c = sgf_getc(file);
            if (c != -1) {
                putchar(c);
            }
        }
        printf("\n");
    sgf_close(file);
    
    return (EXIT_SUCCESS);
}
