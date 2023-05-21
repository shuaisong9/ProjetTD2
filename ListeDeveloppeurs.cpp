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

	for (unsigned int i = 0; i < nElements; i++) {
		retirerDeveloppeur(elements[i]);
	}

	delete[] elements;
	nElements = 0;
	capacite = 0;
}

void ListeDeveloppeurs::afficher() {
	cout << "Voici la liste des developpeurs ainsi que leurs jeux: " << endl << endl;
	for (unsigned int i = 0; i < nElements; i++) {
		elements[i]->afficherJeuxParticipes();
	}
}


void ListeDeveloppeurs::changerTailleListeDeveloppeurs(const unsigned nouvelleCapacite)
{
	if (nouvelleCapacite <= capacite) {
		// La nouvelle capacité est inférieure ou égale à l'ancienne capacité, aucune action nécessaire.
		return;
	}

	// Allouer un nouveau tableau de developpeurs avec la nouvelle capacité
	Developpeur** nouveauTableauJeux = new Developpeur * [nouvelleCapacite];

	// Copier les pointeurs de jeux de l'ancien tableau vers le nouveau tableau
	for (unsigned int i = 0; i < nElements; i++) {
		nouveauTableauJeux[i] = elements[i];
	}

	// Libérer l'ancien tableau de developpeurs
	delete[] elements;

	// Mettre à jour les informations de la liste de developpeurs avec le nouveau tableau et la nouvelle capacité
	elements = nouveauTableauJeux;
	capacite = nouvelleCapacite;
}


void ListeDeveloppeurs::ajouterDeveloppeur(Developpeur* ptrDeveloppeur) {
	
	// Ajouter developpeur seulement si PAS dans le tableau ???
		// Comparer nom? Comparer pointeur? 

	bool developpeurExiste = false;
	for (unsigned int i = 0; i < nElements; i++) {
		if (elements[i] == ptrDeveloppeur) {
			developpeurExiste = true;
		}
	}
	
	if (!developpeurExiste) {
		nElements++;
		if (nElements > capacite) {
			// Si saturation du tableau:
			unsigned nouvelleCapacite = capacite * 2;
			// max(1, b)
			changerTailleListeDeveloppeurs(max(unsigned(1), nouvelleCapacite)); // Gestion du cas capacite == 0
		}

		elements[nElements - 1] = ptrDeveloppeur;	
	}
}

void ListeDeveloppeurs::retirerDeveloppeur(Developpeur* ptrDeveloppeur) {
	
	// Parcourir le tableau elements pour trouver le developpeur à enlever
	for (unsigned int i = 0; i < nElements; i++) {
		if (elements[i]->getNom() == ptrDeveloppeur->getNom()) {
			// Remplacer le developpeur à enlever par le dernier developpeur de la liste
			elements[i] = elements[nElements - 1];
			nElements--;
			//delete ptrDeveloppeur; // Appel du destructeur de Developpeur
			return; // Sortir de la fonction après avoir effectué la suppression
		}
	}	
		// Dans quel ordre?	 
	// Desallouer listeJeux de chaque developpeur
		// appel aux fonctions de detruire listeJeux ---> comment? Recopier code?
	// Desallouer le ptrDeveloppeur
	// Enlever Developpeur de la liste de developpeur comme fait dans enleverJeu() ???
}