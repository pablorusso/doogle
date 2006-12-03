#ifndef __PARSER_WRAPPER__
#define __PARSER_WRAPPER__

#include <string>
#include "DocLexicoData.h"
#include "expat.h"

using namespace std;

class ParserWrapper
{
	private:
		static void charHandler  ( void *userData, const XML_Char *s, int len );
	public:
		static WordPair *Parse( string fileName );
};

#endif

