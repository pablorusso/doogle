#include "Indexer.h"
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>

struct CantTermsComparer
{
     bool operator()( const DocLexicoData & a, const DocLexicoData & b )
     {
          return a.terminos.size() > b.terminos.size();
     }
};

Indexer::Indexer( string targetFolder )
{
	_targetFolder = targetFolder;

	remove( documentsFileName().c_str() );
	remove( documentsIdxFileName().c_str() );
	remove( docLexFileName().c_str() );
	remove( docLexIdxFileName().c_str() );
	remove( lexicalFileName().c_str() );

	_document = new ArchivoDocumentos ( Indexer::documentsFileName(), Indexer::documentsIdxFileName(), ESCRIBIR | LEER );
}

string Indexer::documentsFileName()
{
	return _targetFolder + "documentos.dat";
}
string Indexer::documentsIdxFileName()
{
	return _targetFolder + "I_documentos.dat";
}
string Indexer::docLexFileName()
{
	return _targetFolder + "documentoLexico.dat";
}
string Indexer::docLexIdxFileName()
{
	return _targetFolder + "I_documentoLexico.dat";
}
string Indexer::lexicalFileName()
{
	return _targetFolder + "lexico.dat";
}

float Indexer::deltaCosineForEqual()
{
	// TODO: Parametrizar empiricamente...
	return 0.2;
}

int Indexer::fileFilter( const struct dirent *entry )
{
	if ( (strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0) )
		return 0;
	else
		return -1;
}

double Indexer::calcularNorma( LexicalPair &items, long &cantTerminos )
{
	long pesoTotal = 0;
	cantTerminos = 0;
	LexicalPair::iterator curr = items.begin();
	while ( curr != items.end() )
	{
		long peso = static_cast< long >( curr->second );
		pesoTotal += peso * peso;
		cantTerminos++;

		++curr;
	}

	return sqrt( pesoTotal );
}

void Indexer::buildLeaders()
{
	/*
	// ver como se calcula esto
	int maxLeaders   = docCount / 2;
	int maxFollowers = docCount / 2;

	DocumentLexical leaders("leaders.dat");
	DocumentLexical followers("followers.dat");
	DocumentLexical withoutLeader("noleader.dat");

	docLex.StartRead();
	// El primero siempre es lider (porque vienen ordenados por cantidad de lexico distinto)
	if ( DocLexicoData item = docLex.Read() )
		learders.AddRecord( item );

	// Para cada documento
	while ( DocLexicoData item = docLex.Read() )
	{
		DocLexicoData bestLeader = NULL;
		bool isFollower = false;
		float lessRank  = 0;


		leaders.StartRead();
		// Para cada lider
		while ( DocLexicoData leader = leaders.Read() && ! isFollower )
		{
			// Comparo usando el ranking del coseno el documento con cada lider
			LexicalPair []leaderPairList = leader.GetLexicalPairList();
			LexicalPair []itemPairList   = item.GetLexicalPairList();
			float rank = cosineRank( itemPairList, item.GetNorm(), item.GetLexicalPairCount(), leaderPairList, leader.GetNorm(), leader.GetLexicalPairCount() );

			if ( rank <= DELTA_EQUAL )
			{
				// Si el rankeo del coseno es suficientemente bueno, lo considero seguidor sin ver que pasa mas adelante
				item.SetLeader( leader.GetId() );
				followers.AddRecord( item );

				leader.SetFollowersQuantity( leader.GetFollowersQuantity() + 1 );
				leaders.updateRecord( leader );

				isFollower = true;
			}
			else
			{
				// Si no es tan bueno, me fijo si al menos es el mejor que encontre y me lo guardo
				if ( rank < lessRank )
				{
					lessRank = rank;
					bestLeader = leader;
				}
			}
		}

		if ( ! isFollower )
		{
			// Si no es seguidor natural de nadie

			if ( bestLeader.GetFollowersQuantity < maxFollowers )
			{
				// Si el leader mas cercano todavia acepta seguidores, entonces lo hago seguidor de este
				item.SetLeader( bestLeader.GetId() );
				followers.AddRecord( item );
				bestLeader.SetFollowersQuantity( bestLeader.GetFollowersQuantity() + 1 );
				leaders.updateRecord( bestLeader );
			}
			else
			{
				if ( leaders.GetCount() < maxLeaders )
					// Si aun tengo espacio para liders, lo hago lider
					leaders.AddRecord( item );
				else
					// Si no puede ser ni seguidor ni lider, entonces no encaja con nada y lo mando al otro archivo
					withoutLeader.AddRecord( item );
			}
		}
	}
	*/
}

void Indexer::sortByQuantity()
{
	int partId   = 0;
	uint memSize = 80; // TODO: Deberia depender de la memoria, cada 1 ocupa aprox 120K
	string partName     = _targetFolder + "docLexicoPart";
	string partIdxName  = _targetFolder + "I_docLexicoPart";
	string partName1    = partName + "0";
	string partIdxName1 = partIdxName + "0";

	DocLexicoData data;

	ArchivoDocLexico *documentLexico = new ArchivoDocLexico  ( Indexer::docLexFileName(), Indexer::docLexIdxFileName(), LEER );
	documentLexico->comenzarLectura();
	documentLexico->leer( data );
	while ( ! documentLexico->fin() )
	{
		vector<DocLexicoData> memoria;
		while ( ! documentLexico->fin() && memoria.size() < memSize )
		{
			memoria.push_back( data );
			documentLexico->leer( data );
		}

		// ordeno en memoria
		sort( memoria.begin(), memoria.end(), CantTermsComparer() );

		// Creo la particion
		ostringstream os; os << partId;
		string partitionName = partName + os.str();
		string partitionIdxName = partIdxName + os.str();

		ArchivoDocLexico *particion = new ArchivoDocLexico( partitionName, partitionIdxName, ESCRIBIR | LEER );
		vector< DocLexicoData >::iterator curr;
		for ( curr = memoria.begin(); curr != memoria.end(); curr++ )
			particion->escribir( static_cast< DocLexicoData >( *curr ) );
		delete particion;

		++partId;
	}

	while ( partId > 0 )
	{
		DocLexicoData dato1;
		DocLexicoData dato2;

		ostringstream os2; os2 << partId;

		ArchivoDocLexico *particion1 = new ArchivoDocLexico( partName1, partIdxName1, LEER );

		string partName2    = partName    + os2.str();
		string partIdxName2 = partIdxName + os2.str();
		ArchivoDocLexico *particion2 = new ArchivoDocLexico( partName2, partIdxName2, LEER );

		string partNameNuevo    = partName1 + "tmp";
		string partIdxNameNuevo = partIdxName1 + "tmp";
		ArchivoDocLexico *particionNueva = new ArchivoDocLexico( partNameNuevo, partIdxNameNuevo, ESCRIBIR | LEER );

		particion1->comenzarLectura();
		particion1->leer( dato1 );
		particion2->comenzarLectura();
		particion2->leer( dato2 );
		while ( !particion1->fin() && !particion2->fin() )
		{
			if ( dato1.terminos.size() == dato2.terminos.size() )
			{
				particionNueva->escribir( dato1 );
				particionNueva->escribir( dato2 );

				particion1->leer( dato1 );
				particion2->leer( dato2 );
			}
			else
			{
				if ( dato1.terminos.size() > dato2.terminos.size() )
				{
					particionNueva->escribir( dato1 );
					particion1->leer( dato1 );
				}
				else
				{
					particionNueva->escribir( dato2 );
					particion2->leer( dato2 );
				}
			}
		}


		while ( !particion1->fin() )
		{
			particionNueva->escribir( dato1 );
			particion1->leer( dato1 );
		}
		while ( !particion2->fin() )
		{
			particionNueva->escribir( dato2 );
			particion2->leer( dato2 );
		}

		delete particion1;
		delete particion2;
		delete particionNueva;

		remove ( partName1.c_str() );
		remove ( partIdxName1.c_str() );
		remove ( partName2.c_str() );
		remove ( partIdxName2.c_str() );

		rename ( partNameNuevo.c_str(), partName1.c_str() );
		rename ( partIdxNameNuevo.c_str(), partIdxName1.c_str() );

		--partId;
	}
	delete documentLexico;

	remove ( Indexer::docLexFileName().c_str() );
	remove ( Indexer::docLexIdxFileName().c_str() );
	rename ( partName1.c_str(), Indexer::docLexFileName().c_str() );
	rename ( partIdxName1.c_str(), Indexer::docLexIdxFileName().c_str() );
}

void Indexer::buildLexical( string path, ArchivoDocLexico *documentLexico )
{
	int count, i;
	string tempLex = _targetFolder + "lexicoTemp.dat";
	struct direct **files = NULL;

    count = scandir ( path.c_str(), &files, Indexer::fileFilter, NULL);
	if ( count <= 0 )
		return ;

	for ( i=0; i < count; ++i )
	{
		string fileName = path + "/" + files[i]->d_name;

		// Me fijo si es una carpeta
		bool isAFile = true;
		 if( chdir( fileName.c_str() ) == 0 )
			 isAFile = false;

		if ( isAFile )
		{
			try
			{
				DocLexicoData docLexData;
				WordPair wordsFound;

				ParserWrapper parser( fileName, &wordsFound );
				parser.Parse();

				// GRABO EL LEXICO
				// Paso los terminos nuevos al lexico
				ArchivoLexico *lexical = new ArchivoLexico( Indexer::lexicalFileName(), ESCRIBIR | LEER );
				lexical->mergeWith( tempLex, wordsFound, docLexData.terminos );
				delete lexical;
				// borro el lexico viejo y renombro el nuevo como el original
				remove ( Indexer::lexicalFileName().c_str() );
				rename ( tempLex.c_str(), Indexer::lexicalFileName().c_str() );

				// GRABO EL DOCUMENTO
				DocumentData docData;
				docData.ruta = fileName;
				docData.norma = calcularNorma( docLexData.terminos, docData.cantTermDistintos );
				int newDocId = _document->escribir( docData );

				// GRABO LA ASOCIACION DOCUMENTO-TERMINOS
				docLexData.id = newDocId;
				documentLexico->escribir( docLexData );
			}
			catch ( string ex )
			{
				cout << ex << endl;
			}
		}
		else
		{
			buildLexical( fileName, documentLexico );
		}
	}
}

void Indexer::buildIndex( string path )
{
	ArchivoDocLexico *documentLexico = new ArchivoDocLexico  ( Indexer::docLexFileName(), Indexer::docLexIdxFileName(), ESCRIBIR | LEER );
	buildLexical( path, documentLexico );
	delete documentLexico;

	sortByQuantity();

	buildLeaders();
}


Indexer::~Indexer()
{
	delete _document;
}
