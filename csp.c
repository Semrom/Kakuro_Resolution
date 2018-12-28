#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csp.h"
#include "util.h"

/*********************************************
    Crée le CSP de la grille.
**********************************************/
void initialiser_csp(GrilleDeJeu jeu, CSP* csp) {

    int i, j;

    // Liste des variables.
    csp->V = NULL;
    // Liste des affectations.
    csp->A = NULL;
    // Domaine de chaque case blanche.
    csp->D = (domaines**)malloc(jeu.nbCasesBlanches * sizeof(domaines*));

    for (i = 0; i < jeu.nbCasesBlanches; i++) {
        csp->D[i] = (domaines*)malloc(9 * sizeof(domaines));
    }

    // Déclaration d'une variable du CSP.
    variable var;

    for (i = 0; i < jeu.nbCasesBlanches; i++) {

        for (j = 0; j < 9; j++) {
            // Le domaines valent 1, c'est à dire qu'ils n'ont pas été encore testés.
            csp->D[i][j] = 1;
        }

        // On associe l'adresse de la case blanche à la variable.
        var.case_blanche = &(jeu.cases_blanches[i]);
        // Et on associe à cette même variable son domaine.
        var.domaine = csp->D[i];

        // Enfin, on empile celle-ci dans la liste des variables.
        empiler_fin(&(csp->V), var);
    }

    printf("\nCSP initialisé.\n\n");
}


/*********************************************
    Libère le CSP de la grille.
**********************************************/
void free_csp(GrilleDeJeu jeu, CSP* csp) {

    int i, j;

    for (i = 0; i < jeu.nbCasesBlanches; i++) {

        // Pour chaque variables de la liste des variables du CSP.
        // On dépile la variable en tête.
        for(j = 0; j < getTaille(csp->V); j++)
            depiler_var(&(csp->V));

        // Pour chaque variables de la liste des affectations du CSP.
        // On dépile l'affectation en tête.
        for(j = 0; j < getTaille(csp->A); j++)
            depiler_var(&(csp->A));
    }

    printf("\nCSP libéré.\n\n");
}
