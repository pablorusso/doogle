#include "ParserWrapper.h"

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>

#include "LexicoData.h"

void ParserWrapper::charHandler( void *userData, const XML_Char *s, int len )
{
	WordPair *lex = (WordPair *) userData;

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
				(*lex)[ word ] += 1;
				word = "";
			}
		}
	}

	if ( word != "" )
		(*lex)[ word ] += 1;
}

ParserWrapper::ParserWrapper( string fileName, WordPair *lexico )
{
	_lexico = lexico;
	_parser = XML_ParserCreate( NULL );
	XML_SetCharacterDataHandler( _parser, charHandler );
	XML_SetUserData( _parser, lexico );

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
		{
			string msg = XML_ErrorString( XML_GetErrorCode( _parser ) );
			long line = XML_GetCurrentLineNumber( _parser );
			ostringstream os; os << line;
			throw string( "Error parseando el archivo. Mensaje: " + msg + " en la linea " +  os.str() );
		}

		std::getline( *_xmlFile, buffer );
		done = _xmlFile->fail();
	}
}

ParserWrapper::~ParserWrapper()
{
	XML_ParserFree  ( _parser );
	delete _xmlFile;
}
