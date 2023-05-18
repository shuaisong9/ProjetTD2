#include "Developpeur.hpp"

Developpeur::Developpeur(string& nom, ListeJeux& listeJeux) {
	paireNomJeux_.first = nom;
	paireNomJeux_.second = listeJeux; // ListeJeux vide ??
	// Allocation de elements ??
	//paireNomJeux_.second = new Jeu * []; ?? 

}

Developpeur::~Developpeur() {
	// ...
	delete[] paireNomJeux_.second.elements; // ???
}

string Developpeur::getNom() {
	return paireNomJeux_.first;
}

int Developpeur::compterJeuxParticipes(ListeJeux& listeJeux) {
	// Iteration sur la liste de jeux
	int nbreJeuxParticipes = 0;

	for (int i = 0; i < listeJeux.nElements; i++) {
		if (listeJeux.elements[i]->developpeur == paireNomJeux_.first) {
			nbreJeuxParticipes++;
		}
	}
	return nbreJeuxParticipes;
}


void Developpeur::mettreAJourListeJeux(ListeJeux& listeJeux) {
	int nouvelleCapacite = compterJeuxParticipes(listeJeux);
	
	// Reallocation d’une liste avec la bonne capacite
	Jeu** nouveauxElements = new Jeu * [nouvelleCapacite];
	int nElements = 0;
	
	// Copie des jeux a mettre a jour
	while (nElements < nouvelleCapacite) { /// ?? pour enlever buffer overrun warning
		for (int i = 0; i < listeJeux.capacite; i++) {
			if (listeJeux.elements[i]->developpeur == paireNomJeux_.first) {
				nouveauxElements[nElements] = listeJeux.elements[i];
				nElements++;
			}
		}
	}

	// Desallocation de l'ancien tableau de jeux participes
	delete[] paireNomJeux_.second.elements;

	// Enregistrement du nouveau tableau dans la structure listeJeu
	paireNomJeux_.second.elements = nouveauxElements;
	paireNomJeux_.second.capacite = nouvelleCapacite;
	paireNomJeux_.second.nElements = nElements;
}

void Developpeur::afficherJeuxParticipes() {
	for (int i = 0; i < paireNomJeux_.second.capacite; i++) {
		cout << paireNomJeux_.second.elements[i]->titre;
	}
}