#include <iostream>
#include "Outils.h"
#include "Dsatur.h"

using namespace std;

int main(int argc, char **argv)
{
	string nomFichier = "..\\FichiersCol\\queen11_11.col";
	string test = "..\\FichiersCol\\test.col";

	/* R�cup�ration des informations du graphe : le nombre d'ar�tes et le nombre de sommets */
	int** informationGraphe = lireFichier(nomFichier, 2);
		int nbAretes = informationGraphe[0][1];
		int nbSommets = informationGraphe[0][0];

	/* R�cup�ration des sommets reliant chaque ar�te du graphe */
		int** tab_aretes = lireFichier(nomFichier, 0); // appel fonction dans outils.cpp;

	/* R�cup�ration du degr� de chaque sommet du graphe */
		int** tab_degre_sommet = lireFichier(nomFichier, 1);
	//rangementDegre(tab_degre_sommet, nbSommets,1);

	coloration(tab_aretes, tab_degre_sommet, nbAretes, nbSommets,0);
	
	for (int i = 0; i < nbSommets; i++)
	{
		free(tab_aretes[i]);
		free(tab_degre_sommet[i]);
	}
		
	return 0;
}