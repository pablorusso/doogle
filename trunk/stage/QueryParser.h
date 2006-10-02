#ifndef QUERYPARSER_H_
#define QUERYPARSER_H_

#include <string>
#include <vector>
#include "Query.h"
#include "InvalidTokenException.h"

using namespace std;

class QueryParser
{
private:
	static void tokenize( const string& str, vector<string>& tokens, const string& delimiters = " " );
	static Query *processToken( string token ) throw(InvalidTokenException);
public:
	QueryParser();
	virtual ~QueryParser();
	static void Parse( string text, vector<Query*>& query );
};

#endif /*QUERYPARSER_H_*/
