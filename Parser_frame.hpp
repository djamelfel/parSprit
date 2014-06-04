/*
 Code permettant de parser le fichier de charpente
 */

#ifndef PARSER_FRAME_HPP
#define PARSER_FRAME_HPP

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <string>
#include <vector>


namespace parser
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
	namespace phoenix = boost::phoenix;
	
    template <typename Iterator>
	/**
	 Permet de parser une ligne du fichier de charpente
	 first: itérateur de début de la ligne
	 last: itérateur de fin de la ligne
	 node: numéro du noeud courant
	 floor_1: vecteur des interdits de bas étage
	 floor_2: vecteur des interdits de haut étage
	 */
    bool parse_frame_line(Iterator first, Iterator last, int &node, std::vector<int> &floor_1, std::vector<int> &floor_2)
    {
        using qi::int_;
        using qi::phrase_parse;
	    using qi::_1;
        using ascii::space;
		
        bool r = phrase_parse(
							  first,                          /* itérateur début */
							  last,                           /* itérateur fin */
							  (
							   int_[phoenix::ref(node)=_1] >>
							   '[' >> -(int_[phoenix::push_back(phoenix::ref(floor_1), _1)] % ',') >>
							   ']' >> -('[' >> -(int_[phoenix::push_back(phoenix::ref(floor_2), _1)] % ',') >>
										']')
							   ),						/* Syntaxe d'une ligne : int '[' int, int, ... ']' '[' int, int, ... ']'
														 chaque liste d'entiers peut avoir de 0 à n membres
														 la deuxième liste est facultative */
							  space                           /* on ignore les espaces */
							  );
        if (first != last) /* échec si la syntaxe n'est pas respectée */
            return false;
        return r;
    }
	
	template <typename Iterator>
	/**
	 Permet de parser la premiere ligne di fichier de charpente
	 */
	bool parse_first_line(Iterator first, Iterator last, int &nodeQty)
	{
		using qi::int_;
        using qi::phrase_parse;
	    using qi::_1;
	    using qi::lit;
        using ascii::space;
		
        bool r = phrase_parse(
							  first,                          /* itérateur début */
							  last,                           /* itérateur fin */
							  ( (lit("o") | lit("O")) >> "rdre" >> int_[phoenix::ref(nodeQty)=_1] ),		/* Syntaxe de la ligne : "ordre" int */
							  space                           /* on ignore les espaces */
							  );
        if (first != last) /* échec si la syntaxe n'est pas respectée */
            return false;
        return r;
    }
}

#endif
