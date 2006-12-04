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
		int 		  _posicionSecuencial;
		int		  _tamanio;
		int 		  _cantRegistros;

		int posicionLogicaAReal( int posicion );
		void validarModo( int modoBuscado );
		int  escribirImpl( DocumentData &data );
		void leerImpl( DocumentData& data );
		DocumentData *buscarPorIdImpl( int docId, DocumentData &foundData, int minimo, int maximo );
	public:
        ArchivoDocumentos( std::string nombre, std::string nombreIdx, int modo );

		void comenzarLectura();
		void leerPosicion( int posicion, DocumentData &data );
		bool leer( DocumentData &data );
		int  escribirPosicion( int posicion, DocumentData &data );
		int  escribir( DocumentData &data );
        bool fin();

		DocumentData *buscarPorId( int docId, DocumentData &foundData );
		int size();

        ~ArchivoDocumentos();
};

#endif
