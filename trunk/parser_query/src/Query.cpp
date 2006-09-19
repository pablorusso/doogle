#include "Query.h"

Query::Query( string word, bool included, bool excluded, int weight )
{
	_word = word;
	_includedFlag = included;
	_excludedFlag = excluded;
	_weight = weight;
}

string Query::getWord()
{
	return _word;
}
bool Query::mustBeIncluded()
{
	return _includedFlag;
}
bool Query::mustBeExcluded()
{
	return _excludedFlag;
}
int Query::getWeight()
{
	return _weight;
}

Query::~Query()
{
}

