#ifndef PARSER_FRAME_HPP
#define PARSER_FRAME_HPP

/*!
 * \file Parser_frame.h
 * \brief Récupere les informations fournis par le fichier texte
 * \author djamel fellah - quentin barthelemy
 * \version 3.7
 */

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <string>
#include <vector>

/*! \namespace frame */
namespace parser
{
	/*! \namespace qi */
    namespace qi = boost::spirit::qi;
    
	/*! \namespace ascii */
	namespace ascii = boost::spirit::ascii;
	
	/*! \namespace phoenix */
	namespace phoenix = boost::phoenix;
	
	/*!
	 *	\brief parse ligne
	 *
	 *	Permet de parser une ligne du fichier de charpente
	 *
	 *	\param debut : itérateur de début de la ligne
	 *	\param fin : itérateur de fin de la ligne
	 *	\param noeud : numéro du noeud courant
	 *	\param etage1 : vecteur des interdits de bas étage
	 *	\param etage2 : vecteur des interdits de haut étage
	 */
	template <typename Iterator>
    bool parse_ligne(Iterator debut, Iterator fin, int &noeud, std::vector<int> &etage1, std::vector<int> &etage2) {
        using qi::int_;
        using qi::phrase_parse;
	    using qi::_1;
        using ascii::space;
		
		/*!
		 *	\brief phrase_parse
		 *
		 *	parse une ligne du fichier de charpente
		 *
		 *	\param debut : itérateur de début de la ligne
		 *	\param fin : itérateur de fin de la ligne
		 *	\param string : syntaxe d'une ligne de fichier
		 *	\param space : on ignore les espaces
		 */
        bool resultat = phrase_parse( debut, fin, ( int_[phoenix::ref(noeud)=_1] >> '[' >> -(int_[phoenix::push_back(phoenix::ref(etage1), _1)] % ',') >> ']' >> -('[' >> -(int_[phoenix::push_back(phoenix::ref(etage2), _1)] % ',') >> ']') ), space );
        
		if (debut != fin)
            return false;
        
		return resultat;
    }
	
	/*!
	 *	\brief parse premiere ligne
	 *
	 *	Permet de parser la premiere ligne di fichier de charpente
	 *
	 *	\param debut : itérateur de début de la ligne
	 *	\param fin : itérateur de fin de la ligne
	 *	\param nbrNoeud : nombre de noeud
	 */
	template <typename Iterator>
	bool parse_premiere_ligne(Iterator debut, Iterator fin, int &nbrNoeud) {
		using qi::int_;
        using qi::phrase_parse;
	    using qi::_1;
	    using qi::lit;
        using ascii::space;
		
        bool resultat = phrase_parse( debut, fin, ( (lit("o") | lit("O")) >> "rdre" >> int_[phoenix::ref(nbrNoeud)=_1] ), space );
        
		if (debut != fin)
            return false;
        
		return resultat;
    }
}

#endif
