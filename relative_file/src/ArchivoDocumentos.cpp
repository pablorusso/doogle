#include "ArchivoDocumentos.h"

#include <iostream>
#include <string>

using namespace std;

typedef struct
{
	int 		id;
    int        offset;
}IdocumentFileReg;


void ArchivoDocumentos::validarModo( int modoBuscado )
{
	if ( ( _modo & modoBuscado ) != modoBuscado )
		throw string ( "Modo invalido" );
}

//COSNTRUCTORES
ArchivoDocumentos::ArchivoDocumentos( std::string nombre, std::string nombreIdx, int modo )
{
	_nombre = nombre;
	_nombreIdx = nombreIdx;
	_modo = modo;
	_tamanio = sizeof( IdocumentFileReg );
	_posicionSecuencial = 0;

	ios_base::openmode modeToUse = ios::binary;
	if ( ( modo & LEER ) == LEER )
		modeToUse |= ios::in;
	if ( ( modo & ESCRIBIR ) == ESCRIBIR )
		modeToUse |= ios::out;

    _fstream.open( nombre.c_str(), modeToUse  );
    if ( !_fstream.is_open() )
	{
       _fstream.clear();
       _fstream.open( nombre.c_str(), ios::out | ios::binary );
       _fstream.close();
       _fstream.open( nombre.c_str(), modeToUse );
       if ( !_fstream.is_open() )
          throw string("El archivo " + nombre + " no pudo ser abierto");
	}

    _fstreamIdx.open( nombreIdx.c_str(), modeToUse  );
    if ( !_fstreamIdx.is_open() )
	{
       _fstreamIdx.clear();
       _fstreamIdx.open( nombreIdx.c_str(), ios::out | ios::binary );
       _fstreamIdx.close();
       _fstreamIdx.open( nombreIdx.c_str(), modeToUse );
       if ( !_fstreamIdx.is_open() )
          throw string("El archivo " + nombreIdx + " no pudo ser abierto");
	}

    _fstreamIdx.seekg( 0, ios::end );
	_cantRegistros = _fstreamIdx.tellg() / _tamanio;
}

//METODOS-FUNCIONALIDAD
int ArchivoDocumentos::posicionLogicaAReal( int posicion )
{
	if ( posicion < 0 || posicion > _cantRegistros )
		throw string ( "La posicion esta fuera del rango permitido" );
	return _tamanio * posicion;
}

void ArchivoDocumentos::comenzarLectura()
{
	validarModo( LEER );
	_posicionSecuencial = 0;
	_fstreamIdx.seekg( _posicionSecuencial );
}

int ArchivoDocumentos::escribirImpl( DocumentData &data )
{
	int newId = _cantRegistros+1;
	int offset = _fstream.tellp();

	void *buffer = malloc( _tamanio );
	IdocumentFileReg *datoNuevo = static_cast<IdocumentFileReg *> ( buffer );
	if ( data.id <= 0 )
		datoNuevo->id = _cantRegistros+1;
	else
		datoNuevo->id = data.id;

	datoNuevo->offset = offset;
    _fstreamIdx.write( static_cast<const char*>( buffer ), _tamanio );
	_cantRegistros++;
	delete datoNuevo;

  	string ruta;

	// id
	void *temp = &newId;
	_fstream.write( static_cast<char*>( temp ), sizeof(int) );

	// ruta
	char c;
	for(unsigned i=0; i < data.ruta.length(); i++ )
	{
		c = data.ruta[ i ];
		temp = &c;
		_fstream.write( static_cast<char*>( temp ), sizeof(char) );
	}

	// barra cero
	c = '\0';
	temp = &c;
	_fstream.write( static_cast<char*>( temp ), sizeof(char) );

	return newId;
}

void ArchivoDocumentos::leerImpl( DocumentData& data )
{
	/* lee del archivo un registro */
	void *buffer = malloc( _tamanio );
    _fstreamIdx.read( static_cast<char*>( buffer ), _tamanio );
	if ( _fstream.fail() )
    	throw string("No se pudo leer correctamente del archivo");

	IdocumentFileReg * leido = static_cast<IdocumentFileReg*>( buffer );
	_fstream.seekg( leido->offset );

	delete leido;

    int 		id;
  	string      ruta;
	char        c;

	void* temp = &id;
	_fstream.read( static_cast<char*>( temp ), sizeof(int) );
	temp = &c;
	_fstream.read( static_cast<char*>( temp ), sizeof(char) );
	while( c != '\0' )
	{
		ruta += c;
		_fstream.read( static_cast<char*>( temp ), sizeof(char) );
	}

	data.id    = id;
	data.ruta  = ruta;
}

int ArchivoDocumentos::escribirPosicion( int posicion, DocumentData &data )
{
	validarModo( ESCRIBIR );
	_fstreamIdx.seekp ( posicionLogicaAReal( posicion ), ios::beg );
	return escribirImpl( data );
}

void ArchivoDocumentos::leerPosicion( int posicion, DocumentData& data )
{
	validarModo( LEER );
	_fstreamIdx.seekg ( posicionLogicaAReal( posicion ), ios::beg );
	leerImpl( data );
}

int ArchivoDocumentos::escribir( DocumentData &data )
{
	validarModo( ESCRIBIR );

	_fstreamIdx.seekp( 0, ios::end );
	_fstream.seekp( 0, ios::end );

	return escribirImpl( data );
}

bool ArchivoDocumentos::leer( DocumentData& data )
{
	validarModo( LEER );

	int posicionActual = _fstreamIdx.tellg();
	if ( posicionActual != _posicionSecuencial )
		_fstreamIdx.seekg (  _posicionSecuencial, ios::beg );

	if ( this->fin() ) return false;

	leerImpl( data );

	_posicionSecuencial = _fstreamIdx.tellg();
	return !this->fin();
}

DocumentData *ArchivoDocumentos::buscarPorIdImpl( int docId, DocumentData &foundData, int minimo, int maximo )
{
	if ( minimo > maximo )
		return NULL;

	int posToRead = ( maximo + minimo ) / 2;
	leerPosicion( posToRead, foundData );

	if ( foundData.id == docId )
		return &foundData;
	else
	{
		if ( docId > foundData.id )
			return buscarPorIdImpl( docId, foundData, posToRead+1, maximo );
		return buscarPorIdImpl( docId, foundData, minimo, posToRead-1 );
	}
}

DocumentData *ArchivoDocumentos::buscarPorId( int docId, DocumentData &foundData )
{
	return buscarPorIdImpl( docId, foundData, 0, _cantRegistros-1 );
}

bool ArchivoDocumentos::fin()
{
	return _fstreamIdx.eof()  || _cantRegistros == 0;
}

int ArchivoDocumentos::size()
{
	return _cantRegistros;
}

ArchivoDocumentos::~ArchivoDocumentos()
{
	_fstream.close();
	_fstreamIdx.close();
}
