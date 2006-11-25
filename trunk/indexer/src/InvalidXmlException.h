#ifndef INVALIDXMLEXCEPTION_H_
#define INVALIDXMLEXCEPTION_H_

#include <string>
#include <exception>

using namespace std;

class InvalidXmlException : public std::exception
{
	private:
		string _msg;
	public:
		InvalidXmlException ();
		InvalidXmlException ( const string &parserError, const int &lineNumber );
		InvalidXmlException ( const string &msg );
		~InvalidXmlException() throw();

		virtual const char* what() const throw();
};

#endif
