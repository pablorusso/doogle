#include "Indexer.h"
#include <math.h>

Indexer::Indexer( string targetFolder )
{
	_targetFolder = targetFolder;

	remove( documentsFileName().c_str() );
	remove( documentsIdxFileName().c_str() );
	remove( docLexFileName().c_str() );
	remove( docLexIdxFileName().c_str() );
	remove( lexicalFileName().c_str() );

	_document       = new ArchivoDocumentos ( Indexer::documentsFileName(), Indexer::documentsIdxFileName(), ESCRIBIR | LEER );
	_documentLexico = new ArchivoDocLexico  ( Indexer::docLexFileName(), Indexer::docLexIdxFileName(), ESCRIBIR | LEER );
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
	if ( DocumentLexicalData item = docLex.Read() )
		learders.AddRecord( item );

	// Para cada documento
	while ( DocumentLexicalData item = docLex.Read() )
	{
		DocumentLexicalData bestLeader = NULL;
		bool isFollower = false;
		float lessRank  = 0;


		leaders.StartRead();
		// Para cada lider
		while ( DocumentLexicalData leader = leaders.Read() && ! isFollower )
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

void Indexer::buildLexical( string path )
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

				/*
				cout << "PRUEBA RELACION " << endl;
				LexicalPair::iterator curr = docLexData.terminos.begin();
				while ( curr != docLexData.terminos.end() )
				{
					int idTermino = static_cast< int >( curr->first );
   					long peso = static_cast< long >( curr->second );
   					cout << "(" << idTermino << ", " << peso << ") ";
					++curr;
				}
				*/

				_documentLexico->escribir( docLexData );
			}
			catch ( string ex )
			{
				cout << ex << endl;
			}
		}
		else
		{
			buildLexical( fileName );
		}
	}

	// Tengo que ordenar por cantidad de terminos distintos
	// docLex.sortByQuantity();
}

void Indexer::buildIndex( string path )
{
	buildLexical( path );
	buildLeaders();
}

Indexer::~Indexer()
{
	delete _documentLexico;
	delete _document;
}
