#ifndef INVALIDTOKENEXCEPTION_H_
#define INVALIDTOKENEXCEPTION_H_

#include <string>
#include <exception>

using namespace std;

class InvalidTokenException : public std::exception
{
	private:
		string _msg;
	public:
		InvalidTokenException ();
		InvalidTokenException ( const string &msg );
		~InvalidTokenException() throw();

		virtual const char* what() const throw();
};

#endif /*INVALIDTOKENEXCEPTION_H_*/
