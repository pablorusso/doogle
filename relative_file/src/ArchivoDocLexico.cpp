#include "ArchivoDocLexico.h"

#include <iostream>
#include <string>

using namespace std;

typedef struct
{
	int id;
	int	cantTerminos;
	int	cantSeguidores;
    int	offset;
} IdocLexicoFileReg;


void ArchivoDocLexico::validarModo( int modoBuscado )
{
	if ( ( _modo & modoBuscado ) != modoBuscado )
		throw string ( "Modo invalido" );
}

//COSNTRUCTORES
ArchivoDocLexico::ArchivoDocLexico( std::string nombre, std::string nombreIdx, int modo )
{
	_nombre = nombre;
	_nombreIdx = nombreIdx;
	_modo = modo;
	_tamanio = sizeof( IdocLexicoFileReg );
	_posicionSecuencial = 0;

	ios_base::openmode modeToUse = ios::binary;
	if ( ( modo & LEER ) == LEER )
		modeToUse |= ios::in;
	if ( ( modo & ESCRIBIR ) == ESCRIBIR )
		modeToUse |= ios::out;

	// Archivo original
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

	// Indice
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

	// Calculo la cantida de registros en base al indice
    _fstreamIdx.seekg( 0, ios::end );
	_cantRegistros = _fstreamIdx.tellg() / _tamanio;
}

//METODOS-FUNCIONALIDAD
int ArchivoDocLexico::posicionLogicaAReal( int posicion )
{
	if ( posicion < 0 || posicion > _cantRegistros )
		throw string ( "La posicion esta fuera del rango permitido" );
	return _tamanio * posicion;
}

void ArchivoDocLexico::comenzarLectura()
{
	validarModo( LEER );
	_posicionSecuencial = 0;
	_fstreamIdx.seekg( _posicionSecuencial );
}

int ArchivoDocLexico::escribirImpl( DocLexicoData &data )
{
	int newId = data.id <= 0 ? _cantRegistros+1 : data.id;
	int offset = _fstream.tellp();

	// id
	void *temp = &newId;
	_fstream.write( static_cast<const char *>( temp ), sizeof( int ) );

	// norma
	double norma = data.norma;
	temp = &norma;
	_fstream.write( static_cast<const char *>( temp ), sizeof( double ) );

	// terminos
	LexicalPair::iterator curr = data.terminos.begin();
	while ( curr != data.terminos.end() )
	{
		int idTermino = static_cast< int >( curr->first );
   		temp = &idTermino;
		_fstream.write( static_cast<const char *>( temp ), sizeof( int ) );

		int peso = static_cast< int >( curr->second );
   		temp = &peso;
		_fstream.write( static_cast<const char *>( temp ), sizeof(int) );
		++curr;
	}

	// seguidores
	Seguidores::iterator currSeg = data.seguidores.begin();
	while ( currSeg != data.seguidores.end() )
	{
		int idDoc = static_cast< int >( currSeg->first );
   		temp = &idDoc;
		_fstream.write( static_cast<const char *>( temp ), sizeof(int) );

		int offset_seg = static_cast< int >( currSeg->second );
   		temp = &offset_seg;
		_fstream.write( static_cast<const char *>( temp ), sizeof(int) );

		++currSeg;
	}

	_lastWrite = _fstreamIdx.tellp();

	// indice
	void *buffer = malloc( _tamanio );
	IdocLexicoFileReg *datoNuevo = static_cast<IdocLexicoFileReg *> ( buffer );
	datoNuevo->id = newId;
	datoNuevo->cantTerminos = data.terminos.size();
	datoNuevo->cantSeguidores = data.seguidores.size();
	datoNuevo->offset = offset;
    _fstreamIdx.write( static_cast<const char*>( buffer ), _tamanio );
	delete datoNuevo;

	_cantRegistros++;
	return newId;
}

void ArchivoDocLexico::leerImpl( DocLexicoData& data )
{
	void *buffer = malloc( _tamanio );
    _fstreamIdx.read( static_cast<char*>( buffer ), _tamanio );
	IdocLexicoFileReg * leido = static_cast<IdocLexicoFileReg*>( buffer );
	data.id 	= leido->id;
	int offset = leido->offset;
	int cantTerminos = leido->cantTerminos;
	int cantSeguidores = leido->cantSeguidores;
	delete leido;

	_fstream.seekg( offset );

	// id
	int id = 0;
	void* temp = &id;
	_fstream.read( static_cast<char *>( temp ), sizeof( int ) );

	// norma
	double norma = 0;
	temp = &norma;
	_fstream.read( static_cast<char *>( temp ), sizeof( double ) );
	data.norma = norma;

	// terminos y seguidores
	data.terminos.clear();
	while ( cantTerminos > 0 )
	{
		int idTermino=0; int peso=0;
		temp = &idTermino;
		_fstream.read( static_cast<char *>( temp ), sizeof( int ) );

		temp = &peso;
		_fstream.read( static_cast<char  *>( temp ), sizeof( int ) );

		data.terminos[ idTermino ] = peso;
		cantTerminos--;
	}

	data.seguidores.clear();
	while ( cantSeguidores > 0 )
	{
		int idDoc=0; int offset_seg=0;
		temp = &idDoc;
		_fstream.read( static_cast<char *>( temp ), sizeof( int ) );

		temp = &offset_seg;
		_fstream.read( static_cast<char *>( temp ), sizeof( int ) );

		data.seguidores[ idDoc ] = offset_seg;
		cantSeguidores--;
	}
}

int ArchivoDocLexico::ultimaPosicionEscrita()
{
	return _lastWrite;
}

int ArchivoDocLexico::escribirPosicion( int posicion, DocLexicoData &data )
{
	validarModo( ESCRIBIR );
	_fstreamIdx.seekp ( posicionLogicaAReal( posicion ), ios::beg );
	return escribirImpl( data );
}

void ArchivoDocLexico::leerPosicion( int posicion, DocLexicoData& data )
{
	validarModo( LEER );
	_fstreamIdx.seekg ( posicionLogicaAReal( posicion ), ios::beg );
	leerImpl( data );
}

void ArchivoDocLexico::leerOffset( int offset, DocLexicoData &data )
{
	validarModo( LEER );
	_fstreamIdx.seekg ( offset, ios::beg );
	leerImpl( data );
}

int ArchivoDocLexico::escribir( DocLexicoData &data )
{
	validarModo( ESCRIBIR );

	_fstreamIdx.seekp( 0, ios::end );
	_fstream.seekp( 0, ios::end );

	return escribirImpl( data );
}

bool ArchivoDocLexico::leer( DocLexicoData& data )
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

bool ArchivoDocLexico::fin()
{
	return _fstreamIdx.eof ()  || _cantRegistros == 0;
}

ArchivoDocLexico::~ArchivoDocLexico()
{
	_fstream.close();
	_fstreamIdx.close();
}


