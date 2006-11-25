#ifndef __RELATIVE_FILE__
#define __RELATIVE_FILE__

#include <string>

/*class Campo
{
	private:
		string 	_name;
		string 	_type;
		int  	_len;
}

class CampoVariable : public class Campo
{
	private:
		int _cantCampos;
		Campo []_campos;
		string _name;
		string 	_type = "variable";
		int len; // = for( int i=0; i < _cantCampos; ++i ) len += _campos[i].len;
}

class Registro
{
	private:
		Campo []_campos;
}*/

class RelativeFile
{
	public:
		RelativeFile( std::string fileName, int regQty, int regLen );
		bool Read();
		void Write();
		void Seek();
		bool ReadNext();
		~RelativeFile();
};
#endif
