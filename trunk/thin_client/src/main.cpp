#include <iostream>
#include "QueryParser.h"
#include "InvalidTokenException.h"
#include "SearchResultWriter.h"
#include "CGIClass.h"

void showError( string msg )
{
	cout << "<html><head><title>Error Irrecuperable</title></head><body>" << endl;
	cout << "<div align=center><br><br>Error<br><br><p class=g>" << msg << "</p></div>";
	cout << "<p align=center><font size=-1><a href=\"http://localhost/doogle/\">Volver al buscador</a></font></p>";
	cout << "</b#include <string>ody></html>";
}

int main(int argc, char* argv[])
{
	char* lpszContentLength = getenv("CONTENT_LENGTH");
	if(lpszContentLength == NULL) return -1;
	int nContentLength = atoi(lpszContentLength);
	if(nContentLength == 0) return -1;

	string errorMsg = "";
	CGIClass cgi;
	vector<Query *> query;
	vector<string>  docs;

	string word;
	if ( argc == 1 )
		cin >> word; // POST
	else
		word = argv[1]; // GET

	cgi.Load( word );
	string name  = "bsearch";
	string value = cgi.GetValue( name );

	try
	{
		QueryParser::Parse( value, query );
	}
	catch( InvalidTokenException ex )
	{
		errorMsg = ex.what();
	}

	cout << "Content-type: text/html" << endl << endl << endl;
	try
	{
		SearchResultWriter writer( value, errorMsg, docs );
		cout << writer.getResult();
	}
	catch( ... )
	{
		showError( "Error al mostrar el html con los resultados." );
	}

	return 0;
}
