#ifndef QUERY_H_
#define QUERY_H_
#include <string>

using namespace std;

class Query
{
private:
	string _word;
	bool   _includedFlag;
	bool   _excludedFlag;
	int    _weight;
public:
	Query( string word, bool included, bool excluded, int weight=1 );

	string getWord();
	bool   mustBeIncluded();
	bool   mustBeExcluded();
	int    getWeight();
	
	virtual ~Query();
};

#endif /*QUERY_H_*/
