#include "Jeu.hpp"
#include <iostream>
#include <fstream>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.

using namespace std;
using namespace iter;
using namespace gsl;

#pragma region "Fonctions de base pour vous aider"
typedef uint8_t UInt8;
typedef uint16_t UInt16;
UInt8 lireUint8(istream& fichier)
{
	UInt8 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
UInt16 lireUint16(istream& fichier)
{
	UInt16 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUint16(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}
gsl::span<Jeu*> spanListeJeux(const ListeJeux& liste)
{
	return gsl::span(liste.elements, liste.nElements);
}
gsl::span<Designer*> spanListeDesigners(const ListeDesigners& liste)
{
	return gsl::span(liste.elements, liste.nElements);
}
#pragma endregion

//TODO: Fonction qui cherche un designer par son nom dans une ListeJeux.
// Cette fonction renvoie le pointeur vers le designer si elle le trouve dans
// un des jeux de la ListeJeux. En cas contraire, elle renvoie un pointeur nul.
Designer* trouverDesignerParNom(const ListeJeux& listeJeux, const std::string& nomDesigner) {
	for (unsigned int i = 0; i < listeJeux.nElements; i++) {
		Jeu* jeu = listeJeux.elements[i];
		for (unsigned int j = 0; j < jeu->designers.nElements; j++) {
			Designer* designer = jeu->designers.elements[j];
			if (designer->nom == nomDesigner) {
				return designer;
			}
		}
	}
	return nullptr;
}

Designer* lireDesigner(istream& fichier, ListeJeux& listeJeux)
{
	Designer designer = {}; // On initialise une structure vide de type Designer.
	designer.nom = lireString(fichier);
	designer.anneeNaissance = lireUint16(fichier);
	designer.pays = lireString(fichier);
	// Rendu ici, les champs précédents de la structure designer sont remplis
	// avec la bonne information.

	//TODO: Ajouter en mémoire le designer lu. Il faut revoyer le pointeur créé.
	// Attention, valider si le designer existe déjà avant de le créer, sinon
	// on va avoir des doublons car plusieurs jeux ont des designers en commun
	// dans le fichier binaire. Pour ce faire, cette fonction aura besoin de
	// la liste de jeux principale en paramètre.
	
	Designer* designerExistant = trouverDesignerParNom(listeJeux, designer.nom);
	if (designerExistant != nullptr) {
		cout << "Le designer " << designer.nom << " existe déjà." << endl;
		return designerExistant; // Retourner le pointeur vers le designer existant
	}

	// Le designer n'existe pas encore, l'ajouter en mémoire
	Designer* nouveauDesigner = new Designer(designer);

	// TODO: Ajouter le pointeur vers le nouveau designer à la liste de jeux principale

	cout << "Le designer " << designer.nom << " a été ajouté en mémoire." << endl;
	return nouveauDesigner; // Retourner le pointeur vers le nouveau designer
}

//TODO: Fonction qui change la taille du tableau de jeux de ListeJeux.
// Cette fonction doit recevoir en paramètre la nouvelle capacité du nouveau
// tableau. Il faut allouer un nouveau tableau assez grand, copier ce qu'il y
// avait dans l'ancien, et éliminer l'ancien trop petit. N'oubliez pas, on copie
// des pointeurs de jeux. Il n'y a donc aucune nouvelle allocation de jeu ici !
void changerTailleTableauJeux(ListeJeux& listeJeux, unsigned int nouvelleCapacite) {
	// Si lecture du premier jeu de la liste de jeux
	if (nouvelleCapacite == 1) {
		listeJeux.elements = new Jeu * [nouvelleCapacite];
		listeJeux.capacite = nouvelleCapacite;
		cout << "La taille du tableauJeux a ete changee a 1!" << endl;
		return;
	}	
	
	if (nouvelleCapacite <= listeJeux.capacite) {
		// La nouvelle capacité est inférieure ou égale à l'ancienne capacité, aucune action nécessaire.
		cout << "La taille du tableauJeux n'a pas ete change!" << endl;
		return;
	}

	// Allouer un nouveau tableau de jeux avec la nouvelle capacité
	Jeu** nouveauTableauJeux = new Jeu * [nouvelleCapacite];

	// Copier les pointeurs de jeux de l'ancien tableau vers le nouveau tableau
	for (unsigned int i = 0; i < listeJeux.nElements; i++) {
		nouveauTableauJeux[i] = listeJeux.elements[i];
	}

	// Libérer l'ancien tableau de jeux
	delete[] listeJeux.elements;

	// Mettre à jour les informations de la liste de jeux avec le nouveau tableau et la nouvelle capacité
	listeJeux.elements = nouveauTableauJeux;
	listeJeux.capacite = nouvelleCapacite;
	cout << "La taille du tableauJeux a ete changee a: " << nouvelleCapacite << endl;

	return;
}

//TODO: Fonction pour ajouter un Jeu à ListeJeux.
// Le jeu existant déjà en mémoire, on veut uniquement ajouter le pointeur vers
// le jeu existant. De plus, en cas de saturation du tableau elements, cette
// fonction doit doubler la taille du tableau elements de ListeJeux.
// Utilisez la fonction pour changer la taille du tableau écrite plus haut.

void ajouterJeu(Jeu* ptrJeu, ListeJeux& listeJeux) {
	listeJeux.nElements++;
	if (listeJeux.nElements > listeJeux.capacite) {
		// Si saturation du tableau:
		unsigned nouvelleCapacite = listeJeux.capacite * 2;
		// max(1, b)
		changerTailleTableauJeux(listeJeux, max(unsigned(1), nouvelleCapacite)); // Gestion du cas capacite == 0
	}
	listeJeux.elements[listeJeux.nElements - 1] = ptrJeu;

	cout << "Le jeu " << ptrJeu->titre << " est ajoute a la listeJeux!" << endl;
	return;
}
//TODO: Fonction qui enlève un jeu de ListeJeux.
// Attention, ici il n'a pas de désallocation de mémoire. Elle enlève le
// pointeur de la ListeJeux, mais le jeu pointé existe encore en mémoire.
// Puisque l'ordre de la ListeJeux n'a pas être conservé, on peut remplacer le
// jeu à être retiré par celui présent en fin de liste et décrémenter la taille
// de celle-ci.

void enleverJeu(ListeJeux& listeJeux, Jeu* jeu) {
	// Parcourir le tableau elements pour trouver le jeu à enlever
	for (unsigned int i = 0; i < listeJeux.nElements; i++) {
		if (listeJeux.elements[i] == jeu) {
			// Remplacer le jeu à enlever par le dernier jeu de la liste
			listeJeux.elements[i] = listeJeux.elements[listeJeux.nElements - 1];
			listeJeux.nElements--;
			return; // Sortir de la fonction après avoir effectué la suppression
		}
	}
	return;
}


Jeu* lireJeu(istream& fichier, ListeJeux& listeJeux)
{
	Jeu jeu = {}; // On initialise une structure vide de type Jeu
	jeu.titre = lireString(fichier);
	jeu.anneeSortie = lireUint16(fichier);
	jeu.developpeur = lireString(fichier);
	jeu.designers.nElements = lireUint8(fichier);
	// Rendu ici, les champs précédents de la structure jeu sont remplis avec la
	// bonne information.
	//TODO: Ajouter en mémoire le jeu lu. Il faut revoyer le pointeur créé.
	// Attention, il faut aussi créer un tableau dynamique pour les designers
	// que contient un jeu. Servez-vous de votre fonction d'ajout de jeu car la
	// liste de jeux participé est une ListeJeu. 
	Jeu* ptrJeu = new Jeu(jeu);
	ptrJeu->designers.elements = new Designer * [jeu.designers.nElements]; // ????


	// Afficher un message lorsque l'allocation du jeu est réussie.
	cout << endl << endl;  //TODO: Enlever cet affichage temporaire servant à voir que le code fourni lit bien les jeux.
	cout << "Allocation du jeu " << jeu.titre << " est reussie!" << endl;
	cout << "Il y a " << jeu.designers.nElements << " designers pour ce jeu!" << endl;
	
	
	for ([[maybe_unused]] int i : iter::range(jeu.designers.nElements)) {
		//TODO: Mettre le designer dans la liste des designer du jeu.
		Designer* ptrDesigner = lireDesigner(fichier, listeJeux);
		ptrJeu->designers.elements[i] = ptrDesigner;

		//TODO: Ajouter le jeu à la liste des jeux auquel a participé le designer.
		ajouterJeu(ptrJeu, ptrDesigner->listeJeuxParticipes);
		//ptrDesigner->listeJeuxParticipes = new Jeu * [????]; // ????
		/*ptrDesigner->listeJeuxParticipes.elements[ptrDesigner->listeJeuxParticipes.nElements] = ptrJeu;
		ptrDesigner->listeJeuxParticipes.nElements++;*/
	}
	cout << "Lecture du jeu -- " << jeu.titre << " -- est reussi!" << endl;
	return ptrJeu; //TODO: Retourner le pointeur vers le nouveau jeu.
}


ListeJeux creerListeJeux(const string& nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	int nElements = lireUint16(fichier);
	ListeJeux listeJeux = {};

	cout << "Il y a " << nElements << " jeux dans listeJeux!" << endl;

	for ([[maybe_unused]] int n : iter::range(nElements))
	{
		// lireJeu(fichier); //TODO: Ajouter le jeu à la ListeJeux.
		Jeu* ptrJeu = lireJeu(fichier, listeJeux);
		ajouterJeu(ptrJeu, listeJeux);
	}

	cout << "LECTURE DE LA LISTE DE JEUX REUSSIE!" << endl << endl << endl;
	return listeJeux; //TODO: Renvoyer la ListeJeux.
}

//TODO: Fonction pour détruire un designer (libération de mémoire allouée).
// Lorsqu'on détruit un designer, on affiche son nom pour fins de débogage.
void detruireDesigner(Designer* designer) {
	cout << "destruction designer" << designer->nom << endl;
	
	// Libérer le tableau de jeux participés du designer
	delete[] designer->listeJeuxParticipes.elements;

	// Supprimer le designer
	delete designer;
}
//TODO: Fonction qui détermine si un designer participe encore à un jeu.

bool designerParticipeJeu(Designer* designer, Jeu* jeu)
{
	// Condition sur si designer existe encore???

	// Parcourir la liste des jeux auxquels le designer participe
	for (unsigned int i = 0; i < designer->listeJeuxParticipes.nElements; ++i) {
		// Vérifier si le jeu est présent dans la liste
		if (designer->listeJeuxParticipes.elements[i] == jeu) {
			return true; // Le designer participe au jeu
		}
	}
	return false; // Le designer ne participe pas au jeu
}

//TODO: Fonction pour détruire un jeu (libération de mémoire allouée).
// Attention, ici il faut relâcher toute les cases mémoires occupées par un jeu.
// Par conséquent, il va falloir gérer le cas des designers (un jeu contenant
// une ListeDesigners). On doit commencer par enlever le jeu à détruire des jeux
// qu'un designer a participé (listeJeuxParticipes). Si le designer n'a plus de
// jeux présents dans sa liste de jeux participés, il faut le supprimer.  Pour
// fins de débogage, affichez le nom du jeu lors de sa destruction.
void detruireJeu(Jeu* jeu) {
	cout << "Détruire jeu : " << jeu->titre << endl;
	// Parcourir la liste des designers du jeu
	for (unsigned int i = 0; i < jeu->designers.nElements; ++i) {
		Designer* designer = jeu->designers.elements[i];

		// Retirer le jeu de la liste des jeux auquel le designer participe
		if (designerParticipeJeu(designer, jeu)) {
			enleverJeu(designer->listeJeuxParticipes, jeu);	// le designer pourrait ne plus exister? (sans etre nullptr..(?))
		}		

		// Vérifier si le designer n'a plus de jeux dans sa liste de jeux participés
		if (designer->listeJeuxParticipes.nElements == 0) {
			// Supprimer le designer
			detruireDesigner(designer);
		}
	}

	// Libérer le tableau des designers du jeu
	delete[] jeu->designers.elements;

	// Supprimer le jeu
	delete jeu;
}

//TODO: Fonction pour détruire une ListeJeux et tous ses jeux.
void detruireListeJeux(ListeJeux* listeJeux) 
{
	// Parcourir tous les jeux de la liste
	for (unsigned int i = 0; i < listeJeux->nElements; ++i) {
		Jeu* jeu = listeJeux->elements[i];
		// Détruire le jeu
		detruireJeu(jeu);
	}

	// Libérer le tableau des jeux
	delete[] listeJeux->elements;

	// Réinitialiser les champs de la liste
	listeJeux->nElements = 0;
	listeJeux->capacite = 0;
}



void afficherDesigner(const Designer& d)
{
	cout << "\t" << d.nom << ", " << d.anneeNaissance << ", " << d.pays
			  << endl;
}

//TODO: Fonction pour afficher les infos d'un jeu ainsi que ses designers.
// Servez-vous de la fonction afficherDesigner ci-dessus.
void afficherJeu(const Jeu& jeu)
{
	cout << "Titre: " << jeu.titre << endl;
	cout << "Année de sortie: " << jeu.anneeSortie << endl;
	cout << "Développeur: " << jeu.developpeur << endl;
	cout << "Designers: " << endl;
	for (unsigned int i = 0; i < jeu.designers.nElements; ++i) {
		Designer* designer = jeu.designers.elements[i];
		afficherDesigner(*designer);
	}
}

//TODO: Fonction pour afficher tous les jeux de ListeJeux, séparés par un ligne.
// Servez-vous de la fonction d'affichage d'un jeu crée ci-dessus. Votre ligne
// de séparation doit être différent de celle utilisée dans le main.
void afficherListeJeux(const ListeJeux& liste)
{
	for (unsigned int i = 0; i < liste.nElements; ++i) {
		const Jeu* jeu = liste.elements[i];
		afficherJeu(*jeu);
		cout << " --- " << endl;
	}
}



int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
#pragma region "Bibliothèque du cours"
	// Permet sous Windows les "ANSI escape code" pour changer de couleur
	// https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac
	// les supportent normalement par défaut.
	bibliotheque_cours::activerCouleursAnsi();
#pragma endregion

	int* fuite = new int;  // Pour vérifier que la détection de fuites fonctionne; un message devrait dire qu'il y a une fuite à cette ligne.
	delete fuite;
	cout << "il y'a une fuite dans cette ligne";


	for (int i = 0; i < 5; ++i) {
		cout << i << endl;
	}

	
	static const string ligneSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";


	//TODO: Appeler correctement votre fonction de création de la liste de jeux.
	ListeJeux listeJeux = creerListeJeux("jeux.bin");


	//TODO: Afficher le premier jeu de la liste (en utilisant la fonction).  Devrait être Chrono Trigger.
	cout << ligneSeparation << endl;
	cout << "Premier jeu de la liste :" << endl;
	afficherJeu(*listeJeux.elements[0]);


	////TODO: Appel à votre fonction d'affichage de votre liste de jeux.
	cout << ligneSeparation << endl;
	cout << "Il y a " << listeJeux.nElements << " jeux dans la liste totale de jeux! Les voici: " << endl;
	afficherListeJeux(listeJeux);

	cout << ligneSeparation << endl;
	cout << "Voici les jeux faits par le designer " << listeJeux.elements[0]->designers.elements[0]->nom << " : " << endl;
	afficherListeJeux(listeJeux.elements[0]->designers.elements[0]->listeJeuxParticipes);

	//TODO: Faire les appels à toutes vos fonctions/méthodes pour voir qu'elles fonctionnent et avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.
	
	//TODO: Détruire tout avant de terminer le programme.  Devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.;
	detruireListeJeux(&listeJeux);

}
