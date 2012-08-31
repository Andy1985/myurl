#include "CUrlCheck.h"
#include "Common.h"

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <sstream>
using std::stringstream;

CUrlCheck::CUrlCheck() : myUrl(""), DNS_FAIL_LIMIT(5),URL_GET_LIMIT(5)
{

}

CUrlCheck::~CUrlCheck()
{
	myDnsFailMem.Close();
	myRedirectMem.Close();
	myDnsARecordMem.Close();
	myUrlInfoMem.Close();
	myBlackMem.Close();
}

void CUrlCheck::setDnsFailLimit(const int limit)
{
	DNS_FAIL_LIMIT = limit;
}

void CUrlCheck::setUrlGetLimit(const int limit)
{
	URL_GET_LIMIT = limit;
}

void CUrlCheck::setMem(const string& memStr,vector<mem_t_>& memVec)
{
	memVec.clear();

	CDataParse myDataParse(memStr);
	vector<string> vecBuffer = myDataParse.getVector();

	for (vector<string>::iterator i = vecBuffer.begin(); i != vecBuffer.end(); ++i)
	{
		string::size_type split = i->find(":");
		if (split != string::npos)
		{
			mem_t_ memBuffer;
			memBuffer.ip = i->substr(0,split);
			memBuffer.port = my_atoi((i->substr(split+1,i->length() - split -1 )).c_str());
			
			memVec.push_back(memBuffer);
		}	
	}
}

void CUrlCheck::setDnsFailMem(const string& memStr)
{
	setMem(memStr,dnsFailMem);
	for (vector<mem_t_>::iterator i = dnsFailMem.begin(); i != dnsFailMem.end(); ++i)
	{
		myDnsFailMem.AddrServer(i->ip,i->port);
		if (false == myDnsFailMem.Connect())
		{
			cerr << "myDnsFailMem.Connect() Failed!" << endl;
			return ;
		}
	}
}

void CUrlCheck::setRedirectMem(const string& memStr)
{
	setMem(memStr,redirectMem);
	for (vector<mem_t_>::iterator i = redirectMem.begin(); i != redirectMem.end(); ++i)
	{
		myRedirectMem.AddrServer(i->ip,i->port);
		if (false == myRedirectMem.Connect())
		{
			cerr << "myRedirectMem.Connect() Failed!" << endl;
			return ;
		}
	}
}

void CUrlCheck::setDnsARecordMem(const string& memStr)
{
	setMem(memStr,dnsARecordMem);
	for (vector<mem_t_>::iterator i = dnsARecordMem.begin(); i != dnsARecordMem.end(); ++i)
	{
		myDnsARecordMem.AddrServer(i->ip,i->port);
		if (false == myDnsARecordMem.Connect())
		{
			cerr << "myDnsARecordMem.Connect() Failed!" << endl;
			return ;
		}
	}
}

void CUrlCheck::setUrlInfoMem(const string& memStr)
{
	setMem(memStr,urlInfoMem);
	for (vector<mem_t_>::iterator i = urlInfoMem.begin(); i != urlInfoMem.end(); ++i)
	{
		myUrlInfoMem.AddrServer(i->ip,i->port);
		if (false == myUrlInfoMem.Connect())
		{
			cerr << "myUrlInfoMem.Connect() Failed!" << endl;
			return ;
		}
	}
}

void CUrlCheck::setBlackMem(const string& memStr)
{
	setMem(memStr,blackMem);
	for (vector<mem_t_>::iterator i = blackMem.begin(); i != blackMem.end(); ++i)
	{
		myBlackMem.AddrServer(i->ip,i->port);
		if (false == myBlackMem.Connect())
		{
			cerr << "myBlackMem.Connect() Failed!" << endl;
			return ;
		}
	}
}

int CUrlCheck::Check(const string url)
{
	myUrl.setUrl(url);

	if (myUrl.getDomain() == "")
	{
		cerr << "domain null!!!" << endl;
		return -1;
	}

	RedirectParse();
	DnsParse();

	if (DnsFailGet(myUrl.getDomain()) == -1)
		UrlContentParse();

	return BlackListCheck();
}

int CUrlCheck::DnsFailGet(const string& domain)
{
	return myDnsFailMem.GetAsNum(domain);
}

bool CUrlCheck::DnsFailSet(const string& domain)
{
	bool ret;
	if ("" == myDnsFailMem.Get(domain))
	{
		ret = myDnsFailMem.AddAsNum(domain,1);
	}
	else
	{
		ret = myDnsFailMem.Increment(domain,1);
	}

	return ret;
	
}

string CUrlCheck::RedirectGet(const string& url)
{
	return myRedirectMem.Get(url);
}

bool CUrlCheck::RedirectSet(const string& url,const string& urlRedirect)
{
	return myRedirectMem.Set(url,urlRedirect);
}

string CUrlCheck::DnsARecordGet(const string& domain)
{
	return myDnsARecordMem.Get(domain);
}

bool CUrlCheck::DnsARecordSet(const string& domain,const string& ips)
{
	return myDnsARecordMem.Set(domain,ips);
}

string CUrlCheck::UrlInfoGet(const string& domain)
{
	return myUrlInfoMem.Get(domain);
}

bool CUrlCheck::UrlInfoSet(const string& domain,const string& url,const int urlParseTag)
{
	
	bool ret;

	CDataParse myDataParse;
	string strBuffer = UrlInfoGet(domain);
	if (strBuffer == "")
	{
		vector<string> vecBuffer;
		vecBuffer.push_back("1");
		vecBuffer.push_back(my_int2str(urlParseTag));
		vecBuffer.push_back(url);
		myDataParse.setVector(vecBuffer);
		ret = myUrlInfoMem.Set(domain,myDataParse.getString());
	}
	else
	{
		myDataParse.setString(strBuffer);
		vector<string> vecBuffer = myDataParse.getVector();
		int domainCount = my_str2int(vecBuffer[0]);
		domainCount++;
		vecBuffer[0] = my_int2str(domainCount);
		vecBuffer[1] = my_int2str(urlParseTag);

		if (find(vecBuffer.begin(),vecBuffer.end(),url) == vecBuffer.end() && strBuffer.size() < 1024)
		{
			vecBuffer.push_back(url);
		}
		
		myDataParse.setVector(vecBuffer);

		ret = myUrlInfoMem.Set(domain,myDataParse.getString());
	}

	return ret;
}

string CUrlCheck::BlackGet(const string& data)
{
	return myBlackMem.Get(data);
}

bool CUrlCheck::BlackSet(const string& data,const int keyCheckCode)
{
	stringstream ss;
	ss << keyCheckCode;
	ss << "_";
	ss << GetCurrentTime();

	return myBlackMem.Set(data,ss.str());
}

void CUrlCheck::RedirectParse()
{
	if (DnsFailGet(myUrl.getDomain()) != -1)
	{
		return;
	}
	
	string urlRedirect = RedirectGet(myUrl.getUrl());
	if ("" != urlRedirect)
	{
		myUrl.setUrl(urlRedirect);
		return; 
	}

	CUrlParse myUrlParse(myUrl.getUrl());		
	if (false == myUrlParse.getRedirectUrl(urlRedirect))
	{
		if (false == myUrlParse.dns())
		{
			DnsFailSet(myUrl.getDomain());
		}
	}
	else
	{
		if ("" == urlRedirect)
		{
			RedirectSet(myUrl.getUrl(),myUrl.getUrl());
		}
		else
		{
			RedirectSet(myUrl.getUrl(),urlRedirect);
			myUrl.setUrl(urlRedirect);
		}
	}

}

void CUrlCheck::DnsParse()
{
	int dnsFailCount = DnsFailGet(myUrl.getDomain());
	myUrl.setDnsFailCount(dnsFailCount);		
	if (myUrl.getDnsFailCount() >= DNS_FAIL_LIMIT)
	{
		return;
	}

	CDataParse myDataParse;
	string ipsString = DnsARecordGet(myUrl.getDomain());
	if ("" != ipsString)
	{
		myDataParse.setString(ipsString);
		myUrl.setIps(myDataParse.getVector());
	}
	else
	{
		CDnsParse myDnsParse(myUrl.getDomain());
		vector<string> ipsBuffer;
		myDnsParse.getIps(ipsBuffer);

		if (ipsBuffer.size() == 0)
		{
			DnsFailSet(myUrl.getDomain());
			myUrl.setDnsFailCount(myUrl.getDnsFailCount() + 1);
		}
		else
		{
			myUrl.setIps(ipsBuffer);
			myDataParse.setVector(ipsBuffer);
			DnsARecordSet(myUrl.getDomain(),myDataParse.getString());
		}
	}
}

void CUrlCheck::UrlContentParse()
{
	string urlInfo = UrlInfoGet(myUrl.getDomain());
	if (urlInfo == "")
	{
		myUrl.setKeyCheckReturn(0);
		UrlInfoSet(myUrl.getDomain(),myUrl.getUrl(),0);
	}
	else
	{
		CDataParse myDataParse;
		myDataParse.setString(urlInfo);
		vector<string> vecBuffer = myDataParse.getVector();
		
		if (my_str2int(vecBuffer[0]) >= URL_GET_LIMIT && my_str2int(vecBuffer[1]) == 0)
		{
			vecBuffer.push_back(myUrl.getUrl());
			for (unsigned int i = 1; i < vecBuffer.size(); ++i)
			{
				string& s = vecBuffer[i];
				if (is_tail(s,".pdf")  || 
					is_tail(s,".doc")  || 
					is_tail(s,".docx") ||
					is_tail(s,".ppt")  ||
					is_tail(s,".pptx") ||
					is_tail(s,".xls")  ||
					is_tail(s,".xlsx") ||
					is_tail(s,".bmp")  ||
					is_tail(s,".img")  ||
					is_tail(s,".jpg")  ||
					is_tail(s,".gif")  ||
					is_tail(s,".tgz")  ||
					is_tail(s,".rar")  ||
					is_tail(s,".gz") ||
					is_tail(s,".png"))
				{
					cout << s << " bad tail,not check" << endl;
					continue;
				}

				CUrlParse myUrlParse(s);
				string urlContent; 

				if (true == myUrlParse.getUrlContent(urlContent))
				{
					CKeyCheck myKeyCheck(urlContent);
					int keyCheckCode = myKeyCheck.Check();
					if (keyCheckCode > 0)
					{
						myUrl.setKeyCheckReturn(keyCheckCode);
						break;
					}
				}
				else
				{
					myUrl.setKeyCheckReturn(-1);
				}
			}

			UrlInfoSet(myUrl.getDomain(),myUrl.getUrl(),1);
		}
		else
		{
			myUrl.setKeyCheckReturn(0);
			UrlInfoSet(myUrl.getDomain(),myUrl.getUrl(),my_str2int(vecBuffer[1]));
		}
	}


}

int CUrlCheck::BlackListCheck()
{
/*
	if ("" != BlackGet(myUrl.getDomain()))
	{
		return -3;
	}
*/

	//cout << myUrl.getDomain() << endl;
	for (vector<string>::const_iterator i = myUrl.getIps().begin(); i != myUrl.getIps().end(); ++i)
	{
		if (BlackGet(*i) != "")
		{
			return -2;
		}
	}

/*
	if (myUrl.getDnsFailCount() >= DNS_FAIL_LIMIT)
	{
		BlackSet(myUrl.getDomain());
		return -3;
	}
*/

	if (myUrl.getKeyCheckReturn() > 0)
	{
		for (vector<string>::const_iterator i = myUrl.getIps().begin(); i != myUrl.getIps().end(); ++i)
		{
			BlackSet(*i,myUrl.getKeyCheckReturn());
		}
		return myUrl.getKeyCheckReturn();
	}

	return 0;
}
