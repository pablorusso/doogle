#include <string>
#include <vector>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "Query.h"
#include "QueryParser.h"
#include "Search.h"

using namespace std;

struct my_msgbuf
{
	int mtype;
	char mtext;
};

void usage()
{
	cerr <<	"usage: search_engine [type] [method] [indexerOutputPath] [consulta]" << endl;
	cerr << "       ex: search_engine 2 all /home/pablo/facultad/output t1" << endl;
	cerr << "       [type]   1=consola 2=cola de mensajes" << endl;
	cerr << "       [method] lead=lideres all=todos" << endl;
	::exit( 1 );
}

int createChannel( string path )
{
	int msqid;
	key_t key;

	if ( ( key = ftok(path.c_str(), 'B') ) == -1)
	{
		perror("ftok");
		exit(1);
	}


	if ( ( msqid = msgget(key, 0x0100 | 0x0080 | 0x0020 | 0x0010 | 0x0004 | 0x0002 | IPC_CREAT ) ) == -1)
	{
		perror("msgget");
		exit(1);
	}

	return msqid;
}

void destroyChannel( int chanId )
{
	if ( msgctl( chanId, IPC_RMID, NULL ) == -1 )
	{
		perror("msgctl");
		exit(1);
	}
}

bool procesar( Search *search, string queryString, vector< string > &result, bool useLeader )
{
	bool isOk;
	vector<Query *> query;
	try
	{
		QueryParser::Parse( queryString, query );

		if ( useLeader )
			search->doSearch( query, result );
		else
			search->doSearchAll( query, result );

		isOk = true;
	}
	catch( InvalidTokenException ex )
	{
		result.push_back( ex.what() );
		isOk = false;
	}
	catch( string msg )
	{
		result.push_back( msg );
		isOk = false;
	}

	// libero el vector de queries
	vector<Query*>::iterator vecIt;
	for( vecIt = query.begin(); vecIt != query.end(); vecIt++ )
		delete *vecIt;

	return isOk;
}

int useMsgQueue( int argc, char* argv[] )
{
	string method;
	string indexFolder;
	if ( argc != 4 )
		usage();
	else
	{
		method = argv[2];
		indexFolder = argv[3];
	}
	struct my_msgbuf buf;

	int msqCGItoSearchid = createChannel( "/home/pablo/facultad/datos/doogle/www/lib/mq_cgi_search" );
	int msqSearchToCGIid = createChannel( "/home/pablo/facultad/datos/doogle/www/lib/mq_search_cgi" );
	msgctl(msqCGItoSearchid, IPC_RMID, NULL);
	msgctl(msqSearchToCGIid, IPC_RMID, NULL);

	cout << endl << "Creando canal cgi a search " ;
	msqCGItoSearchid = createChannel( "/home/pablo/facultad/datos/doogle/www/lib/mq_cgi_search" );
	cout << endl << "creado el canal: " << msqCGItoSearchid;
	cout << endl << "Creando canal search a cgi " ;
	msqSearchToCGIid = createChannel( "/home/pablo/facultad/datos/doogle/www/lib/mq_search_cgi" );
	cout << endl << "creado el canal: " << msqSearchToCGIid;
	cout << endl << "Escuchando...";
	cout << std::flush;

	Search *search = new Search( indexFolder );
	string temp = "";

	for ( ; ; )  // Nunca termina
	{
		if ( msgrcv( msqCGItoSearchid, (struct msgbuf *)&buf, sizeof(buf), 0, 0) == -1 )
		{
			perror("msgrcv");
			exit(1);
		}

		cout << endl << "[search_engine] - CGI to search_engine = " << buf.mtext << std::flush;
		if ( buf.mtext != '\0' )
			temp += buf.mtext;
		else
		{
			cout << endl << "Palabra completa: " << temp ;

			// leo el metodo a usar
			if ( msgrcv( msqCGItoSearchid, (struct msgbuf *)&buf, sizeof(buf), 0, 0) == -1 )
			{
				perror("msgrcv");
				exit(1);
			}

			bool useLeader = buf.mtext == '1';
			vector<string> documentos;
			bool isOk = procesar( search, temp, documentos, useLeader );

			struct my_msgbuf codigoInicial;
			codigoInicial.mtype = 1;
			codigoInicial.mtext = isOk ? '1' : '2'; // 1 OK, 2 error
			if ( msgsnd( msqSearchToCGIid, (struct msgbuf *)&codigoInicial, sizeof(codigoInicial), 0) == -1 )
				perror("msgsnd");

			cout << endl << "Enviando respuesta..." << temp ;
			vector<string>::iterator vecIt;
			for( vecIt = documentos.begin(); vecIt != documentos.end(); vecIt++ )
			{
				string document = static_cast< string >(*vecIt);

				for( uint i = 0; i < document.length(); i++ )
				{
					struct my_msgbuf reply;
					reply.mtype = 1;
					reply.mtext = document[i];
					if ( msgsnd( msqSearchToCGIid, (struct msgbuf *)&reply, sizeof(reply), 0) == -1 )
						perror("msgsnd");
				}

				struct my_msgbuf barraCero;
				barraCero.mtype = 1;
				barraCero.mtext = '\0';
				if ( msgsnd( msqSearchToCGIid, (struct msgbuf *)&barraCero, sizeof(barraCero), 0) == -1 )
					perror("msgsnd");
			}

			struct my_msgbuf barraCeroFinal;
			barraCeroFinal.mtype = 1;
			barraCeroFinal.mtext = '\0';
			if ( msgsnd( msqSearchToCGIid, (struct msgbuf *)&barraCeroFinal, sizeof(barraCeroFinal), 0) == -1 )
				perror("msgsnd");

			temp = "";
			cout << endl << "Escuchando... " << std::flush;
		}
	}

	delete search;
	destroyChannel( msqCGItoSearchid );
	destroyChannel( msqSearchToCGIid );
	return 0;
}



int useConsole( int argc, char* argv[] )
{
	if ( argc < 5 )
		usage();

	string method = argv[2];

	string indexFolder;
	indexFolder = argv[3];

	string word = "";
	for( int i = 4; i < argc; i++ )
	{
		if ( word.size() > 0 ) word += " ";
		word += argv[i];
	}

	vector<string> documentos;

	cout << endl << "comienza la busqueda!" << endl;

	Search *search = new Search( indexFolder );
	bool isOk = procesar( search, word, documentos, method != "all" );
	if ( !isOk )
		cout << "hubo un error!" << endl;

	vector<string>::iterator docIt;
	for( docIt = documentos.begin(); docIt != documentos.end(); docIt++ )
		cout << endl << "encontre: " << static_cast< string > ( *docIt );

	cout << endl << "fin de la busqueda" << endl;

	delete search;
	return 0;
}


int main( int argc, char* argv[] )
{
	if ( argc < 3 ) usage();

	string type = argv[1];
	if ( type == "1" )
		useConsole( argc, argv );
	else
		useMsgQueue( argc, argv );
}
