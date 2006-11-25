#ifndef __PARSER_WRAPPER__
#define __PARSER_WRAPPER__

#include <string>
#include "expat.h"
#include "InvalidXmlException.h"
#include "CantOpenFileException.h"

using namespace std;

class ParserWrapper
{
	private:
		XML_Parser _parser;

		static void endElement   ( void *userData, const char *name );
		static void charHandler  ( void *userData, const XML_Char *s, int len );
		static void startElement ( void *userData, const char *name, const char **atts );

	public:
		ParserWrapper( void *userData );
		void Parse( string fileName ) throw(InvalidXmlException, CantOpenFileException);
		~ParserWrapper();
};

#endif
