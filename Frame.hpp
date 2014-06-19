#ifndef FRAME_HPP
#define FRAME_HPP

/*!
 * \file Frame.h
 * \brief Permet le calcul des différentes classes
 * \author djamel fellah - quentin barthelemy
 * \version 3.7
 */

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

/*! \namespace frame */
namespace frame {
	using namespace std;
	
	/*! \class Frame
	 *  \brief classe gere le calcul des différentes classes pour supprimer le contenu de l'étage 2
	 */
class Frame {
	private:
		unsigned nbrSommet;	/*!< Nombre de sommet de la classe */
		
		/*!< interdit est une structure regroupant les interdits d'étage 1 et 2 */
		struct interdit {
			unordered_set<int> etage1;
			unordered_set<int> etage2;
		};
		
		/*!< forbiddenColumnMap représente toute les classes d'interdits */
		unordered_map<unsigned, unordered_map<unsigned, interdit> > forbiddenColumnMap;
		
		/*!< identifiant de la prochaine classe */
		unsigned generalColumnKey;
		
	public:
		/*!
		 *  \brief Constructeur
		 *
		 *  Constructeur de la classe Frame
		 *
		 *  \param generalColumnKey : identifiant de la prochaine classe
		 *  \param nbrSommet : nombre de sommet de la classe
		 */
		Frame(unsigned size) : generalColumnKey(2), nbrSommet(size) {
		}
		
		/*!
		 *  \brief Destructeur
		 *
		 *  Destructeur de la classe Frame
		 */
		~Frame(void) {
		}
		
		/*!
		 *  \brief Ajout d'interdit
		 *  
		 *	Permet d'ajouter un sommet interdit par rapport au sommet de référence et de la classe d'interdit
		 *  
		 *	\param column : classe d'interdit
		 *  \param refVertex : sommet de référence
		 *  \param etage : l'étage dans lequel enregistrer le sommet
		 *  \param vertex : le sommet à ajouter dans le set d'interdit
		 */
		void ajoutInterdit(unsigned column, unsigned refVertex, unsigned etage, int vertex) {
			try {
				//recherche de la colonne
				unordered_map<unsigned, unordered_map<unsigned, interdit>>::iterator itMap1;
				itMap1 = forbiddenColumnMap.find(column);
				
				//Si it == forbiddenColumnMap.end() alors l'élément n'a pas été trouver dans la map
				if(itMap1 == forbiddenColumnMap.end() ) {
 					//déclaration de la map de second niveau
					unordered_map<unsigned, interdit> vertexCharpMap(nbrSommet);
					pair<unsigned, unordered_map<unsigned, interdit>> newPairForMap1(column, vertexCharpMap);
					
					/* ajoute la classe de depart a forbiddenColumnMap */
					forbiddenColumnMap.insert(newPairForMap1);
				}
				
				unordered_map<unsigned, interdit>::iterator it = forbiddenColumnMap.at(column).find(refVertex);
				
				//Si it == vertexCharp.end() alors l'élément n'a pas été trouver dans la map
				if(it != forbiddenColumnMap.at(column).end()) {
					if(etage == 1)
						forbiddenColumnMap.at(column).at(refVertex).etage1.insert(vertex);
					else if(etage == 2)
						forbiddenColumnMap.at(column).at(refVertex).etage2.insert(vertex);
					else
						throw "lvl = 1 ou 2";
				}
				else{
					//Création d'une nouvelle pair et insertion de celle-ci dans la map
					interdit prohib;
					if(vertex != (int)NULL) {
						if(etage == 1)
							prohib.etage1.insert(vertex);
						else if(etage == 2)
							prohib.etage2.insert(vertex);
						else
							throw "lvl = 1 ou 2";
					}
					
					pair<unsigned, interdit> mapPair (refVertex, prohib);
					forbiddenColumnMap.at(column).insert(mapPair);
				}
			}
			catch(string e) {
				cout << e << endl;
				exit(EXIT_FAILURE);
			}
		}
		
		/*!
		 *  \brief Suppression d'interdit
		 *  
		 *	Permet de supprimer un sommet interdit par rapport au sommet de référence et de la classe d'interdit
		 *  
		 *	\param column : classe d'interdit
		 *  \param refVertex : sommet de référence
		 *  \param etage : l'étage dans lequel enregistrer le sommet.
		 *  \param vertex : le sommet à supprimer dans le set d'interdit
		 */
		void supprimeInterdit(unsigned column, unsigned refVertex, int vertex, unsigned etage) {
			try {
				if(etage == 1) {
					unordered_set<int>::iterator it = forbiddenColumnMap.at(column).at(refVertex).etage1.find(vertex);
					if(it !=  forbiddenColumnMap.at(column).at(refVertex).etage1.end())
						forbiddenColumnMap.at(column).at(refVertex).etage1.erase(vertex);
				}
				else if(etage == 2) {
					unordered_set<int>::iterator it = forbiddenColumnMap.at(column).at(refVertex).etage2.find(vertex);
					if(it !=  forbiddenColumnMap.at(column).at(refVertex).etage2.end())
						forbiddenColumnMap.at(column).at(refVertex).etage2.erase(vertex);
					
				}
				else
					throw "lvl = 1 ou 2";
			}
			catch(string e) {
				cout << e << endl;
				exit(EXIT_FAILURE);
			}
		}
		
		/*!
		 *	\brief cherche element
		 *	
		 *	\param set : unordered_set
		 *	\param elem : élément à rechercher dans le set
		 *	\return true si l'élément passé en second paramètre est présent dans le vecteur passé en premier paramètre
		 */
		bool findInSet(unordered_set<int> &set, int elem) {
			unordered_set<int>::iterator it = set.find(elem);
			
			return (it != set.end());
		}
		
		
		/*!
		 *	\brief Ajouter Classe ?
		 *	
		 *	Cherche si l'on a besoin de créer une nouvelle classe
		 *	
		 *	\param column : une classe dans laquel on va vérifier
		 *	\param etage : le vecteur d'interdit d'étage 1
		 *	\param vertexToFind : le sommet à trouver
		 */
		bool needAnewClass(unordered_map<unsigned, interdit> &column, unordered_set<int> etage, int vertexToFind) {
			bool res = false;
			
			unordered_set<int>::iterator it1 = etage.begin();
			
			for(; it1 != etage.end(); it1++) {
				if(!findInSet(column.at(*it1).etage1,vertexToFind)
				   && !findInSet(column.at(vertexToFind).etage1, *it1))
					res = true;
			}
			return res;
		}
		
		/*!
		 *	\brief traitement des classes
		 *
		 *	Execution de l'algorithme sur un sommet
		 *	
		 *	\param colum : une classe dans laquel on va traiter les sommets
		 *	\param actualStruct : les étages d'interdits d'un sommet
		 *	\param itMap1End : l'itérateur de fin de la map de classe
		 *	\param vertex : le numéro du sommet actuel
		 */
		void verTreatment(unordered_map<unsigned, interdit> &column, interdit &actualStruct, unordered_map<unsigned,
						  unordered_map<unsigned, interdit>>::iterator &itMap1End, unsigned vertex) {
			unordered_map<unsigned, interdit>::iterator itMap2b;
			unordered_set<int>::iterator itSetB;
			
			unordered_set<int>::iterator itEtage1;
			unordered_set<int>::iterator itEtage1End = actualStruct.etage1.end();
			unordered_set<int>::iterator itEtage2 = actualStruct.etage2.begin();
			unordered_set<int>::iterator itEtage2End = actualStruct.etage2.end();
			
			unordered_set<int> valueToSupressInVect2;
			unordered_set<int> prohib = actualStruct.etage1;
			
			int columnkey, elemSet1, elemSet2;
			
			//Parcours de l'étage 2
			while((!actualStruct.etage2.empty()) && (itEtage2 != itEtage2End)) {
				elemSet2 = *itEtage2;
				itEtage1 = actualStruct.etage1.begin();
				itEtage1End = actualStruct.etage1.end();
				//Parcours de l'étage 1
				while((!actualStruct.etage1.empty()) && (itEtage1 != itEtage1End)) {
					elemSet1 =  *itEtage1;
					
					//Si élément d'étage 1 n'est pas présent dans liste d'interdit du sommet lu dans l'étage2
					//et que l'élément lu de l'étage 1 ne comprend pas dans ses interdits l'élément lu de l'étage 2 alors
					if(!findInSet(column.at(elemSet2).etage1, elemSet1)
					   && !findInSet(column.at(elemSet1).etage1, elemSet2)
					   && !actualStruct.etage2.empty()) {
						columnkey = generalColumnKey;

						//Création d'une nouvelle classe
						pair<unsigned, unordered_map<unsigned, interdit>> newPairForMap1 (generalColumnKey, column);
						forbiddenColumnMap.insert(newPairForMap1);
						generalColumnKey++;
						
						itMap1End = forbiddenColumnMap.end();

						//Supprime tous les éléments des etages 1 et 2 afin de pouvoir insérer la valeur négative dans l'étage 1
						forbiddenColumnMap.at(columnkey).at(elemSet1).etage1.clear();
						forbiddenColumnMap.at(columnkey).at(elemSet1).etage2.clear();

						//Insertion de la valeur négative dans l'étage 1
						forbiddenColumnMap.at(columnkey).at(elemSet1).etage1.insert(-(elemSet2));
						
						//parcours de la nouvelle colonne
						for(itMap2b = forbiddenColumnMap.at(columnkey).begin(); itMap2b != forbiddenColumnMap.at(columnkey).end(); itMap2b++) {
							
							//Parcours du set1
							if((*itMap2b).second.etage1.find(elemSet1) != (*itMap2b).second.etage1.end()) {
								(*itMap2b).second.etage1.erase(elemSet1);
								(*itMap2b).second.etage1.insert(elemSet2);
							}
							
							//parcours set2
							if((*itMap2b).second.etage2.find(elemSet1) != (*itMap2b).second.etage2.end()) {
								(*itMap2b).second.etage2.erase(elemSet1);
								(*itMap2b).second.etage2.insert(elemSet2);
							}
						}
						supprimeInterdit(columnkey,vertex, elemSet2, 2);
						
						//insertion en niveau 1
						if(!findInSet(forbiddenColumnMap.at(columnkey).at(vertex).etage1, elemSet2))
							forbiddenColumnMap.at(columnkey).at(vertex).etage1.insert(elemSet2);
						
						//insertion du sommet dans la liste d'interdit de second niveau
						column.at(elemSet1).etage2.insert(elemSet2);
						
						printTable();
						verTreatment(column, column.at(elemSet1),itMap1End, elemSet1);
						
						//Insert la valeur de l'étage 2 dans l'étage 1
						prohib.insert(elemSet2);
						
						//suppression en niveau 2
						valueToSupressInVect2.insert(elemSet2);
					}
					else {
						if(!actualStruct.etage2.empty()
						   && !findInSet(prohib,elemSet2)
						   && !needAnewClass(column, actualStruct.etage1, elemSet2)) {
							prohib.insert(elemSet2);
							
							valueToSupressInVect2.insert(elemSet2);
						}
					}
					
					if(!actualStruct.etage1.empty())
						itEtage1++;
				}
				actualStruct.etage1 = prohib;
				
				if(!actualStruct.etage2.empty() && (itEtage2 != actualStruct.etage2.end()))
					itEtage2++;
			}
			unordered_set<int>::iterator itSet2B;
			//Supression des valeurs au niveau 2
			for(itSetB = valueToSupressInVect2.begin(); itSetB != valueToSupressInVect2.end(); itSetB++) {
				itSet2B = actualStruct.etage2.find(*itSetB);
				if(itSet2B != actualStruct.etage2.end()){
					actualStruct.etage2.erase(*itSet2B);
					itEtage2End = actualStruct.etage2.end();
				}
			}
		}
		
		/*!
		 * \brief Execute l'algorithme
		 */
		void treatment() {
			unordered_map<unsigned, unordered_map<unsigned, interdit>>::iterator itMap1 = forbiddenColumnMap.begin();
			unordered_map<unsigned, unordered_map<unsigned, interdit>>::iterator itMap1End;
			
			unordered_map<unsigned, interdit>::iterator itMap2;
			unordered_map<unsigned, interdit>::iterator itMap2End;
			
			itMap1End = forbiddenColumnMap.end();
			
			//Mise en ordre de la classe 1
			unordered_map<unsigned, interdit> vertexCharpMap(itMap1->second);
			pair<unsigned, unordered_map<unsigned, interdit>> newPairForMap1 (1, vertexCharpMap);
			forbiddenColumnMap.erase(1);
			forbiddenColumnMap.insert(newPairForMap1);
			
			/* affiche classe de base */
			cout << endl << "Classe de base :" << endl;
			printTable();
			cout << endl;
			
			//Parcours des classes
			for	(int i =0; i<4; i++)
			for(itMap1 = forbiddenColumnMap.begin(); itMap1 != itMap1End; itMap1 ++) {

				itMap2End = itMap1->second.end();
				//Parcours des lignes (sommets)
				for(itMap2 = (*itMap1).second.begin(); itMap2 != itMap2End; itMap2++) {
					//si l'étage 2 du sommet n'est pas vide
					if(!itMap2->second.etage2.empty()) {
						verTreatment(itMap1->second, itMap2->second, itMap1End, itMap2->first);
					}
				}
			}
		}
		
		/*!
		 * \brief Print le tableau
		 */
		void printTable(void) {
			unordered_map<unsigned, unordered_map<unsigned, interdit>>::iterator itMap1;
			unordered_map<unsigned, interdit>::iterator itMap2;
			unordered_set<int>::iterator itSet;
			
			for(itMap1 = forbiddenColumnMap.begin(); itMap1 != forbiddenColumnMap.end(); itMap1++) {
				cout << "classe n " << (*itMap1).first << " :" << endl;
				for(itMap2 = (*itMap1).second.begin(); itMap2 != (*itMap1).second.end(); itMap2++) {
					cout << " " << (*itMap2).first;
					
					cout << "[";
					for(itSet = (*itMap2).second.etage1.begin(); itSet != (*itMap2).second.etage1.end(); itSet++) {
						cout << " " << (*itSet);
					}
					cout << "]";
					
					cout << "[";
					for(itSet = (*itMap2).second.etage2.begin(); itSet != (*itMap2).second.etage2.end(); itSet++) {
						cout << " " << (*itSet);
					}
					cout << "]" << endl;
				}
				cout << endl;
			}
		}
	};
}

#endif