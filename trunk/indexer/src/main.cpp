#include <cstdlib>
#include <string>
#include <iostream>

#include "Indexer.h"

using namespace std;

static void usage()
{
	cerr <<	"usage: indexer [path]\n ex: indexer /home/me/html/\n";
	::exit( 1 );
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

int main( int argc, char *argv[] )
{
	if ( argc != 2 ) usage();
	string path( argv[1] );

	Indexer indexer;
	indexer.buildIndex( path );

	std::cout << endl;
 	return 0;
}
