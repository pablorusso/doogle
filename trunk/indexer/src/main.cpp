#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include "expat.h"

using namespace std;

static void usage()
{
	cerr <<	"usage: indexer [path]\n ex: indexer /home/me/html/\n";
	::exit( 1 );
}

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
	string word = "";
	// lexico miLexico = ( Lexico ) *userData;
 	for ( int i = 0; i < len; i++)
	{
		if ( s[i] != ' ' )
		{
			word += s[i];
		}
		else
		{
			if ( word != "" )
			{
				cout << endl << word;
				// lexico.addWord( word );
				word = "";
			}
		}
	}

	if ( word != "" )
	{
		cout << endl << word;
		word = "";
	}
}

static void endElement(void *userData, const char *name)
{
	//printf("\n%s\n", name);
}

int selecc_arch( const struct dirent *entry )
{
	if ( (strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0) )
		return 0;
	else
		return -1;
}

void parsearPath( string path )
{
	int count, i;
	struct direct **files = NULL;
	int selecc_arch( const struct dirent *entry );

	cout << endl << endl << "Listando contenido de la carperta: " << path;
    count = scandir ( path.c_str(), &files, selecc_arch, NULL);
	if ( count <= 0 )
	{
		cout << "No hay archivos en la carpeta";
		exit(0);
	}


	for ( i=0; i < count; ++i )
	{
		string fileName = path + "/" + files[i]->d_name;
		cout << endl << "Examinando: " << fileName << " - ";

		std::ifstream xmlFile( fileName.c_str() );
		if ( ! xmlFile.is_open() )
		{
			std::cout << "No se pudo abrir el archivo";
		}
		else
		{
			XML_Parser parser = XML_ParserCreate( NULL );
			XML_SetUserData( parser, NULL );
			XML_SetElementHandler( parser, startElement, endElement );
			XML_SetCharacterDataHandler( parser, charHandler );

			std::string buffer;
			bool done   = false;
			bool failed = false;

			std::getline( xmlFile, buffer );
			if ( xmlFile.fail() )
			{
				// En caso de un archivo vacio, fallaria, hay que ver como determinar que algo es una carpeta
				cout << " Entrando en subcarpeta: " << fileName;
				parsearPath ( fileName );
				failed = true;
			}

			while ( !done && !failed )
			{
				if ( ! XML_Parse(parser, buffer.c_str(), buffer.length(), done) )
				{
					std::cout << "El archivo no esta bien formado" ;
					failed = true;
					// std::cout << XML_ErrorString(XML_GetErrorCode(parser)) << " At line " << XML_GetCurrentLineNumber(parser);
				}

				std::getline(xmlFile, buffer);
				done = xmlFile.fail();
			}

			XML_ParserFree(parser);

			if ( !failed )
				std::cout << "Lexico importado con exito";
		}
	}
}

int main( int argc, char *argv[] )
{
	if ( argc != 2 )
		usage();
	string path( argv[1] );

	parsearPath( path );

	std::cout << endl;

 	return 0;
}
