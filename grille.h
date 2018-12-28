#ifndef __GRILLE__
#define __GRILLE__

#define NOMBRE 0
#define TEXT_MAX 50
#define is_num(c)(('0' <= (c)) && ((c) <= '9'))

FILE* fichier;
int nbLignes;
int nbColonnes;
int id_blanc;

// Enumération des diiférentes couleurs possibles.
enum couleurs{
	BLANC = 0,
	NOIR = 1
};

// Structure d'une case blanche.
typedef struct {
	int id;
	int contrainte_horizontale;
	int contrainte_verticale;
	int valeur;
} CaseBlanche;

// Structure d'une contrainte de somme.
typedef struct {
	int valeur;
	CaseBlanche* cases_associees;
	int nbCases;
} ContrainteSomme;

// Structure d'une case.
typedef struct {
	int couleur;
	int id;
	ContrainteSomme contrainte_horizontale;
	ContrainteSomme contrainte_verticale;
} Case;

// Structure de la grille de jeu.
typedef struct {
	Case** matrice;
	CaseBlanche* cases_blanches;
	int nbCasesBlanches;
} GrilleDeJeu;

void initialiser_grille(GrilleDeJeu* jeu, char* file, char* mode);
void recuperer_taille_grille(char* file, char* mode);
void modeliser_grille(char* file, char* mode, GrilleDeJeu* jeu);
void recuperer_cases_blanches(Case** matrice, CaseBlanche* cases_blanches);
int contient_contrainte(CaseBlanche* cases_blanches, int identifiant);
void afficher_grille(GrilleDeJeu* jeu);
void creer_contrainte_somme(GrilleDeJeu* jeu);
void libererMatrice(Case **a, int m);

#endif
