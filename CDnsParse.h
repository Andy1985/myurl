#ifndef CDNSPARSE_H
#define CDNSPARSE_H

#include <string>
using std::string;

#include <vector>
using std::vector;

class CDnsParse
{
	public:
		CDnsParse(const string domain);
		~CDnsParse();
	
		bool getIps(vector<string>& ips);

	private:
		string domain;		

};

#endif /*CDNSPARSE_H*/
