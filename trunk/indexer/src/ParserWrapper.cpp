#include "ParserWrapper.h"

#include <string>
#include <fstream>
#include <iostream>

#include "ArchivoLexico.h"
#include "LexicoData.h"

void ParserWrapper::startElement( void *userData, const char *name, const char **atts )
{
  	/*
	printf("\n %s", name);
  	while (*atts)
	{
    	printf(" %s='%s'", atts[0], atts[1]);
    	atts += 2;
  	}
	*/
}

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

void ParserWrapper::endElement( void *userData, const char *name )
{
	//printf("\n%s\n", name);
}

ParserWrapper::ParserWrapper( string fileName, ArchivoLexico &lexico )
{
	_parser = XML_ParserCreate( NULL );
	XML_SetElementHandler( _parser, startElement, endElement );
	XML_SetCharacterDataHandler( _parser, charHandler );
	XML_SetUserData( _parser, &lexico );

	_xmlFile = new ifstream( fileName.c_str() );
	if ( ! _xmlFile->is_open() )
		throw CantOpenFileException( fileName );
}

void ParserWrapper::Parse() throw(InvalidXmlException, CantOpenFileException)
{
	std::string buffer;
	std::getline( *_xmlFile, buffer );
	bool done = _xmlFile->fail();

	while ( !done )
	{
		if ( ! XML_Parse( _parser, buffer.c_str(), buffer.length(), done ) )
			throw InvalidXmlException( XML_ErrorString( XML_GetErrorCode( _parser ) ), XML_GetCurrentLineNumber( _parser ) );

		std::getline( *_xmlFile, buffer );
		done = _xmlFile->fail();
	}
}

ParserWrapper::~ParserWrapper()
{
	XML_ParserFree  ( _parser );
	delete _xmlFile;
}
