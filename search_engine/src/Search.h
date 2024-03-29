#ifndef SEARCH_H
#define	SEARCH_H

#include <map>
#include <vector>
#include <string>
#include "Query.h"
#include "QueryParser.h"

#include "ArchivoLexico.h"
#include "ArchivoDocumentos.h"
#include "ArchivoDocLexico.h"
#include "ArchivoConsts.h"

class Search
{
	private:
		string _indexFolder;
		double doCosine( map<int, int> &include, map<int, int> &exclude, LexicalPair &lexicoDocumento, LexicalPair &queryPair, double normaDoc  );
	public:
		Search( string indexFolder );
		void doSearch( vector<Query *> query,  vector< string > &result );
		void doSearchAll( vector<Query *> query,  vector< string > &result );
};
#endif

