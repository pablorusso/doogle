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
string Indexer::leaderFileName()
{
	return _targetFolder + "leaders.dat";
}
string Indexer::leaderIdxFileName()
{
	return _targetFolder + "I_leaders.dat";
}
string Indexer::followersFileName()
{
	return _targetFolder + "followers.dat";
}
string Indexer::followersIdxFileName()
{
	return _targetFolder + "I_followers.dat";
}
string Indexer::noLeaderFileName()
{
	return _targetFolder + "noLeader.dat";
}
string Indexer::noLeaderIdxFileName()
{
	return _targetFolder + "I_noLeader.dat";
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

void Indexer::buildLeaders( ArchivoDocLexico *docLex )
{
	long docCount = _document->size();
	DocLexicoData data;

	// ver como se calcula esto
	uint maxLeaders   = sqrt( docCount );
	uint maxFollowers = sqrt( docCount );

	map< int, DocLexicoData > leaders;

	ArchivoDocLexico *followers     = new ArchivoDocLexico( followersFileName(), followersIdxFileName(), ESCRIBIR | LEER );
	ArchivoDocLexico *withoutLeader = new ArchivoDocLexico( noLeaderFileName(), noLeaderIdxFileName(), ESCRIBIR | LEER );

	docLex->comenzarLectura();
	docLex->leer( data );
	if ( !docLex->fin() )
		leaders[ data.id ] = data;

	// Para cada documento
	docLex->leer( data );
	while ( !docLex->fin() )
	{
		bool isFollower = false; float lessRank  = 0;
		int bestLeaderId = -1;

		map< int, DocLexicoData >::iterator curr = leaders.begin();
		while ( curr != leaders.end() && ! isFollower )
		{
			DocLexicoData itemLeader = static_cast< DocLexicoData >( curr->second );

			// Comparo usando el ranking del coseno el documento con cada lider
			double rank = MathIndex::cosineRank( itemLeader.terminos, data.terminos, MathIndex::norm( itemLeader.terminos ), MathIndex::norm( data.terminos ) );
			if ( rank <= deltaCosineForEqual() )
			{
				// Si el rankeo del coseno es suficientemente bueno, lo considero seguidor sin ver que pasa mas adelante
				followers->escribir( data );

				long offset = followers->ultimaPosicionEscrita();
				itemLeader.seguidores[ data.id ] = offset;
				isFollower = true;
			}
			else
			{
				// Si no es tan bueno, me fijo si al menos es el mejor que encontre y me lo guardo
				if ( rank < lessRank )
				{
					lessRank = rank;
					bestLeaderId = itemLeader.id;
				}
			}

			++curr;
		}


		if ( ! isFollower )
		{
			DocLexicoData bestLeader = static_cast< DocLexicoData > ( leaders[ bestLeaderId ] );
			// Si no es seguidor natural de nadie
			if ( bestLeader.seguidores.size() < maxFollowers )
			{
				// Si el rankeo del coseno es suficientemente bueno, lo considero seguidor sin ver que pasa mas adelante
				followers->escribir( data );

				long offset = followers->ultimaPosicionEscrita();
				bestLeader.seguidores[ data.id ] = offset;
			}
			else
			{
				if ( leaders.size() < maxLeaders )
					// Si aun tengo espacio para liders, lo hago lider
					leaders[ data.id ] = data;
				else
					// Si no puede ser ni seguidor ni lider, entonces no encaja con nada y lo mando al otro archivo
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
		DocLexicoData itemLeader = static_cast< DocLexicoData >( curr->second );
		aLeaders->escribir( itemLeader );
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
				docData.norma = MathIndex::norm( docLexData.terminos, docData.cantTermDistintos );
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

void Indexer::buildIndex( string fromPath )
{
	ArchivoDocLexico *documentLexico = new ArchivoDocLexico  ( Indexer::docLexFileName(), Indexer::docLexIdxFileName(), ESCRIBIR | LEER );
	buildLexical( fromPath, documentLexico );
	delete documentLexico;

	sortByQuantity();

	documentLexico = new ArchivoDocLexico  ( Indexer::docLexFileName(), Indexer::docLexIdxFileName(), ESCRIBIR | LEER );
	buildLeaders( documentLexico );
	delete documentLexico;
}


Indexer::~Indexer()
{
	delete _document;
}
