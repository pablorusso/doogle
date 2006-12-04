#include "Query.h"

Query::Query( string word, bool included, bool excluded, int weight )
{
	_word = word;
	for( int i = 0; i < _word.length(); i++ )
	{
		int asciiCode = ( int ) _word[ i ];
		if ( asciiCode >= 65 && asciiCode <= 90 )
			asciiCode += 32;
		_word[ i ] = ( char ) asciiCode;
	}

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

