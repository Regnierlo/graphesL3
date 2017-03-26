#include "Outils.h"

void lireFichier(string nameFile)
{
	ifstream fichier(nameFile, ios::in);  // on ouvre le fichier en lecture

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

		// --> Récupération du nombre d'arêtes du graphe
		int i = 0;
		do{
			truncateS = truncateString(monTableau.at(i), ' '); // decoupe de la chaine
			premier_caractere = truncateS.front()[0];
			i++;
		} while (premier_caractere != 'p');

		std::string::size_type sz;
		numLigne = std::stoi(truncateS.at(3), &sz);

		// --> Création du tableau des arêtes
		int **tab_aretes = new int*[numLigne]; // on initialise un tableau pouvant contenir le nombre d'arête
		numLigne = 0; // On remet le compteur de ligne à 0;
		
		for (i ; i < monTableau.size(); i++)
		{
			truncateS = truncateString(monTableau.at(i), ' ');
			tab_aretes[numLigne] = new int[2];
			tab_aretes[numLigne][0] = std::stoi(truncateS.at(1), &sz);
			tab_aretes[numLigne][1] = std::stoi(truncateS.at(2), &sz);
			numLigne++;
		}
		for (int x = 0 ; x < numLigne; x++)
		{
			cout << "Sommet 1 : " << tab_aretes[x][0] << " et sommet 2 : " << tab_aretes[x][1];
			cout << endl;
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