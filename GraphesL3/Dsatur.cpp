#include "Dsatur.h"

/************************************************************************************************************************************************************************************
COLORATION
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
En entr�e : 
	- un tableau 2D pour les ar�tes
	- un tableau 2D pour le degr� de chaque sommet
	- le nombre d'ar�tes
	- le nombre de sommet
	- un entier pour si on veut afficher le tableau des couleurs (=2) ou le tableau de DSAT (=1)
*************************************************************************************************************************************************************************************/

int* coloration(int** tabArete, int** tabDegre, int nbAretes, int nbSommets, int affichage)
{
	/* TABLEAUX */

		/// Tableau 1D pour la coloration de chaque sommet, indice de la ligne = (sommet-1)
			int* couleur_sommet = new int[nbSommets];

		/// Tableau 2D pour le degr� de saturation de chaque sommet (v), en premi�re colonne dsat(v), puis les couleurs des sommets adjacents
			/// le nombre de colonnes suivant la colonne 0 = dsat(v), qui au maximum sera �gal au nombre de degr� max d'un sommet
			int **dsat_sommet = new int*[nbSommets];
	
	/* ETAPE 1 : ordonner les sommets par ordre d�croissant de degr�s */	
		int** degres_decroissant = rangementDegre(tabDegre, nbSommets,0);

	/* ETAPE INTERMEDIAIRE : initialisation des tableaux et variables */
		/// Variables
			int degre_maximal = degres_decroissant[0][1];		/// Permet de conna�tre quel est le degr� maximal du graphe

			int couleur_dernier_sommet = 1;	/// Permet de conna�tre la derni�re couleur appliqu�e
			bool fin_Coloration = false;	/// Permet de savoir si la fin de la coloration a �t� atteinte : sert pour la boucle
			int sommet_a_Colorier = 0;		/// Sommet que l'on va peut �tre colorier
			int indice_a_Colorier = -1;		/// Utilis� dans le but de ne pas confondre sommet et indice, pour une meilleure visibilit� du code
			int sommet_Colorie = 0;			/// Sommet que l'on vient de colorier

			int dsat_maximum = -1;		/// DSAT maximum des sommets
			int indice_1_egalite = -1;  /// Utilis� en cas d'�galit� du DSAT max (�tape 3)
			int indice_2_egalite = -1;  /// Utilis� en cas d'�galit� du DSAT max (�tape 3)
			int couleur_minimale = 1;	/// La couleur la plus petite du graphe

			int nb_restant = 0; /// Le nombre de sommets restant � colorier
			int nbCouleurs = 0; /// Le nombre de couleurs du graphe
	
		/// Tableaux
			for (int i = 0; i < nbSommets; i++)
			{
				couleur_sommet[i] = 0;

				dsat_sommet[i] = new int[(degre_maximal+1)]; /// le nombre de couleur differentes des sommets adajcents peut �tre au maximum �gal au nombre maximal de sommets adjacents
				dsat_sommet[i][0] = tabDegre[i][0];
				for (int j = 1 ; j < (degre_maximal+1) ; j++)
					dsat_sommet[i][j] = -1;
			}
	
	/* ETAPE 2 : colorer un sommet de degr� maximum avec la couleur 1 */
	
		// R�cup�ration du sommet � colorier
			sommet_a_Colorier = degres_decroissant[0][0]; // �a marche
			//cout << "sommet avec degre max " << sommet_a_Colorier << " avec degre = " << degres_decroissant[0][1] << endl;
			indice_a_Colorier = sommet_a_Colorier - 1;
		
		// Coloration du sommet 
			couleur_sommet[indice_a_Colorier] = couleur_dernier_sommet;
			sommet_Colorie = sommet_a_Colorier;

			//cout << "On colorie en premier le sommet " << sommet_Colorie << endl;

		// Mise � jour tableau DSAT
			dsat_MAJ(couleur_dernier_sommet,sommet_Colorie, nbAretes, tabArete, dsat_sommet, degre_maximal+1, tabDegre);
		

	/* ETAPES 3, 4 et 5 : en boucle tant que la coloration n'est pas finie */

		while (!fin_Coloration)
		{
		/* ETAPE 3 : choisir un sommet non colori� avec DSAT maximum (en cas d'�galit�, choisir un sommet de degr� max) */
			// On choisit un sommet avec DSAT maximum
			dsat_maximum = 0;
			sommet_a_Colorier = 0;
			indice_a_Colorier = -1;
			couleur_minimale = 1;
			
			for (int i = 0; i < nbSommets; i++)
			{
				/// En premier, on r�cup�re les sommets non colori�s
				if (couleur_sommet[i] == 0)
				{
					//// Si le sommet v est non colori�, on regarde si dsat(v) est sup�rieur au dsat maximum
					if (dsat_sommet[i][0] > dsat_maximum)
					{
						///// Si c'est le cas, on r�cup�re ce dernier sommet
						dsat_maximum = dsat_sommet[i][0];
						sommet_a_Colorier = i + 1;
						indice_a_Colorier = i;
					}
					//// Sinon, on regarde si il y a une �galit�
					else 
						if (dsat_sommet[i][0] == dsat_maximum && indice_a_Colorier != -1)
						{
							///// En cas d'�galit�, on choisit celui avec le degr� le plus grand
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
			} /// On a trouv� le sommet � colorier
		//	cout << "Sommet a colorier : " << sommet_a_Colorier << " (indice : " << indice_a_Colorier << ")" << endl;
		
		/* ETAPE 4 : colorier ce sommet avec la plus petite couleur possible */

			/// On cherche dans les sommets adjacents COLORIES au sommet que l'on va colorier quelle est la couleur la plus petite
			
				triCouleur(dsat_sommet[indice_a_Colorier], nbSommets);
				
				couleur_minimale = rechercheCouleur(dsat_sommet[indice_a_Colorier], degre_maximal + 1);
			//	cout << "La plus petite couleur possible est : " << couleur_minimale << endl;

			/// Coloration du sommet 
				couleur_sommet[indice_a_Colorier] = couleur_minimale;
				couleur_dernier_sommet = couleur_minimale;
				sommet_Colorie = sommet_a_Colorier;

			/// Mise � jour tableau DSAT
				dsat_MAJ(couleur_dernier_sommet, sommet_Colorie, nbAretes, tabArete, dsat_sommet, degre_maximal+1, tabDegre);
		
		/* ETAPE 5 : on v�rifie si tous les sommets ont �t� colori�s */
			
			nb_restant = 0;
			for (int i = 0; i < nbSommets; i++)
			{
				if (couleur_sommet[i] == 0)
					nb_restant++;
			}
			if (nb_restant == 0)
				fin_Coloration = true;
		//	else
			//	cout << "Il reste " << nb_restant << " sommets a colorier" << endl;
			
		};

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
	if (affichage == 2)
		for (int i = 0; i < nbSommets; i++)
			cout << endl << "sommet " << i + 1 << " = couleur( " << couleur_sommet[i] << " )" << endl;

	for (int i = 0; i < nbSommets; i++)
	{
		free(degres_decroissant[i]);
		dsat_sommet[i];
	}	
	free(degres_decroissant);
	free(dsat_sommet);
	
	return couleur_sommet;
}


/*
	Fonction qui permet de conna�tre le nombre de couleurs utilis�es dans le graphe
	Retourne le nombre de couleur
	A besoin en entr�es du nombre de sommets du graphe, et du tableau regroupant les sommets et leur couleur
*/
int combienCouleur(int nbSommets, int* couleurSommet)
{
	int nb = 1;
/*	int* temp = new int[nbSommets+2];
	int nb = 1;
	temp[0] = nbSommets;
	temp[nbSommets + 1] = -1;
	for (int i = 1; i < nbSommets+1; i++)
		temp[i] = couleurSommet[i-1];
	
	/// On cr�� un tableau temporaire dans le m�me style que les lignes du tableau dsat pour r�utiliser la fonction de tri
	triCouleur(temp, nbSommets+1);
	for (int i = 0; i < nbSommets-1; i++)
		if (temp[i] != temp[i + 1])
			nb++;
	
	free(temp);*/
	for (int i = 0; i < nbSommets ; i++)
		if (couleurSommet[i] > nb)
			nb = couleurSommet[i];
	return nb;
}

/*
	Fonction qui met � jour le tableau g�rant les degr�s de saturation des sommets du graphe
	A besoin en entr�es: 
		- de la couleur que l'on veut ajouter
		- le sommet que l'on vient de colorier
		- le nombre d'ar�tes du graphe,
		- le tableau g�rant les sommets reliant les ar�tes 
		- le tableau g�rant les degr�s de saturation
		- le nombre maximal de sommets adjacents du graphe pour un sommet
	Ainsi, on trouve tous les sommets adjacents du sommet que l'on vient de colorier
		---> si ceux-ci n'ont pas d�j� un sommet adjacent de m�me couleur on augmente leur degr� de saturation et on ajoute la couleur
*/
void dsat_MAJ(int couleur, int sommet_Colorie, int nbAretes, int** tabArete, int** dsat_sommet, int degreMax, int** tabDegre)
{
	// Mise � jour du tableau g�rant le degr� de saturation
	/// On r�cup�re gr�ce au tableau g�rant les sommets reliant les ar�tes les sommets adjacents au sommet colori�
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
		//	cout << "sommet adjacent " << indice_sommet_dsat+1 << " et sommet colorie " << sommet_Colorie << endl;
			do
			{
				if (dsat_sommet[indice_sommet_dsat][inc] == -1 && !couleurDejaMise(dsat_sommet[indice_sommet_dsat], couleur, degreMax))
				{
					dsat_sommet[indice_sommet_dsat][0]++;
					if (dsat_sommet[indice_sommet_dsat][0] > tabDegre[indice_sommet_dsat][0])
						dsat_sommet[indice_sommet_dsat][0] = 1;
					dsat_sommet[indice_sommet_dsat][inc] = couleur;
					mis = true;}
				else
					inc++;
			} while (!mis && inc < degreMax);
		}
	}
}

/*
	Fonction qui v�rifie si un sommet � d�j� un sommet adjacent de la couleur demand�e
	Retourne un bool�en : vrai si d�j� sommet de la couleur, faux sinon
	A besoin en entr�e :
		- de la ligne du tableau g�rant les degr�s de saturation correspondant au sommet auquel on veut v�rifier les sommets adjacents
		- de la couleur que l'on veut v�rifier parmi les sommets adjacents
		- du nombre de sommets adjacents max
*/
bool couleurDejaMise(int* dsat, int couleurAVerifier,  int degreMax)
{
	bool temp = false;
	int j = 1;
	do{
		if (dsat[j] == couleurAVerifier)
		{
			temp = true;
		}
			
		j++;
	} while (dsat[j] != -1 && j < degreMax && !temp);
	return temp;
}

/*
	/// Fonction qui retourne l'indice du sommet que l'on cherche dans le tableau des degr�s d�croissant
	/// Attention, demande en entr�e le tableau des degr�s d�croissants, le sommet � trouver (et non l'indice) et le nombre de sommets du graphe
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
		-- Si un sommet adjacent � la m�me couleur on ne pourra pas l'utilis�e
	Retourne la couleur minimale
	A besoin en entr�e :
		- de la ligne du tableau g�rant les degr�s de saturation correspondant au sommet que l'on veut v�rifier
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
			if (temp == 0)
				temp++;
			trouve = true;
		}
	}
	return temp;
}

/*
	Fonction qui permet de trier les couleurs des sommets adjacents du sommet que l'on veut v�rifier dans un ordre croissant
	A besoin en entr�e :
		- de la ligne du tableau g�rant les degr�s de saturation correspondant au sommet que l'on veut trier
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

	//cout << endl;
	free(tableau_temp);
	free(tableau_vide);
	
}


/*
	Fonction qui permet d'ordonner les sommets par ordre d�croissant de degr�s
	Retourne le tableau ordonn�
	A besoin en entr�e du tableau � trier, du nombre de sommets et si on veut l'afficher ou non
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
		temp[i][0] = sommet + 1; // sommet ayant le degr�
		temp[i][1] = degreMin; // degr�
		degreMin = 0;
		dejaFait[sommet] = true;
	} 

	/* AFFICHAGE TABLEAU */
	
	if(affichage == 1)
		for (int i = 0; i < nbSommets; i++)
			cout << "Sommet : " << temp[i][0] << " a pour degre ( " << temp[i][1] << " )" << endl;
	
	return temp;

}




/************************************************************************************************************************************************************************************
GLOUTON
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int** lister_permutation(int nbSommets, int** sommets_ordonnes)
	1) r�cup�re dans une premier temps chaque couleur des sommets ordonn�es et les met dans une tableau 1D
	2) puis on r�cup�re le nombre de couleurs diff�rentes dans ce tableau gr�ce � la fonction combienCouleur
	3) Le cas particulier est s'il y a deux couleurs : il n'y aura qu'une permutation 1 <-> 2
		-> sinon, le nombre de permutations est �gale � la somme des nombres inf�rieures au nb de couleurs, genre si nbCouleurs = 5 alors nbPermutations = 4 + 3 + 2 + 1
	4) Ensuite, gr�ce � des boucles for on calcule toutes les permutations possibles pour ce nombre de couleurs, par exemple si nous avons quatre couleurs,
		la liste sera : 1-2, 1-3, 1-4, 2-3, 2-4, 3-4

void ordonnancement(int nbSommets, int* couleur, int** sommets_ordre_temp)
	permet de ranger les sommets en fonction de leur couleur (ordre croissant) pour la premi�re �tape de GLOUTON
	-> comme on utilise des pointeurs ne renvoie rien et range directement sur le tableau mis en param�tre

void triTableauPermutation(int nbSommets, int** tableau, int** resultat)
	= Une fois les deux couleurs permut�es, on range les sommets en fonction de leur couleur (ordre croissant) du tableau  mis en param�tre 
		et on retourne le r�sultat dans un tableau diff�rent


int** creerVoisinages(int** tabAretes, int nbAretes, int nbSommets, int degreMax)
	= Comme nous ne voulions pas utiliser le tableau DSAT cr�� auparavant mais une autre m�thode, nous avons utulis� une sorte de matrice d'adjacence
	-> en reprenant le tableau des ar�tes, nous mettons dans un tableau 2D les sommets voisins de chaque sommet
	

int couleurOptimisee(int* voisinnage, int** sommets_ordre_temp, int sommet, int nbSommets)
	= on cherche la couleur la plus petite possible que peut prendre le sommet, en regardant dans son voisinage quelles couleurs sont d�j� utilis�es
	1) on r�cup�re dans un tableau temporaire toutes les couleurs utilis�es dans le voisinage du sommet de fa�on ordonn�e
	2) si la premi�re couleur de ce tableau (c'est � dire la plus petite de toutes les couleurs du voisinnage) est sup�rieure � 1 alors la couleur optimale sera = 1
	3) sinon, on parcourt le tableau. Si la (couleur d'une case +1) n'est pas �gale � la suivante cela veut dire qu'il y a un trou dans la suite de nombre :
	-> la couleur optimale sera ce trou


int** optimisation(int nbSommets, int** temp_ordonnes, int** voisinnage)
	1) on cr�� une copie du tableau ordonn� de sommets mis en param�tre
	2) pour chaque sommet de cette copie du tableau on r�cup�re sa couleur optimis�e gr�ce � la fonction couleurOptimis�e qui prend en param�tre le voisinnage de ce sommet
	3) si la couleur optimis�e est �gale � la couleur du sommet le sommet est OK
	-> sinon on remplace sa couleur par la couleur optimis�e
	
void permutation(int nbSommets, int nbCouleurs_ancien, int** sommets_ordonnes, int** sommets_ordre_temp, int** ordre_optimise, int** tab_permutations, int** voisinnage)
	2) dans un premier temps on r�cup�re les couleurs � permuter gr�ce au tableau de permutations calcul� pr�c�demment
	3) on regarde dans le tableau des sommets ordonn�s quel sommet � une des deux couleurs et on la permute
	4) on tri ce nouveau tableau gr�ce � triTableauPermutation qui renvoie ce nouveau tableau rang� de fa�on croissante
	5) puis on lance la fonction d'optimisation pour ce tableau ordonn�
	6) on r�cup�re dans un tableau 1D la couleur de chaque sommet et on compte combien il y en a
	7) s'il y en a moins qu'avant, la colirsation a �t� optimis�e
	
*************************************************************************************************************************************************************************************/
int** lister_permutation(int nbSommets, int** sommets_ordonnes)
{
	int* couleurs = new int[nbSommets];
	for (int i = 0; i < nbSommets; i++)
		couleurs[i] = sommets_ordonnes[i][1];

	int nbCouleurs = combienCouleur(nbSommets, couleurs);
//	cout << "nb couleur" << nbCouleurs;
	int nbPermutations = 0;
	if (nbCouleurs == 2)
		nbPermutations = 1;
	else
	for (int i = (nbCouleurs - 1); i > 0; i--)
		nbPermutations += i;

//	cout << " - Le nombre de permutation sera " << nbPermutations << endl;
	int** tab_permutations = new int*[nbPermutations];

	for (int i = 0; i < nbPermutations; i++)
		tab_permutations[i] = new int[2];

	int variable_temp = 0;
	int variable_boucle = 0;
	int j = 0;
	for (int i = 0; i < nbCouleurs; i++)
	{
		variable_temp += (nbCouleurs - (i + 1));
		j = i + 2;
		/// On aura (nbCouleurs-1) boucles en tout
		for (variable_boucle; variable_boucle < variable_temp; variable_boucle++)
		{
			tab_permutations[variable_boucle][0] = i + 1;
			tab_permutations[variable_boucle][1] = j;
			j++;
		}
		variable_boucle = variable_temp;
	}
	//for (int i = 0; i < nbPermutations; i++)
		//cout << tab_permutations[i][0] << 
	//affichage(nbPermutations, tab_permutations);
	free(couleurs);
	return tab_permutations;
}

void ordonnancement(int nbSommets, int* couleur, int** sommets_ordre_temp)
{
	bool* dejaFait = new bool[nbSommets];
	int couleurMin = nbSommets;
	int indice = 0;
	for (int i = 0; i < nbSommets; i++)
		dejaFait[i] = false;
	
	for (int i = 0; i < nbSommets; i++)
	{
		for (int j = 0; j < nbSommets; j++)
		if (couleur[j] < couleurMin && !dejaFait[j])
		{
			couleurMin = couleur[j];
			indice = j;
		}
		sommets_ordre_temp[i][0] = indice + 1;
		sommets_ordre_temp[i][1] = couleurMin;
		dejaFait[indice] = true;
		couleurMin = nbSommets;
	}
	free(dejaFait);
}

void triTableauPermutation(int nbSommets, int** tableau, int** resultat)
{
	bool* dejaFait = new bool[nbSommets];
	int couleurMin = nbSommets;
	int indice = 0;
	int sommet = 0;
	for (int i = 0; i < nbSommets; i++)
		dejaFait[i] = false;

	for (int i = 0; i < nbSommets; i++)
	{
		for (int j = 0; j < nbSommets; j++)
		if (tableau[j][1] < couleurMin && !dejaFait[j])
		{
			couleurMin = tableau[j][1];
			sommet = tableau[j][0];
			indice = j;
		}
		resultat[i][0] = sommet;
		resultat[i][1] = couleurMin;
		dejaFait[indice] = true;
		couleurMin = nbSommets;
	}
	free(dejaFait);
}

int permutation(int nbSommets, int nbCouleurs_ancien, int** sommets_ordonnes, int** sommets_ordre_temp, int** ordre_optimise, int** tab_permutations, int** voisinnage)
{
	int couleur_a_permuter_1;
	int couleur_a_permuter_2;
	int nbCouleurs_nouveau = nbCouleurs_ancien;
	int nbPermutations = 0;
	if (nbCouleurs_ancien == 2)
		nbPermutations = 1;
	else
		for (int i = (nbCouleurs_ancien - 1); i > 0; i--)
			nbPermutations += i;

	int** temp_ordonnes = new int*[nbSommets];
	for (int i = 0; i < nbSommets; i++)
		temp_ordonnes[i] = new int[2];

	for (int permutation = 0; permutation < nbPermutations; permutation++)
	{
		couleur_a_permuter_1 = tab_permutations[permutation][0];	//cout << "couleur a permuter " << couleur_a_permuter_1 << " et ";
		couleur_a_permuter_2 = tab_permutations[permutation][1];	//cout << "couleur a permuter " << tab_permutations[permutation][1] << endl;
	
		for (int j = 0; j < nbSommets; j++)
		{
			sommets_ordre_temp[j][0] = sommets_ordonnes[j][0];
			if (sommets_ordonnes[j][1] == couleur_a_permuter_1)
				sommets_ordre_temp[j][1] = couleur_a_permuter_2;

			else
				if (sommets_ordonnes[j][1] == couleur_a_permuter_2)
					sommets_ordre_temp[j][1] = couleur_a_permuter_1;
				else
					sommets_ordre_temp[j][1] = sommets_ordonnes[j][1];
		}
		triTableauPermutation(nbSommets, sommets_ordre_temp, temp_ordonnes);
		
		int ** tableau_optimise;
		
		tableau_optimise = optimisation(nbSommets, temp_ordonnes, voisinnage);
		
		/* TEST SI MEILLEURES OPTIMISATION */

		int* couleurs = new int[nbSommets];
		int tentative_desesperee_de_comprendre = 0;
		for (int i = 0; i < nbSommets; i++)
		{
			tentative_desesperee_de_comprendre = tableau_optimise[i][1];
			couleurs[i] = tentative_desesperee_de_comprendre;
		}
	//	for (int i = 0; i < nbSommets; i++)
		//	cout << couleurs[i] << endl;

		cout << "Tentative d'optimiser n " << permutation << " = new(" << combienCouleur(nbSommets, couleurs) << ") vs old(" << nbCouleurs_ancien << ")" << endl;
		if (combienCouleur(nbSommets, couleurs) < nbCouleurs_ancien)
		{
			cout << "Optimisation possible avec ";
			nbCouleurs_nouveau = combienCouleur(nbSommets, couleurs);

			cout << nbCouleurs_nouveau << " au lieu de " << nbCouleurs_ancien << endl;
			for (int fin = 0; fin < nbSommets; fin++)
			{
				ordre_optimise[fin][0] = tableau_optimise[fin][0];
				ordre_optimise[fin][1] = tableau_optimise[fin][1];
			}
			nbCouleurs_ancien = nbCouleurs_nouveau;
		}
		free(couleurs);
		
	}	
	return nbCouleurs_nouveau;
}

int** optimisation(int nbSommets, int** temp_ordonnes, int** voisinnage)
{
	int sommet_temp = 0;
	int couleur_minimale = 0;
	bool* dejaOptimal = new bool[nbSommets];
	int** temp = new int*[nbSommets];
	int sommet;
	int couleur;
	for (int i = 0; i < nbSommets; i++)
	{
		temp[i] = new int[2];
		sommet = temp_ordonnes[i][0];
		couleur = temp_ordonnes[i][1];
		temp[i][0] = sommet;
		temp[i][1] = couleur;
	}

	for (int i = 0; i < nbSommets; i++)
	{
		sommet_temp = temp_ordonnes[i][0];
		couleur_minimale = couleurOptimisee(voisinnage[sommet_temp - 1], temp_ordonnes, sommet_temp, nbSommets);
		if (couleur_minimale == temp_ordonnes[i][1])
			dejaOptimal[i] = true;
		else
			dejaOptimal[i] = false;
	
		if (!dejaOptimal[i])
			temp[i][1] = couleur_minimale;
	}
	free(dejaOptimal);
	return temp;
}

int** creerVoisinages(int** tabAretes, int nbAretes, int nbSommets, int degreMax)
{
	int** voisinnages = new int*[nbSommets];
	int temp = 0;
	int inc = 1;
	for (int i = 0; i < nbSommets; i++)
		voisinnages[i] = new int[degreMax+1];

	for (int i = 0; i < nbSommets; i++)
		for (int j = 0; j < degreMax+1; j++)
			voisinnages[i][j] = 0;

	for (int i = 0; i < nbSommets; i++)
	{
		temp = 0;
		inc = 1;
		for (int j = 0; j < nbAretes; j++)
		{
			if (tabAretes[j][0] == (i + 1))
			{
				temp++;
				voisinnages[i][inc] = tabAretes[j][1];
				inc++;
			}
			else
			if (tabAretes[j][1] == (i + 1))
			{
				temp++;
				voisinnages[i][inc] = tabAretes[j][0];
				inc++;
			}

		}
		voisinnages[i][0] = temp;
	}
	return voisinnages;
}

int couleurOptimisee(int* voisinnage, int** sommets_ordre_temp, int sommet, int nbSommets)
{
	int* temp_couleur = new int[voisinnage[0]];
	for (int i = 0; i < voisinnage[0]; i++)
		temp_couleur[i] = -1;
	bool trouve = false;
	int inc = 0;
	int truc = 0;
	for (int i = 0; i < nbSommets; i++)
	{
		for (int j = 0; j < voisinnage[0]; j++)
		{
			if (sommets_ordre_temp[i][0] == voisinnage[(j + 1)] && !trouve)
			{
				truc = sommets_ordre_temp[i][1];
				temp_couleur[inc] = truc;
				inc++;
				trouve = true;
			}
		}
		trouve = false;
	}
	inc = 0;
	int couleur_optimale = 0;
	if (temp_couleur[0] > 1)
		couleur_optimale = 1;		
	else
	{
		while (!trouve && inc < voisinnage[0])
		{
			if ((temp_couleur[inc] + 1) == temp_couleur[inc + 1] || temp_couleur[inc] == temp_couleur[inc+1])
				inc++;
			else
			{
				couleur_optimale = temp_couleur[inc] + 1;
				trouve = true;
			}
		};
	}
	free(temp_couleur);
	return couleur_optimale;
}

void affichage(int nb, int** tableau)
{
	for (int i = 0; i < nb; i++)
		cout << " sommet " << tableau[i][0] << " -> couleur " << tableau[i][1] << endl;
	cout << endl;
}