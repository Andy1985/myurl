#include "CUrlData.h"

CUrlData::CUrlData(string url_) : url(url_),dnsFailCount(0),keyCheckReturn(0)
{
	setDomain();
	ips.clear();
}

CUrlData::CUrlData()
{

}

CUrlData::~CUrlData() {}

string CUrlData::getDomain() const
{
	return domain;
}

string CUrlData::getHead() const
{
	return head;
}

string CUrlData::getUrl() const
{
	return url;
}

void CUrlData::setUrl(const string url)
{
	this->url = url;
	setDomain();
	ips.clear();
}

void CUrlData::setDnsFailCount(const int dnsFailCount)
{
	this->dnsFailCount = dnsFailCount;
}

int CUrlData::getDnsFailCount() const
{
	return dnsFailCount;

}

void CUrlData::addIps(const string ip)
{
	ips.push_back(ip);
}

void CUrlData::setIps(const vector<string> ips_)
{
	for (vector<string>::const_iterator i = ips_.begin(); i != ips_.end(); ++i)
	{
		ips.push_back(*i);
	}
}

const vector<string>& CUrlData::getIps() const
{
	return ips;
}

int CUrlData::getKeyCheckReturn() const
{
	return keyCheckReturn;
}

void CUrlData::setKeyCheckReturn(const int keyCheckReturn)
{
	this->keyCheckReturn = keyCheckReturn;
}

void CUrlData::setDomain()
{
	string::size_type begin = url.find("http://");
	if (begin != string::npos)
	{
		head = "http://";
		begin += strlen("http://");
		if (begin != string::npos)
		{
			string::size_type end = url.find("/",begin);
			domain = url.substr(begin,end-begin);

			begin = domain.find(":");
			if (begin != string::npos)
			{
				domain = domain.substr(0,begin);
			}
		}
	}

	begin = url.find("https://");
	
	if (begin != string::npos)
	{
		head = "https://";
		begin += strlen("https://");
		if (begin != string::npos)
		{
			string::size_type end = url.find("/",begin);
			domain = url.substr(begin,end-begin);

			begin = domain.find(":");
			if (begin != string::npos)
			{
				domain = domain.substr(0,begin);
			}
		}
	}
}
