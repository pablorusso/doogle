#include <iostream>
#include <fstream>
#include "QueryParser.h"

using namespace std;

void showError( string msg )
{
	cout << "<div align=center><br><br>Error<br><br><p class=g>" << msg << "</p></div>";
}

void showEmpty()
{
	cout << "<div><p class=g>No se encontraron resultados.</p></div>";
}

void showDoc( string docPath, string docTitle, string docDesc, string docSize )
{
	cout << "<div><p class=g><a class=l href=\"" << docPath << "\">" << docTitle << "</a>";
	cout << "<table cellpadding=0 cellspacing=0 border=0><tr><td class=j>";
	cout << "<font size=-1>" << docDesc << "<br>";
	cout << "<font color=#008000>" << docPath << " - " << docSize << " - </font>";
	cout << "</td></tr></table></p></div>";
}

void processLine( string str, string word, string qty, string time )
{
	bool found = false;
	string *strToFind = new string[4];
	strToFind[0] = "$WORD$";
	strToFind[1] = "$CANTIDAD$";
	strToFind[2] = "$TIEMPO$";
	strToFind[3] = "$DOCS$";

	for ( int i = 0; i < 4; ++i )
	{
		string::size_type pos = str.find ( strToFind[i], 0 );
		if ( pos != string::npos )
		{
			switch ( i )
			{
				case 0:
				{
					str.replace( pos, strToFind[i].size(), word );
					break;
				}

				case 1:
				{
					str.replace( pos, strToFind[i].size(), qty );
					break;
				}

				case 2:
				{
					str.replace( pos, strToFind[i].size(), time );
					break;
				}

				case 3:
				{
					// showError( "El formato de la consulta es invalido. Reintente" );
					// showEmpty();
					showDoc( "http://www.clarin.com.ar", "Diario clarin", "Diario clarin de la republica argentina", "20k" );
					found = true;
					break;
				}
			}
		}
	}

	if ( !found )
		cout << str << endl;
}

int main(int argc, char* argv[])
{
	// parseo el query
	string word = "";
	string buff;
	cin >> buff;

	string::size_type pos = buff.find ( "bsearch=", 0 );
	if ( pos != string::npos )
	{
		pos = pos + 8;
		string::size_type pos2 = buff.find ( "&", pos );
		if ( pos2 != string::npos )
			word = buff.substr( pos, pos2 - pos );
	}

	vector<Query *> query;
	if ( word != "" )
	{
		QueryParser::Parse( word, query );
		
		// llamar al buscador
	}

	ifstream in;
	in.open("result.template");
	if ( ! in )
	{
		cout << "<html><head><title>ERROR GRAVE</title></head><body>" << endl;
		showError( "Error al abrir el html que muestra los resultados." );
		cout << "</body></html>";
		return 0;
	}

	try
	{
		string str;
		getline( in, str );
		cout << "Content-type: text/html" << endl << endl << endl;
		// cout << buff;
		// return 0;

		while ( in )
		{
			processLine( str, word, "1000", "5" );
			getline( in, str );
		}
	}
	catch( ... )
	{
	}
	in.close();

	return 0;
}
