#include <iostream>
#include <stdio.h>
#include <thread>
#include <Windows.h>
#include "Outils.h"

using namespace std;

int main(int argc, char **argv)
{
	//Création des processus
		//Thread 1 -> traitement du fichier
		thread t1([]() {
			lireFichier("..\\FichiersCol\\queen11_11.col"); // appel fonction dans outils.cpp
		});

		//Thread 2 -> action utilisateur
		thread t2([](){
			bool fin = false;
			int nb = 100000000;


			while (!fin)
			{
				if (GetKeyState(VK_END))
				{
					for (int i = 0; i < nb; i++)
					{
						cout << i << endl;
					}
					fin = true;
				}
			}
		});

		t1.join();
		t2.join();

	return 0;
}