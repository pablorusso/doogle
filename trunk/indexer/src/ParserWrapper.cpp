#include "ParserWrapper.h"

#include <string>
#include <fstream>
#include <iostream>

#include "ArchivoLexico.h"
#include "LexicoData.h"

void ParserWrapper::charHandler( void *userData, const XML_Char *s, int len )
{
	ArchivoLexico* lex = ( ArchivoLexico* ) userData;

	string word = "";
 	for ( int i = 0; i < len; i++)
	{
		if ( s[i] != ' ' )
		{
			word += s[i];
		}
		else
		{
			if ( word != "" )
			{
				if ( lex != NULL )
				{
					LexicalData data;
					data.termino = word;
					lex->escribir( data );
				}
				word = "";
			}
		}
	}

	if ( word != "" )
	{
		if ( lex != NULL )
		{
			LexicalData data;
			data.termino = word;
			lex->escribir( data );
		}

		word = "";
	}
}

ParserWrapper::ParserWrapper( string fileName, ArchivoLexico &lexico )
{
	_parser = XML_ParserCreate( NULL );
	XML_SetCharacterDataHandler( _parser, charHandler );
	XML_SetUserData( _parser, &lexico );

	_xmlFile = new ifstream( fileName.c_str() );
	if ( ! _xmlFile->is_open() )
		throw string( "No se pudo abrir el archivo " + fileName );
}

void ParserWrapper::Parse()
{
	std::string buffer;
	std::getline( *_xmlFile, buffer );
	bool done = _xmlFile->fail();

	while ( !done )
	{
		if ( ! XML_Parse( _parser, buffer.c_str(), buffer.length(), done ) )
			throw string( XML_ErrorString( XML_GetErrorCode( _parser ) ) + " en la linea " + XML_GetCurrentLineNumber( _parser ) );

		std::getline( *_xmlFile, buffer );
		done = _xmlFile->fail();
	}
}

ParserWrapper::~ParserWrapper()
{
	XML_ParserFree  ( _parser );
	delete _xmlFile;
}
