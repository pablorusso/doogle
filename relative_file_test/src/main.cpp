#include <iostream>
#include <cstdlib>
#include <sstream>

#include "ArchivoLexico.h"
#include "ArchivoDocumentos.h"
#include "ArchivoDocLexico.h"
#include "ArchivoConsts.h"


using namespace std;

void probarArchivoLexico( string name )
{
	cout << endl << "LEXICO" << endl;

	LexicoData dato;
	ArchivoLexico *ptrArch = new ArchivoLexico( name , ESCRIBIR );
	try
	{
		for( int i=1; i <= 5 ; i++ )
		{
			ostringstream os; os << i;
			dato.termino = "termino " + os.str();
			ptrArch->escribir( dato );
		}
	}
	catch ( string a )
	{
		cout << endl << a << endl ;
	}
	delete ptrArch;

	ptrArch = new ArchivoLexico( name , LEER );
	try
	{
		ptrArch->comenzarLectura();
		while( !ptrArch->fin() )
		{
			ptrArch->leer( dato );
			cout << "** id: " << dato.id << " - palabra: " << dato.termino << endl;
		}
	}
	catch ( string a )
	{
		cout << endl << a << endl ;
	}
	delete ptrArch;
}

void probarArchivoDocumentos( string name )
{
	cout << endl << "DOCUMENTOS" << endl;
	DocumentData dato;
	ArchivoDocumentos *ptrArch = new ArchivoDocumentos( name, "I_" + name, ESCRIBIR );
	try
	{
		for( int i=1; i <= 5 ; i++ )
		{
			ostringstream os; os << i;

			dato.ruta = "/usr/data/test" + os.str() + ".html";
			dato.norma = 1.234;
			dato.cantTermDistintos = 15;
			ptrArch->escribir( dato );
		}
	}
	catch ( string a )
	{
		cout << endl << a << endl ;
	}
	delete ptrArch;

	ptrArch = new ArchivoDocumentos( name, "I_" + name, LEER );
	try
	{
		ptrArch->comenzarLectura();
		while( !ptrArch->fin() )
		{
			ptrArch->leer( dato );
			cout << "** id: " << dato.id << " - ruta: " << dato.ruta << " - norma: " << dato.norma << " - termDist: " << dato.cantTermDistintos << endl;
		}
	}
	catch ( string a )
	{
		cout << endl << a << endl ;
	}
	delete ptrArch;
}

void probarArchivoDocLexico( string name )
{
	cout << endl << "DOCUMENTO-LEXICO-SEGUIDORES" << endl;

	DocLexicoData dato;
	ArchivoDocLexico *ptrArch = new ArchivoDocLexico( name, "I_" + name, ESCRIBIR );
	try
	{
		for( int i = 1; i <= 5 ; i++ )
		{
			ostringstream os; os << i;

			dato.seguidores[1] = 1000;
			dato.seguidores[3] = 3000;
			dato.seguidores[5] = 5000;

			dato.terminos[1] = 1;
			dato.terminos[3] = 3;
			dato.terminos[5] = 5;

			ptrArch->escribir( dato );
		}
	}
	catch ( string a )
	{
		cout << endl << a << endl ;
	}
	delete ptrArch;

	ptrArch = new ArchivoDocLexico( name, "I_" + name, LEER );
	try
	{
		ptrArch->comenzarLectura();
		while( !ptrArch->fin() )
		{
			ptrArch->leer( dato );
			cout << endl << "** id: " << dato.id << " - TERMINOS: ";
			LexicalPair::iterator curr = dato.terminos.begin();
			while ( curr != dato.terminos.end() )
			{
				int idTermino = static_cast< int >( curr->first );
   				long peso = static_cast< long >( curr->second );
   				cout << "(" << idTermino << ", " << peso << ") ";
				++curr;
			}

			cout << "SEGUIDORES: ";
			Seguidores::iterator currSeg = dato.seguidores.begin();
			while ( currSeg != dato.seguidores.end() )
			{
				int idDoc = static_cast< int >( currSeg->first );
   				long offset = static_cast< long >( currSeg->second );
   				cout << "(" << idDoc << ", " << offset << ") ";
				++currSeg;
			}
		}
	}
	catch ( string a )
	{
		cout << endl << a << endl ;
	}
	delete ptrArch;

	cout << endl;
}


int main(int argc, char *argv[])
{
	probarArchivoLexico( "lexico.dat" );
	probarArchivoDocumentos( "documentos.dat" );
	probarArchivoDocLexico( "docLexico.dat" );
    return 0;
}
