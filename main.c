#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grille.h"
#include "csp.h"
#include "backtrack.h"

char *nom_fichier = "grilles/enonce.txt";
char *mode_fichier = "r+";

int main(int argc, char const *argv[])
{
	// 1) Déclaration de la grille de jeu et mise des valeurs par défaut.
	GrilleDeJeu jeu;
	jeu.matrice = NULL;
	jeu.cases_blanches = NULL;
	jeu.nbCasesBlanches = 0;

	// 2) Initialisation de la grille de jeu.
	initialiser_grille(&jeu, nom_fichier, mode_fichier);

	// 3) Modélisation des cases de la grille de jeu.
	modeliser_grille(nom_fichier, mode_fichier, &jeu);

	// 4) Affichage de la grille sous la forme du fichier source.
	afficher_grille(&jeu);

	// 5) Déclaration du CSP
	CSP csp;
	int resolu = 0;

	// 6) Initialisation du CSP
	initialiser_csp(jeu, &csp);

	// 7) Résolution de la grille de Kakuro avec Backtrack.
	resolu = backtrack(jeu, &csp);

	if (resolu == 1) {
		printf("\nSolution trouvée :\n\n");
	} else {
		printf("\nAucune solution trouvée :\n\n");
	}

	// 8) Affichage de la solution.
	afficher_grille(&jeu);

	// 9) Libération de la mémoire.
	free_csp(jeu, &csp);
	libererMatrice(jeu.matrice, nbLignes);
	free(jeu.cases_blanches);

	printf("Programme terminé.\n\n");

	return 0;
}
