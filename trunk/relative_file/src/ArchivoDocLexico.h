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
		int			  _modo;
		long 		  _posicionSecuencial;
		long		  _tamanio;
		long 		  _cantRegistros;

		long posicionLogicaAReal( long posicion );
		void validarModo( int modoBuscado );
		void escribirImpl( const DocLexicoData data );
		void leerImpl( DocLexicoData& data );
	public:
        ArchivoDocLexico( std::string nombre, int modo );

		void comenzarLectura();
		void leerPosicion( int posicion, DocLexicoData &data );
		bool leer( DocLexicoData &data );
		void escribirPosicion( int posicion, const DocLexicoData data );
		void escribir( const DocLexicoData data );
        bool fin();

        ~ArchivoDocLexico();
};

#endif
