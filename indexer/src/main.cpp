#include <cstdlib>
#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>

#include "ParserWrapper.h"
#include "Lexical.h"

using namespace std;

static void usage()
{
	cerr <<	"usage: indexer [path]\n ex: indexer /home/me/html/\n";
	::exit( 1 );
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

	cout << endl << "Listando contenido de la carperta: " << path << endl;
    count = scandir ( path.c_str(), &files, selecc_arch, NULL);
	if ( count <= 0 )
	{
		cout << "No hay archivos en la carpeta";
		exit(0);
	}

	for ( i=0; i < count; ++i )
	{
		string fileName = path + "/" + files[i]->d_name;

		// Me fijo si es una carpeta
		bool isAFile = true;
		 if( chdir( fileName.c_str() ) == 0 )
			 isAFile = false;

		cout << "Importando lexico desde " << fileName << ": ";
		if ( isAFile )
		{
			Lexical *lex = new Lexical();

			try
			{
				ParserWrapper parser ( fileName, lex );
				parser.Parse();

				std::cout << "Lexico importado con exito" << endl;
			}
			catch ( InvalidXmlException ex )
			{
				std::cout << "Archivo mal formado. Error: " << ex.what() << endl;
			}
			catch ( CantOpenFileException )
			{
				std::cout << "No se puede abrir el archivo" << endl;
			}

			delete lex;
		}
		else
		{
			cout << "Es un directorio, navegando hacia adentro..." << endl;
			parsearPath( fileName );
		}
	}
}

int main( int argc, char *argv[] )
{
	if ( argc != 2 ) usage();
	string path( argv[1] );
	parsearPath( path );

	std::cout << endl;
 	return 0;
}
