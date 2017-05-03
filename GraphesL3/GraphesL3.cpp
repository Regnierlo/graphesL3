#include <iostream>
#include <stdio.h>
#include <thread>
#include <Windows.h>
#include "Outils.h"
#include "Dsatur.h"
#include "GraphesL3.h"

using namespace std;

static GraphesL3 g;
static string nomFichier;
static int nbSommets; //nombre de sommets
static int **t; //tableau des colorations

int main(int argc, char **argv)
{
	//Création des processus

		//Thread 2 -> action utilisateur
		thread t2([](){
			g.sauvegardeDemandee(/*t, size, nomFichier*/);
		});


		//Thread 1 -> traitement du fichier
		thread t1([]() {

			// Interdit la sauvegarde tant que la coloration n'a pas commencé
			g.interdireSauvegarde();

			string test1 = "..\\FichiersCol\\test.col";
			string test2 = "..\\FichiersCol\\queen11_11.col"; // INTERNET : 11, DSATUR : 15, PAS D'AMELIORATION POSSIBLE
			string test3 = "..\\FichiersCol\\queen12_12.col "; // INTERNET : ??, DSATUR 17, AMELIORATION DE 16
			string test4 = "..\\FichiersCol\\test_leretour.col";
			string test5 = "..\\FichiersCol\\queen14_14.col"; // INTERNET : ??, DSATUR 20, AMELIORATION DE 19
			string test6 = "..\\FichiersCol\\anna.col"; // INTERNET : 11, DSATUR : 11, PAS D'AMELIORATION POSSIBLE
			string test7 = "..\\FichiersCol\\queen13_13.col"; // INTERNET : 13, DSATUR : 19, AMELIORATION POSSIBLE de 18
			string test8 = "..\\FichiersCol\\le450_25a.col"; // INTERNET : 25, DSATUR : 25, PAS D'AMELIORATION POSSIBLE

			nomFichier = test7;

			/* Récupération des informations du graphe : le nombre d'arêtes et le nombre de sommets */
				int** informationGraphe = lireFichier(nomFichier, 2);
				int nbAretes = informationGraphe[0][1];
				nbSommets = informationGraphe[0][0];
				int nbCouleurs;

				cout << "Le graphe choisi comporte " << nbAretes << " aretes pour " << nbSommets << " sommets." << endl << endl;

		/* Récupération des sommets reliant chaque arête du graphe */
		int** tab_aretes = lireFichier(nomFichier, 0); // appel fonction dans outils.cpp;

		/* Récupération du degré de chaque sommet du graphe */
				cout << "Premiere etape, lecture du fichier : on recupere un tableau pour le degre de chaque sommet" << endl;
		int** tab_degre_sommet = lireFichier(nomFichier, 1);
	

		/* Coloration DSATUR */
				cout << "Puis, on colore les sommets grace a une coloration DSATUR" << endl;
			int* coloration_sommet = coloration(tab_aretes, tab_degre_sommet, nbAretes, nbSommets, 0);
		

			nbCouleurs = combienCouleur(nbSommets, coloration_sommet);
				cout << "---> DSATUR a trouvé une coloration en " << nbCouleurs << " couleurs." << endl;
				
				t = new int*[nbSommets];

		/* Préparation permutations et optimisation coloration */
			int** sommets_ordonnes;
			int** sommets_ordre_temp;
			int** tab_permutations;
				int** voisinnages = creerVoisinages(tab_aretes, nbAretes, nbSommets, nbSommets);

			// Première étape : on ordonne pour la première fois les sommets en fonction de la couleur
			sommets_ordonnes = new int*[nbSommets];
			sommets_ordre_temp = new int*[nbSommets];
				
			for (int i = 0; i < nbSommets; i++)
			{
				t[i] = new int[2];
				sommets_ordonnes[i] = new int[2];
				sommets_ordonnes[i][0] = i;
				sommets_ordonnes[i][1] = coloration_sommet[i];

				sommets_ordre_temp[i] = new int[2];
				sommets_ordre_temp[i][0] = 0;
				sommets_ordre_temp[i][1] = -1;
			}

				cout << endl << "On ordonne les sommets en fonction de leur couleur (ordre croissant)" << endl;
			ordonnancement(nbSommets, coloration_sommet, sommets_ordonnes);

			for (int i = 0; i < nbSommets; i++)
			{
				t[i][0] = sommets_ordonnes[i][0];
				t[i][1] = sommets_ordonnes[i][1];
			}

				cout << endl << "On liste les permutations possibles pour graphe en fonction du nombre de couleurs" << endl;
			tab_permutations = lister_permutation(nbSommets, t);

	
			//Autorise la coloration
			g.autoriserSauvegarde();

				cout << endl << "Puis on lance la permutation et l'optimisation des couleurs " << endl;
				int optimisation = permutation(nbSommets, nbCouleurs, sommets_ordonnes, sommets_ordre_temp, t, tab_permutations, voisinnages);
		
				if (optimisation == nbCouleurs)
					cout << endl << "----> L'algorithme de glouton itere n'a pas trouve une meilleure optmisation que DSATUR" << endl;

			for (int i = 0; i < nbSommets; i++)
			{
				free(tab_aretes[i]);
				free(tab_degre_sommet[i]);
			}
			free(tab_aretes);
			free(tab_degre_sommet);
			
			//test de sauvegarde
			/*for (int i = 0; i < 10000; i++)
			{
				cout << "Thread 1 : " << i << endl;
				}
				*/

			g.interdireSauvegarde();
			g.finProg();
		});


		t2.join();
		t1.join();

	return 0;
}

//Fonction privée
void GraphesL3::autoriserSauvegarde()
{
	peutSauvegarder = true;
}

void GraphesL3::interdireSauvegarde()
{
	peutSauvegarder = false;
}

void GraphesL3::finProg()
{
	fin = true;
}

void GraphesL3::sauvegardeDemandee(/*int **t, int size, string nomFichier*/)
{
	//Tant qu'on ne dit pas que l'utilisateur ne peu plus sauvegarder on accepte la sauvegarde
	while (!fin)
	{
		while (peutSauvegarder)
		{
			//cout << "while save" << endl;
		//Touche 'fin' pour pouvoir sauvegarder
		if (GetAsyncKeyState(VK_END) != 0)
		{
				//Action de sauvegarde
				cout << "save" << endl;
				sauvegarderFichier(t, nbSommets, nomFichier);
			}
		}
	}
}