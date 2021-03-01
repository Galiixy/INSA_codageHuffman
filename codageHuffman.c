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

//------------------------------------------------- STRUCTURES
typedef struct Arbre
{
    int nbOccurences;
    char caractere;
    struct Arbre *b[2]; //branche
} Arbre;

typedef struct Liste
{
    Arbre *arbre;
    struct Liste *suivante;
} Liste;

//------------------------------------------------- Declarations des fonctions

// Mode d'emploi : prend le nom d'un fichier
// retourne son contenu dans une chaine de caractere
char *LectureFichier(char *nomFichier);

// Mode d'emploi : prend une chaine de caractere
// complete le tableau histo : le contenu de chaque case (ASCII)
// correspond au nombre de fois que le caractere a ete rencontre
void ConstruitHisto(char *contenu, int histo[128]);

// Mode d'emploi : prend un tableau de caractere
// complete la structure liste en triant par ordre croissant
void Tri(int histo[128], Liste **ptrListe);

// Mode d'emploi : prend un pointeur liste et arbre
// place l'element arbre dans la liste de maniere triee
void trierListe(Liste **ptrListe, Arbre *arbre);

// Methode d'affichage de la lise chaine
void AfficherListe(Liste *);

// Mode d'emploi : prend une liste chaine compose d'arbre
// retourne un pointeur arbre
Arbre * ConstruireArbre(Liste *liste);

// Mode d'emploi : prend un arbre en parametre
// cree le fichier.dot
void VisualiserArbre(Arbre * arbre, char *nomFichier);

// Mode d'emploi : prend un arbre en parametre
// Methode recursive pour afficher chacune des branches
void VisualiserBranche(Arbre *arbre, FILE *fichierParam);

void SupprArbre(Arbre *arbre);

//------------------------------------------------- MAIN
int main()
{
    char nomFichier[30];
    printf("Saisir nom de fichier : ");
    scanf("%s", nomFichier);

    char *contenu = LectureFichier(nomFichier);
    int histo[128];

    printf("%s\n", contenu);

    ConstruitHisto(contenu, histo);
    free(contenu);

    for (int i = 0; i < 128; i++)
    {
        printf("tcode ASCII : %d, Nb apparition : %d \n\r", i, histo[i]);
    }

    Liste *liste = (Liste *)malloc(sizeof(Liste));
    liste->arbre = NULL;
    liste->suivante = NULL;
    Liste **ptrListe = &liste;

    Tri(histo, ptrListe);
    AfficherListe(liste);

    Arbre *arbre = ConstruireArbre(*ptrListe);

    printf("Saisir nom de fichier param pour Graphiz(dot): ");
    nomFichier[30] = '\0';
    scanf("%s", nomFichier);
    VisualiserArbre(arbre, nomFichier);

    SupprArbre(arbre);
    return 0;
}

//------------------------------------------------- Definitions des fonctions

char *LectureFichier(char *nom)
{
    FILE *fichier = fopen(nom, "r"); // Ouverture en lecture
    int longueur = 0;
    if (fichier == NULL)
    {
        printf("Impossible d'ouvrir le fichier");
        exit(-1);
    }

    fseek(fichier, 0, SEEK_END); // on va a la derniere ligne du fichier
    longueur = ftell(fichier);   // longueur de la chaine de caractere du fichier

    if (longueur == 0)
    {
        printf("Impossible de lire un fichier vide");
        exit(-1);
    }

    fseek(fichier, 0, SEEK_SET);                  // on retourne au debut pour lire le fichier
    char *contenu = (char *)malloc(longueur + 1); // on construit la variable contenu en fonction de la longueur du fichier

    contenu[longueur] = '\0'; // initialisation a vide pour eviter que le contenu ait des donnees incoherentes

    fread(contenu, longueur, 1, fichier); // lecture du fichier
    fclose(fichier); //fermeture
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

void Tri(int histo[128], Liste **ptrListe)
{
    int i;
    for (i = 0; i < 128; ++i)
    {
        if (histo[i] != 0)
        {
            Arbre *feuille = (Arbre *)malloc(sizeof(Arbre));
            char caractere;
            caractere = (char)i;
            feuille->caractere = caractere;
            feuille->nbOccurences = histo[i];
            feuille->b[0] = NULL;
            feuille->b[1] = NULL;
            trierListe(ptrListe, feuille);
        }
    }
}

void trierListe(Liste **ptrListe, Arbre *arbre)
{
    Liste *liste = *ptrListe;
    if (liste != NULL)
    {
        while (liste->arbre != NULL)
        {
            if (liste->arbre->nbOccurences < arbre->nbOccurences)
            { // creation de du maillon de fin de liste
                if (liste->suivante == NULL)
                {
                    Liste *newListe = (Liste *)malloc(sizeof(Liste));
                    newListe->arbre = NULL;
                    newListe->suivante = NULL;
                    liste->suivante = newListe;
                }
                liste = liste->suivante;
            }
            else
            {
                // permutation
                // duplication des valeurs de l'element actuel pour le mettre dans un
                // nouveau maillon mis a la suite
                Liste *listeBis = (Liste *)malloc(sizeof(Liste));
                listeBis->arbre = liste->arbre;
                listeBis->suivante = liste->suivante;
                liste->suivante = listeBis;
                liste->arbre = arbre;
                return;
            }
        }
        liste->arbre = arbre;
    }
}

void AfficherListe(Liste *liste)
{
    if (liste != NULL)
    {
        printf("Car : %c ; Nb : %d\n\r", liste->arbre->caractere, liste->arbre->nbOccurences);
        if (liste->suivante != NULL)
            AfficherListe(liste->suivante);
    }
}

Arbre * ConstruireArbre(Liste *liste)
{
    Liste *tmp = NULL;
    Arbre *ptrRacine;

    while (liste != NULL && liste->suivante != NULL)
    {
        Arbre *feuille = (Arbre *)malloc(sizeof(Arbre));
        feuille->b[0] = liste->arbre;
        feuille->b[1] = liste->suivante->arbre;
        feuille->caractere = '\0';
        feuille->nbOccurences = feuille->b[0]->nbOccurences + feuille->b[1]->nbOccurences;
      
        tmp = liste->suivante->suivante;

        free(liste->suivante);
        free(liste);

        liste = tmp;
        Liste **ptrListe = &liste;
        trierListe(ptrListe, feuille);

        ptrRacine = feuille;

        /* Affichage pour test */
        printf("1ere branche : %c ; %d | ", ptrRacine->b[0]->caractere, ptrRacine->b[0]->nbOccurences);
        printf("2e branche : %c ; %d | ", ptrRacine->b[1]->caractere, ptrRacine->b[1]->nbOccurences);
        printf("Branche FUSION : %c ; %d\n\r", ptrRacine->caractere, ptrRacine->nbOccurences);
    }
    free(liste);
    return ptrRacine;
}
//*
void VisualiserArbre(Arbre * arbre, char *nomFichier)
{

    FILE *fichierParam = fopen(nomFichier, "w+");

    fputs("digraph {\n", fichierParam);
    VisualiserBranche(arbre, fichierParam);
    fputs("}", fichierParam);

    fclose(fichierParam);
}
//*/
void VisualiserBranche(Arbre *arbre, FILE *fichierParam)
{
    char idBranche[25];
    idBranche[24] = '\0';
    char idSousBranche[25];
    idSousBranche[24] = '\0';
    char label[5];
    label[4] = '\0';
    
    //creation du node
    sprintf(idBranche, "%ld", (long)arbre);
    fputs(idBranche, fichierParam);
    fputs(";\n", fichierParam);

    if (arbre->b[0] != NULL || arbre->b[1] != NULL)
    {
        //creation de la boite
        fputs(idBranche, fichierParam);
        fputs("[label=\"", fichierParam);
        sprintf(label, "%d", arbre->nbOccurences);
        fputs(label, fichierParam);
        fputs("\"];\n", fichierParam);

        if (arbre->b[0] != NULL)
        {
            //lien avec la branche
            fputs(idBranche, fichierParam);
            fputs("->", fichierParam);
            sprintf(idSousBranche, "%ld", (long) arbre->b[0]);
            fputs(idSousBranche, fichierParam);
            fputs(";\n", fichierParam);
            VisualiserBranche(arbre->b[0], fichierParam);
        }
        if (arbre->b[1] != NULL)
        {
            //lien avec la branche
            fputs(idBranche, fichierParam);
            fputs("->", fichierParam);
            idSousBranche[10] = '\0';
            sprintf(idSousBranche, "%ld", (long) arbre->b[1]);
            fputs(idSousBranche, fichierParam);
            fputs(";\n", fichierParam);
            VisualiserBranche(arbre->b[1], fichierParam);
        }
    }
    else
    {
        //creation de la boite
        fputs(idBranche, fichierParam);
        fputs("[label=\"", fichierParam);
        sprintf(label, "%c", arbre->caractere);
        fputs(label, fichierParam);
        fputs("(", fichierParam);
        sprintf(label, "%d", arbre->nbOccurences);
        fputs(label, fichierParam);
        fputs(")\"];\n", fichierParam);

        // Shape & color
        fputs(idBranche, fichierParam);
        fputs("[shape=box, style=filled, fillcolor=powderblue];\n", fichierParam);
    }
}

void SupprArbre(Arbre *arbre) {
    if(arbre->b[0] != NULL) {
        SupprArbre(arbre->b[0]);
    }
    if (arbre->b[1] != NULL)
    {
        SupprArbre(arbre->b[1]);
    }
    free(arbre);
}
