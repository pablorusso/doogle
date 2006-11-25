#ifndef __PARSER_WRAPPER__
#define __PARSER_WRAPPER__

#include <string>
#include "Lexical.h"
#include "expat.h"
#include "InvalidXmlException.h"
#include "CantOpenFileException.h"

using namespace std;

class ParserWrapper
{
	private:
		ifstream *_xmlFile;
		XML_Parser _parser;

		static void endElement   ( void *userData, const char *name );
		static void charHandler  ( void *userData, const XML_Char *s, int len );
		static void startElement ( void *userData, const char *name, const char **atts );

	public:
		ParserWrapper( string fileName, Lexical* lexico );
		void Parse() throw(InvalidXmlException, CantOpenFileException);
		~ParserWrapper();
};

#endif

