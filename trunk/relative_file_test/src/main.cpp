#include "Lexical.h"

int main()
{
	Lexical *lex = new Lexical();
	lex->AddWord( "Prueba1" );
	lex->AddWord( "Prueba2" );
	delete lex;

	return 0;
}
