#include "SearchResultWriter.h"
#include <sstream>

SearchResultWriter::SearchResultWriter( string &word, string &errorMsg, vector<string> &docs )
{
	_word = word;
	_errorMsg = errorMsg;
	_docs = docs;
	_version = "0.2.5";

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
string SearchResultWriter::showItem( string docPath, string docTitle, string docDesc, string docSize )
{
	string part1 = "<div><p class=g><a class=l href=\"" + docPath + "\">" + docTitle + "</a>";
	string part2 = "<table cellpadding=0 cellspacing=0 border=0><tr><td class=j>";
	string part3 = "<font size=-1>" + docDesc + "<br>";
	string part4 = "<font color=#008000>" + docPath + " - " + docSize + " - </font>";
	string part5 = "</td></tr></table></p></div>";
	return part1 + part2 + part3 + part4 + part5;
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
						itemHtml += showItem( "http://www.clarin.com.ar", "Diario clarin", "Diario clarin de la republica argentina", "20k" );
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
