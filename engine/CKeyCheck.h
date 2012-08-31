#ifndef CKEYCHECK_H
#define CKEYCHECK_H

#include <string>
using std::string;

class CKeyCheck
{
	public:
		CKeyCheck(const string& toCheck);
		~CKeyCheck();
	
		int Check(); /*-1: failed; 0: normal >0: return code*/

	private:
		string toCheck;
};
#endif /*CKEYCHECK_H*/
