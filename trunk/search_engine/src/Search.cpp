#include "Search.h"
#include <map>
#include "MathIndex.h"

Search::Search( string indexFolder )
{
	_indexFolder = indexFolder;
	if ( _indexFolder[ _indexFolder.length()-1 ] != '/' )
		_indexFolder = _indexFolder + "/";
}

double Search::doCosine( map<int, char> &include, map<int, char> &exclude, LexicalPair &lexicoDocumento, LexicalPair &queryPair  )
{
	bool found = false;
	map<int, char>::iterator excludeIt = exclude.begin();
	while( excludeIt != exclude.end() && !found )
	{
		found = ( lexicoDocumento.find( static_cast< int > ( excludeIt->first ) ) != lexicoDocumento.end() );
		++excludeIt;
	}
	if ( found ) return 0;

	found = include.size() == 0;
	map<int, char>::iterator includeIt = include.begin();
	while( includeIt != include.end() && !found )
	{
		found = ( lexicoDocumento.find(static_cast< int > ( includeIt->first ))!=lexicoDocumento.end() );
		++includeIt;
	}

	if ( !found ) return 0;

	return MathIndex::cosineRank( lexicoDocumento, queryPair, MathIndex::norm( lexicoDocumento ), MathIndex::norm( queryPair ) );
}

void Search::doSearch( vector<Query *> query,  vector< string > &result )
{
	LexicalPair queryPair;
	map<int, char> include;
	map<int, char> exclude;
	map<double, int> encontrados;

	// Construye el pair idTermino, peso a partir de la consulta
	// TODO: eliminar los stops words
	LexicoData lexData;
	string lexFN = _indexFolder + LEX_FN;
	ArchivoLexico *lexico = new ArchivoLexico( lexFN, LEER );
	vector<Query *>::iterator queryIt;
	for( queryIt = query.begin(); queryIt != query.end(); queryIt++ )
	{
		Query *pQ = *queryIt;

		lexico->buscarTermino( pQ->getWord(), lexData );
		int id = lexData.id;

		if ( pQ->mustBeExcluded() )
			exclude[ id ] = '1';
		else
		{
			if ( pQ->mustBeIncluded() )
				include[ id ] = '1';
			queryPair[ id ] = pQ->getWeight();
		}
	}
	delete lexico;

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
		if ( rankCos > bestCos )
		{
			bestCos = rankCos;
			bestLeaderData = leaderData; // debe faltar el constructor de copia
		}
	}
	delete lideres;

	// Si hay algun lider bueno, entonces rankeo sus seguidores
	if ( bestCos != 0 )
	{
		encontrados[ bestCos ] = bestLeaderData.id;

		DocLexicoData seguidorData;

		string seguidorFN    = _indexFolder + FOLLOW_FN;
		string seguidorIdxFN = _indexFolder + FOLLOW_IDX_FN;
		ArchivoDocLexico *seguidores = new ArchivoDocLexico( seguidorFN, seguidorIdxFN, LEER );
		Seguidores::iterator currSeg = bestLeaderData.seguidores.begin();
		while ( currSeg != bestLeaderData.seguidores.end() )
		{
			long offset = static_cast< long >( currSeg->second );
			seguidores->leerOffset( offset, seguidorData );

			double rankCos = doCosine( include, exclude, seguidorData.terminos, queryPair );
			if ( rankCos > 0 )
				encontrados[ rankCos ] = seguidorData.id;
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
			encontrados[ rankCos ] = noLiderData.id;
	}
	delete noLideres;

	DocumentData docData;
	string docFN    = _indexFolder + DOC_FN;
	string docIdxFN = _indexFolder + DOC_IDX_FN;
	ArchivoDocumentos *documentos = new ArchivoDocumentos( docFN, docIdxFN, LEER );
	map<double, int>::iterator encIt;
	for( encIt = encontrados.begin(); encIt != encontrados.end(); encIt++ )
	{
		int docId = static_cast< int > ( encIt->second );
		documentos->buscarPorId( docId, docData );
		result.push_back( docData.ruta );
	}
	delete documentos;
}

