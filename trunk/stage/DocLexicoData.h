#ifndef __DOCLEXICODATA_H__
#define __DOCLEXICODATA_H__

#include <string>
#include <map>
#include "LexicoData.h"

using namespace std;

// id - offset
typedef map<int, long> Seguidores;

class DocLexicoData
{
	public:
		int  id;
		Seguidores  seguidores;
		LexicalPair terminos;
};

#endif
