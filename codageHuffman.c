/*************************************************************************
                                CodageHuffman
                             -------------------
    debut                : 04/02/2021
    copyright            : (C) 2021 par Gaelle FERREIRA
    e-mail               : gaelle.ferreira@insa-lyon.fr
*************************************************************************/
//------------------------------------------------- Librairies
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------- CONSTANTES
#define LongMaxFichier 1024

//------------------------------------------------- Declarations des fonctions

// Mode d'emploi : prend le nom d'un fichier 
// retourne son contenu dans une chaine de caractere
char *LectureFichier(char *nom);

// Mode d'emploi : prend une chaine de caractere 
// complete le tableau histo : le contenu de chaque case (ASCII)
// correspond au nombre de fois que le caractere a ete rencontre
void ConstruitHisto(char *contenu, int histo[128]);

//------------------------------------------------- MAIN
int main()
{
    char nomFichier[30];
    printf("Saisir nom de fichier : ");
    scanf("%s", &nomFichier);
    
    char *contenu = LectureFichier(nomFichier);
    int histo[128];

    printf("%s\n", contenu);

    ConstruitHisto(contenu, histo);

    for (int i = 0; i < 128; i++)
    {
        printf("code ASCII : %d, Nb apparition : %d \n\r", i, histo[i]);
    }

    return 0;
}

//------------------------------------------------- Definitions des fonctions

char *LectureFichier(char *nom)
{
    FILE *fichier = fopen(nom, "r"); // Ouverture en lecture

    if (fichier == NULL)
    {
        printf("Impossible d'ouvrir le fichier");
        exit(-1);
    }

    // instanciation de la memoire
    char *contenu = (char *)malloc(LongMaxFichier * sizeof(char));
    char *caractere = (char *)malloc(1 * sizeof(char));

    // initialisation a vide pour eviter que le contenu ait des donnees incoherentes
    sprintf(contenu, "%c", '\0');

    while (!feof(fichier))
    { //jusqua la fin du fichier
        sprintf(caractere, "%c", fgetc(fichier));
        strcat(contenu, caractere);
    }
    return contenu;
}

void ConstruitHisto(char *contenu, int histo[128])
{
    int i = 0;
    int codeASCII;

    for (i = 0; i < 128; i++)
    {
        histo[i] = 0;
    }

    i = 0;
    while (contenu[i] != '\0')
    {
        codeASCII = (int)contenu[i];
        
        if (codeASCII < 0)
        {
            break; // retour chariot de fin de ligne pas pris en compte par ASCII
        }
        histo[codeASCII]++;
        i++;
    }
}
