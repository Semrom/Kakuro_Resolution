#include <stdio.h>
#include <stdlib.h>
#include "grille.h"
#include "util.h"

/*********************************************************
 * Lit un caractère et le stocke dans le buffer "texte"
 *********************************************************/
char lireCar(void){
  texte[longTexte++] = fgetc(fichier);
  texte[longTexte] = '\0';
  return texte[longTexte - 1];
}


/******************************************************
 * Remet le buffer au dernier caractère lu
 * et enlève le caractère courant du buffer "texte"
 ******************************************************/
void delireCar() {
  char c;
  c = texte[longTexte - 1];
  texte[--longTexte] = '\0';
  ungetc(c, fichier);
}


/*********************************************************
 * Retourne le caractère lu ou traite le cas d'un nombre.
 *********************************************************/
int lire(void) {
  char c;
  texte[longTexte = 0] = '\0';
  c = lireCar();

	if (is_num(c)) {
		do {
		  c = lireCar();
		} while (is_num(c));

		delireCar();

		return NOMBRE;
  	}

	return c;
}


/*********************************************************
 * Ajoute un élément au début d'une liste de variables.
 *********************************************************/
void empiler_debut(listeDeVariables* liste, variable var) {

    // On alloue de la mémoire pour le nouvel élément (nouvelle liste avec une variable).
    listeDeVariables nouveau = (variables*) malloc(sizeof(variables));
    // Le nouvel élément est le deuxième paramètre de la fonction.
    nouveau->var = var;
    // Le suivant est affecté à la tête de liste (puisque on insère au début).
    nouveau->suivant = *liste;
    // Maintenant le nouvel élément est en tête de liste.
    *liste = nouveau;

}


/*********************************************************
 * Ajoute un élément à la fin d'une liste de variables.
 *********************************************************/
void empiler_fin(listeDeVariables* liste, variable var) {

    // La variable i est en fait "inutile" (voir plus bas).
    int i = 0;

    // Déclaration d'un itérateur du même type que la liste de variables.
    listeDeVariables lv = NULL;

    // On alloue de la mémoire pour le nouvel élément (nouvelle liste avec une variable).
    variables* nouveau = (variables*) malloc(sizeof(variables));
    // Le nouvel élément est le deuxième paramètre de la fonction.
    nouveau->var = var;
    // Il n'y a pas de suivant (on l'ajoute à la fin).
    nouveau->suivant = NULL;

    // Affecter cette nouvelle liste à la liste de variables du CSP.
    if (*liste == NULL) {

        *liste = nouveau;

    } else {

        // On cherche à aller à la fin de la liste.
        for (lv = (*liste); lv->suivant != NULL; lv = lv->suivant) {
            i++;
        }

        lv->suivant = nouveau;
    }
}


/*********************************************************
 * Dépile et retourne la première variable de liste.
 *********************************************************/
variable depiler_var(listeDeVariables* liste) {

    // Variable temporaire pour sauvegarder la variable suivante de celle à supprimer.
    variables* tmp = NULL;
    // Variable de retour de la fonction.
    variable retour = {NULL, NULL};
    // Si la liste n'est pas vide.
    if(*liste != NULL)
    {
        // La variable de retour prend le premier élément de la liste.
        retour = (*liste)->var;
        // On sauvegarde l'élément suivant.
        tmp = (*liste)->suivant;
        // On libère la mémoire allouée au premier élément de la liste.
        free(*liste);
    }

    // L'élément suivant, sauvegardé, devient la tête de liste.
    (*liste) = tmp;

    return retour;
}


/*******************************************************************
 * Retourne la taille de la liste de variables (nombre d'éléments).
 *******************************************************************/
int getTaille(listeDeVariables liste) {

    // Nombre de variables de la liste.
    int nbVariables = 0;
    // Itérateur pour le parcours.
    listeDeVariables lv = NULL;

    for(lv = liste; lv != NULL; lv = lv->suivant)
        nbVariables++;

    return nbVariables;
}


/**************************************************************
    Vérifie si le domaine d'une variable est vide.
    Retourne la première valeur non testé du domaine ou -1.
***************************************************************/
int domaineVide(domaines* d) {
    int i;

    for(i = 0; i < 9; i++)
    {
        if(d[i] != 0)
            return i+1;
    }

    return -1;
}


/******************************************************************************
    Vérifie si une valeur est consistante. Renvoie 1 en cas de succès, 0 sinon.
*******************************************************************************/
int valeur_consistante(GrilleDeJeu jeu, variable var, int val) {

    if (absent_sur_ligne(jeu, var, val) == 1 && absent_sur_colonne(jeu, var, val) == 1 && calculer_somme(&jeu, var, val) == 1) {
        return 1;
    }

    return 0;
}


/**************************************************************************
    Vérifie si une valeur est absente de la ligne où se trouve la variable.
***************************************************************************/
int absent_sur_ligne(GrilleDeJeu jeu, variable var, int val) {

    int i, j, colonne;

    for (i = 0; i < nbLignes; i++) {
        for (j = 0; j < nbColonnes; j++) {
            if (jeu.matrice[i][j].id == var.case_blanche->id) {

                colonne = j;
                colonne++;
                // On explore les cases blanches à droite de la variable.
                while (colonne < nbColonnes && jeu.matrice[i][colonne].couleur != NOIR) {
                    
                    if (jeu.cases_blanches[jeu.matrice[i][colonne].id].valeur == val) {
                        return 0;
                    }

                    colonne++;
                }

                colonne = j;
                colonne--;
                // On explore les cases blanches à gauche de la variable.
                while (colonne >= 0 && jeu.matrice[i][colonne].couleur != NOIR) {
                    if (jeu.cases_blanches[jeu.matrice[i][colonne].id].valeur == val) {
                        return 0;
                    }

                    colonne--;
                }

                return 1;
            }
        }
    }

    return 0;
}


/**************************************************************************
    Vérifie si une valeur est absente de la colonne où se trouve la variable.
***************************************************************************/
int absent_sur_colonne(GrilleDeJeu jeu, variable var, int val) {

    int i, j, ligne;

    for (i = 0; i < nbLignes; i++) {
        for (j = 0; j < nbColonnes; j++) {
            if (jeu.matrice[i][j].id == var.case_blanche->id) {

                ligne = i;
                ligne++;
                // On explore les cases blanches en dessous de la variable.
                while (ligne < nbLignes && jeu.matrice[ligne][j].couleur != NOIR) {
                    if (jeu.cases_blanches[jeu.matrice[ligne][j].id].valeur == val) {
                        return 0;
                    }

                    ligne++;
                }

                ligne = i;
                ligne--;
                // On explore les cases blanches au dessus de la variable.
                while (ligne >= 0 && jeu.matrice[ligne][j].couleur != NOIR) {

                    if (jeu.cases_blanches[jeu.matrice[ligne][j].id].valeur == val) {
                        return 0;
                    }

                    ligne--;
                }

                return 1;
            }
        }
    }

    return 0;
}


/****************************************************
    Calcule la somme d'une série de cases blanches.
    Retourne 1 si somme OK, 0 sinon.
*****************************************************/
int calculer_somme(GrilleDeJeu* jeu, variable var, int val) {

    int i, j, k, l, rempli_h = 0, rempli_v = 0, somme;

    /************ VERIFIE SI LA LIGNE OU LA COLONNE DE SOMME EST REMPLIE **************/
    for (i = 0; i < nbLignes; i++) {
        for (j = 0; j < nbColonnes; j++) {
            if (jeu->matrice[i][j].contrainte_verticale.valeur == var.case_blanche->contrainte_verticale) {
                for (k = 0; k < jeu->matrice[i][j].contrainte_verticale.nbCases; k++) {
                    if (jeu->matrice[i][j].contrainte_verticale.cases_associees[k].valeur == 0 && jeu->matrice[i][j].contrainte_verticale.cases_associees[k].id != var.case_blanche->id) {
                        rempli_v = 0;
                        break;
                    } else {
                        rempli_v = 1;
                    }
                }
            }

            if (jeu->matrice[i][j].contrainte_horizontale.valeur == var.case_blanche->contrainte_horizontale) {
                for (k = 0; k < jeu->matrice[i][j].contrainte_horizontale.nbCases; k++) {
                    if (jeu->matrice[i][j].contrainte_horizontale.cases_associees[k].valeur == 0 && jeu->matrice[i][j].contrainte_horizontale.cases_associees[k].id != var.case_blanche->id) {
                        rempli_h = 0;
                        break;
                    } else {
                        rempli_h = 1;
                    }
                }
            }
        }
    }

    // Si la ligne ou la colonne n'est pas remplie, alors échec.
    if (rempli_h == 0 && rempli_v == 0) {
        return 1;

    // Sinon.
    } else {

        /************ CALCUL DE LA SOMME **************/
        for (i = 0; i < nbLignes; i++) {
            for (j = 0; j < nbColonnes; j++) {
                if (rempli_v == 1 && jeu->matrice[i][j].contrainte_verticale.valeur == var.case_blanche->contrainte_verticale) {
                    somme = 0;
                    for (k = 0; k < jeu->matrice[i][j].contrainte_verticale.nbCases; k++) {
                        if (jeu->matrice[i][j].contrainte_verticale.cases_associees[k].valeur == 0) {
                            jeu->matrice[i][j].contrainte_verticale.cases_associees[k].valeur = val;
                            l = k;
                        }

                        somme = somme + jeu->matrice[i][j].contrainte_verticale.cases_associees[k].valeur;
                    }
                    jeu->matrice[i][j].contrainte_verticale.cases_associees[l].valeur = 0;
                    if (somme == jeu->matrice[i][j].contrainte_verticale.valeur) {
                        return 1;
                    } else {
                        return 0;
                    }
                }

                if (rempli_h == 1 && jeu->matrice[i][j].contrainte_horizontale.valeur == var.case_blanche->contrainte_horizontale) {
                    somme = 0;
                    for (k = 0; k < jeu->matrice[i][j].contrainte_horizontale.nbCases; k++) {
                        if (jeu->matrice[i][j].contrainte_horizontale.cases_associees[k].valeur == 0) {
                            jeu->matrice[i][j].contrainte_horizontale.cases_associees[k].valeur = val;
                            l = k;
                        }

                        somme = somme + jeu->matrice[i][j].contrainte_horizontale.cases_associees[k].valeur;
                    }
                    jeu->matrice[i][j].contrainte_horizontale.cases_associees[l].valeur = 0;
                    if (somme == jeu->matrice[i][j].contrainte_horizontale.valeur) {
                        return 1;
                    } else {
                        return 0;
                    }
                }
            }
        }
    }

    return 0;
}


/*************************************************************************
    Met à jour les valeurs des cases associées aux contraintes de somme.
**************************************************************************/
void mettreAJourContraintesSomme(variable var, int val, GrilleDeJeu* jeu) {

    int i, j, k;

    for (i = 0; i < nbLignes; i++) {
        for (j = 0; j < nbColonnes; j++) {
            if (jeu->matrice[i][j].contrainte_verticale.valeur == var.case_blanche->contrainte_verticale) {
                for (k = 0; k < jeu->matrice[i][j].contrainte_verticale.nbCases; k++) {
                    if (jeu->matrice[i][j].contrainte_verticale.cases_associees[k].id == var.case_blanche->id) {
                        jeu->matrice[i][j].contrainte_verticale.cases_associees[k].valeur = val;
                    }
                }
            }

            if (jeu->matrice[i][j].contrainte_horizontale.valeur == var.case_blanche->contrainte_horizontale) {
                for (k = 0; k < jeu->matrice[i][j].contrainte_horizontale.nbCases; k++) {
                    if (jeu->matrice[i][j].contrainte_horizontale.cases_associees[k].id == var.case_blanche->id) {
                        jeu->matrice[i][j].contrainte_horizontale.cases_associees[k].valeur = val;
                    }
                }
            }
        }
    }
}
