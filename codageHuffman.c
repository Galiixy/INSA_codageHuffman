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
    int longueur = 0 ;
    if (fichier == NULL)
    {
        printf("Impossible d'ouvrir le fichier");
        exit(-1);
    }

    fseek(fichier, 0, SEEK_END); // on va a la derniere ligne du fichier
    longueur = ftell(fichier); // longueur de la chaine de caractere du fichier

    if (longueur == 0){
        printf("Impossible de lire un fichier vide");
        exit(-1);
    }
    
    fseek(fichier, 0, SEEK_SET); // on retourne au debut pour lire le fichier
    char * contenu = (char *) malloc(longueur); // on construit la variable contenu en fonction de la longueur du fichier
    
    for (int i = 0; i <= longueur; i++)
    {
        contenu[i] = '\0'; // initialisation a vide pour eviter que le contenu ait des donnees incoherentes
    }
    
    fread(contenu,longueur,1,fichier); // lecture du fichier
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
