#pragma once
#include <string>
#include <iostream>

#include "ListeJeux.hpp"
#include "Jeu.hpp"
#include "Developpeur.hpp"

using namespace std;


class ListeDeveloppeurs
{
	//TODO: En faire une classe qui suit les principes OO.
	//TODO: On veut pouvoir ajouter et enlever un Developpeur* de la liste, avec réallocation dynamique tel que faite pour ListeJeux.
	//NOTE: Le code sera principalement copié de certaines fonctions écrites pour la partie 1, mais mises dans une classe.
public:
	ListeDeveloppeurs();
	~ListeDeveloppeurs();

	void ajouterDeveloppeur(Developpeur* ptrDeveloppeur);
	void changerTailleListeDeveloppeurs(const unsigned nouvelleCapacite);
	void retirerDeveloppeur(Developpeur* ptrDeveloppeur);

	unsigned nElements;
	unsigned capacite;
	Developpeur** elements;



private:

};
