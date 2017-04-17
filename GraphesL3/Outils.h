#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int** lireFichier(string nameFile, int quelTableau);
vector<string> truncateString(string s, char delim);
void sauvegarderFichier(int* t, int size, string nameFile);