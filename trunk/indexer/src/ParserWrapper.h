#ifndef __PARSER_WRAPPER__
#define __PARSER_WRAPPER__

#include <string>
#include "ArchivoLexico.h"
#include "expat.h"

using namespace std;

class ParserWrapper
{
	private:
		ifstream *_xmlFile;
		XML_Parser _parser;

		static void charHandler  ( void *userData, const XML_Char *s, int len );
	public:
		ParserWrapper( string fileName, ArchivoLexico &lexico );
		LexicalPair Parse();
		~ParserWrapper();
};

#endif

