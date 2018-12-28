#ifndef __UTIL__
#define __UTIL__

#include "csp.h"

int longTexte;
char texte[TEXT_MAX];

char lireCar(void);
void delireCar();
int lire(void);
void empiler_debut(listeDeVariables* liste, variable var);
void empiler_fin(listeDeVariables* liste, variable var);
variable depiler_var(listeDeVariables* liste);
int getTaille(listeDeVariables liste);
int domaineVide(domaines* d);
int valeur_consistante(GrilleDeJeu jeu, variable var, int val);
int absent_sur_ligne(GrilleDeJeu jeu, variable var, int val);
int absent_sur_colonne(GrilleDeJeu jeu, variable var, int val);
int calculer_somme(GrilleDeJeu* jeu, variable var, int val);
void mettreAJourContraintesSomme(variable var, int val, GrilleDeJeu* jeu);

#endif
