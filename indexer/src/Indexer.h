#ifndef INDEXER_H_
#define INDEXER_H_

#include <cstdlib>
#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>

#include "ParserWrapper.h"

#include "ArchivoLexico.h"
#include "ArchivoDocumentos.h"
#include "ArchivoDocLexico.h"
#include "ArchivoConsts.h"

using namespace std;

class Indexer
{
	private:
		ArchivoLexico	*_lexical( "lexical.dat", ESCRIBIR | LEER );
		Document 		*_doc( "documents.dat" );
		DocumentLexical *_docLex( "documentLexicalTmp.dat" );

		static int DeltaCosineForEqual;
		static int fileFilter( const struct dirent *entry );

		void buildLexical( string path );
		void buildLeaders();

	public:
		Indexer();
		void buildIndex( string path );
		~Indexer();
};

#endif
