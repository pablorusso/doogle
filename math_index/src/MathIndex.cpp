#include "MathIndex.h"

double MathIndex::norm( LexicalPair &items )
{
	long dummy = 0;
	return norm( items, dummy );
}

double MathIndex::norm( LexicalPair &items, long &cantTerminos )
{
	long pesoTotal = 0;
	cantTerminos = 0;
	LexicalPair::iterator curr = items.begin();
	while ( curr != items.end() )
	{
		long peso = static_cast< long >( curr->second );
		pesoTotal += peso * peso;
		cantTerminos++;

		++curr;
	}

	return sqrt( pesoTotal );
}


double MathIndex::cosineRank( LexicalPair &items1, LexicalPair &items2, double norm1, double norm2 )
{
	if ( norm1 == 0 || norm2 == 0 )
		return 1;

	double denominador = norm1 * norm2;

	double numerador = 0;
	LexicalPair::iterator it1 = items1.begin();
	LexicalPair::iterator it2 = items2.begin();
	while ( it1 != items1.end() && it2 != items2.end() )
	{
		int id1 = static_cast< int >( it1->first );
		int id2 = static_cast< int >( it2->first );

		if ( id1 == id2 )
		{
			long peso1 = static_cast< long >( it1->second );
			long peso2 = static_cast< long >( it2->second );
			numerador += peso1 * peso2;

			++it1;
			++it2;
		}
		else
		{
			if ( id1 < id2 )
				++it1;
			else
				++it2;
		}
	}

	return numerador / denominador;
}
