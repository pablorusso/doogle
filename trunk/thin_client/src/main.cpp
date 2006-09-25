#include <iostream>
#include "QueryParser.h"
#include "InvalidTokenException.h"
#include "SearchResultWriter.h"

void showError( string msg )
{
	cout << "<html><head><title>Error Irrecuperable</title></head><body>" << endl;
	cout << "<div align=center><br><br>Error<br><br><p class=g>" << msg << "</p></div>";
	cout << "<p align=center><font size=-1><a href=\"http://localhost/doogle/\">Volver al buscador</a></font></p>";
	cout << "</b#include <string>ody></html>";
}

string parseQueryString( string buff )
{
	string::size_type pos = buff.find ( "bsearch=", 0 );
	if ( pos != string::npos )
	{
		pos = pos + 8;
		string::size_type pos2 = buff.find ( "&", pos );
		if ( pos2 != string::npos )
			return buff.substr( pos, pos2 - pos );
	}
	return "";
}

int main(int argc, char* argv[])
{
	string errorMsg = "";
	string word;

	if ( argc == 1 )
	{ // POST
		cin >> word;
		word = parseQueryString( word );
	}
	else
	{  // GET
		word = argv[1];
	}

	vector<Query *> query;
	vector<string>  docs;
	cout << "Content-type: text/html" << endl << endl << endl;
	try
	{
		// Parseo la consulta
		QueryParser::Parse( word, query );
		// llamo al buscador

	}
	catch( InvalidTokenException ex )
	{
		errorMsg = ex.what();
	}

	try
	{
		SearchResultWriter writer( word, errorMsg, docs );
		cout << writer.getResult();
	}
	catch( ... )
	{
		showError( "Error al mostrar el html con los resultados." );
	}

	return 0;
}
