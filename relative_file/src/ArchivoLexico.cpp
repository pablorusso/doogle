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
	_fstream.seekg( _posicionSecuencial );
}

int ArchivoLexico::escribirImpl( const LexicoData data )
{
	void *buffer = malloc( _tamanio );

	LexicoFileReg *datoNuevo = static_cast<LexicoFileReg *> ( buffer );
	int newId = data.id <= 0 ? _cantRegistros+1 : data.id;

	datoNuevo->id = newId;
	strcpy( datoNuevo->termino, data.termino.c_str() );
    _fstream.write( static_cast<const char*>( buffer ), _tamanio );
	delete datoNuevo;

	_cantRegistros++;

	return newId;
}

void ArchivoLexico::leerImpl( LexicoData& data )
{
	void *buffer = malloc( _tamanio );
    _fstream.read( static_cast<char*>( buffer ), _tamanio );

	LexicoFileReg *datoLeido = static_cast<LexicoFileReg *> ( buffer );
	data.id = datoLeido->id;
	data.termino = datoLeido->termino;
	delete datoLeido;
}

int ArchivoLexico::escribirPosicion( int posicion, const LexicoData data )
{
	validarModo( ESCRIBIR );
	_fstream.seekp ( posicionLogicaAReal( posicion ), ios::beg );
	return escribir( data );
}

void ArchivoLexico::leerPosicion( int posicion, LexicoData& data )
{
	validarModo( LEER );
	_fstream.seekg ( posicionLogicaAReal( posicion ), ios::beg );
	leerImpl( data );
}

int ArchivoLexico::escribir( const LexicoData data )
{
	validarModo( ESCRIBIR );

	_fstream.seekp( 0, ios::end );
	return escribirImpl( data );
}

bool ArchivoLexico::leer( LexicoData& data )
{
	validarModo( LEER );

	long posicionActual = _fstream.tellg();
	if ( posicionActual != _posicionSecuencial )
		_fstream.seekg ( posicionLogicaAReal( _posicionSecuencial ), ios::beg );

	leerImpl( data );

	_posicionSecuencial = _fstream.tellg();
	return !this->fin();
}

bool ArchivoLexico::fin()
{
	return _fstream.eof ();
}

void ArchivoLexico::mergeWith( std::string newLexName, WordPair words, LexicalPair &mergedItems )
{
	long nextId = _cantRegistros+1;
	LexicoData data;
	ArchivoLexico tempLexico( newLexName, ESCRIBIR );

	comenzarLectura();

	leer( data );
	WordPair::iterator curr = words.begin();
	while ( !fin() && curr != words.end() )
	{
		std::string word = static_cast< std::string >( curr->first );
		long peso = static_cast< long >( curr->second );

		if ( data.termino == word )
		{
			// lo paso a los items mergeados
			int newId = tempLexico.escribir( data );
			mergedItems[ newId ] = peso;

			leer( data );
			++curr;
		}
		else
		{
			if ( data.termino < word )
			{
				tempLexico.escribir( data );
				leer( data );
			}
			else
			{
				LexicoData newData;
				newData.id = nextId;
				newData.termino = word;
				tempLexico.escribir( newData );

				mergedItems[ newData.id ] = peso;

				nextId++;
				++curr;
			}
		}
	}

	while ( ! fin() )
	{
		tempLexico.escribir( data );
		leer( data );
	}

	while ( curr != words.end() )
	{
		std::string word = static_cast< std::string >( curr->first );
		long peso = static_cast< long >( curr->second );
		LexicoData newData;
		newData.id = nextId;
		newData.termino = word;
		tempLexico.escribir( newData );

		mergedItems[ newData.id ] = peso;
		nextId++;
		++curr;
	}
}

ArchivoLexico::~ArchivoLexico()
{
	_fstream.close();
}
