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

void listarArchivos( string lexFN, string docFN, string docIdxFN, string docLexFN, string docLexIdxFN)
{
	ArchivoLexico     *ptrArch1 = new ArchivoLexico( lexFN, LEER );
	ArchivoDocumentos *ptrArch2 = new ArchivoDocumentos( docFN, docIdxFN, LEER );
	ArchivoDocLexico  *ptrArch3 = new ArchivoDocLexico( docLexFN, docLexIdxFN, LEER );
	try
	{
		LexicoData dato1;
		DocumentData dato2;
		DocLexicoData dato3;

		cout << endl << "LEXICO" << endl;
		ptrArch1->comenzarLectura();
		while( !ptrArch1->fin() )
		{
			ptrArch1->leer( dato1 );
			cout << "** id: " << dato1.id << " - palabra: " << dato1.termino << endl;
		}

		cout << endl << "DOCUMENTOS" << endl;
		ptrArch2->comenzarLectura();
		while( !ptrArch2->fin() )
		{
			ptrArch2->leer( dato2 );
			cout << "** id: " << dato2.id << " - ruta: " << dato2.ruta << " - norma: " << dato2.norma << " - termDist: " << dato2.cantTermDistintos << endl;
		}

		cout << endl << "DOCUMENTO-LEXICO-SEGUIDORES" << endl;
		ptrArch3->comenzarLectura();
		while( !ptrArch3->fin() )
		{
			ptrArch3->leer( dato3 );
			cout << endl << "** id: " << dato3.id << " - TERMINOS: ";
			LexicalPair::iterator curr = dato3.terminos.begin();
			while ( curr != dato3.terminos.end() )
			{
				int idTermino = static_cast< int >( curr->first );
   				long peso = static_cast< long >( curr->second );
   				cout << "(" << idTermino << ", " << peso << ") ";
				++curr;
			}

			cout << "SEGUIDORES: ";
			Seguidores::iterator currSeg = dato3.seguidores.begin();
			while ( currSeg != dato3.seguidores.end() )
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

	delete ptrArch1;
	delete ptrArch2;
	delete ptrArch3;

	cout << endl;
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

		string docFN    = "";
		string docIdxFN = "";
		string docLexFN = "";
		string docLexIdxFN = "";
		string lexFN    = "";

		Indexer *indexer = new Indexer( "/home/pablo/facultad/output/" );
		try
		{
			lexFN = indexer->lexicalFileName();
			docFN = indexer->documentsFileName();
			docIdxFN = indexer->documentsIdxFileName();
			docLexFN = indexer->docLexFileName();
			docLexIdxFN = indexer->docLexIdxFileName();

			indexer->buildIndex( path );
			cout << endl;
		}
		catch ( string ex )
		{
			cout << endl << "Error indexando: " << ex << endl;
		}
		delete indexer;

		listarArchivos( lexFN, docFN, docIdxFN, docLexFN, docLexIdxFN );
	}
	catch ( string ex )
	{
		cout << endl << "Error: " << ex << endl;
	}

 	return 0;
}


/*
float cosineRank( LexicalPair []pairList1, float norm1, int pairList1Count, LexicalPair []pairList2, float norm2, int pairList2Count )
{
	int i = 0;
	int j = 0;

	AxBxC / n(A)
	float numerador   = 0;
	float denominador = 0;
	while( i < pairList1Count )
	{
		while ( i < pairList1Count && j < pairList2Count )
		{
			int id1 = pairList1[i].GetId();
			int id2 = pairList2[j].GetId();

			if ( id1 == id2 )
			{
				numerador   = numerador + pairList1[i].GetWeight() * pairList2[j].GetWeight();
				denominador = denominador + ( pairList1[i].GetWeight() + pairList2[j].GetWeight() ) * ( pairList1[i].GetWeight() + pairList2[j].GetWeight() );
				i++;
				j++;
			}
			else
			{
				if ( id1 > id2 )
				{
					j++;
				}
				else
				{
					i++
				}
			}
		}
	}

	if ( denominador == 0 )
		return 1;

	// falta la raiz cuadrada y ver como usar las normas
	return numerador / denominador;
}
*/
