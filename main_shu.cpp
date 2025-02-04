﻿#include "Jeu.hpp"
#include <iostream>
#include <fstream>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp"
#include "debogage_memoire.hpp"  //NOTE: Incompatible avec le "placement new", ne pas utiliser cette entête si vous utilisez ce type de "new" dans les lignes qui suivent cette inclusion.


#include "Developpeur.hpp"

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
Designer* trouverDesigner(Designer& designer, ListeJeux& listeJeux)
{
	Designer* ptrDesigner = nullptr;
	
	for (int i = 0; i < listeJeux.nElements; i++){	// Remplacer par span??
		for (int j = 0; j < listeJeux.elements[i]->designers.nElements; j++){
			if (designer.nom == listeJeux.elements[i]->designers.elements[j]->nom){
				ptrDesigner = listeJeux.elements[i]->designers.elements[j];
				return ptrDesigner;
			}
		}
	}
	return ptrDesigner;
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
	Designer* ptrDesigner = trouverDesigner(designer, listeJeux);
	if (ptrDesigner == nullptr){	// Peut-on faire cette comparaison ==?
		// Si designer n'exsite pas encore:
		ptrDesigner = new Designer(designer);
		cout << "Designer existant!" << endl;
	}

	// Afficher un message lorsque l'allocation du designer est réussie.
	cout << "Designer nouveau!" << endl;
	cout << designer.nom << endl;  //TODO: Enlever cet affichage temporaire servant à voir que le code fourni lit bien les jeux.
	
	cout << "Allocation du designer est reussie!";
	return ptrDesigner; //TODO: Retourner le pointeur vers le designer crée.
}

//TODO: Fonction qui change la taille du tableau de jeux de ListeJeux.
// Cette fonction doit recevoir en paramètre la nouvelle capacité du nouveau
// tableau. Il faut allouer un nouveau tableau assez grand, copier ce qu'il y
// avait dans l'ancien, et éliminer l'ancien trop petit. N'oubliez pas, on copie
// des pointeurs de jeux. Il n'y a donc aucune nouvelle allocation de jeu ici !
void changerTailleListeJeux(const unsigned nouvelleCapacite, ListeJeux& listeJeux)
{	
	Jeu** nouveauxElements = new Jeu* [nouvelleCapacite]; // Allocation d'un nouveau tableau
	for (int i = 0; i < listeJeux.capacite; i++){
		nouveauxElements[i] = listeJeux.elements[i];
	}
	// Desallocation de l'ancien tableau
	delete[] listeJeux.elements;

	// Enregistrement du nouveau tableau dans la structure listeJeu
	listeJeux.elements = nouveauxElements;
	listeJeux.capacite = nouvelleCapacite;

	// nElements?? --> ne devrait pas changer, parce que la quantite de donnees ne change pas
	// On fait juste copier le contenu d'un tableau a un autre
}

//TODO: Fonction pour ajouter un Jeu à ListeJeux.
// Le jeu existant déjà en mémoire, on veut uniquement ajouter le pointeur vers
// le jeu existant. De plus, en cas de saturation du tableau elements, cette
// fonction doit doubler la taille du tableau elements de ListeJeux.
// Utilisez la fonction pour changer la taille du tableau écrite plus haut.
void ajouterJeu(Jeu* ptrJeu, ListeJeux& listeJeux){ 	
	listeJeux.nElements++;
	if (listeJeux.nElements > listeJeux.capacite){	
		// Si saturation du tableau:
		unsigned nouvelleCapacite = listeJeux.capacite * 2;
		// max(1, b)
		changerTailleListeJeux(max(unsigned(1), nouvelleCapacite), listeJeux); // Gestion du cas capacite == 0
	}

	listeJeux.elements[listeJeux.nElements - 1] = ptrJeu;
}


//TODO: Fonction qui enlève un jeu de ListeJeux.
// Attention, ici il n'a pas de désallocation de mémoire. Elle enlève le
// pointeur de la ListeJeux, mais le jeu pointé existe encore en mémoire.
// Puisque l'ordre de la ListeJeux n'a pas être conservé, on peut remplacer le
// jeu à être retiré par celui présent en fin de liste et décrémenter la taille
// de celle-ci.
void retirerJeu(Jeu& jeu, ListeJeux listeJeux)	// En reference ou pointeur ptrJeu??
{
	// [...]
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
	ptrJeu->designers.elements = new Designer*[jeu.designers.nElements]; // ????
	// new Designer[jeu.designers.nElements];

	// Afficher un message lorsque l'allocation du jeu est réussie.
	cout << jeu.titre << endl;  //TODO: Enlever cet affichage temporaire servant à voir que le code fourni lit bien les jeux.
	cout << "Allocation du jeu est reussie!" << endl;
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

	return ptrJeu; //TODO: Retourner le pointeur vers le nouveau jeu.
}

ListeJeux creerListeJeux(const string& nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	int nElements = lireUint16(fichier); 

	ListeJeux listeJeux = {};
	
	for([[maybe_unused]] int n : iter::range(nElements))
	{
		// lireJeu(fichier); //TODO: Ajouter le jeu à la ListeJeux.
		Jeu* ptrJeu = lireJeu(fichier, listeJeux);
		ajouterJeu(ptrJeu, listeJeux);
	}

	return listeJeux; //TODO: Renvoyer la ListeJeux.
}

//TODO: Fonction pour détruire un designer (libération de mémoire allouée).
// Lorsqu'on détruit un designer, on affiche son nom pour fins de débogage.
void detruireDesigner() {
	// Desallocation de memoire pour listeJeuxParticipes
	// Desallocation de memoire pour Designer
}


//TODO: Fonction qui détermine si un designer participe encore à un jeu.
// Verifier qu'un designer est encore dans la liste designers d'un jeu XXX
// OU Verifier qu'un jeu est encore dans la liste de jeu participes d'un designer
void determinerParticipationDesignerAJeu(Jeu* ptrJeu, Designer* ptrDesigner) {
	ptrDesigner->listeJeuxParticipes; // iter
		// Si on trouve le jeu 
	retirerJeu(*ptrJeu, ptrDesigner->listeJeuxParticipes);
	
	// si le jeu est le dernier jeu de listeJeuxParticipes
	detruireDesigner();
}


//TODO: Fonction pour détruire un jeu (libération de mémoire allouée).
// Attention, ici il faut relâcher toute les cases mémoires occupées par un jeu.
// Par conséquent, il va falloir gérer le cas des designers (un jeu contenant
// une ListeDesigners). On doit commencer par enlever le jeu à détruire des jeux
// qu'un designer a participé (listeJeuxParticipes). Si le designer n'a plus de
// jeux présents dans sa liste de jeux participés, il faut le supprimer.  Pour
// fins de débogage, affichez le nom du jeu lors de sa destruction.
void detruireJeu(Jeu* ptrJeu) {
	// Iteration sur listeDesigners du jeu 
		// for each Designer: 
		Designer* ptrDesigner; // element du tableau listeDesigners
		determinerParticipationDesignerAJeu(ptrJeu, ptrDesigner);
}


//TODO: Fonction pour détruire une ListeJeux et tous ses jeux.
void detruireListeJeux(ListeJeux& listeJeux) {
	// Iteration sur le tableau de jeux (listeJeux.elements)
		// for each Jeu:
		Jeu* ptrJeu; // element du tableau
		detruireJeu(ptrJeu);
}


void afficherDesigner(const Designer& d)
{
	cout << "\t" << d.nom << ", " << d.anneeNaissance << ", " << d.pays
			  << endl;
}

//TODO: Fonction pour afficher les infos d'un jeu ainsi que ses designers.
// Servez-vous de la fonction afficherDesigner ci-dessus.

//TODO: Fonction pour afficher tous les jeux de ListeJeux, séparés par un ligne.
// Servez-vous de la fonction d'affichage d'un jeu crée ci-dessus. Votre ligne
// de séparation doit être différent de celle utilisée dans le main.


int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	#pragma region "Bibliothèque du cours"
	 // Permet sous Windows les "ANSI escape code" pour changer de couleur
	 // https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac
	 // les supportent normalement par défaut.
	 bibliotheque_cours::activerCouleursAnsi(); 
	#pragma endregion

	int* fuite = new int;  // Pour vérifier que la détection de fuites fonctionne; un message devrait dire qu'il y a une fuite à cette ligne.

	Designer** tabDesigners = new Designer * [1];
	Jeu** tabJeux = new Jeu * [1];
	


	ListeJeux listeJeux = { 1, 1, tabJeux };




	//creerListeJeux("jeux.bin"); //TODO: Appeler correctement votre fonction de création de la liste de jeux.
	//ListeJeux listeJeu = creerListeJeux("jeux.bin");
	//cout << listeJeu.nElements << endl;
	//for (int i = 0; i < listeJeu.nElements; i++) {
	//	cout << listeJeu.elements[i] << endl;
	//}

	

	static const string ligneSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";
	cout << ligneSeparation << endl;
	cout << "Premier jeu de la liste :" << endl;
	//TODO: Afficher le premier jeu de la liste (en utilisant la fonction).  Devrait être Chrono Trigger.

	cout << ligneSeparation << endl;

	//ListeJeux liste = {};
	//Developpeur dev("nom",liste)

	//TODO: Appel à votre fonction d'affichage de votre liste de jeux.
	
	//TODO: Faire les appels à toutes vos fonctions/méthodes pour voir qu'elles fonctionnent et avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.

	//TODO: Détruire tout avant de terminer le programme.  Devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
}
