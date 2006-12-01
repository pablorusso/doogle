#include <iostream>
#include <cstdlib>
#include <sstream>

#include "ArchivoLexico.h"
#include "LexicoData.h"

using namespace std;
//TIPO DE RAGISTRO DE ARCHIVO_LEXICO.

int main(int argc, char *argv[])
{
   // string cadena;
   // cout << "nombre para el archivo : " << endl;
   // cin >> cadena;
   //ArchivoLexico *ptrArch = new ArchivoLexico( cadena , sizeof(DatoLexico) );
	/* cout << "ARCHIVO : " << endl;
		cout << "nombre   :" << ptrArch->getNombre() << endl;
		cout << "tamanio  :" << ptrArch->getTamanio() << endl;
		cout << "estado   :" << ptrArch->getEstado();
		*/

	LexicoData dato;


	ArchivoLexico *ptrArch = new ArchivoLexico( "Z.dat" , ESCRIBIR );
	try
	{
		for( int i=1; i <= 5 ; i++ )
		{
			dato.id = i*5;

			ostringstream os;
			os << i;
			dato.termino = "termino " + os.str();
			ptrArch->escribir( dato );
		}
	}
	catch ( string a )
	{
		cout << endl << a << endl ;
	}
	delete ptrArch;

	ptrArch = new ArchivoLexico( "Z.dat" , LEER );
	try
	{
		ptrArch->comenzarLectura();
		for( int i=1; i <= 5 ; i++ )
		{
			ptrArch->leer( dato );
			cout << "REGISTRO: " << i << " - id: " << dato.id << " - palabra: " << dato.termino << endl;
		}

	}
	catch ( string a )
	{
		cout << endl << a << endl ;
	}
	delete ptrArch;

    return 0;
}
