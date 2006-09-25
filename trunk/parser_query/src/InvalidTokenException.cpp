#include "InvalidTokenException.h"

InvalidTokenException::~InvalidTokenException() throw() { };

InvalidTokenException::InvalidTokenException()
{
	_msg = "El formato de la consulta es invalido.";
};

InvalidTokenException::InvalidTokenException ( const string &msg )
{
	_msg = msg;
}

const char* InvalidTokenException::what() const throw()
{
	return _msg.c_str();
}
