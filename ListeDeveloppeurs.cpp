#include "ListeDeveloppeurs.hpp"

ListeDeveloppeurs::ListeDeveloppeurs() {
	// ???
	nElements = 0;
	capacite = 1;
	elements = new Developpeur * [capacite]; 
}

ListeDeveloppeurs::~ListeDeveloppeurs() {
	// Iter sur elements (tableau de developpeurs)
		// Appel retirerDeveloppeur() a chaque elem

	delete[] elements;

}



void ListeDeveloppeurs::changerTailleListeDeveloppeurs(const unsigned nouvelleCapacite)
{
	if (nouvelleCapacite <= capacite) {
		// La nouvelle capacit� est inf�rieure ou �gale � l'ancienne capacit�, aucune action n�cessaire.
		return;
	}

	// Allouer un nouveau tableau de jeux avec la nouvelle capacit�
	Developpeur** nouveauTableauJeux = new Developpeur * [nouvelleCapacite];

	// Copier les pointeurs de jeux de l'ancien tableau vers le nouveau tableau
	for (unsigned int i = 0; i < nElements; i++) {
		nouveauTableauJeux[i] = elements[i];
	}

	// Lib�rer l'ancien tableau de jeux
	delete[] elements;

	// Mettre � jour les informations de la liste de jeux avec le nouveau tableau et la nouvelle capacit�
	elements = nouveauTableauJeux;
	capacite = nouvelleCapacite;
}


void ListeDeveloppeurs::ajouterDeveloppeur(Developpeur* ptrDeveloppeur) {
	nElements++;
	if (nElements > capacite) {
		// Si saturation du tableau:
		unsigned nouvelleCapacite = capacite * 2;
		// max(1, b)
		changerTailleListeDeveloppeurs(max(unsigned(1), nouvelleCapacite)); // Gestion du cas capacite == 0
	}

	elements[nElements - 1] = ptrDeveloppeur;
}

void ListeDeveloppeurs::retirerDeveloppeur(Developpeur* ptrDeveloppeur) {
		// Dans quel ordre?
		 
	// Desallouer listeJeux de chaque developpeur
		// appel aux fonctions de detruire listeJeux ---> comment? Recopier code?
	// Desallouer le ptrDeveloppeur
	// Enlever Developpeur de la liste de developpeur comme fait dans enleverJeu() ???
}