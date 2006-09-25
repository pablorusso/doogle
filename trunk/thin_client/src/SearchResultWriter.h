#ifndef SearchResultWriter_H_
#define SearchResultWriter_H_

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class SearchResultWriter
{
	private:
		ifstream _resTemplate;
		string _version;
		string _word;
		string _errorMsg;
		vector<string> _docs;

		string showMessage( string msg );
		string showItem( string docPath, string docTitle, string docDesc, string docSize );
		string processLine( string str, string word, string qty, string version );
	public:
		SearchResultWriter( string &word, string &errorMsg, vector<string> &docs );
		virtual ~SearchResultWriter();
		string getResult();
};

#endif /*SearchResultWriter_H_*/
