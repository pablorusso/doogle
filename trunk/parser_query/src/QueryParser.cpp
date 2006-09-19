#include "includes/QueryParser.h"

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

Query *QueryParser::processToken( string token )
{
	bool mustInclude = token[0] == '+';
	bool mustExclude = token[0] == '-';

	if ( mustInclude || mustExclude )
		token.erase(0,1);

//	if ( token.length() == 0 )
		// error

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
			char *ep = 0;
			weight = std::strtol( ws[1].c_str(), &ep, 10);
			// if ( weight == 0 )
			 	// error
		}
	//	else error

	Query *q = new Query(word, mustInclude, mustExclude, weight );
	return q;
}

void QueryParser::tokenize(const string& str, vector<string>& tokens, const string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}
