#ifndef __ARCHIVODOCLEXICO_H__
#define __ARCHIVODOCLEXICO_H__

#include <fstream>
#include <string>
#include "ArchivoConsts.h"
#include "DocLexicoData.h"

class ArchivoDocLexico
{
	private:
    	std::fstream  _fstream;
		std::fstream  _fstreamIdx;

        std::string   _nombre;
		std::string   _nombreIdx;
		int			  _modo;
		long 		  _posicionSecuencial;
		long		  _tamanio;
		long 		  _cantRegistros;
		long		  _lastWrite;

		long posicionLogicaAReal( long posicion );
		void validarModo( int modoBuscado );
		int  escribirImpl( const DocLexicoData data );
		void leerImpl( DocLexicoData& data );
	public:
        ArchivoDocLexico( std::string nombre, std::string nombreIdx, int modo );

		void comenzarLectura();
		void leerOffset( long offset, DocLexicoData &data );
		void leerPosicion( int posicion, DocLexicoData &data );
		bool leer( DocLexicoData &data );
		int  escribirPosicion( int posicion, const DocLexicoData data );
		int  escribir( const DocLexicoData data );
        bool fin();
		long ultimaPosicionEscrita();

        ~ArchivoDocLexico();
};

#endif
