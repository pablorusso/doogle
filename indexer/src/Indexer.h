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

		void buildLexical( string lexicoFileName, string path, ArchivoDocLexico *documentLexico, ArchivoLexico *stopWords );
		void buildLeaders( ArchivoDocLexico *docLex );
		void sortByQuantity();

		string documentsFileName();
		string documentsIdxFileName();
		string docLexFileName();
		string docLexIdxFileName();
		string lexicalFileName();
		string lexStopFileName();
		string leaderFileName();
		string leaderIdxFileName();
		string followersFileName();
		string followersIdxFileName();
		string noLeaderFileName();
		string noLeaderIdxFileName();
	public:
		Indexer( string targetFolder );
		void buildIndex( string path, string stopWordPath );
		~Indexer();
};

#endif
