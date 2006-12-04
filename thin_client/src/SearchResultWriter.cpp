#include "SearchResultWriter.h"
#include <sstream>

SearchResultWriter::SearchResultWriter( string &word, string &errorMsg, vector<string> &docs )
{
	_word = word;
	_errorMsg = errorMsg;
	_docs = docs;
	_version = "1.0.0";

	_resTemplate.open( "result.template" );
}

SearchResultWriter::~SearchResultWriter()
{
	_resTemplate.close();
}

string SearchResultWriter::showMessage( string msg )
{
	return "<div><p class=g>" + msg + "</p></div>";
}
string SearchResultWriter::showItem( string docPath, string docTitle )
{
	string part1 = "<div><p class=g><a class=l href=\"" + docPath + "\">" + docTitle + "</a>";
	string part2 = "<table cellpadding=0 cellspacing=0 border=0><tr><td class=j>";
	string part3 = "<font color=#008000>" + docPath + "</font>";
	string part4 = "</td></tr></table></p></div>";
	return part1 + part2 + part3 + part4;
}

int SearchResultWriter::splitString(const string& input, const string& delimiter, vector<string>& results, bool includeEmpties )
{
    int iPos = 0;
    int newPos = -1;
    int sizeS2 = (int)delimiter.size();
    int isize = (int)input.size();

    if(
        ( isize == 0 )
        ||
        ( sizeS2 == 0 )
    )
    {
        return 0;
    }

    vector<int> positions;

    newPos = input.find (delimiter, 0);

    if( newPos < 0 )
    {
        return 0;
    }

    int numFound = 0;

    while( newPos >= iPos )
    {
        numFound++;
        positions.push_back(newPos);
        iPos = newPos;
        newPos = input.find (delimiter, iPos+sizeS2);
    }

    if( numFound == 0 )
    {
        return 0;
    }

    for( uint i=0; i <= positions.size(); ++i )
    {
        string s("");
        if( i == 0 )
        {
            s = input.substr( i, positions[i] );
        }
        int offset = positions[i-1] + sizeS2;
        if( offset < isize )
        {
            if( i == positions.size() )
            {
                s = input.substr(offset);
            }
            else if( i > 0 )
            {
                s = input.substr( positions[i-1] + sizeS2,
                      positions[i] - positions[i-1] - sizeS2 );
            }
        }
        if( includeEmpties || ( s.size() > 0 ) )
        {
            results.push_back(s);
        }
    }
    return numFound;
}

string SearchResultWriter::processLine( string str, string word, string qty, string version )
{
	int strToFindLength = 4;
	string *strToFind = new string[strToFindLength];
	strToFind[0] = "$CONTENT$";
	strToFind[1] = "$WORD$";
	strToFind[2] = "$CANTIDAD$";
	strToFind[3] = "$VERSION$";

	for ( int i = 0; i < strToFindLength; ++i )
	{
		string::size_type pos = str.find ( strToFind[i], 0 );
		if ( pos != string::npos )
		{
			switch ( i )
			{
				case 0:
				{
					string itemHtml = "";

					if ( _errorMsg.length() != 0 )
						itemHtml += showMessage( _errorMsg );
					else
					{
						// iterar por los documentos y mostrar
						vector< string >::iterator docIt;
						for( docIt = _docs.begin(); docIt != _docs.end(); docIt++ )
						{
							string docPath = static_cast< string > ( *docIt );

							string delim = "/";
							vector<string> split;
							splitString( docPath, delim, split, true );
							string docName = split[ split.size() - 1 ];
							itemHtml += showItem( docPath, docName );
						}
					}

					str.replace( pos, strToFind[i].size(), itemHtml );
					break;
				}
				case 1:
				{
					str.replace( pos, strToFind[i].size(), word );
					break;
				}
				case 2:
				{
					str.replace( pos, strToFind[i].size(), qty );
					break;
				}
				case 3:
				{
					str.replace( pos, strToFind[i].size(), version );
					break;
				}
			}
		}
	}

	return str + "\n";
}

string SearchResultWriter::getResult()
{
	string result;
	string str;

	std::stringstream my_stringstream;
	my_stringstream <<  _docs.size();
	string qty = my_stringstream.str();

	getline( _resTemplate, str );
	while ( _resTemplate )
	{

		result += processLine( str, _word, qty, _version );
		getline( _resTemplate, str );
	}

	return result;
}
