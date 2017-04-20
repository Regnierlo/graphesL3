#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int** rangementDegre(int** tabDegre, int nbSommets, int affichage);
int rechercheCouleur(int* dsat, int nb_sommets_max);
int* coloration(int** tabArete, int** tabDegre, int nbAretes, int nbSommets, int affichage);
int rechercheDegre(int** tabDegreDecroissant, int sommetATrouve, int nbSommets);
void triCouleur(int* dsat_a_trier, int nbSommets);
bool couleurDejaMise(int* dsat, int couleurAVerifier, int degreMax);
int combienCouleur(int nbSommets, int* couleurSommet);

void glouton(int* couleur, int nbSommets, int** sommets_ordonnes, int** sommets_ordre_temp);
void ordonnancement(int nbSommets, int** sommets_ordonnes, int** sommets_ordre_temp);
void permutation(int nbSommets, int nbCouleurs_ancien, int** sommets_ordonnes, int** sommets_ordre_temp, int** ordre_optimise, int** tab_permutations);


void dsat_MAJ(int couleur, int sommet_Colorie, int nbAretes, int** tabArete, int** dsat_sommet, int degreMax, int** tabDegre);

void affichage(int nb, int** tableau);

int** lister_permutation(int nbSommets, int** sommets_ordonnes);