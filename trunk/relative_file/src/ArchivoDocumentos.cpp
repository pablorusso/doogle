#include "ArchivoDocumentos.h"

#include <iostream>
#include <string>

using namespace std;

typedef struct
{
	int 		id;
    long        offset;
}IdocumentFileReg;


void ArchivoDocumentos::validarModo( int modoBuscado )
{
	if ( ( _modo & modoBuscado ) != modoBuscado )
		throw string ( "Modo invalido" );
}

//COSNTRUCTORES
ArchivoDocumentos::ArchivoDocumentos( std::string nombre, int modo )
{
	_nombre = nombre;
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
          throw string("El archivo no pudo ser abierto");
	}

	string nombreIdx = "I_" + nombre;
    _fstreamIdx.open( nombreIdx.c_str(), modeToUse  );
    if ( !_fstreamIdx.is_open() )
	{
       	_fstreamIdx.clear();
       _fstreamIdx.open( nombreIdx.c_str(), ios::out | ios::binary );
       _fstreamIdx.close();
       _fstream.open( nombreIdx.c_str(), modeToUse );
       if ( !_fstreamIdx.is_open() )
          throw string("El archivo no pudo ser abierto");
	}

    _fstreamIdx.seekg( 0, ios::end );
	_cantRegistros = _fstreamIdx.tellg() / _tamanio;
}

//METODOS-FUNCIONALIDAD
long ArchivoDocumentos::posicionLogicaAReal( long posicion )
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

void ArchivoDocumentos::escribirImpl( DocumentData data )
{
	int newId = _cantRegistros+1;
	long offset = _fstream.tellp();

	void *buffer = malloc( _tamanio );
	IdocumentFileReg *datoNuevo = static_cast<IdocumentFileReg *> ( buffer );
	datoNuevo->id = _cantRegistros+1;
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

	// norma
	temp = &data.norma;
	_fstream.write( static_cast<char*>( temp ), sizeof( double ) );

	// cantTermDistintos
	temp = &data.cantTermDistintos;
	_fstream.write( static_cast<char*>( temp ), sizeof( long ) );
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
    double      norma;
    long        cantTerm;
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

	temp = &norma;
	_fstream.read( static_cast<char*>( temp ), sizeof( double ) );
	temp = &cantTerm;
	_fstream.read( static_cast<char*>( temp ), sizeof( long ) );

	data.id    = id;
	data.ruta  = ruta;
	data.norma = norma;
	data.cantTermDistintos = cantTerm;
}

void ArchivoDocumentos::escribirPosicion( int posicion, DocumentData data )
{
	validarModo( ESCRIBIR );
	_fstreamIdx.seekp ( posicionLogicaAReal( posicion ), ios::beg );
	escribirImpl( data );
}

void ArchivoDocumentos::leerPosicion( int posicion, DocumentData& data )
{
	validarModo( LEER );
	_fstreamIdx.seekg ( posicionLogicaAReal( posicion ), ios::beg );
	leerImpl( data );
}

void ArchivoDocumentos::escribir( const DocumentData data )
{
	validarModo( ESCRIBIR );

	long posicionActual = _fstreamIdx.tellp();
	if ( posicionActual != _posicionSecuencial )
		_fstreamIdx.seekp ( _posicionSecuencial, ios::beg );

	escribirImpl( data );

	_posicionSecuencial = _fstreamIdx.tellp();
}

bool ArchivoDocumentos::leer( DocumentData& data )
{
	validarModo( LEER );

	long posicionActual = _fstreamIdx.tellg();
	if ( posicionActual != _posicionSecuencial )
		_fstreamIdx.seekg (  _posicionSecuencial, ios::beg );

	leerImpl( data );

	_posicionSecuencial = _fstreamIdx.tellg();
	return this->fin();
}

bool ArchivoDocumentos::fin()
{
	bool esEof = ( _fstreamIdx.peek() == char_traits<char>::eof() );
  	return esEof;
}

ArchivoDocumentos::~ArchivoDocumentos()
{
	_fstream.close();
	_fstreamIdx.close();
}
