#ifndef __CSP__
#define __CSP__

#include "grille.h"

// Type pour les domaines.
typedef int domaines;

// Structure pour les variables du CSP.
typedef struct {
    CaseBlanche* case_blanche;
    domaines* domaine;
} variable;

// Structure pour une liste de variables du CSP.
typedef struct _liste {
    variable var;
    struct _liste* suivant;
} variables;

// Création des types de liste.
typedef variables* listeDeVariables;
typedef variables* affectations;

// Structure du CSP.
typedef struct {
	listeDeVariables V;
    domaines** D;
    affectations A;
} CSP;

void initialiser_csp(GrilleDeJeu jeu, CSP* csp);
void free_csp(GrilleDeJeu jeu, CSP* csp);

#endif
