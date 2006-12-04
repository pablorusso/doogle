#include <cstdlib>
#include <string>
#include <iostream>

#include "Indexer.h"

using namespace std;

void usage()
{
	cerr <<	"usage: indexer [inputPath] [outputPath]\n ex: indexer /home/pablo/facultad/input /home/pablo/facultad/output\n";
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

void listarArchivos( string targetFolder, string lexFN, string docFN, string docIdxFN, string liderFN, string liderIdxFN, string seguidorFN, string seguidorIdxFN, string noLiderFN, string noLiderIdxFN )
{
	lexFN = targetFolder + lexFN;
	docFN = targetFolder + docFN;
	docIdxFN = targetFolder + docIdxFN;
	liderFN = targetFolder + liderFN;
	liderIdxFN = targetFolder + liderIdxFN;
	seguidorFN = targetFolder + seguidorFN;
	seguidorIdxFN = targetFolder + seguidorIdxFN;
	noLiderFN = targetFolder + noLiderFN;
	noLiderIdxFN = targetFolder + noLiderIdxFN;

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
		string outputPath;
		string inputPath;
		if ( argc != 3 )
		 	usage();
		else
		{
			// inputPath = "/home/pablo/facultad/input";
			// outputPath = "/home/pablo/facultad/output";
			inputPath  = argv[1];
			outputPath = argv[2];
		}

		if ( outputPath[ outputPath.length()-1 ] != '/' )
			outputPath = outputPath + "/";

		if ( inputPath[ inputPath.length()-1 ] == '/' )
			inputPath.erase( inputPath.length()-1, 1 );

		Indexer *indexer = new Indexer( outputPath );
		try
		{
			indexer->buildIndex( inputPath );
			cout << endl;
		}
		catch ( string ex )
		{
			cout << endl << "Error indexando: " << ex << endl;
		}
		delete indexer;

		listarArchivos( outputPath, LEX_FN, DOC_FN, DOC_IDX_FN, LEAD_FN, LEAD_IDX_FN, FOLLOW_FN, FOLLOW_IDX_FN, NOLEAD_FN, NOLEAD_IDX_FN );
	}
	catch ( string ex )
	{
		cout << endl << "Error: " << ex << endl;
	}

 	return 0;
}
