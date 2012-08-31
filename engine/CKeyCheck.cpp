#include "CKeyCheck.h"
#include "Common.h"

extern int my_keycheck(const char* data);

CKeyCheck::CKeyCheck(const string& toCheck_) : toCheck(toCheck_)
{

}

CKeyCheck::~CKeyCheck()
{
	
}

int CKeyCheck::Check()
{
	string_tolower(toCheck);

	return my_keycheck(toCheck.c_str());
}
