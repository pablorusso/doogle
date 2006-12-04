#ifndef __DOCUMENTDATA_H__
#define __DOCUMENTDATA_H__

#include <string>

using namespace std;

class DocumentData
{
	public:
		int 		id;
    	std::string ruta;
        double      norma;
        int        cantTermDistintos;

		DocumentData();
};

#endif

