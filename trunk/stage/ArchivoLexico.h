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
		long 		  _posicionSecuencial;
		long		  _tamanio;
		long 		  _cantRegistros;

		long posicionLogicaAReal( long posicion );
		void validarModo( int modoBuscado );
		int  escribirImpl( const LexicoData data );
		void leerImpl( LexicoData& data );
		LexicoData *buscarTerminoImpl( string word, LexicoData &foundData, long minimo, long maximo );
	public:
        ArchivoLexico( std::string nombre, int modo );

		void comenzarLectura();
		void leerPosicion( int posicion, LexicoData &data );
		bool leer( LexicoData &data );
		int  escribirPosicion( int posicion, const LexicoData data );
		int  escribir( const LexicoData data );
        bool fin();

		void mergeWith( std::string newLexName, WordPair words, LexicalPair &mergedItems );
		LexicoData *buscarTermino( string word, LexicoData &foundData );
        ~ArchivoLexico();
};

#endif
