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
		void escribirImpl( const LexicoData data );
		void leerImpl( LexicoData& data );
	public:
        ArchivoLexico( std::string nombre, int modo );

		void comenzarLectura();
		void leerPosicion( int posicion, LexicoData &data );
		bool leer( LexicoData &data );
		void escribirPosicion( int posicion, const LexicoData data );
		void escribir( const LexicoData data );
        bool fin();

		void mergeWith( std::string newLexName, WordPair words, LexicalPair &mergedItems );
        ~ArchivoLexico();
};

#endif
