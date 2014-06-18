/*
 Code permettant la création de la frameente ainsi que l'exécution de l'algorithme
 permettant de générer les classes de coloration
 */

#ifndef FRAME_HPP
#define FRAME_HPP

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

namespace frame {
	using namespace std;
class Frame {
	private:
		unsigned nbrSommet;
		
		/**
		 *  Structure contenant 2 set représentant les étages d'interdits
		 */
		struct interdit {
			unordered_set<int> stage1;
			unordered_set<int> stage2;
		};
		
		/**
		 *  forbiddenColumnMap représente toute les classes d'interdits
		 */
		unordered_map<unsigned, unordered_map<unsigned, interdit> > forbiddenColumnMap;
		
		unsigned generalColumnKey;
		
	public:
		Frame(unsigned size) : generalColumnKey(2), nbrSommet(size) {
		}
		
		
		~Frame(void) {
		}
		
		/**
		 *  Permet d'ajouter un sommet interdit par rapport au sommet de référence et de la classe d'interdit
		 *  column: classe d'interdit
		 *  refVertex: sommet de référence
		 *  stage: l'étage dans lequel enregistrer le sommet
		 *  vertex: le sommet à ajouter dans le set d'interdit
		 */
		void ajoutInterdit(unsigned column, unsigned refVertex, unsigned stage, int vertex) {
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
					if(stage == 1)
						forbiddenColumnMap.at(column).at(refVertex).stage1.insert(vertex);
					else if(stage == 2)
						forbiddenColumnMap.at(column).at(refVertex).stage2.insert(vertex);
					else
						throw "lvl = 1 ou 2";
				}
				else{
					//Création d'une nouvelle pair et insertion de celle-ci dans la map
					interdit prohib;
					if(vertex != (int)NULL) {
						if(stage == 1)
							prohib.stage1.insert(vertex);
						else if(stage == 2)
							prohib.stage2.insert(vertex);
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
		
		/**
		 *  Permet de supprimer un sommet interdit par rapport au sommet de référence et de la classe d'interdit
		 *  column: classe d'interdit
		 *  refVertex: sommet de référence
		 *  stage: l'étage dans lequel enregistrer le sommet.
		 *  vertex: le sommet à supprimer dans le set d'interdit
		 */
		void supprimeInterdit(unsigned column, unsigned refVertex, int vertex, unsigned stage) {
			try {
				if(stage == 1) {
					unordered_set<int>::iterator it = forbiddenColumnMap.at(column).at(refVertex).stage1.find(vertex);
					if(it !=  forbiddenColumnMap.at(column).at(refVertex).stage1.end())
						forbiddenColumnMap.at(column).at(refVertex).stage1.erase(vertex);
				}
				else if(stage == 2) {
					unordered_set<int>::iterator it = forbiddenColumnMap.at(column).at(refVertex).stage2.find(vertex);
					if(it !=  forbiddenColumnMap.at(column).at(refVertex).stage2.end())
						forbiddenColumnMap.at(column).at(refVertex).stage2.erase(vertex);
					
				}
				else
					throw "lvl = 1 ou 2";
			}
			catch(string e) {
				cout << e << endl;
				exit(EXIT_FAILURE);
			}
		}
		
		/**
		 Renvoi le set associé au sommet de référence en fonction du niveau souhaité et de la classe d'interdit
		 column: classe d'interdit
		 refVertex: sommet de référence
		 lvl: niveau d'interdit
		 */
		unordered_set<int> getForbidden(unsigned column, unsigned refVertex, unsigned lvl) const {
			try {
				if(lvl == 1)
					return forbiddenColumnMap.at(column).at(refVertex).stage1;
				else if(lvl == 2)
					return forbiddenColumnMap.at(column).at(refVertex).stage2;
				else
					throw "lvl = 1 ou 2";
			}
			catch(string e) {
				cout << e << endl;
				exit(EXIT_FAILURE);
			}
		}
		
		/**
		 Renvoi vrai si l'élément passé en second paramètre est présent dans le vecteur passé en premier paramètre
		 set: unordered_set
		 elem: élément à rechercher dans le set
		 */
		bool findInSet(unordered_set<int> &set, int elem) {
			unordered_set<int>::iterator it = set.find(elem);
			
			return (it != set.end());
		}
		
		
		/**
		 Cherche si l'on a besoin de créer une nouvelle classe
		 column: une classe dans laquel on va vérifier
		 stage: le vecteur d'interdit d'étage 1
		 vertexToFind: le sommet à trouver
		 */
		bool needAnewClass(unordered_map<unsigned, interdit> &column, unordered_set<int> stage, int vertexToFind) {
			bool res = false;
			
			unordered_set<int>::iterator it1 = stage.begin();
			
			for(; it1 != stage.end(); it1++) {
				if(!findInSet(column.at(*it1).stage1,vertexToFind)
				   && !findInSet(column.at(vertexToFind).stage1, *it1))
					res = true;
			}
			return res;
		}
		
		/**
		 Execute une partie de l'algorithme sur un sommet
		 colum: une classe dans laquel on va traiter les sommets
		 actualStruct: les étages d'interdits d'un sommet
		 itMap1End: l'itérateur de fin de la map de classe
		 vertex: le numéro du sommet actuel
		 */
		void verTreatment(unordered_map<unsigned, interdit> &column, interdit &actualStruct, unordered_map<unsigned,
						  unordered_map<unsigned, interdit>>::iterator &itMap1End, unsigned vertex) {
			unordered_map<unsigned, interdit>::iterator itMap2b;
			unordered_set<int>::iterator itSetB;
			
			unordered_set<int>::iterator itStage1;
			unordered_set<int>::iterator itStage1End = actualStruct.stage1.end();
			unordered_set<int>::iterator itStage2 = actualStruct.stage2.begin();
			unordered_set<int>::iterator itStage2End = actualStruct.stage2.end();
			
			unordered_set<int> valueToSupressInVect2;
			unordered_set<int> prohib = actualStruct.stage1;
			
			int columnkey, elemSet1, elemSet2;
			
			//Parcours de l'étage 2
			while((!actualStruct.stage2.empty()) && (itStage2 != itStage2End)) {
				elemSet2 = *itStage2;
				itStage1 = actualStruct.stage1.begin();
				itStage1End = actualStruct.stage1.end();
				//Parcours de l'étage 1
				while((!actualStruct.stage1.empty()) && (itStage1 != itStage1End)) {
					elemSet1 =  *itStage1;
					
					//Si élément d'étage 1 n'est pas présent dans liste d'interdit du sommet lu dans l'étage2
					//et que l'élément lu de l'étage 1 ne comprend pas dans ses interdits l'élément lu de l'étage 2 alors
					if(!findInSet(column.at(elemSet2).stage1, elemSet1)
					   && !findInSet(column.at(elemSet1).stage1, elemSet2)
					   && !actualStruct.stage2.empty()) {
						columnkey = generalColumnKey;
//						cout << " ----- AVANT TRAITEMENT -----" << endl;
//						printTable();
						//Création d'une nouvelle classe
						unordered_map<unsigned, interdit> vertexCharpMap(column);
						pair<unsigned, unordered_map<unsigned, interdit>> newPairForMap1 (generalColumnKey, vertexCharpMap);
						generalColumnKey++;
						forbiddenColumnMap.insert(newPairForMap1);
						itMap1End = forbiddenColumnMap.end();
						
						//Supprime tous les éléments des etages 1 et 2 afin de pouvoir insérer la valeur négative dans l'étage 1
						forbiddenColumnMap.at(columnkey).at(elemSet2).stage1.clear();
						forbiddenColumnMap.at(columnkey).at(elemSet2).stage2.clear();

						//Insertion de la valeur négative dans l'étage 1
						forbiddenColumnMap.at(columnkey).at(elemSet2).stage1.insert(-(elemSet1));
//						cout << " ----- APRES TRAITEMENT -----" << endl;
//						printTable();
						//parcours de la nouvelle colonne
						for(itMap2b = forbiddenColumnMap.at(columnkey).begin(); itMap2b != forbiddenColumnMap.at(columnkey).end(); itMap2b++) {
							
							//Parcours du set1
							if((*itMap2b).second.stage1.find(elemSet1) != (*itMap2b).second.stage1.end()) {
								(*itMap2b).second.stage1.erase(elemSet1);
								(*itMap2b).second.stage1.insert(elemSet2);
							}
							
							//parcours set2
							if((*itMap2b).second.stage2.find(elemSet1) != (*itMap2b).second.stage2.end()) {
								(*itMap2b).second.stage2.erase(elemSet1);
								(*itMap2b).second.stage2.insert(elemSet2);
							}
						}
						supprimeInterdit(columnkey,vertex, elemSet2, 2);
						
						//insertion en niveau 1
						if(!findInSet(forbiddenColumnMap.at(columnkey).at(vertex).stage1, elemSet2))
							forbiddenColumnMap.at(columnkey).at(vertex).stage1.insert(elemSet2);
						
						//insertion du sommet dans la liste d'interdit de second niveau
						column.at(elemSet1).stage2.insert(elemSet2);
						
						verTreatment(column, column.at(elemSet1),itMap1End, elemSet1);
						
						//Insert la valeur de l'étage 2 dans l'étage 1
						prohib.insert(elemSet2);
						
						//suppression en niveau 2
						valueToSupressInVect2.insert(elemSet2);
					}
					else {
						if(!actualStruct.stage2.empty()
						   && !findInSet(prohib,elemSet2)
						   && !needAnewClass(column, actualStruct.stage1, elemSet2)) {
							prohib.insert(elemSet2);
							//SIPROB
							//itStage2 = actualStruct.stage2.erase(elemSet2);
							valueToSupressInVect2.insert(elemSet2);
							//itStage2End = actualStruct.stage2.end();
						}
					}
					
					if(!actualStruct.stage1.empty())
						itStage1++;
				}
				actualStruct.stage1 = prohib;
				
				if(!actualStruct.stage2.empty() && (itStage2 != actualStruct.stage2.end()))
					itStage2++;
			}
			unordered_set<int>::iterator itSet2B;
			//Supression des valeurs au niveau 2
			for(itSetB = valueToSupressInVect2.begin(); itSetB != valueToSupressInVect2.end(); itSetB++) {
				itSet2B = actualStruct.stage2.find(*itSetB);
				if(itSet2B != actualStruct.stage2.end()){
					actualStruct.stage2.erase(*itSet2B);
					itStage2End = actualStruct.stage2.end();
				}
			}
		}
		
		/**
		 * Execute l'algorithme
		 */
		void treatment() {
			unordered_map<unsigned, unordered_map<unsigned, interdit>>::iterator itMap1;
			unordered_map<unsigned, unordered_map<unsigned, interdit>>::iterator itMap1End;
			
			unordered_map<unsigned, interdit>::iterator itMap2;
			unordered_map<unsigned, interdit>::iterator itMap2End;
			
			itMap1End = forbiddenColumnMap.end();
			//Parcours des colonnes
			for(itMap1 = forbiddenColumnMap.begin(); itMap1 != itMap1End; itMap1 ++) {
				cout << endl << endl << "Classe de base :" << endl;
				printTable();
				cout << endl;
				itMap2End = itMap1->second.end();
				//Parcours des lignes (sommets)
				for(itMap2 = (*itMap1).second.begin(); itMap2 != itMap2End; itMap2++) {
					//si l'étage 2 du sommet n'est pas vide
					if(!itMap2->second.stage2.empty()) {
						verTreatment(itMap1->second, itMap2->second, itMap1End, itMap2->first);
					}
				}
			}
		}
		
		/**
		 * Print le tableau
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
					for(itSet = (*itMap2).second.stage1.begin(); itSet != (*itMap2).second.stage1.end(); itSet++) {
						cout << " " << (*itSet);
					}
					cout << "]";
					
					cout << "[";
					for(itSet = (*itMap2).second.stage2.begin(); itSet != (*itMap2).second.stage2.end(); itSet++) {
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