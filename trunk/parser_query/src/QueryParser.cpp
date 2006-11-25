#include "QueryParser.h"
#include <sstream>

QueryParser::QueryParser()
{
}

QueryParser::~QueryParser()
{
}

void QueryParser::Parse( string text, vector<Query*>& query )
{
	if ( text.empty() ) return ;

	vector<string> tokens;
	vector<string>::iterator tokens_iter;
    tokenize( text, tokens );

  	for (tokens_iter=tokens.begin(); tokens_iter != tokens.end(); tokens_iter++)
  	{
  		Query *q = processToken ( *tokens_iter );
  		query.push_back( q );
  	}
}

Query *QueryParser::processToken( string token ) throw(InvalidTokenException)
{
	bool mustInclude = token[0] == '+';
	bool mustExclude = token[0] == '-';

	if ( mustInclude || mustExclude )
		token.erase(0,1);

	if ( token.length() == 0 )
		throw InvalidTokenException();

	vector<string> ws;
	tokenize( token, ws, "^" );

	string word = "";
	int weight  = 1;
	if ( ws.size() == 1 )
		word = token;
	else
		if ( ws.size() == 2 )
		{
			word = ws[0];

			istringstream is( ws[1] );
			is >> weight;
			if ( weight == 0 )
				throw InvalidTokenException();
		}
		else
			throw InvalidTokenException();

	Query *q = new Query(word, mustInclude, mustExclude, weight );
	return q;
}

void QueryParser::tokenize(const string& str, vector<string>& tokens, const string& delimiters)
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}
