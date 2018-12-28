#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grille.h"
#include "util.h"


/**************************************************************
    Initialise la grille de jeu selon la taille récupérée.
***************************************************************/
void initialiser_grille(GrilleDeJeu* jeu, char* file, char* mode) {

    int i;

    // Cette fonction initialisera les variables "nbLignes" et "nbColonnes".
    recuperer_taille_grille(file, mode);

    jeu->matrice = (Case **)malloc((nbLignes) * sizeof(Case*));

    for(i = 0; i < nbLignes; i++)
        jeu->matrice[i] = (Case *)malloc((nbColonnes) * sizeof(Case));

    if (jeu->matrice == NULL) {
        printf("\nProblème d'allocation pour 'jeu->matrice' (vaut NULL).\n\n");
        exit(0);
    }

}


/**************************************************************
    Récupère la taille de la grille de Kakuro.
***************************************************************/
void recuperer_taille_grille(char* file, char* mode) {

    fichier = fopen(file, mode);

    int caractereActuel = 0;
    int colonne = 0;

    nbLignes = 0;
    nbColonnes = 0;

    do {

        caractereActuel = fgetc(fichier);

        if (caractereActuel == '\n') {
            nbLignes++;

            if (colonne > nbColonnes) {
                nbColonnes = colonne + 1;
            }

            caractereActuel = fgetc(fichier);

            if (caractereActuel != EOF) {
                colonne = 0;
                ungetc(caractereActuel, fichier);
            }

        }

        if (caractereActuel == ' ') {
            colonne++;
        }

    } while(caractereActuel != EOF);

    printf("\nLe fichier possède une grille de %d lignes et de %d colonnes (max.) :\n\n", nbLignes, nbColonnes);

}


/*******************************************************
    Modélise et affiche la matrice de la grille de jeu.
********************************************************/
void modeliser_grille(char* file, char* mode, GrilleDeJeu* jeu) {

	int ligne = 0;
	int colonne = 0;

    // Parcours de la matrice pour initialiser la structure aux valeurs par défaut.
    for (ligne = 0; ligne < nbLignes; ligne++) {
        for (colonne = 0; colonne < nbColonnes; colonne++) {
            jeu->matrice[ligne][colonne].id = -1;
            jeu->matrice[ligne][colonne].contrainte_verticale.valeur = -1;
            jeu->matrice[ligne][colonne].contrainte_horizontale.valeur = -1;
        }
    }

    // Réinitialisation des variables.
    ligne = 0;
    colonne = 0;
    id_blanc = 0;
    int caractereActuel;
    texte[longTexte = 0] = '\0';
	fichier = fopen(file, mode);

    // Association fichier ==> matrice.
	if (fichier != NULL) {

        do
        {
            caractereActuel = lire();

            if (caractereActuel == '\n') {
                ++ligne;
            }

            if (colonne == 0 && ligne < nbLignes) {

	        	if (caractereActuel == '\\') {

	        		caractereActuel = lire();

	        		if (caractereActuel == ' ') {

	        			jeu->matrice[ligne][colonne].couleur = NOIR;
	        			++colonne;

	        		} else if (caractereActuel == NOMBRE) {

                        jeu->matrice[ligne][colonne].couleur = NOIR;
	        			jeu->matrice[ligne][colonne].contrainte_horizontale.valeur = atoi(texte);
	        		}

	        	} else if (caractereActuel == ' ') {
                    ++colonne;
                }

	        } else if (colonne > 0 && colonne < nbColonnes - 1) {

                if (caractereActuel == NOMBRE) {

            		jeu->matrice[ligne][colonne].couleur = NOIR;
                    jeu->matrice[ligne][colonne].contrainte_verticale.valeur = atoi(texte);

            		caractereActuel = lire();

            		if (caractereActuel == '\\') {

            			caractereActuel = lire();

                        if (caractereActuel == NOMBRE) {

                            jeu->matrice[ligne][colonne].contrainte_horizontale.valeur = atoi(texte);

            			} else if (caractereActuel == ' ') {
            				++colonne;
            			}
            		}
            	} else if (caractereActuel == ' ') {
                    ++colonne;

                } else if (caractereActuel == '.') {

            				jeu->matrice[ligne][colonne].couleur = BLANC;
            				jeu->matrice[ligne][colonne].id = id_blanc++;

            	} else if (caractereActuel == '\\') {

            			caractereActuel = lire();

            			if (caractereActuel == ' ') {

            				jeu->matrice[ligne][colonne].couleur = NOIR;
                            ++colonne;

            			} else if (caractereActuel == NOMBRE) {

            				jeu->matrice[ligne][colonne].couleur = NOIR;
            				jeu->matrice[ligne][colonne].contrainte_horizontale.valeur = atoi(texte);
            			}
            	}

            } else if (colonne == nbColonnes - 1) {

                if (caractereActuel == '\\') {

                    jeu->matrice[ligne][colonne].couleur = NOIR;
                    colonne = 0;

                } else if (caractereActuel == NOMBRE) {

                    jeu->matrice[ligne][colonne].couleur = NOIR;
                    jeu->matrice[ligne][colonne].contrainte_verticale.valeur = atoi(texte);

                    caractereActuel = lire();

                    if (caractereActuel == '\\') {
                        colonne = 0;
                    }
                } else if (caractereActuel == '.') {
                    jeu->matrice[ligne][colonne].couleur = BLANC;
                    jeu->matrice[ligne][colonne].id = id_blanc++;
                    colonne = 0;
                }
            }

        } while (caractereActuel != EOF);

        fclose(fichier);

        // Le nombre de cases blanches est maintenant connu.
        jeu->nbCasesBlanches = id_blanc;

        jeu->cases_blanches = malloc((id_blanc) * sizeof(CaseBlanche));

        if (jeu->cases_blanches == NULL) {
            printf("\nProblème d'allocation pour 'jeu->cases_blanches' (vaut NULL).\n\n");
            exit(0);
        }

        // Récupération des cases blanches.
        recuperer_cases_blanches(jeu->matrice, jeu->cases_blanches);

        // Ajout des cases blanches des contraintes de somme.
        creer_contrainte_somme(jeu);

    } else {
    	printf("Erreur lors de l'ouverture du fichier\n");
        exit(-1);
    }
}


/********************************************************************
    Affecte les sommes horizontales et verticales aux cases blanches.
********************************************************************/
void recuperer_cases_blanches(Case** matrice, CaseBlanche* cases_blanches) {

    int i, j, k = 0, ligne, colonne, pos;

    for (i = 0; i < nbLignes; i++) {

        for (j = 0; j < nbColonnes; j++) {

            ligne = i;
            colonne = j;

             if (matrice[i][j].couleur == NOIR) {

                 if (matrice[i][j].contrainte_verticale.valeur != -1) {

                     ligne++;

                     while (ligne < nbLignes && matrice[ligne][j].couleur == BLANC) {
                         cases_blanches[k].id = matrice[ligne][j].id;
                         cases_blanches[k].contrainte_verticale = matrice[i][j].contrainte_verticale.valeur;
                         cases_blanches[k].contrainte_horizontale = matrice[i][j].contrainte_horizontale.valeur;
                         cases_blanches[k].valeur = 0;

                         k++;

                         ligne++;
                     }
                 }

                if (matrice[i][j].contrainte_horizontale.valeur != -1) {

                    colonne++;

                    while (colonne < nbColonnes && matrice[i][colonne].couleur == BLANC) {
                        if ((pos = contient_contrainte(cases_blanches, matrice[i][colonne].id)) != -1) {
                            cases_blanches[pos].contrainte_horizontale = matrice[i][j].contrainte_horizontale.valeur;
                        } else {

                            cases_blanches[k].id = matrice[i][colonne].id;
                            cases_blanches[k].contrainte_horizontale = matrice[i][j].contrainte_horizontale.valeur;
                            cases_blanches[k].contrainte_verticale = matrice[i][j].contrainte_verticale.valeur;
                            cases_blanches[k].valeur = 0;

                            k++;
                        }

                        colonne++;
                    }
                }
             }
        }
    }

    /**** DEBUG ****
    for (i = 0; i < id_blanc; i++) {
        printf("ID : %d, CV : %d, CH : %d\n", cases_blanches[i].id, cases_blanches[i].contrainte_verticale, cases_blanches[i].contrainte_horizontale);
    }
    ****************/
}


/********************************************************************
    Affecte les sommes aux cases blanches correspondantes.
********************************************************************/
void creer_contrainte_somme(GrilleDeJeu* jeu) {

    int ligne, colonne, i, j, compteur;

    for (ligne = 0; ligne < nbLignes; ligne++) {
        for (colonne = 0; colonne < nbColonnes; colonne++) {

            if (jeu->matrice[ligne][colonne].couleur == NOIR) {

                if (jeu->matrice[ligne][colonne].contrainte_horizontale.valeur != -1) {

                    compteur = 0;
                    j = colonne;
                    j++;
                    while(j < nbColonnes && jeu->matrice[ligne][j].couleur != NOIR) {
                        compteur++;
                        j++;
                    }

                    jeu->matrice[ligne][colonne].contrainte_horizontale.cases_associees = malloc((compteur) * sizeof(CaseBlanche));
                    jeu->matrice[ligne][colonne].contrainte_horizontale.nbCases = compteur;
                    compteur = 0;
                    j = colonne;
                    j++;
                    while(j < nbColonnes && jeu->matrice[ligne][j].couleur != NOIR) {
                        jeu->matrice[ligne][colonne].contrainte_horizontale.cases_associees[compteur].id = jeu->matrice[ligne][j].id;
                        jeu->matrice[ligne][colonne].contrainte_horizontale.cases_associees[compteur].valeur = jeu->cases_blanches[jeu->matrice[ligne][j].id].valeur;
                        compteur++;
                        j++;
                    }
                }

                if (jeu->matrice[ligne][colonne].contrainte_verticale.valeur != -1) {

                    compteur = 0;
                    i = ligne;
                    i++;
                    while(i < nbLignes && jeu->matrice[i][colonne].couleur != NOIR) {
                        compteur++;
                        i++;
                    }

                    jeu->matrice[ligne][colonne].contrainte_verticale.cases_associees = malloc((compteur) * sizeof(CaseBlanche));
                    jeu->matrice[ligne][colonne].contrainte_verticale.nbCases = compteur;
                    compteur = 0;
                    i = ligne;
                    i++;
                    while(i < nbLignes && jeu->matrice[i][colonne].couleur != NOIR) {
                        jeu->matrice[ligne][colonne].contrainte_verticale.cases_associees[compteur].id = jeu->matrice[i][colonne].id;
                        jeu->matrice[ligne][colonne].contrainte_verticale.cases_associees[compteur].valeur = jeu->cases_blanches[jeu->matrice[i][colonne].id].valeur;
                        compteur++;
                        i++;
                    }
                }
            }
        }
    }
}


/*************************************************************************
    Recherche si une case blanche a déjà été traitée avec une contrainte.
***************************************************************************/

int contient_contrainte(CaseBlanche* cases_blanches, int identifiant) {
    int i;

    for (i = 0; i < id_blanc; i++) {
        if (cases_blanches[i].id == identifiant) {
            return i;
        }
    }

    return -1;
}


/******************************************************
    Affiche la grille sous la forme du fichier source.
*******************************************************/
void afficher_grille(GrilleDeJeu* jeu) {

    int ligne, colonne;

    for (ligne = 0; ligne < nbLignes; ligne++) {
        for (colonne = 0; colonne < nbColonnes; colonne++) {

            if (jeu->matrice[ligne][colonne].couleur == NOIR) {

                if (jeu->matrice[ligne][colonne].contrainte_verticale.valeur != -1 && jeu->matrice[ligne][colonne].contrainte_horizontale.valeur != -1) {
                    printf("%d\\%d ", jeu->matrice[ligne][colonne].contrainte_verticale.valeur, jeu->matrice[ligne][colonne].contrainte_horizontale.valeur);
                }

                if (jeu->matrice[ligne][colonne].contrainte_verticale.valeur != -1 && jeu->matrice[ligne][colonne].contrainte_horizontale.valeur == -1) {
                    printf("%d\\ ", jeu->matrice[ligne][colonne].contrainte_verticale.valeur);
                }

                if (jeu->matrice[ligne][colonne].contrainte_verticale.valeur == -1 && jeu->matrice[ligne][colonne].contrainte_horizontale.valeur != -1) {
                    printf("\\%d ", jeu->matrice[ligne][colonne].contrainte_horizontale.valeur);
                }

                if (jeu->matrice[ligne][colonne].contrainte_verticale.valeur == -1 && jeu->matrice[ligne][colonne].contrainte_horizontale.valeur == -1) {
                    printf("\\ ");
                }
            } else if (jeu->matrice[ligne][colonne].couleur == BLANC && jeu->cases_blanches[jeu->matrice[ligne][colonne].id].valeur != 0) {
                printf("%d ", jeu->cases_blanches[jeu->matrice[ligne][colonne].id].valeur);
            } else {
                printf(". ");
            }
        }

        printf("\n");
    }

    printf("\n");
}

/******************************************************
    Libère la matrice de la grille de jeu.
*******************************************************/
void libererMatrice(Case **a, int m) {
    int i;
    for (i = 0; i < m; ++i) {
        free(a[i]);
    }
    free(a);
    printf("Matrice libérée.\n\n");
}
