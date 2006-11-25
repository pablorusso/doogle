#include "CantOpenFileException.h"

CantOpenFileException::~CantOpenFileException() throw() { };

CantOpenFileException::CantOpenFileException()
{
	_msg = "No se puede abrir el archivo";
};

CantOpenFileException::CantOpenFileException( const string &fileName )
{
	_msg = "No se puede abrir el archivo: " + fileName;
}

const char* CantOpenFileException::what() const throw()
{
	return _msg.c_str();
}
