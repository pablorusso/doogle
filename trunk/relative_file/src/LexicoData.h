#ifndef __LEXICODATA_H__
#define __LEXICODATA_H__

#include <string>
#include <map>

using namespace std;

// palabra - peso
typedef map<std::string, int> WordPair;
// id - peso
typedef map<int, int> LexicalPair;

class LexicoData
{
	public:
		int 		id;
    	std::string termino;

		LexicoData();
};

#endif
