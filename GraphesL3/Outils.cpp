#include "Outils.h"

/*
	// Fonction qui lit un fichier .col en entrée
	/// récupère le nombre d'arêtes du graphe à tester
	/// créé un tableau dynamique en deux dimensions d'entiers
	//// retourne un tableau de taille n (n = le nombre d'arêtes du graphe) listant les sommets reliant chaque arête
*/

int** lireFichier(string nameFile, int quelTableau)
{
	ifstream fichier(nameFile, ios::in);  // on ouvre le fichier en lecture
	
	int** temp = NULL;
	if (fichier)  // si l'ouverture a réussi
	{
		bool continuer = true;
		vector <string> monTableau;

		/// Fichier : début lecture

		while (!fichier.eof())
		{
			monTableau.push_back(""); //creation d'une ligne vide

			getline(fichier, monTableau.back()); //lecture d'une ligne du fichier

			int ligne = monTableau.size() - 1; //je recupere la taille du tableau (-1 pour la ligne 0)

			if ( monTableau[ligne].empty() ) //si la ligne est vide
				monTableau.pop_back(); //on la retire du tableau
		}

		fichier.close();  // on ferme le fichier

		/// Fichier : fin lecture ///

		/// Tableau d'arêtes : début traitement 

		vector<string> truncateS; // vector qui recupere les chaines decoupees
		int numLigne = 0;
		char premier_caractere;

		// --> Récupération du nombre d'arêtes du graphe : on cherche le début de ligne p et on regarde le quatrième mot
		int i = 0;
		do{
			truncateS = truncateString(monTableau.at(i), ' '); // decoupe de la chaine
			premier_caractere = truncateS.front()[0];
			i++;
		} while (premier_caractere != 'p');

		std::string::size_type sz;

		switch (quelTableau)
		{
			// On travaille sur les sommets reliant les arêtes du graphe
			case 0:
			{
				numLigne = std::stoi(truncateS.at(3), &sz);

				// --> Création du tableau des arêtes
				int **tab_aretes = new int*[numLigne]; // on initialise un tableau pouvant contenir le nombre d'arête
				numLigne = 0; // On remet le compteur de ligne à 0;

				for (i; i < (int)monTableau.size(); i++) // On caste en (int) pour éviter un warning
				{
					truncateS = truncateString(monTableau.at(i), ' ');
					tab_aretes[numLigne] = new int[2];
					tab_aretes[numLigne][0] = std::stoi(truncateS.at(1), &sz);
					tab_aretes[numLigne][1] = std::stoi(truncateS.at(2), &sz);
					numLigne++;
				}
				/*for (int x = 0; x < numLigne; x++)
				{
					cout << x << " -> Sommets : " << tab_aretes[x][0] << " et " << tab_aretes[x][1];
					cout << endl;
				}*/
				temp = tab_aretes;
			}break;
			case 1:
			{
				numLigne = std::stoi(truncateS.at(2), &sz);
				
				// Création du tableau des degrés des sommets
				int **tab_degre = new int*[numLigne];
				int sommet1 = 0;
				int sommet2 = 0;
				// ON aurait pu faire un tableau une dimension et utiliser les indices mais harmoniser les façons de procédé
				
				// On déclare un tableau de taille n (le nombre de sommets) et on initialise une case qui contiendra son degré
				/// indice du tableau = sommet - 1 
				/// on suppose que les sommets commençent à 1

				for (int j = 0; j < numLigne; j++)
				{
					tab_degre[j] = new int[1];
					tab_degre[j][0] = 0;
				}
					
				for (i; i < (int)monTableau.size(); i++)
				{
					truncateS = truncateString(monTableau.at(i), ' ');
					sommet1 = std::stoi(truncateS.at(1), &sz) - 1;
					sommet2 = std::stoi(truncateS.at(2), &sz) - 1;
					tab_degre[sommet1][0]++;
					tab_degre[sommet2][0]++;
				}
				/*for (int x = 0; x < numLigne; x++)
				{
					cout << " Sommets  " << (x+1) << " : degre( " << tab_degre[x][0] << " )";
					cout << endl;
				}*/
				temp = tab_degre;
			}break;
			case 2:
			{
				int** information = new int*[0];
				information[0] = new int[2];
				information[0][0] = std::stoi(truncateS.at(2), &sz);
				information[0][1] = std::stoi(truncateS.at(3), &sz);
				temp = information;
			}break;
			default: 
				cout << "AH AH ! Pauvre malheureux, tu t'es lourdement fourvoyé !" << endl; 	 
		}
		/*for (int i = 0 ; i < monTableau.size() ; i++)
		{
			truncateS = truncateString(monTableau.at(i), ' '); // decoupe de la chaine
			for (int j = 0 ; j < truncateS.size() ; j++)
			{
				//cout << "chaine " << j << " : " << truncateS.at(j) << '\t'; // affichage de la chaine - a modifier
			}
			cout << endl;
		} */
	}
	else  // sinon
		cerr << "Impossible d'ouvrir le fichier : " << nameFile << endl;
	return temp;
}


/*
	// Fonction qui tronque une chaîne de caractère donnée en paramètre
	// --> en fonction d'un délimateur donné
	// Retourne un vecteur
*/

vector<string> truncateString(string s, char delim)
{
	vector<string> res; // vector retourne
	string tmp = ""; // initialisation la chaine a rien

	for (char& c : s) // pour chaque caractere c de la chaine s
	{
		if (c == delim) 
		{
			res.push_back(tmp); // ajout de la chaine dans le vector res
			tmp = ""; // reinitialisation de la chaine pour la suivante
		}
		else
			tmp += c; // on ajoute le caractere a la chaine
	}

	if (tmp != "")
		res.push_back(tmp); // ajout de la chaine dans le vector res si la chaine est non vide

	return res;
}


