#include "Frame.hpp"
#include "Parser_frame.hpp"

#include <iostream>
#include <iomanip>      // std::setprecision
#include <fstream>		// filebuf
#include <ctime>		// mesure temps execution

using namespace std;

int main(int __argc, char** __argv) {
	
	/* on passe en argument de ligne de commande le chemin vers le fichier
	 décrivant la charpente */
	if(__argc != 2) {
		cout << "Usage : " << __argv[0] << " path_to_graph_frame" << endl;
		return EXIT_FAILURE;
	}
	
	/* initialisation du tampon du fichier contenant la charpente */
	filebuf fileBuffer;

	if(!fileBuffer.open(__argv[1], ios::in)) {
		cout << "Erreur à l'ouverture du fichier : " << __argv[0]<< endl;
		return EXIT_FAILURE;
	}
	
	/* initialisation du flux d'entrée à partir du tampon */
	istream is(&fileBuffer);
	
	/* variable dans laquelle on stockera temporairement chacune des lignes du fichier */
    string str;
	
	/* ordre du graphe */
	int numeroNoeud;
	
	/* première ligne */
	if(getline(is, str)) {
		if (parser::parse_premiere_ligne(str.begin(), str.end(), numeroNoeud)) {
			cout << "--- Parse correctement effectuer ---" << endl << endl;
		}
		else {
			cout << "-------------------------" << endl;
            cout << "Probleme de syntaxe, parse echoué" << endl;
            cout << "-------------------------" << endl;
			return EXIT_FAILURE;
		}
	}
	
	/* charpente */
	frame::Frame myCharp(numeroNoeud);
	
	int i = 0;
	/* lecture du reste du fichier */
    while (getline(is, str)) {
		/* si on va au-delà du nombre de noeuds déclarés... */
		if( i++ > numeroNoeud) {
			cout << "Nombre de neoud incorrect" << endl;
			return EXIT_FAILURE;
		}
		
		/* cas d'arrêt */
        if (str.empty())
            break;
		
		/* noeud courant */
		int noeud;
		
		/* les deux étages d'interdits du noeud courant */
		vector<int> &etage1 = *(new vector<int>());
		vector<int> &etage2 = *(new vector<int>());
		
		/* parsing via la syntaxe SPIRIT définie dans Parser */
		if (parser::parse_ligne(str.begin(), str.end(), noeud, etage1, etage2)) {
			/* insertion des données dans la structure Frame */
			if(etage1.size() == 0) {
				myCharp.ajoutInterdit(1, noeud, 1, NULL);
			}
			for(int forbidden : etage1) {
				myCharp.ajoutInterdit(1, noeud, 1, forbidden);
			}
			for(int forbidden : etage2) {
				myCharp.ajoutInterdit(1, noeud, 2, forbidden);
			}
        }
        else { /* erreur de syntaxe dans le fichier */
            cout << "-------------------------" << endl;
            cout << "Probleme de syntaxe, parse echoué" << endl;
            cout << "-------------------------" << endl;
			return EXIT_FAILURE;
        }
    }
	
	/* nombre de noeuds décrits inférieur au nombre de noeuds déclaré... */
	if(i<numeroNoeud) {
		cout << "Nombre de neoud incorrect" << endl;
		return EXIT_FAILURE;
	}
	
    cout << "Parse terminer" << endl;
	
	long double sysTime = time(0);
	sysTime *= 1000;
	myCharp.treatment();

	cout << "Solution : " << endl << endl;
	myCharp.printTable();
	cout << "Temps d'execution: " << setprecision(5) << (time(0)*1000) - sysTime << " ms"<< endl;
	
	return EXIT_SUCCESS;
}

