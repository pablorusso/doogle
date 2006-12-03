#include "ArchivoDocLexico.h"

#include <iostream>
#include <string>

using namespace std;

typedef struct
{
	int 		id;
	int			cantTerminos;
	int			cantSeguidores;
    long        offset;
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
long ArchivoDocLexico::posicionLogicaAReal( long posicion )
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

void ArchivoDocLexico::escribirImpl( DocLexicoData data )
{
	int newId   = _cantRegistros+1;
	long offset = _fstream.tellp();

	// id
	void *temp = &newId;
	_fstream.write( static_cast<const char *>( temp ), sizeof( int ) );

	// terminos
	int cantTerminos = 0;
	int idTermino; long peso;
	LexicalPair::iterator curr = data.terminos.begin();
	while ( curr != data.terminos.end() )
	{
		idTermino = static_cast< int >( curr->first );
   		temp = &idTermino;
		_fstream.write( static_cast<const char *>( temp ), sizeof( int ) );

		peso = static_cast< long >( curr->second );
   		temp = &peso;
		_fstream.write( static_cast<const char *>( temp ), sizeof(long) );

		++curr;
		cantTerminos++;
	}

	// seguidores
	int cantSeguidores = 0;
	int idDoc; long offset_seg;
	Seguidores::iterator currSeg = data.seguidores.begin();
	while ( currSeg != data.seguidores.end() )
	{
		idDoc = static_cast< int >( currSeg->first );
   		temp = &idDoc;
		_fstream.write( static_cast<const char *>( temp ), sizeof(int) );

		offset = static_cast< long >( currSeg->second );
   		temp = &offset_seg;
		_fstream.write( static_cast<const char *>( temp ), sizeof(long) );

		++currSeg;
		cantSeguidores++;
	}

	// indice
	void *buffer = malloc( _tamanio );
	IdocLexicoFileReg *datoNuevo = static_cast<IdocLexicoFileReg *> ( buffer );
	datoNuevo->id = newId;
	datoNuevo->cantTerminos = cantTerminos;
	datoNuevo->cantSeguidores = cantSeguidores;
	datoNuevo->offset = offset;
    _fstreamIdx.write( static_cast<const char*>( buffer ), _tamanio );
	delete datoNuevo;

	_cantRegistros++;
}

void ArchivoDocLexico::leerImpl( DocLexicoData& data )
{
	void *buffer = malloc( _tamanio );
    _fstreamIdx.read( static_cast<char*>( buffer ), _tamanio );
	IdocLexicoFileReg * leido = static_cast<IdocLexicoFileReg*>( buffer );
	data.id 	= leido->id;
	long offset = leido->offset;
	int cantTerminos = leido->cantTerminos;
	int cantSeguidores = leido->cantSeguidores;
	delete leido;

	_fstream.seekg( offset );

	int id = 0;
	void* temp = &id;
	_fstream.read( static_cast<char *>( temp ), sizeof( int ) );

	// terminos y seguidores
	LexicalPair terminos = data.terminos;
    Seguidores  seguidores = data.seguidores;

	int idTermino; long peso;
	while ( cantTerminos > 0 )
	{
		temp = &idTermino;
		_fstream.read( static_cast<char *>( temp ), sizeof( int ) );

		temp = &peso;
		_fstream.read( static_cast<char  *>( temp ), sizeof( long ) );

		terminos[ idTermino ] = peso;
		cantTerminos--;
	}

	int idDoc; long offset_seg;
	while ( cantTerminos > 0 )
	{
		temp = &idDoc;
		_fstream.read( static_cast<char *>( temp ), sizeof( int ) );

		temp = &offset_seg;
		_fstream.read( static_cast<char *>( temp ), sizeof( long ) );

		seguidores[ idDoc ] = offset_seg;
		cantSeguidores--;
	}
}

void ArchivoDocLexico::escribirPosicion( int posicion, DocLexicoData data )
{
	validarModo( ESCRIBIR );
	_fstreamIdx.seekp ( posicionLogicaAReal( posicion ), ios::beg );
	escribirImpl( data );
}

void ArchivoDocLexico::leerPosicion( int posicion, DocLexicoData& data )
{
	validarModo( LEER );
	_fstreamIdx.seekg ( posicionLogicaAReal( posicion ), ios::beg );
	leerImpl( data );
}

void ArchivoDocLexico::escribir( const DocLexicoData data )
{
	validarModo( ESCRIBIR );

	long posicionActual = _fstreamIdx.tellp();
	if ( posicionActual != _posicionSecuencial )
		_fstreamIdx.seekp ( _posicionSecuencial, ios::beg );

	escribirImpl( data );

	_posicionSecuencial = _fstreamIdx.tellp();
}

bool ArchivoDocLexico::leer( DocLexicoData& data )
{
	validarModo( LEER );

	long posicionActual = _fstreamIdx.tellg();
	if ( posicionActual != _posicionSecuencial )
		_fstreamIdx.seekg (  _posicionSecuencial, ios::beg );

	leerImpl( data );

	_posicionSecuencial = _fstreamIdx.tellg();
	return this->fin();
}

bool ArchivoDocLexico::fin()
{
	bool esEof = ( _fstreamIdx.peek() == char_traits<char>::eof() );
  	return esEof;
}

ArchivoDocLexico::~ArchivoDocLexico()
{
	_fstream.close();
	_fstreamIdx.close();
}


