#include <iostream>
#include "Outils.h"
#include "Dsatur.h"

using namespace std;

int main(int argc, char **argv)
{
	int** tab_aretes;
	int** tab_degre_sommet;
	tab_aretes = lireFichier("..\\FichiersCol\\queen11_11.col", 0); // appel fonction dans outils.cpp
	tab_degre_sommet = lireFichier("..\\FichiersCol\\queen11_11.col", 1); // appel fonction dans outils.cpp

	return 0;
}