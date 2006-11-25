#include "InvalidXmlException.h"
#include <sstream>
#include <string>

InvalidXmlException::~InvalidXmlException() throw() { };

InvalidXmlException::InvalidXmlException()
{
	_msg = "El xhtml no esta bien formado. Error al parsear.";
};

InvalidXmlException::InvalidXmlException ( const string &parserError, const int &lineNumber )
{
	ostringstream os;
	os << lineNumber;
	string s_lineNumber = os.str();

	_msg = parserError + " en la linea " + s_lineNumber;
}

InvalidXmlException::InvalidXmlException ( const string &msg )
{
	_msg = msg;
}

const char* InvalidXmlException::what() const throw()
{
	return _msg.c_str();
}
