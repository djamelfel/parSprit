#include "Frame.hpp"
#include "Parser_frame.hpp"

#include <iostream>
#include <fstream>
#include <ctime>

int
main(int __argc, char** __argv)
{
	char a;
	/* on passe en argument de ligne de commande le chemin vers le fichier
	 décrivant la charpente */
	if(__argc != 2) {
		std::cout << "Usage : " << __argv[0] << " path_to_graph_frame" << std::endl;
		//return EXIT_FAILURE;
	}
	
	/* initialisation du tampon du fichier contenant la charpente */
	std::filebuf fb;
	if(!fb.open(__argv[1] /*"C:/Users/maxime/Documents/Visual Studio 2012/Projects/ProjetLO45/Debug/toto2.txt"*/, std::ios::in)) {
		std::cout << "Error while opening file" << std::endl;
		std::cin >> a;
		return EXIT_FAILURE;
	}
	
	/* initialisation du flux d'entrée à partir du tampon */
	std::istream is(&fb);
	
	/* variable dans laquelle on stockera temporairement chacune des lignes du fichier */
    std::string str;
	
	/* ordre du graphe */
	int nodeNumber;
	
	/* première ligne */
	if(getline(is, str))
	{
		if (parser::parse_first_line(str.begin(), str.end(), nodeNumber))
        {
			std::cout << "Parsing succeeded\n";
			std::cout << "Ordre du graphe: " << nodeNumber << std::endl;
		}
		else
		{
			std::cout << "-------------------------\n";
            std::cout << "Syntax error, parsing failed\n";
            std::cout << "-------------------------\n";
			return EXIT_FAILURE;
		}
	}
	
	/* charpente */
	frame::Frame myCharp(nodeNumber);
	
	int i = 0;
	/* lecture du reste du fichier */
    while (getline(is, str))
    {
		/* si on va au-delà du nombre de noeuds déclarés... */
		if(i++>nodeNumber) {
			std::cout << "Incorrect number of nodes\n";
			return EXIT_FAILURE;
		}
		
		/* cas d'arrêt */
        if (str.empty())
            break;
		
		/* noeud courant */
		int node;
		
		/* les deux étages d'interdits du noeud courant */
		std::vector<int> &floor_1 = *(new std::vector<int>());
		std::vector<int> &floor_2 = *(new std::vector<int>());
		
		/* parsing via la syntaxe SPIRIT définie dans Parser */
		if (parser::parse_frame_line(str.begin(), str.end(), node, floor_1, floor_2))
        {
			/* messages pour debug... */
            std::cout << "Parsing succeeded\n";
			
			/* insertion des données dans la structure Frame */
			if(floor_1.size() == 0) {
				std::cout << "myCharp.addForbidden(1," << node << ",1,NULL)\n";
				myCharp.addForbidden(1, node, 1, NULL);
			}
			for(int forbidden : floor_1) {
				std::cout << "myCharp.addForbidden(1," << node << ",1," << forbidden << ")\n";
				myCharp.addForbidden(1, node, 1, forbidden);
			}
			for(int forbidden : floor_2) {
				std::cout << "myCharp.addForbidden(1," << node << ",2," << forbidden << ")\n";
				myCharp.addForbidden(1, node, 2, forbidden);
			}
        }
        else /* erreur de syntaxe dans le fichier */
        {
            std::cout << "-------------------------\n";
            std::cout << "Syntax error, parsing failed\n";
            std::cout << "-------------------------\n";
			return EXIT_FAILURE;
        }
		
    }
	
	/* nombre de noeuds déclaré supérieur au nombre de noeuds décrits... */
	if(i<nodeNumber) {
		std::cout << "Incorrect number of nodes\n";
		return EXIT_FAILURE;
	}
	
    std::cout << "Parsing finished" << std::endl;
	
	long double sysTime = time(0);
	sysTime *= 1000;
	myCharp.treatment();
	//sysTime = (time(0)*1000) - sysTime;
	std::cout << "Running time: " << (time(0)*1000) - sysTime << " ms"<< std::endl;
	myCharp.printTable();
	
	return EXIT_SUCCESS;
}

