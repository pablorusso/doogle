#ifndef __ARCHIVODOCUMENTOS_H__
#define __ARCHIVODOCUMENTOS_H__

#include <fstream>
#include <string>
#include "ArchivoConsts.h"
#include "DocumentData.h"

class ArchivoDocumentos
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

		long posicionLogicaAReal( long posicion );
		void validarModo( int modoBuscado );
		int  escribirImpl( const DocumentData data );
		void leerImpl( DocumentData& data );
	public:
        ArchivoDocumentos( std::string nombre, std::string nombreIdx, int modo );

		void comenzarLectura();
		void leerPosicion( int posicion, DocumentData &data );
		bool leer( DocumentData &data );
		int  escribirPosicion( int posicion, const DocumentData data );
		int  escribir( const DocumentData data );
        bool fin();

		long size();

        ~ArchivoDocumentos();
};

#endif
