#include <cstdlib>
#include <string>
#include <iostream>

#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>

#include "ParserWrapper.h"
#include "Lexical.h"

// hay que parametrizarla bien
#define DELTA_EQUAL 0.2

/*
	TODO:
	- Armar las clases:
			Lexical                	-> 	Lexico
			Document			   	-> 	Documentos
			DocumentLexical		   	-> 	Relacion entre documentos y terminos, se usa dos veces una
										para lideres y otra para seguidores
			DocumentRelation		->	Relacion entre documentos lideres y seguidores

	- Armar los datas:
			LexicalData				->	(idTermino, termino, peso)
			DocumentData			->	(idDocumento, ubicacion)
			DocumentLexicalData		->	(idDocumento, cantTerminosDistintos, norma, (lista de LexicalPair) )
			DocumentRelationData	-> 	(idDocumento, idDocumento) [lider, seguidor]
			LexicalPair				-> 	(idTermino, peso)

	- Indexer
			Encapsular la secuencia de generacion de archivos en clases
			Probar!

	- Calculo del coseno
			Ver para que se usan las normas, no lo entiendo bien
			Probar el algoritmo con distintos deltas para encontrar el optimo
			Ver como calcular raiz cuadrada

	- Index
			Crear el proyecto
			Agregar lo de sockets
			Levantar los archivos y resolver consultas

	- Buscador
			Agregar lo de sockets para que llame al index usandolos
*/


using namespace std;

static void usage()
{
	cerr <<	"usage: indexer [path]\n ex: indexer /home/me/html/\n";
	::exit( 1 );
}

int selecc_arch( const struct dirent *entry )
{
	if ( (strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0) )
		return 0;
	else
		return -1;
}

float cosineRank( LexicalPair []pairList1, float norm1, int pairList1Count, LexicalPair []pairList2, float norm2, int pairList2Count )
{
	int i = 0;
	int j = 0;

	AxBxC / n(A)
	float numerador   = 0;
	float denominador = 0;
	while( i < pairList1Count )
	{
		while ( i < pairList1Count && j < pairList2Count )
		{
			int id1 = pairList1[i].GetId();
			int id2 = pairList2[j].GetId();

			if ( id1 == id2 )
			{
				numerador   = numerador + pairList1[i].GetWeight() * pairList2[j].GetWeight();
				denominador = denominador + ( pairList1[i].GetWeight() + pairList2[j].GetWeight() ) * ( pairList1[i].GetWeight() + pairList2[j].GetWeight() );
				i++;
				j++;
			}
			else
			{
				if ( id1 > id2 )
				{
					j++;
				}
				else
				{
					i++
				}
			}
		}
	}

	if ( denominador == 0 )
		return 1;

	// falta la raiz cuadrada y ver como usar las normas
	return numerador / denominador;
}

void buildLeaders( DocumentLexical &docLex, int docCount )
{
	// ver como se calcula esto
	int maxLeaders   = docCount / 2;
	int maxFollowers = docCount / 2;

	DocumentLexical leaders("leaders.dat");
	DocumentLexical followers("followers.dat");
	DocumentLexical withoutLeader("noleader.dat");

	docLex.StartRead();
	// El primero siempre es lider (porque vienen ordenados por cantidad de lexico distinto)
	if ( DocumentLexicalData item = docLex.Read() )
		learders.AddRecord( item );

	// Para cada documento
	while ( DocumentLexicalData item = docLex.Read() )
	{
		DocumentLexicalData bestLeader = NULL;
		bool isFollower = false;
		float lessRank  = 0;


		leaders.StartRead();
		// Para cada lider
		while ( DocumentLexicalData leader = leaders.Read() && ! isFollower )
		{
			// Comparo usando el ranking del coseno el documento con cada lider
			LexicalPair []leaderPairList = leader.GetLexicalPairList();
			LexicalPair []itemPairList   = item.GetLexicalPairList();
			float rank = cosineRank( itemPairList, item.GetNorm(), item.GetLexicalPairCount(), leaderPairList, leader.GetNorm(), leader.GetLexicalPairCount() );

			if ( rank <= DELTA_EQUAL )
			{
				// Si el rankeo del coseno es suficientemente bueno, lo considero seguidor sin ver que pasa mas adelante
				item.SetLeader( leader.GetId() );
				followers.AddRecord( item );

				leader.SetFollowersQuantity( leader.GetFollowersQuantity() + 1 );
				leaders.updateRecord( leader );

				isFollower = true;
			}
			else
			{
				// Si no es tan bueno, me fijo si al menos es el mejor que encontre y me lo guardo
				if ( rank < lessRank )
				{
					lessRank = rank;
					bestLeader = leader;
				}
			}
		}

		if ( ! isFollower )
		{
			// Si no es seguidor natural de nadie

			if ( bestLeader.GetFollowersQuantity < maxFollowers )
			{
				// Si el leader mas cercano todavia acepta seguidores, entonces lo hago seguidor de este
				item.SetLeader( bestLeader.GetId() );
				followers.AddRecord( item );
				bestLeader.SetFollowersQuantity( bestLeader.GetFollowersQuantity() + 1 );
				leaders.updateRecord( bestLeader );
			}
			else
			{
				if ( leaders.GetCount() < maxLeaders )
					// Si aun tengo espacio para liders, lo hago lider
					leaders.AddRecord( item );
				else
					// Si no puede ser ni seguidor ni lider, entonces no encaja con nada y lo mando al otro archivo
					withoutLeader.AddRecord( item );
			}
		}
	}
}

void buildLexical( string path, Lexical &lexical, Document &doc, DocumentLexical &docLex )
{
	int count, i;
	struct direct **files = NULL;
	int selecc_arch( const struct dirent *entry );

	cout << endl << "Listando contenido de la carperta: " << path << endl;
    count = scandir ( path.c_str(), &files, selecc_arch, NULL);
	if ( count <= 0 )
	{
		cout << "No hay archivos en la carpeta";
		exit(0);
	}

	DocumentLexical docLex( "documentLexicalTmp.dat" );
	for ( i=0; i < count; ++i )
	{
		string fileName = path + "/" + files[i]->d_name;

		// Me fijo si es una carpeta
		bool isAFile = true;
		 if( chdir( fileName.c_str() ) == 0 )
			 isAFile = false;

		cout << "Importando lexico desde " << fileName << ": ";
		if ( isAFile )
		{
			DocumentData docData( fileName );
			doc.AddRecord( docData );

			Lexical lex( "lexicalPartition.dat" );
			try
			{
				ParserWrapper parser ( fileName, lex );
				parser.Parse();

				std::cout << "Lexico importado con exito" << endl;
			}
			catch ( InvalidXmlException ex )
			{
				std::cout << "Archivo mal formado. Error: " << ex.what() << endl;
			}
			catch ( CantOpenFileException )
			{
				std::cout << "No se puede abrir el archivo" << endl;
			}

			LexicalPair []mergedItems = lexical.MergeWith( lex );

			int count = lex->GetCount();
			DocumentLexicalData docLexData( docData.GetId(), count );
			for( int i = 0; i < count; ++i )
				docLexData.AddLexicalPair( mergedItems[i] );
			docLex.AddRecord( docLexData );
		}
		else
		{
			cout << "Es un directorio, navegando hacia adentro..." << endl;
			parsearPath( fileName );
		}
	}
}

int main( int argc, char *argv[] )
{
	if ( argc != 2 ) usage();
	string path( argv[1] );

	Lexical 		lexical( "lexical.dat" );
	Document 		doc( "documents.dat" );
	DocumentLexical docLex( "documentLexicalTmp.dat" );

	buildLexical( path, lexical, doc, docLex );

	docLex.sortByQuantity();

	buildLeaders( docLex, doc.GetCount() );

	std::cout << endl;
 	return 0;
}
