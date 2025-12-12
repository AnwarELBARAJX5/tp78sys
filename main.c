
/************************************************************
 Utilisation du SGF
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "sgf-header.h"

void sgf_remove(int adr_inode);
int main() {
/* Ancien main
    init_sgf();
    
    printf("\nLISTE DES FICHIERS\n\n");
    list_directory();
    

    OFILE *file = sgf_open("essai.txt", WRITE_MODE);
     printf("TEST ECRITURE \n");
         sgf_puts(file, "Ceci est un petit texte qui occupe\n");
         sgf_puts(file, "quelques blocs sur ce disque fictif.\n");
         sgf_puts(file, "Le bloc faisant 128 octets, il faut\n");
         sgf_puts(file, "que je remplisse pour utiliser\n");
         sgf_puts(file, "plusieurs blocs.\n");
    sgf_close(file);
    printf("Ecriture terminée\n");
    printf("\nVERIFICATION DE LA LECTURE\n");
        list_directory();
    file = sgf_open("essai.txt", READ_MODE);
    printf("Contenu du fichier\n");
            for (int c; ((c = sgf_getc(file)) > 0); ) {
                putchar(c);
            }
     sgf_close(file);

    int inode_essai = find_inode("essai.txt");
    printf("\nSuppression de essai.txt\n");
    sgf_remove(inode_essai);
    delete_inode("essai.txt");
    printf("Verification apres suppression :\n");
    list_directory();
    */
    init_sgf();
    printf("\nTEST MODE AJOUT (APPEND)\n");
    printf("Reinitialisation de 'test.txt'...\n");
    OFILE* file = sgf_open("test.txt", WRITE_MODE);
    if (file != NULL) {
        sgf_close(file);
    } else {
        printf("Erreur impossible de créer le fichier.\n");
        return EXIT_FAILURE;
    }
    printf("Ajout de 300 caracteres\n");
    for (int i = 0; i < 300; i++) {
        file = sgf_open("test.txt", APPEND_MODE);
        sgf_putc(file, 'X');
        sgf_close(file);
    }
    printf("\nVerification de la taille ==300 ??\n");
    list_directory();

    return (EXIT_SUCCESS);
}
