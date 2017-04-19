#include "Dsatur.h"

/*
	Coloration DSATUR
*/
void coloration(int** tabArete, int** tabDegre, int nbAretes, int nbSommets, int affichage)
{
	// TABLEAUX

		/// Tableau 1D pour la coloration de chaque sommet, indice de la ligne = (sommet-1)
			int *couleur_sommet = new int[nbSommets];

		/// Tableau 2D pour le degré de saturation de chaque sommet (v), en première colonne dsat(v), puis les couleurs des sommets adjacents
			/// le nombre de colonnes suivant la colonne 0 = dsat(v), qui au maximum sera égal au nombre de degré max d'un sommet
			int **dsat_sommet = new int*[nbSommets];
	

	/* ETAPE 1 : ordonner les sommets par ordre décroissant de degrés */
	
		int** degres_decroissant = rangementDegre(tabDegre, nbSommets,0);

	/* ETAPE INTERMEDIAIRE : initialisation des tableaux et variables */
	
		/// Variables
			int degre_maximal = degres_decroissant[0][1];		/// Permet de connaître quel est le degré maximal du graphe

			int couleur_dernier_sommet = 1;	/// Permet de connaître la dernière couleur appliquée
			bool fin_Coloration = false;	/// Permet de savoir si la fin de la coloration a été atteinte : sert pour la boucle
			int sommet_a_Colorier = 0;		/// Sommet que l'on va peut être colorier
			int indice_a_Colorier = -1;		/// Utilisé dans le but de ne pas confondre sommet et indice, pour une meilleure visibilité du code
			int sommet_Colorie = 0;			/// Sommet que l'on vient de colorier

			int dsat_maximum = -1;		/// DSAT maximum des sommets
			int indice_1_egalite = -1;  /// Utilisé en cas d'égalité du DSAT max (étape 3)
			int indice_2_egalite = -1;  /// Utilisé en cas d'égalité du DSAT max (étape 3)
			int couleur_minimale = 1;	/// La couleur la plus petite du graphe

			int nb_restant = 0; /// Le nombre de sommets restant à colorier
			int nbCouleurs = 0; /// Le nombre de couleurs du graphe
	
		/// Tableaux
			for (int i = 0; i < nbSommets; i++)
			{
				couleur_sommet[i] = 0;

				dsat_sommet[i] = new int[(degre_maximal+1)]; /// le nombre de couleur differentes des sommets adajcents peut être au maximum égal au nombre maximal de sommets adjacents
				dsat_sommet[i][0] = 0;
				for (int j = 1 ; j < (degre_maximal+1) ; j++)
					dsat_sommet[i][j] = -1;
			}
	
	
	/* ETAPE 2 : colorer un sommet de degré maximum avec la couleur 1 */
	
		// Récupération du sommet à colorier
			sommet_a_Colorier = degres_decroissant[0][0]; // ça marche
			indice_a_Colorier = sommet_a_Colorier - 1;
		
		// Coloration du sommet 
			couleur_sommet[indice_a_Colorier] = couleur_dernier_sommet;
			sommet_Colorie = sommet_a_Colorier;

		// Mise à jour tableau DSAT
			dsat_MAJ(couleur_dernier_sommet,sommet_Colorie, nbAretes, tabArete, dsat_sommet, degre_maximal+1);

	/* ETAPES 3, 4 et 5 : en boucle tant que la coloration n'est pas finie  */

		while (!fin_Coloration)
		{
		/* ETAPE 3 : choisir un sommet non colorié avec DSAT maximum (en cas d'égalité, choisir un sommet de degré max) */
			// On choisit un sommet avec DSAT maximum
			dsat_maximum = 0;
			sommet_a_Colorier = 0;
			indice_a_Colorier = -1;
			couleur_minimale = 1;
			
			for (int i = 0; i < nbSommets; i++)
			{
				/// En premier, on récupère les sommets non coloriés
				if (couleur_sommet[i] == 0)
				{
					//// Si le sommet v est non colorié, on regarde si dsat(v) est supérieur au dsat maximum
					if (dsat_sommet[i][0] > dsat_maximum)
					{
						///// Si c'est le cas, on récupère ce dernier sommet
						dsat_maximum = dsat_sommet[i][0];
						sommet_a_Colorier = i + 1;
						indice_a_Colorier = i;
					}
					//// Sinon, on regarde si il y a une égalité
					else 
						if (dsat_sommet[i][0] == dsat_maximum && indice_a_Colorier != -1)
						{
							///// En cas d'égalité, on choisit celui avec le degré le plus grand
							indice_1_egalite = rechercheDegre(degres_decroissant, (i + 1), nbSommets);
							indice_2_egalite = rechercheDegre(degres_decroissant, (sommet_a_Colorier), nbSommets);
							if (indice_1_egalite < indice_2_egalite)
							{
								dsat_maximum = dsat_sommet[i][0];
								sommet_a_Colorier = i + 1;
								indice_a_Colorier = i;
							}
						}
				}
			} /// On a trouvé le sommet à colorier
			cout << "Sommet a colorier : " << sommet_a_Colorier << " (indice : " << indice_a_Colorier << ")" << endl;

		/* ETAPE 4 : colorier ce sommet avec la plus petite couleur possible */

			/// On cherche dans les sommets adjacents COLORIES au sommet que l'on va colorier quelle est la couleur la plus petite

				triCouleur(dsat_sommet[indice_a_Colorier], nbSommets);
				couleur_minimale = rechercheCouleur(dsat_sommet[indice_a_Colorier], degre_maximal + 1);
				cout << "La plus petite couleur possible est : " << couleur_minimale << endl;

			/// Coloration du sommet 
				couleur_sommet[indice_a_Colorier] = couleur_minimale;
				couleur_dernier_sommet = couleur_minimale;
				sommet_Colorie = sommet_a_Colorier;

			/// Mise à jour tableau DSAT
				dsat_MAJ(couleur_dernier_sommet, sommet_Colorie, nbAretes, tabArete, dsat_sommet, degre_maximal+1);
				
		/* ETAPE 5 : on vérifie si tous les sommets ont été coloriés */
			
			nb_restant = 0;
			for (int i = 0; i < nbSommets; i++)
			{
				if (couleur_sommet[i] == 0)
					nb_restant++;
			}
			if (nb_restant == 0)
				fin_Coloration = true;
			else
				cout << "Il reste " << nb_restant << " sommets a colorier" << endl;
		};

		nbCouleurs = combienCouleur(nbSommets, couleur_sommet);
		cout << endl << "La fin de la coloration a ete atteinte avec " << nbCouleurs << " couleurs differentes." << endl;

		for (int i = 0; i < nbSommets; i++)
			cout << endl << "sommet " << i+1 << " = couleur( " << couleur_sommet[i] << ")" << endl;

	/// Si AFFICHAGE TABLEAU DSATUR SELECTIONNE

	if (affichage == 1)
	{
		cout << "sommets : " << nbSommets << endl;
		for (int i = 0; i < nbSommets; i++)
		{
			cout << "sommet n" << i+1 << " a pour dsat : " << dsat_sommet[i][0] << "-> " << endl;
			for (int j = 1; j < (degre_maximal + 1); j++)
				cout << "[" << j << "=" << dsat_sommet[i][j] << "] ";
			cout << endl << endl;
		}
	}

	for (int i = 0; i < nbSommets; i++)
		free(degres_decroissant[i]);
	free(degres_decroissant);
	
}


/*
	Fonction qui permet de connaître le nombre de couleurs utilisées dans le graphe
	Retourne le nombre de couleur
	A besoin en entrées du nombre de sommets du graphe, et du tableau regroupant les sommets et leur couleur
*/
int combienCouleur(int nbSommets, int* couleurSommet)
{
	int* temp = new int[nbSommets+2];
	int nb = 1;
	temp[0] = nbSommets;
	temp[nbSommets + 1] = -1;
	for (int i = 1; i < nbSommets+1; i++)
		temp[i] = couleurSommet[i-1];
	
	/// On créé un tableau temporaire dans le même style que les lignes du tableau dsat pour réutiliser la fonction de tri
	triCouleur(temp, nbSommets+1);
	for (int i = 0; i < nbSommets-1; i++)
		if (temp[i] != temp[i + 1])
			nb++;
	
	free(temp);
	return nb;
}


/*
	Fonction qui met à jour le tableau gérant les degrés de saturation des sommets du graphe
	A besoin en entrées: 
		- de la couleur que l'on veut ajouter
		- le sommet que l'on vient de colorier
		- le nombre d'arêtes du graphe,
		- le tableau gérant les sommets reliant les arêtes 
		- le tableau gérant les degrés de saturation
		- le nombre maximal de sommets adjacents du graphe pour un sommet
	Ainsi, on trouve tous les sommets adjacents du sommet que l'on vient de colorier
		---> si ceux-ci n'ont pas déjà un sommet adjacent de même couleur on augmente leur degré de saturation et on ajoute la couleur
*/
void dsat_MAJ(int couleur, int sommet_Colorie, int nbAretes, int** tabArete, int** dsat_sommet, int degreMax)
{
	
	// Mise à jour du tableau gérant le degré de saturation
	/// On récupère grâce au tableau gérant les sommets reliant les arêtes les sommets adjacents au sommet colorié
	int inc = 1;
	int indice_sommet_dsat = 0;
	bool trouve = false;
	bool mis = false;

	for (int i = 0; i < nbAretes; i++)
	{
		indice_sommet_dsat = -1;
		if (tabArete[i][0] == sommet_Colorie || tabArete[i][1] == sommet_Colorie)
		{
			if (tabArete[i][0] == sommet_Colorie)
				indice_sommet_dsat += tabArete[i][1];
			else
				indice_sommet_dsat += tabArete[i][0];
			inc = 1;
			mis = false;
			//cout << "indice " << indice_sommet_dsat << " et sommet colorie " << sommet_Colorie << endl;
			do
			{
				if (dsat_sommet[indice_sommet_dsat][inc] == -1 && !couleurDejaMise(dsat_sommet[indice_sommet_dsat], couleur, degreMax))
				{
					dsat_sommet[indice_sommet_dsat][0]++;
					dsat_sommet[indice_sommet_dsat][inc] = couleur;
					mis = true;}
				else
					inc++;
			} while (!mis && inc < degreMax);
		}
	}
	

}


/*
	Fonction qui vérifie si un sommet à déjà un sommet adjacent de la couleur demandée
	Retourne un booléen : vrai si déjà sommet de la couleur, faux sinon
	A besoin en entrée :
		- de la ligne du tableau gérant les degrés de saturation correspondant au sommet auquel on veut vérifier les sommets adjacents
		- de la couleur que l'on veut vérifier parmi les sommets adjacents
		- du nombre de sommets adjacents max
*/

bool couleurDejaMise(int* dsat, int couleurAVerifier,  int degreMax)
{
	bool temp = false;
	int j = 0;
	do{
		if (dsat[j] == couleurAVerifier)
			temp = true;
		j++;
	} while (dsat[j] != -1 && j < degreMax && !temp);
	return temp;
}

/*
	/// Fonction qui retourne l'indice du sommet que l'on cherche dans le tableau des degrés décroissant
	/// Attention, demande en entrée le tableau des degrés décroissants, le sommet à trouver (et non l'indice) et le nombre de sommets du graphe
*/
int rechercheDegre(int** tabDegreDecroissant, int sommetATrouve, int nbSommets)
{
	int i = 0;
	bool trouve = false;
	int temp = -1;

	while (!trouve || i < nbSommets)
	{
		if (tabDegreDecroissant[i][0] == sommetATrouve)
		{
			temp = i;
			trouve = true;
		}
		i++;
	}
	//cout << "le sommet " << sommetATrouve << " a pour degre " << tabDegreDecroissant[temp][1] << " et indice " << temp << endl;
	return temp;
}

/*
	Fonction qui recherche la couleur minimale que l'on peut mettre
		-- Si un sommet adjacent à la même couleur on ne pourra pas l'utilisée
	Retourne la couleur minimale
	A besoin en entrée :
		- de la ligne du tableau gérant les degrés de saturation correspondant au sommet que l'on veut vérifier
		- du nombre de sommets adjacents maximum
*/
int rechercheCouleur(int* dsat, int nb_sommets_max)
{
	int j = 1;
	int temp = 1;
	bool trouve = false;
	while (j < nb_sommets_max && !trouve)
	{
		if ((dsat[j]+1) == dsat[j + 1])
			j++;
		else
		{
			temp = dsat[j] + 1;
			trouve = true;
		}
	}
	return temp;
}

/*
	Fonction qui permet de trier les couleurs des sommets adjacents du sommet que l'on veut vérifier dans un ordre croissant
	A besoin en entrée :
		- de la ligne du tableau gérant les degrés de saturation correspondant au sommet que l'on veut trier
		- du nombre de sommets adjacents maximum
*/
void triCouleur(int* dsat_trier, int nbSommets) /// dsat, indice sommet a trie, nb couleur max = nombre sommet max
{
	int inc = 1;
	while (dsat_trier[inc] != -1)
	{
		inc++;
	};
	int *tableau_temp = new int[inc];
	for (int i = 0; i < (inc); i++)
		tableau_temp[i] = dsat_trier[i];

	int *tableau_vide = new int[(inc-1)];
	int variable_temp = 1;
	int couleur_min = nbSommets;

	for (int i = 0; i < (inc-1); i++)
	{
		int j = 1;
		couleur_min = nbSommets;
		while (j < (inc))
		{
			if (tableau_temp[j] < couleur_min && tableau_temp[j] > 0)
			{
				variable_temp = j;
				couleur_min = tableau_temp[j];
			}
			j++;
		};
		tableau_temp[variable_temp] = -2;
		tableau_vide[i] = couleur_min;
	}

	int thefuck = 0;
	for (int i = 1; i < (inc); i++)
	{
		thefuck = tableau_vide[i - 1];
		dsat_trier[i] = thefuck;
	}

	free(tableau_temp);
	free(tableau_vide);
	
}


/*
	Fonction qui permet d'ordonner les sommets par ordre décroissant de degrés
	Retourne le tableau ordonné
	A besoin en entrée du tableau à trier, du nombre de sommets et si on veut l'afficher ou non
*/
int** rangementDegre(int** tabDegre, int nbSommets, int affichage)
{
	int** temp = new int*[nbSommets];
	bool* dejaFait = new bool[nbSommets];
	int degreMin = 0;
	int sommet = 0;
	for (int i = 0; i < nbSommets; i++)
	{
		temp[i] = new int[2];
		dejaFait[i] = false;
	}
	
	for (int i = 0; i < nbSommets; i++)
	{
		for (int j = 0; j < nbSommets; j++)
		{
			if (tabDegre[j][0] > degreMin && dejaFait[j] == false)
			{
				degreMin = tabDegre[j][0];
				sommet = j;
			}
		}
		temp[i][0] = sommet + 1; // sommet ayant le degré
		temp[i][1] = degreMin; // degré
		degreMin = 0;
		dejaFait[sommet] = true;
	} 

	/* AFFICHAGE TABLEAU */
	
	if(affichage == 1)
		for (int i = 0; i < nbSommets; i++)
			cout << "Sommet : " << temp[i][0] << " a pour degre ( " << temp[i][1] << " )" << endl;
	
	return temp;

}