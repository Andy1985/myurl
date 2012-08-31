#ifndef CDATAPARSE_H
#define CDATAPARSE_H

#include <string>
using std::string;

#include <vector>
using std::vector;

class CDataParse
{
	public:
		CDataParse();
		CDataParse(const string& ss);
		CDataParse(const vector<string>& vs);
		~CDataParse();

		void setString(const string& ss);
		const vector<string>& getVector();

		void setVector(const vector<string>& vs);
		const string& getString();


	private:
		string ss;
		vector<string> vs;
	
};

#endif /*CDATAPARSE_H*/
