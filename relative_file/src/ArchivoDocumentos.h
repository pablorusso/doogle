#ifndef __ARCHIVODOCUMENTOS_H__
#define __ARCHIVODOCUMENTOS_H__

#include <fstream>
#include <string>
#include "DocumentData.h"

#define LEER 1
#define ESCRIBIR 2

class ArchivoDocumentos
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
		void escribirImpl( const DocumentData data );
		void leerImpl( DocumentData& data );
	public:
		//CONSTRUCTORES
        ArchivoDocumentos( std::string nombre, int modo );

        //METODOS - FUNCIONALIDAD
		void comenzarLectura();

		void leerPosicion( int posicion, DocumentData &data );
		bool leer( DocumentData &data );

		void escribirPosicion( int posicion, const DocumentData data );
		void escribir( const DocumentData data );

        bool fin();

        //DESTRUCTOR
        ~ArchivoDocumentos();
};

#endif
