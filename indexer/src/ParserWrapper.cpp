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
		int asciiCode = ( int ) s[i];

		bool isNumber     = asciiCode >= 48 && asciiCode <= 57;
		bool validCharCap = asciiCode >= 65 && asciiCode <= 90;
		bool validChar    = asciiCode >= 97 && asciiCode <= 122;
		bool accent       = ( asciiCode >= 160 && asciiCode <= 165 ) || asciiCode == 233;

		if ( isNumber || validCharCap || validChar || accent )
		{
			if ( asciiCode >= 65 && asciiCode <= 90 )
				asciiCode += 32;
			char letra = ( char ) asciiCode;

			word += letra;
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
	_fileName = fileName;
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
			int line = XML_GetCurrentLineNumber( _parser );
			ostringstream os; os << line;
			throw string( "Error parseando el archivo: " + _fileName + ". Mensaje: " + msg + " en la linea " +  os.str() );
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
