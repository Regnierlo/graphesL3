#include "Outils.h"

void lireFichier(string nameFile)
{
	ifstream fichier(nameFile, ios::in);  // on ouvre le fichier en lecture

	if (fichier)  // si l'ouverture a réussi
	{
		bool continuer = true;
		vector <string> monTableau;

		while (!fichier.eof())
		{
			monTableau.push_back("");//creation d'une ligne vide

			getline(fichier, monTableau.back());//lecture d'une ligne du fichier

			int ligne = monTableau.size() - 1;//je recupere la taille du tableau (-1 pour la ligne 0)

			if (monTableau[ligne].empty())//si la ligne est vide
				monTableau.pop_back();//on la retire du tableau
		}

		fichier.close();  // on ferme le fichier

		vector<string> truncateS; // vector qui recupere les chaines decoupees

		for (int i = 0; i < monTableau.size(); i++)
		{
			truncateS = truncateString(monTableau.at(i), ' '); // decoupe de la chaine
			
			for (int j = 0; j < truncateS.size(); j++)
			{
				cout << "chaine " << j << " : " << truncateS.at(j) << '\t'; // affichage de la chaine - a modifier
			}
			cout << endl;
		}
	}
	else  // sinon
		cerr << "Impossible d'ouvrir le fichier : " << nameFile << endl;
}

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