#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include "expat.h"

using namespace std;

static void startElement(void *userData, const char *name, const char **atts)
{
  /*	printf("\n %s", name);
  	while (*atts)
	{
    	printf(" %s='%s'", atts[0], atts[1]);
    	atts += 2;
  	}*/
}

static void charHandler(void *userData, const XML_Char *s, int len)
{
  	for ( int i = 0; i < len; i++)
    	putchar( s[i] );
}

static void endElement(void *userData, const char *name)
{
	//printf("\n%s\n", name);
}

int main( int argc, char *argv[] )
{
	/*
	/////////// Process command-line options //////////////////////////////
	::opterr = 1;
	char const opts[] = "v";
	for ( int opt; (opt = ::getopt( argc, argv, opts )) != EOF; )
		switch ( opt ) {

			case 'v': // Display version and exit.
				cout << "parser_html_test " << version << endl;
				::exit( 0 );

			case '?': // Bad option.
				usage();
		}
	argc -= ::optind, argv += ::optind;
	if ( argc > 1 )
		usage();

	/////////// Get file file to parse ////////////////////////////////////
	char const *file_name;
	if ( !argc )
	{
		static char file_name_buf[ NAME_MAX + 1 ];
		if ( !cin.getline( file_name_buf, NAME_MAX ) )
			::exit( 2 );
		file_name = file_name_buf;
	}
	else
	{
		file_name = *argv;
	}
	*/
	string fileName = "test.html";

	XML_Parser parser = XML_ParserCreate( NULL );
    XML_SetUserData( parser, NULL );
	XML_SetElementHandler( parser, startElement, endElement );
    XML_SetCharacterDataHandler( parser, charHandler );

	std::ifstream xmlFile( fileName.c_str() );
    if ( ! xmlFile.is_open() )
    {
    	std::cout << "Unable to open " << fileName << "\n";
        return 0;
	}

	std::string buffer;
	bool done;
	do
	{
		std::getline(xmlFile, buffer);
		done = xmlFile.fail();
		if ( ! XML_Parse(parser, buffer.c_str(), buffer.length(), done) )
		{
			std::cout << XML_ErrorString(XML_GetErrorCode(parser)) << " At line " << XML_GetCurrentLineNumber(parser);
			break;
		}
	}
	while (!done);

	XML_ParserFree(parser);
 	return 0;
}

//*****************************************************************************
//
//	Miscellaneous function(s)
//
//*****************************************************************************

void usage() {
	cerr <<	"usage: parser_html_test [options] [file]\n"
	"options:\n"
	"========\n"
	"-v : Print version number and exit\n";
	::exit( 1 );
}
