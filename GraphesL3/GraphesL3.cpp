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
	//Création des processus

		static GraphesL3 g;
		static string nomFichier;
		static int size;
		static int **t;

		//Thread 1 -> traitement du fichier
		thread t1([]() {
			string nomFichier = "..\\FichiersCol\\queen11_11.col";
			string test1 = "..\\FichiersCol\\test.col";
			string test2 = "..\\FichiersCol\\queen11_11.col";
			string test3 = "..\\FichiersCol\\queen12_12.col";
			string test4 = "..\\FichiersCol\\test_leretour.col";

			string nomFichier = test4;
			/* Récupération des informations du graphe : le nombre d'arêtes et le nombre de sommets */
			/*int** informationGraphe = lireFichier(nomFichier, 2);
			int nbAretes = informationGraphe[0][1];
			int nbSommets = informationGraphe[0][0];
			*/
			/* Récupération des sommets reliant chaque arête du graphe */
			//int** tab_aretes = lireFichier(nomFichier, 0); // appel fonction dans outils.cpp;

			/* Récupération du degré de chaque sommet du graphe */
			int** tab_degre_sommet = lireFichier(nomFichier, 1);
			//rangementDegre(tab_degre_sommet, nbSommets,1);

			coloration(tab_aretes, tab_degre_sommet, nbAretes, nbSommets, 0);
	
			for (int i = 0; i < nbSommets; i++)
			{
				free(tab_aretes[i]);
				free(tab_degre_sommet[i]);
			}
			//int** tab_degre_sommet = lireFichier(nomFichier, 1);
			
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

//Fonction privée
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