#include <cstdlib>
#include <string>
#include <iostream>

#include "Indexer.h"

using namespace std;

void usage()
{
	cerr <<	"usage: indexer [path]\n ex: indexer /home/me/html/\n";
	::exit( 1 );
}

void listarDocLexicoData( DocLexicoData dato )
{
	cout << "** id: " << dato.id << " - TERMINOS: ";
	LexicalPair::iterator curr = dato.terminos.begin();
	while ( curr != dato.terminos.end() )
	{
		int idTermino = static_cast< int >( curr->first );
		long peso = static_cast< long >( curr->second );
		cout << "(" << idTermino << ", " << peso << ") ";
		++curr;
	}
}

void listarArchivos( string lexFN, string docFN, string docIdxFN, string liderFN, string liderIdxFN, string seguidorFN, string seguidorIdxFN, string noLiderFN, string noLiderIdxFN )
{
	ArchivoLexico     *lexicoFile 		= new ArchivoLexico( lexFN, LEER );
	ArchivoDocumentos *documentosFile 	= new ArchivoDocumentos( docFN, docIdxFN, LEER );
	ArchivoDocLexico  *lideresFile 		= new ArchivoDocLexico( liderFN, liderIdxFN, LEER );
	ArchivoDocLexico  *seguidoresFile 	= new ArchivoDocLexico( seguidorFN, seguidorIdxFN, LEER );
	ArchivoDocLexico  *sinLiderFile 	= new ArchivoDocLexico( noLiderFN, noLiderIdxFN, LEER );

	LexicoData    dato1;
	DocumentData  dato2;
	DocLexicoData dato3;
	DocLexicoData dato4;
	try
	{
		cout << endl << "LEXICO";
		lexicoFile->comenzarLectura();
		while( lexicoFile->leer( dato1 ) )
			cout << endl << "** id: " << dato1.id << " - palabra: " << dato1.termino;

		cout << endl << "DOCUMENTOS";
		documentosFile->comenzarLectura();
		while( documentosFile->leer( dato2 ) )
			cout << endl << "** id: " << dato2.id << " - ruta: " << dato2.ruta << " - norma: " << dato2.norma << " - termDist: " << dato2.cantTermDistintos;

		cout << endl << "DOCUMENTO-TERMINOS";
		lideresFile->comenzarLectura();
		while( lideresFile->leer( dato3 ) )
		{
			cout << endl << "Lider: ";
			listarDocLexicoData( dato3 );

			Seguidores::iterator currSeg = dato3.seguidores.begin();
			while ( currSeg != dato3.seguidores.end() )
			{
   				long offset = static_cast< long >( currSeg->second );
				cout << endl << "-------->Seguidor: ";
				seguidoresFile->leerOffset( offset, dato4 );
   				listarDocLexicoData( dato4 );

				++currSeg;
			}
		}


		cout << endl << "DOCUMENTO-TERMINOS SIN LIDER";
		sinLiderFile->comenzarLectura();
		while( sinLiderFile->leer( dato3 ) )
		{
			cout << endl << "Sin lider: ";
			listarDocLexicoData( dato3 );
		}
	}
	catch ( string a )
	{
		cout << endl << a << endl ;
	}

	delete lexicoFile;
	delete documentosFile;
	delete lideresFile;
	delete seguidoresFile;
	delete sinLiderFile;

	cout << endl << endl;
}


int main( int argc, char *argv[] )
{
	try
	{
		string path;
		if ( argc != 2 )
			path = "/home/pablo/facultad/input"; //usage();
		else
			path = argv[1];

		string lexFN    	 = "";
		string docFN 		 = "";
		string docIdxFN 	 = "";
		string liderFN 		 = "";
		string liderIdxFN 	 = "";
		string seguidorFN 	 = "";
		string seguidorIdxFN = "";
		string noLiderFN 	 = "";
		string noLiderIdxFN  = "";

		Indexer *indexer = new Indexer( "/home/pablo/facultad/output/" );
		try
		{
			lexFN    	 	= indexer->lexicalFileName();
			docFN 		 	= indexer->documentsFileName();
			docIdxFN 	 	= indexer->documentsIdxFileName();
			liderFN 		= indexer->leaderFileName();
			liderIdxFN 	 	= indexer->leaderIdxFileName();
			seguidorFN 	 	= indexer->followersFileName();
			seguidorIdxFN 	= indexer->followersIdxFileName();
			noLiderFN 	 	= indexer->noLeaderFileName();
			noLiderIdxFN  	= indexer->noLeaderIdxFileName();

			indexer->buildIndex( path );
			cout << endl;
		}
		catch ( string ex )
		{
			cout << endl << "Error indexando: " << ex << endl;
		}
		delete indexer;

		listarArchivos( lexFN, docFN, docIdxFN, liderFN, liderIdxFN, seguidorFN, seguidorIdxFN, noLiderFN, noLiderIdxFN );
	}
	catch ( string ex )
	{
		cout << endl << "Error: " << ex << endl;
	}

 	return 0;
}
