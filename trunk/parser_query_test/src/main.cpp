#include <iostream>
#include <vector>
#include "QueryParser.h"

using namespace std;

void doTest( string test )
{
	vector<Query*> output;
	vector<Query*>::iterator output_iter;

	cout << "------------------------------------------------------" << endl;
	cout << "phrase: " << test << endl << endl;
	QueryParser::Parse( test, output );
	for (output_iter=output.begin(); output_iter != output.end(); output_iter++)
	{
  		cout << "word: " << (*output_iter)->getWord() << endl;
		cout << "included: " << (*output_iter)->mustBeIncluded() << endl;
		cout << "excluded: " << (*output_iter)->mustBeExcluded() << endl;
		cout << "weight: " << (*output_iter)->getWeight() << endl;

		delete *output_iter;
	}
	cout << endl << "------------------------------------------------------" << endl;
}

int main()
{
	// validos
	doTest( "t1 t2 t3" );
	doTest( "+t1 t2 t3" );
	doTest( "-t1 t2 t3" );
	doTest( "t1^2 t2 t3" );
	doTest( "-t1 +t2 t3^4" );

	// invalidos
	doTest( "&t1" );
	doTest( "" );
	doTest( "++t1" );
	doTest( "t1^n" );
	doTest( "t1^^n" );
	doTest( "t1^^n" );
}
