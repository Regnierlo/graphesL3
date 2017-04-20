#include <iostream>
#include <stdio.h>
#include <thread>
#include <Windows.h>
#include "Outils.h"
#include "Dsatur.h"
#include "GraphesL3.h"

using namespace std;


int main(int argc, char **argv)
{
	//Cr�ation des processus

		static GraphesL3 g;
		static string nomFichier;
		static int size; //nombre de sommets
		static int **t; //tableau des colorations

		//Thread 1 -> traitement du fichier
		thread t1([]() {
			//Interdit la sauvegarde tant que la coloration n'a pas commenc�
			g.interdireSauvegarde();

			string test1 = "..\\FichiersCol\\test.col";
			string test2 = "..\\FichiersCol\\queen11_11.col";
			string test3 = "..\\FichiersCol\\queen12_12.col";
			string test4 = "..\\FichiersCol\\test_leretour.col";
			string test5 = "..\\FichiersCol\\queen14_14.col";

			string nomFichier = test5;

			/* R�cup�ration des informations du graphe : le nombre d'ar�tes et le nombre de sommets */
				int** informationGraphe = lireFichier(nomFichier, 2);
				int nbAretes = informationGraphe[0][1];
				int nbSommets = informationGraphe[0][0];
				int nbCouleurs;
				size = nbSommets;


		/* R�cup�ration des sommets reliant chaque ar�te du graphe */
		int** tab_aretes = lireFichier(nomFichier, 0); // appel fonction dans outils.cpp;

		/* R�cup�ration du degr� de chaque sommet du graphe */
		int** tab_degre_sommet = lireFichier(nomFichier, 1);
	

		/* Coloration DSATUR */
			int* coloration_sommet = coloration(tab_aretes, tab_degre_sommet, nbAretes, nbSommets, 0);
		
			nbCouleurs = combienCouleur(nbSommets, coloration_sommet);
			t = new int*[size];

		/* Pr�paration permutations et optimisation coloration */
			int** sommets_ordonnes;
			int** sommets_ordre_temp;
			int** tab_permutations;

			// Premi�re �tape : on ordonne pour la premi�re fois les sommets en fonction de la couelur
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

			
			ordonnancement(nbSommets, coloration_sommet, sommets_ordonnes);

			for (int i = 0; i < nbSommets; i++)
			{
				t[i][0] = sommets_ordonnes[i][0];
				t[i][1] = sommets_ordonnes[i][1];
			}

			tab_permutations = lister_permutation(nbSommets, t);
		//	cout << endl;

		//	cout << " -On ordonne pour une premiere fois le tableau des sommets et de leur couleur- " << endl;
			//affichage(nbSommets, t);
	
		//	cout << endl;
			//Autorise la coloration
			g.autoriserSauvegarde();

			int** voisinnages = creerVoisinages(tab_aretes, nbAretes, nbSommets, nbSommets);
		
			permutation(nbSommets, nbCouleurs, sommets_ordonnes, sommets_ordre_temp, t, tab_permutations, voisinnages);

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
			}*/


			g.interdireSauvegarde();
		});

		//Thread 2 -> action utilisateur
		thread t2([](){
			g.sauvegardeDemandee(t, size, nomFichier);
		});

		t1.join();
		t2.join();

	return 0;
}

//Fonction priv�e
void GraphesL3::autoriserSauvegarde()
{
	fin = false;
}

void GraphesL3::interdireSauvegarde()
{
	fin = true;
}

void GraphesL3::sauvegardeDemandee(int **t, int size, string nomFichier)
{
	//Tant qu'on ne dit pas que l'utilisateur ne peu plus sauvegarder on accepte la sauvegarde
	while (!fin)
	{
		//Touche 'fin' pour pouvoir sauvegarder
		if (GetAsyncKeyState(VK_END) != 0)
		{
				//Action de sauvegarde
					sauvegarderFichier(t,size,nomFichier);
		}
		
	}
}