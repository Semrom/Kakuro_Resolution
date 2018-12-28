#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backtrack.h"
#include "csp.h"
#include "grille.h"
#include "util.h"

/***********************************************************
    Algorithme backtrack.
    Retourne 1 si une solution a été trouvée, 0 sinon.
************************************************************/
int backtrack(GrilleDeJeu jeu, CSP* csp) {

    int i;

    // Pour la prochaine variable à tester.
    variable var;
    // Vaut 1 si une valeur est consistante.
    int consistant = 0;
    // Pour la valeur du domaine.
    int valeur;

    // Tant que la liste des variables du CSP n'est pas vide.
    while(csp->V != NULL) {

        // Prochaine variable à tester.
        var = depiler_var(&(csp->V));
        consistant = 0;

        // Tant qu'il y a des valeurs dans le domaine de la variable à tester.
        while((valeur = domaineVide(var.domaine)) != -1) {

            var.domaine[valeur - 1] = 0;
            consistant = 0;

            // Si la valeur est consistante.
            if ((consistant = valeur_consistante(jeu, var, valeur)) == 1) {

                // On affecte la valeur consistante à la case correpsondante.
                jeu.cases_blanches[var.case_blanche->id].valeur = valeur;

                // On met à jour les valeurs des cases associées aux contraintes de somme.
                mettreAJourContraintesSomme(var, valeur, &jeu);

                // On empile cette affectation dans la liste des affectations.
                empiler_debut(&(csp->A), var);

                break;
            }
        }

        // Si aucune valeur n'a été consistante.
        if (consistant == 0) {

            // On remet la valeur de la case à 0
            var.case_blanche->valeur = 0;

            // Si aucune affectation n'a pu être possible.
            if (csp->A == NULL) {

                return 0;
            }

            // Réinitialisation du domaine.
            for(i = 0; i < 9; i++)
            {
                var.domaine[i] = 1;
            }

            // On remet la variable au sommet de la liste.
            empiler_debut(&(csp->V), var);
            // On remet la variable parcourue au sommet de la liste.
            empiler_debut(&(csp->V), csp->A->var);
            // On enlève l'affectation actuelle de la liste.
            depiler_var(&(csp->A));
        }
    }
    
    return 1;
}
