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
		ArchivoDocumentos *_document;
		ArchivoDocLexico  *_documentLexico;
		string 			   _targetFolder;

		static float deltaCosineForEqual();
		static int fileFilter( const struct dirent *entry );

		void buildLexical( string path, ArchivoDocLexico *documentLexico );
		void buildLeaders();
		double calcularNorma( LexicalPair &items, long &cantTerminos );
		void sortByQuantity();
	public:
		string documentsFileName();
		string documentsIdxFileName();
		string docLexFileName();
		string docLexIdxFileName();
		string lexicalFileName();

		Indexer( string targetFolder );
		void buildIndex( string path );
		~Indexer();
};

#endif
