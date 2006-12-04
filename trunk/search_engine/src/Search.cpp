#include "Search.h"
#include <map>
#include "MathIndex.h"

Search::Search( string indexFolder )
{
	_indexFolder = indexFolder;
	if ( _indexFolder[ _indexFolder.length()-1 ] != '/' )
		_indexFolder = _indexFolder + "/";
}

double Search::doCosine( map<int, int> &include, map<int, int> &exclude, LexicalPair &lexicoDocumento, LexicalPair &queryPair  )
{
	bool found = false;
	map<int, int>::iterator excludeIt = exclude.begin();
	while( excludeIt != exclude.end() && !found )
	{
		found = ( lexicoDocumento.find( static_cast< int > ( excludeIt->second ) ) != lexicoDocumento.end() );
		++excludeIt;
	}
	if ( found ) return -1;

	found = include.size() == 0;
	map<int, int>::iterator includeIt = include.begin();
	while( includeIt != include.end() && !found )
	{
		found = ( lexicoDocumento.find(static_cast< int > ( includeIt->second ))!=lexicoDocumento.end() );
		++includeIt;
	}

	if ( !found ) return -1;

	if ( queryPair.size() == 0 ) return 1;
	return MathIndex::cosineRank( lexicoDocumento, queryPair, MathIndex::norm( lexicoDocumento ), MathIndex::norm( queryPair ) );
}

void Search::doSearch( vector<Query *> query,  vector< string > &result )
{
	// bool mustIncludeNotInLex = false;
	bool mustExcludeNotInLex = false;
	LexicalPair queryPair;
	map<int, int> include;
	map<int, int> exclude;
	multimap<double, int> encontrados;

	// Construye el pair idTermino, peso a partir de la consulta
	// TODO: eliminar los stops words
	LexicoData lexData;
	string lexFN = _indexFolder + LEX_FN;
	ArchivoLexico *lexico = new ArchivoLexico( lexFN, LEER );
	vector<Query *>::iterator queryIt;
	for( queryIt = query.begin(); queryIt != query.end(); queryIt++ )
	{
		Query *pQ = *queryIt;

		bool found = lexico->buscarTermino( pQ->getWord(), lexData ) != NULL;
		int id = -1;
		if ( found )
			id = lexData.id;

		if ( pQ->mustBeExcluded() )
		{
			exclude[ id ] = id;
			mustExcludeNotInLex = true;
		}
		else
		{
			if ( pQ->mustBeIncluded() )
				include[ id ] = id;

			if ( found )
				queryPair[ id ] = pQ->getWeight();
		}
/*		}
		else
		{
			if ( pQ->mustBeIncluded() )
				mustIncludeNotInLex = true;
			if ( pQ->mustBeExcluded() )
				mustExcludeNotInLex = true;
		}*/
	}
	delete lexico;

	if ( queryPair.size() > 0 || mustExcludeNotInLex )
	{
		double bestCos = 0;
		DocLexicoData bestLeaderData;
		DocLexicoData leaderData;

		// Rankea los lideres y obtengo el mejor (si existe)
		string leaderFN    = _indexFolder + LEAD_FN;
		string leaderIdxFN = _indexFolder + LEAD_IDX_FN;
		ArchivoDocLexico *lideres = new ArchivoDocLexico( leaderFN, leaderIdxFN, LEER );
		lideres->comenzarLectura();
		while( lideres->leer( leaderData ) )
		{
			double rankCos = doCosine( include, exclude, leaderData.terminos, queryPair );
			if ( rankCos >= bestCos )
			{
				bestCos = rankCos;
				bestLeaderData = leaderData; // debe faltar el constructor de copia
			}
		}
		delete lideres;

		// Si hay algun lider bueno, entonces rankeo sus seguidores
		if ( bestCos != 0 )
		{
			encontrados.insert( multimap<double, int>::value_type( 1-bestCos, bestLeaderData.id ) );

			DocLexicoData seguidorData;

			string seguidorFN    = _indexFolder + FOLLOW_FN;
			string seguidorIdxFN = _indexFolder + FOLLOW_IDX_FN;
			ArchivoDocLexico *seguidores = new ArchivoDocLexico( seguidorFN, seguidorIdxFN, LEER );
			Seguidores::iterator currSeg = bestLeaderData.seguidores.begin();
			while ( currSeg != bestLeaderData.seguidores.end() )
			{
				int offset = static_cast< int >( currSeg->second );
				seguidores->leerOffset( offset, seguidorData );

				double rankCos = doCosine( include, exclude, seguidorData.terminos, queryPair );
				if ( rankCos > 0 )
					encontrados.insert( multimap<double, int>::value_type( 1-rankCos, seguidorData.id ) );
				++currSeg;
			}
			delete seguidores;
		}
		/*else { */

		// Busco en los documentos sin lideres
		DocLexicoData noLiderData;

		string noLiderFN    = _indexFolder + NOLEAD_FN;
		string noLiderIdxFN = _indexFolder + NOLEAD_IDX_FN;
		ArchivoDocLexico *noLideres = new ArchivoDocLexico( noLiderFN, noLiderIdxFN, LEER );
		noLideres->comenzarLectura();
		while( noLideres->leer( noLiderData ) )
		{
			double rankCos = doCosine( include, exclude, noLiderData.terminos, queryPair );
			if ( rankCos > 0 )
				encontrados.insert( multimap<double, int>::value_type( 1-rankCos, noLiderData.id ) );
		}
		delete noLideres;

		DocumentData docData;
		string docFN    = _indexFolder + DOC_FN;
		string docIdxFN = _indexFolder + DOC_IDX_FN;
		ArchivoDocumentos *documentos = new ArchivoDocumentos( docFN, docIdxFN, LEER );
		multimap<double, int>::iterator encIt;
		for( encIt = encontrados.begin(); encIt != encontrados.end(); encIt++ )
		{
			int docId = static_cast< int > ( encIt->second );
			documentos->buscarPorId( docId, docData );
			result.push_back( docData.ruta );
		}
		delete documentos;
	}
}

