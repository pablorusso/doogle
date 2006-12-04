#include <string>
#include <vector>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

struct my_msgbuf
{
	long mtype;
	char mtext;
};

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

void procesar( string queryString, vector< string > &result )
{
		/*
	vector<Query *> query;
	vector<string>  docs;

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
	*/

	result.push_back( "/home/pablo/facultad/datos/doogle/www/lib/mq_cgi_search" );
	result.push_back( "/home/pablo/facultad/datos/doogle/www/lib/mq_search_cgi" );
}

int main( int argc, char* argv[] )
{
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

	string temp = "";
	for ( ; ; )  // Nunca termina
	{
		if ( msgrcv( msqCGItoSearchid, (struct msgbuf *)&buf, sizeof(buf), 0, 0) == -1 )
		{
			perror("msgrcv");
			exit(1);
		}

		cout << "[search_engine] - CGI to search_engine = " << buf.mtext << std::flush;
		if ( buf.mtext != '\0' )
			temp += buf.mtext;
		else
		{
			cout << endl << "Palabra completa: " << temp ;

			vector<string> documentos;
			procesar( temp, documentos );

			struct my_msgbuf codigoInicial;
			codigoInicial.mtype = 1;
			codigoInicial.mtext = '1'; // > 0 todo OK, < 0, el mensaje es un error
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

	destroyChannel( msqCGItoSearchid );
	destroyChannel( msqSearchToCGIid );
	return 0;
}
