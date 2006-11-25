#ifndef __LEXICAL__
#define __LEXICAL__

#include <string>

using namespace std;

class Lexical
{
	public:
		Lexical();

		int AddWord( string word );
		void SortByWord();
		void SortByQuantity();
		string getWordById( int id );
		int getIdByWord( string word );
};

#endif
