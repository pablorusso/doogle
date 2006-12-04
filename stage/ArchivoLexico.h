#ifndef __ARCHIVOLEXICO_H__
#define __ARCHIVOLEXICO_H__

#include <fstream>
#include <string>
#include "ArchivoConsts.h"
#include "LexicoData.h"

class ArchivoLexico
{
	private:
    	std::fstream  _fstream;
        std::string   _nombre;
		int			  _modo;
		int 		  _posicionSecuencial;
		int		  _tamanio;
		int 		  _cantRegistros;

		int posicionLogicaAReal( int posicion );
		void validarModo( int modoBuscado );
		int  escribirImpl( LexicoData &data );
		void leerImpl( LexicoData& data );
		LexicoData *buscarTerminoImpl( string word, LexicoData &foundData, int minimo, int maximo );
	public:
        ArchivoLexico( std::string nombre, int modo );

		void comenzarLectura();
		void leerPosicion( int posicion, LexicoData &data );
		bool leer( LexicoData &data );
		int  escribirPosicion( int posicion, LexicoData &data );
		int  escribir( LexicoData &data );
        bool fin();

		void mergeWith( std::string newLexName, WordPair words, LexicalPair &mergedItems );
		LexicoData *buscarTermino( string word, LexicoData &foundData );
        ~ArchivoLexico();
};

#endif
