#ifndef CGICLASS_H
#define	CGICLASS_H

#include <string>
#include <map>

using namespace std;
typedef map<string, string> MAPA_NOMBRE_VALOR;

class CGIClass
{
	public:
		CGIClass();
		~CGIClass();

		void Load( const string &data );
		string GetValue(const string &name);
	protected:
		MAPA_NOMBRE_VALOR _mapNameValue;
	private:
		static int h2c(char hi, char lo);
		static string url_decode(const string& s);
		void buildMap( string buff );
};

#endif
