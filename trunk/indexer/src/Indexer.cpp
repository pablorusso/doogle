#include "Indexer.h"
#include <math.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>

#include "MathIndex.h"

struct CantTermsComparer
{
     bool operator()( const DocLexicoData &a, const DocLexicoData &b )
     {
          return a.terminos.size() > b.terminos.size();
     }
};

Indexer::Indexer( string targetFolder )
{
	_targetFolder = targetFolder;

	remove( lexicalFileName().c_str() );
	remove( lexStopFileName().c_str() );
	remove( documentsFileName().c_str() );
	remove( documentsIdxFileName().c_str() );
	remove( leaderFileName().c_str() );
	remove( leaderIdxFileName().c_str() );
	remove( followersFileName().c_str() );
	remove( followersIdxFileName().c_str() );
	remove( noLeaderFileName().c_str() );
	remove( noLeaderIdxFileName().c_str() );
	remove( docLexFileName().c_str() );
	remove( docLexIdxFileName().c_str() );

	_document = new ArchivoDocumentos ( Indexer::documentsFileName(), Indexer::documentsIdxFileName(), ESCRIBIR | LEER );
}

string Indexer::documentsFileName()
{
	return _targetFolder + DOC_FN;
}
string Indexer::documentsIdxFileName()
{
	return _targetFolder + DOC_IDX_FN;
}
string Indexer::docLexFileName()
{
	return _targetFolder + "documentLex.dat";
}
string Indexer::docLexIdxFileName()
{
	return _targetFolder + "documentLex.idx";
}
string Indexer::lexicalFileName()
{
	return _targetFolder + LEX_FN;
}
string Indexer::lexStopFileName()
{
	return _targetFolder + LEX_STOP_FN;
}

string Indexer::leaderFileName()
{
	return _targetFolder + LEAD_FN;
}
string Indexer::leaderIdxFileName()
{
	return _targetFolder + LEAD_IDX_FN;
}
string Indexer::followersFileName()
{
	return _targetFolder + FOLLOW_FN;
}
string Indexer::followersIdxFileName()
{
	return _targetFolder + FOLLOW_IDX_FN;
}
string Indexer::noLeaderFileName()
{
	return _targetFolder + NOLEAD_FN;
}
string Indexer::noLeaderIdxFileName()
{
	return _targetFolder + NOLEAD_IDX_FN;
}

float Indexer::deltaCosineForEqual()
{
	return 0.75;
}

int Indexer::fileFilter( const struct dirent *entry )
{
	if ( (strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0) )
		return 0;
	else
		return -1;
}

void Indexer::buildLeaders( ArchivoDocLexico *docLex )
{
	int docCount 	  = _document->size();
	uint maxLeaders   = ( int ) ceil( sqrt( docCount ) );
	uint maxFollowers = ( int ) ceil( sqrt( docCount / 2 ) );

	DocLexicoData data;
	map< int, DocLexicoData > leaders;

	ArchivoDocLexico *followers     = new ArchivoDocLexico( followersFileName(), followersIdxFileName(), ESCRIBIR | LEER );
	ArchivoDocLexico *withoutLeader = new ArchivoDocLexico( noLeaderFileName(), noLeaderIdxFileName(), ESCRIBIR | LEER );

	docLex->comenzarLectura();
	if ( docLex->leer( data ) )
		leaders[ data.id ] = data;

	// Para cada documento
	while ( docLex->leer( data ) )
	{
		bool isFollower = false; float lessRank = -1;
		int bestLeaderId = -1;

		map< int, DocLexicoData >::iterator curr = leaders.begin();
		while ( curr != leaders.end() && ! isFollower )
		{
			DocLexicoData *itemLeader = static_cast< DocLexicoData *>( &curr->second );

			// Comparo usando el ranking del coseno el documento con cada lider
			double rank = MathIndex::cosineRank( itemLeader->terminos, data.terminos, MathIndex::norm( itemLeader->terminos ), MathIndex::norm( data.terminos ) );
			if ( rank >= deltaCosineForEqual() )
			{
				// Si el rankeo del coseno es suficientemente bueno, lo considero seguidor sin ver que pasa mas adelante
				followers->escribir( data );

				int offset = followers->ultimaPosicionEscrita();
				itemLeader->seguidores[ data.id ] = offset;
				isFollower = true;
			}
			else
			{
				// Si no es tan bueno, me fijo si al menos es el mejor que encontre y me lo guardo
				if ( rank > lessRank && itemLeader->seguidores.size() < maxFollowers )
				{
					lessRank = rank;
					bestLeaderId = itemLeader->id;
				}
			}

			++curr;
		}


		// Si no es seguidor natural de nadie
		if ( ! isFollower )
		{
			// Si aun tengo espacio para liders, lo hago lider
			if ( leaders.size() < maxLeaders )
				leaders[ data.id ] = data;
			else
			{
				if ( bestLeaderId != -1 )
				{
					DocLexicoData *bestLeader = static_cast< DocLexicoData *> ( & (leaders[ bestLeaderId ]) );

					followers->escribir( data );
					int offset = followers->ultimaPosicionEscrita();
					bestLeader->seguidores[ data.id ] = offset;
				}
				else
					withoutLeader->escribir( data );
			}
		}
	}

	delete followers;
	delete withoutLeader;

	// Paso a disco los lideres que tenia en memoria ordenados por id
	ArchivoDocLexico *aLeaders = new ArchivoDocLexico( leaderFileName(), leaderIdxFileName(), ESCRIBIR | LEER );
	map< int, DocLexicoData >::iterator curr = leaders.begin();
	while ( curr != leaders.end() )
	{
		DocLexicoData *itemLeader = static_cast< DocLexicoData *>( &(curr->second) );
		aLeaders->escribir( *itemLeader );
		++curr;
	}
	delete aLeaders;
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
			particion->escribir( static_cast< DocLexicoData &>( *curr ) );
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

void Indexer::buildLexical( string lexicoFileName, string path, ArchivoDocLexico *documentLexico, ArchivoLexico *stopWords )
{
	int count, i;
	string tempLex = lexicoFileName + "tmp";
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
				ArchivoLexico *lexical = new ArchivoLexico( lexicoFileName, ESCRIBIR | LEER );
				lexical->mergeWith( tempLex, wordsFound, docLexData.terminos, stopWords );
				delete lexical;

				// borro el lexico viejo y renombro el nuevo como el original
				remove ( lexicoFileName.c_str() );
				rename ( tempLex.c_str(), lexicoFileName.c_str() );

				if ( documentLexico != NULL )
				{
					DocumentData docData;
					double norma = MathIndex::norm( docLexData.terminos, docData.cantTermDistintos );
					if ( docData.cantTermDistintos != 0 )
					{
						// GRABO EL DOCUMENTO
						docData.ruta = fileName;
						docData.norma = norma;
						int newDocId = _document->escribir( docData );

						// GRABO LA ASOCIACION DOCUMENTO-TERMINOS
						docLexData.id = newDocId;
						documentLexico->escribir( docLexData );
					}
				}
			}
			catch ( string ex )
			{
				cout << ex << endl;
			}
		}
		else
		{
			buildLexical( lexicoFileName, fileName, documentLexico, stopWords );
		}
	}
}

void Indexer::buildIndex( string fromPath, string stopWordPath )
{
	ArchivoLexico *lexicoStop = NULL;
	if ( stopWordPath != "" )
	{
		buildLexical( Indexer::lexStopFileName(), stopWordPath, NULL, NULL );
		lexicoStop = new ArchivoLexico  ( Indexer::lexStopFileName(), LEER );
	}

	ArchivoDocLexico *documentLexico = new ArchivoDocLexico  ( Indexer::docLexFileName(), Indexer::docLexIdxFileName(), ESCRIBIR | LEER );
	buildLexical( Indexer::lexicalFileName(), fromPath, documentLexico, lexicoStop );
	delete documentLexico;

	sortByQuantity();

	documentLexico = new ArchivoDocLexico  ( Indexer::docLexFileName(), Indexer::docLexIdxFileName(), ESCRIBIR | LEER );
	buildLeaders( documentLexico );
	delete documentLexico;

	if ( lexicoStop != NULL ) delete lexicoStop;
}


Indexer::~Indexer()
{
	delete _document;

	remove( docLexFileName().c_str() );
	remove( docLexIdxFileName().c_str() );
}
