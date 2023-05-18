#pragma once
#include <string>
#include <iostream>

#include "ListeJeux.hpp"
#include "Jeu.hpp"

using namespace std;

class Developpeur
{
	//TODO: En faire une classe qui suit les principes OO.
	//TODO: La destruction d'un Developpeur doit s'assurer que la désallocation de ListeJeux est faite.
	//TODO: Les méthodes à faire...
public:
	Developpeur(string& nom, ListeJeux& listeJeux);

	~Developpeur();

	string getNom();

	int compterJeuxParticipes(ListeJeux& listeJeux);

	void mettreAJourListeJeux(ListeJeux& listeJeux);

	void afficherJeuxParticipes();
private:


	std::pair<std::string, ListeJeux> paireNomJeux_;

};
