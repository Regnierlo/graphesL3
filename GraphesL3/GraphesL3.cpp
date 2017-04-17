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

		//Thread 1 -> traitement du fichier
		thread t1([]() {
			string nomFichier = "..\\FichiersCol\\queen11_11.col";

			/* Récupération des informations du graphe : le nombre d'arêtes et le nombre de sommets */
			int** informationGraphe = lireFichier(nomFichier, 2);
			int nbAretes = informationGraphe[0][1];
			int nbSommets = informationGraphe[0][0];

			/* Récupération des sommets reliant chaque arête du graphe */
			int** tab_aretes = lireFichier(nomFichier, 0); // appel fonction dans outils.cpp;

			/* Récupération du degré de chaque sommet du graphe */
			int** tab_degre_sommet = lireFichier(nomFichier, 1);
			g.interdireSauvegarde();
		});

		//Thread 2 -> action utilisateur
		thread t2([](){
			g.actionUtilisateur();
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

void GraphesL3::actionUtilisateur()
{
	int nb = 1000;

	//Tant qu'on ne dit pas que l'utilisateur ne peu plus sauvegarder on accepte la sauvegarde
	while (!fin)
	{
		//Touche 'fin' pour pouvoir sauvegarder
		if (GetAsyncKeyState(VK_END) != 0)
		{
			//Action de sauvegarde (modifer le for)
			for (int i = 0; i < nb; i++)
			{
				cout << i << endl;
			}
		}
		
	}
}