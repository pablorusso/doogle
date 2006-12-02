#ifndef __DOCLEXICODATA_H__
#define __DOCLEXICODATA_H__

#include <string>
#include <map>

using namespace std;

// id - peso
typedef map<int, long> LexicalPair;
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
