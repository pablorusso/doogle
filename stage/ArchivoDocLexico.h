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
		int 		  _posicionSecuencial;
		int		  	  _tamanio;
		int 		  _cantRegistros;
		int		  	  _lastWrite;

		int posicionLogicaAReal( int posicion );
		void validarModo( int modoBuscado );
		int  escribirImpl( DocLexicoData &data );
		void leerImpl( DocLexicoData& data );
	public:
        ArchivoDocLexico( std::string nombre, std::string nombreIdx, int modo );

		void comenzarLectura();
		void leerOffset( int offset, DocLexicoData &data );
		void leerPosicion( int posicion, DocLexicoData &data );
		bool leer( DocLexicoData &data );
		int  escribirPosicion( int posicion, DocLexicoData &data );
		int  escribir( DocLexicoData &data );
        bool fin();
		int ultimaPosicionEscrita();

        ~ArchivoDocLexico();
};

#endif
