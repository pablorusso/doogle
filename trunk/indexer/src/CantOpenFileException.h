#ifndef CANTOPENFILEEXCEPTION_H_
#define CANTOPENFILEEXCEPTION_H_

#include <string>
#include <exception>

using namespace std;

class CantOpenFileException : public std::exception
{
	private:
		string _msg;
	public:
		CantOpenFileException ();
		CantOpenFileException ( const string &fileName );
		~CantOpenFileException() throw();

		virtual const char* what() const throw();
};

#endif
