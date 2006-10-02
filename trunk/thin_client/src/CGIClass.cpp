#include "CGIClass.h"
#include <sstream>

CGIClass::CGIClass()
{
}

CGIClass::~CGIClass()
{
}

void CGIClass::Load( const string &data )
{
	buildMap( data );
}

string CGIClass::GetValue( const string &name )
{
	return url_decode( _mapNameValue[ name ] );
}

int CGIClass::h2c(char hi, char lo)
{
	std::string s;
    int value = -1;
    s += hi;
    s += lo;
    std::istringstream is(s);
    is >> std::hex >> value;
    return value;
}

string CGIClass::url_decode(const string& s)
{
	string result;
	std::istringstream iss(s);
	char c;

	for (iss >> c;!iss.eof();iss >> c)
	{
    	if ('+' == c)
		{
        	result += ' ';
		}
		else
		{
			if ('%' == c)
			{
        		char c1 = 0, c2 = 0;
            	iss >> c1;
            	iss >> c2;
            	if (!iss.eof())
				{
            		int r = h2c(c1,c2);
                	if (r != -1)
	                	result += static_cast<char>(r);
				}
			}
			else
			{
            	result += c;
			}
		}
	}

	return result;
}

void CGIClass::buildMap( string buff )
{
	string lastName;
	string lastValue;

	// cout << "<br>" << "MAP: ENTRE CON BUFFER[" << buff << "]" << "<br>";

	string::size_type posInicial = 0;
	string::size_type posIgual = buff.find ( "=", posInicial );
	string::size_type posAmp   = buff.find ( "&", posInicial );
	// cout << "MAP: ENCONTRE INICIAL EN " << posInicial << "<br>";
	// cout << "MAP: ENCONTRE IGUAL   EN " << posIgual << "<br>";
	// cout << "MAP: ENCONTRE AMP     EN " << posAmp << "<br>";

	while ( posIgual != string::npos )
	{
		// Si encontre ampersan freno ahi, sino voy hasta el final del string
		string::size_type final = string::npos;
		if ( posAmp != string::npos )
			final = posAmp;
		else
			final = buff.length();

		// obtengo los valores
		lastName  = buff.substr( posInicial, posIgual - posInicial );
		lastValue = buff.substr( posIgual+1, final-posIgual-1 );
		_mapNameValue[lastName] = lastValue;
		//cout << "MAP: ENCONTRE NAME:" << lastName << "<br>";
		//cout << "MAP: COORDENADAS NAME:" << buff << "(" << posInicial << ", " << posIgual - posInicial << ") <br>";
		//cout << "MAP: ENCONTRE VALUE: " << lastValue << "<br>";
		//cout << "MAP: COORDENADAS NAME:" << buff << "(" << posIgual+1 << ", " << final - posIgual - 1 << ") <br>";
		//cout << "MAP: DATO:" << lastName << " - " << lastValue << "<br>";


		// busco el que sigue
		posInicial = final+1;
		posIgual = buff.find ( "=", posInicial+1 );
		posAmp   = buff.find ( "&", posInicial+1 );
		//cout << "MAP: ENCONTRE INICIAL EN " << posInicial << "<br>";
		//cout << "MAP: ENCONTRE IGUAL   EN " << posIgual << "<br>";
		//cout << "MAP: ENCONTRE AMP     EN " << posAmp << "<br>";
	}
	// cout << "MAP: SALI" << "<br>";
}
