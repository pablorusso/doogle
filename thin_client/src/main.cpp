#include <iostream>
#include "SearchResultWriter.h"
#include "CGIClass.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf
{
	long mtype;
	char mtext;
};

void showError( string msg )
{
	cout << "<html><head><title>Error Irrecuperable</title></head><body>" << endl;
	cout << "<div align=center><br><br>Error<br><br><p class=g>" << msg << "</p></div>";
	cout << "<p align=center><font size=-1><a href=\"http://localhost/doogle/\">Volver al buscador</a></font></p>";
	cout << "</body></html>";
}

int openChannel( string path )
{
	int msqid;
	key_t key;

	if ( ( key = ftok(path.c_str(), 'B') ) == -1)
	{
		perror("ftok");
		exit(1);
	}
	if ( ( msqid = msgget(key, 0x0100 | 0x0080 | 0x0020 | 0x0010 | 0x0004 | 0x0002 ) ) == -1)
	{
		perror("msgget");
		exit(1);
	}

	return msqid;
}

int main(int argc, char* argv[])
{
	vector<string> docs;
	string errorMsg = "";
	CGIClass cgi;
	string word;

	if ( argc == 1 )
		cin >> word; // POST
	else
		word = argv[1]; // GET

	cgi.Load( word );
	string name  = "bsearch";
	string value = cgi.GetValue( name );

	int msqCGItoSearchid = openChannel( "/home/pablo/facultad/datos/doogle/www/lib/mq_cgi_search" );
	int msqSearchToCGIid = openChannel( "/home/pablo/facultad/datos/doogle/www/lib/mq_search_cgi" );

	for( uint i = 0; i < value.length(); i++ )
	{
		struct my_msgbuf busqueda;
		busqueda.mtype = 1;
		busqueda.mtext = value[i];
		if ( msgsnd( msqCGItoSearchid, (struct msgbuf *)&busqueda, sizeof(busqueda), 0) == -1 )
			perror("msgsnd");
	}

	struct my_msgbuf barraCero;
	barraCero.mtype = 1;
	barraCero.mtext = '\0';
	if ( msgsnd( msqCGItoSearchid, (struct msgbuf *)&barraCero, sizeof(barraCero), 0) == -1 )
		perror("msgsnd");

	struct my_msgbuf errorCode;
	struct my_msgbuf reply;
	string currentDoc  = "";
	char currentChar = '\0';
	bool salir = false;
	bool isError = false;

	if ( msgrcv( msqSearchToCGIid, (struct msgbuf *)&errorCode, sizeof(errorCode), 0, 0) == -1 )
	{
		perror("msgrcv");
		exit(1);
	}
	isError = ( errorCode.mtext != '1' );

	while( ! salir )
	{
		if ( msgrcv( msqSearchToCGIid, (struct msgbuf *)&reply, sizeof(reply), 0, 0) == -1 )
		{
			perror("msgrcv");
			exit(1);
		}
		currentChar = reply.mtext;
		if ( currentDoc != "" || currentChar != '\0' )
		{
			if ( currentChar != '\0' )
				currentDoc += currentChar;
			else
			{
				docs.push_back( currentDoc );
				currentDoc = "";
			}
		}
		else salir = true;
	}


	cout << "Content-type: text/html" << endl << endl << endl;

	if ( isError )
	{
		vector<string>::iterator itDoc;
		for( itDoc = docs.begin(); itDoc != docs.end(); itDoc++ )
		{
			if ( errorMsg != "" ) errorMsg += " - ";
			errorMsg += static_cast<string>( *itDoc );
		}
	}

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
