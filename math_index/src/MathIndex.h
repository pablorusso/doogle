#ifndef __MATHINDEX_H__
#define __MATHINDEX_H__

#include <math.h>
#include "LexicoData.h"

class MathIndex
{
	public:
		static double norm( LexicalPair &items );
		static double norm( LexicalPair &items, long &cantTerminos );
		static double cosineRank( LexicalPair &items1, LexicalPair &items2, double norm1, double norm2 );
};

#endif
