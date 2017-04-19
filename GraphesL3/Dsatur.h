#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int** rangementDegre(int** tabDegre, int nbSommets, int affichage);
int rechercheCouleur(int* dsat, int nb_sommets_max);
void coloration(int** tabArete, int** tabDegre, int nbAretes, int nbSommets, int affichage);
int rechercheDegre(int** tabDegreDecroissant, int sommetATrouve, int nbSommets);
void triCouleur(int* dsat_a_trier, int nbSommets);
bool couleurDejaMise(int* dsat, int couleurAVerifier, int degreMax);
int combienCouleur(int nbSommets, int* couleurSommet);
void dsat_MAJ(int couleur, int sommet_Colorie, int nbAretes, int** tabArete, int** dsat_sommet, int degreMax);