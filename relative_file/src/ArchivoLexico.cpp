#include "ArchivoLexico.h"

#include <iostream>
#include <string>

using namespace std;

typedef struct
{
	int 	id;
    char 	termino[25];
} LexicoFileReg;

void ArchivoLexico::validarModo( int modoBuscado )
{
	if ( ( _modo & modoBuscado ) != modoBuscado )
		throw string ( "Modo invalido" );
}

//COSNTRUCTORES
ArchivoLexico::ArchivoLexico( std::string nombre, int modo )
{
	_nombre = nombre;
	_modo = modo;
	_tamanio = sizeof( LexicoFileReg );
	_posicionSecuencial = 0;

	ios_base::openmode modeToUse = ios::binary;
	if ( ( modo & LEER ) == LEER )
		modeToUse |= ios::in;
	if ( ( modo & ESCRIBIR ) == ESCRIBIR )
		modeToUse |= ios::out;

	//Inicializo el flujo de entrada/salida
    _fstream.open( nombre.c_str(), modeToUse  );

    //Se verifica si el archivo abre correctamente, sino lo crea.
    if ( !_fstream.is_open() )
	{
		/* limpia los flags de control de estado del archivo */
       	_fstream.clear();

       /* crea el archivo */
       _fstream.open( nombre.c_str(), ios::out | ios::binary );
       _fstream.close();

       /* Se reabre el archivo para lectura/escritura binario */
       _fstream.open( nombre.c_str(), modeToUse );

       /* Se verifica la creacion del archivo */
       if ( !_fstream.is_open() )
          throw string("El archivo no pudo ser abierto");
	}

	_fstream.seekg( 0, ios::end );
	_cantRegistros = _fstream.tellg() / _tamanio;
}

//METODOS-FUNCIONALIDAD
long ArchivoLexico::posicionLogicaAReal( long posicion )
{
	if ( posicion < 0 || posicion > _cantRegistros )
		throw string ( "La posicion esta fuera del rango permitido" );
	return _tamanio * posicion;
}

void ArchivoLexico::comenzarLectura()
{
	validarModo( LEER );
	_posicionSecuencial = 0;
}

void ArchivoLexico::escribirImpl( const LexicoData data )
{
	void *buffer = malloc( _tamanio );

	LexicoFileReg *datoNuevo = static_cast<LexicoFileReg *> ( buffer );
	datoNuevo->id = _cantRegistros+1;
	strcpy( datoNuevo->termino, data.termino.c_str() );
	/* escribe el registro en el archivo */
    _fstream.write( static_cast<const char*>( buffer ), _tamanio );
	/* chequea si se ha producido un error */
    if ( _fstream.fail() )
      throw string("No se pudo escribir correctamente en el archivo");

	_cantRegistros++;

	delete datoNuevo;
}

void ArchivoLexico::leerImpl( LexicoData& data )
{
	/* lee del archivo un registro */
	void *buffer = malloc( _tamanio );
    _fstream.read( static_cast<char*>( buffer ), _tamanio );
	if ( _fstream.fail() )
    	throw string("No se pudo leer correctamente del archivo");

	LexicoFileReg *datoLeido = static_cast<LexicoFileReg *> ( buffer );
	data.id = datoLeido->id;
	data.termino = datoLeido->termino;
	delete datoLeido;
}

void ArchivoLexico::escribirPosicion( int posicion, const LexicoData data )
{
	validarModo( ESCRIBIR );
	_fstream.seekp ( posicionLogicaAReal( posicion ), ios::beg );
	escribir( data );
}

void ArchivoLexico::leerPosicion( int posicion, LexicoData& data )
{
	validarModo( LEER );

	_fstream.seekg ( posicionLogicaAReal( posicion ), ios::beg );
	leerImpl( data );
}

void ArchivoLexico::escribir( const LexicoData data )
{
	validarModo( ESCRIBIR );

	long posicionActual = _fstream.tellp();
	if ( posicionActual != _posicionSecuencial )
		_fstream.seekp ( posicionLogicaAReal( _posicionSecuencial ), ios::beg );

	escribirImpl( data );

	_posicionSecuencial = _fstream.tellp();
}

bool ArchivoLexico::leer( LexicoData& data )
{
	validarModo( LEER );

	long posicionActual = _fstream.tellg();
	if ( posicionActual != _posicionSecuencial )
		_fstream.seekg ( posicionLogicaAReal( _posicionSecuencial ), ios::beg );

	leerImpl( data );

	_posicionSecuencial = _fstream.tellg();
	bool esEof = _fstream.peek() == char_traits<char>::eof();
	return esEof;
}

bool ArchivoLexico::fin()
{
	/* para comprobar el fin lee un char del buffer, sin retirarlo y lo compara con el fin de archivo */
	bool esEof = ( _fstream.peek() == char_traits<char>::eof() );
	/*
	if ( esEof )
    	_fstream.clear();
	*/
  	return esEof;
}

ArchivoLexico::~ArchivoLexico()
{
	_fstream.close();
}


