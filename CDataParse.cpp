#include <sstream>
using std::stringstream;

#include "CDataParse.h"

CDataParse::CDataParse()
{
	ss.clear();
	vs.clear();
}

CDataParse::CDataParse(const string& ss_)
{
	ss.clear();
	ss = string(ss_);
}

CDataParse::CDataParse(const vector<string>& vs_) : vs(vs_)
{
}


CDataParse::~CDataParse()
{

}

void CDataParse::setString(const string& ss_)
{
	ss.clear();
	ss = string(ss_);
}

const vector<string>& CDataParse::getVector()
{
	string::size_type begin = ss.find("|");
	
	while(begin != string::npos)
	{
		begin += 1;
		if (begin != string::npos)
		{
			string::size_type end = ss.find("|",begin);
			vs.push_back(ss.substr(begin,end-begin));
			begin = end;
		}
	}

	return vs;
}

void CDataParse::setVector(const vector<string>& vs_)
{
	vs.clear();
	for (vector<string>::const_iterator i = vs_.begin(); i != vs_.end(); ++i)
	{
		vs.push_back(*i);
	}
}

const string& CDataParse::getString()
{
	ss.clear();
	for (vector<string>::iterator i = vs.begin(); i != vs.end(); ++i)
	{
		ss += "|";
		ss += *i;
	}

	return ss;
}
