#include "ParserWrapper.h"

#include <string>
#include <fstream>
#include <iostream>

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
	Lexical* lex = ( Lexical* ) userData;

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
					lex->AddWord( word );
				word = "";
			}
		}
	}

	if ( word != "" )
	{
		if ( lex != NULL )
			lex->AddWord( word );

		word = "";
	}
}

void ParserWrapper::endElement( void *userData, const char *name )
{
	//printf("\n%s\n", name);
}

ParserWrapper::ParserWrapper()
{
	_parser = XML_ParserCreate( NULL );
	XML_SetElementHandler( _parser, startElement, endElement );
	XML_SetCharacterDataHandler( _parser, charHandler );
}

void ParserWrapper::Parse( string fileName ) throw(InvalidXmlException, CantOpenFileException)
{
	std::ifstream xmlFile( fileName.c_str() );
	if ( ! xmlFile.is_open() )
		throw CantOpenFileException( fileName );

	std::string buffer;
	std::getline( xmlFile, buffer );
	bool done = xmlFile.fail();

	XML_ParserReset( _parser, NULL );
	while ( !done )
	{
		if ( ! XML_Parse( _parser, buffer.c_str(), buffer.length(), done ) )
			throw InvalidXmlException( XML_ErrorString( XML_GetErrorCode( _parser ) ), XML_GetCurrentLineNumber( _parser ) );

		std::getline( xmlFile, buffer );
		done = xmlFile.fail();
	}
}

void ParserWrapper::SetLexical( Lexical* lexico )
{
	_lexico = lexico;
	XML_SetUserData( _parser, _lexico );
}

Lexical* ParserWrapper::GetLexical()
{
	return _lexico;
}

ParserWrapper::~ParserWrapper()
{
	XML_ParserReset ( _parser, NULL );
	XML_ParserFree  ( _parser );
}
