#ifndef CURLCHECK_H
#define CURLCHECK_H

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <cstring>
#include "CUrlParse.h"
#include "CDnsParse.h"
#include "CMemcache.h"
#include "CDataParse.h"
#include "CKeyCheck.h"
#include "CUrlData.h"
#include "Common.h"

struct mem_t_
{
	string ip;
	unsigned int port;

	mem_t_() : ip(""), port(0) {}
};

class CUrlCheck
{
	public:
		CUrlCheck();
		~CUrlCheck();

		void setDnsFailLimit(const int limit);
		void setUrlGetLimit(const int limit);

		void setDnsFailMem(const string& memStr);
		void setRedirectMem(const string& memStr);
		void setDnsARecordMem(const string& memStr);
		void setUrlInfoMem(const string& memStr);
		void setBlackMem(const string& memStr);

		int Check(const string url);

	private:
		CUrlData myUrl;

		int DNS_FAIL_LIMIT;
		int URL_GET_LIMIT;
		
		vector<mem_t_> dnsFailMem;
		vector<mem_t_> redirectMem;
		vector<mem_t_> dnsARecordMem;
		vector<mem_t_> urlInfoMem;
		vector<mem_t_> blackMem;

		CMemcache myDnsFailMem;
		CMemcache myRedirectMem;
		CMemcache myDnsARecordMem;
		CMemcache myUrlInfoMem;
		CMemcache myBlackMem;

		string domainWhiteList;

		void setMem(const string& memStr,vector<mem_t_>& memVec);

		int DnsFailGet(const string& domain);
		bool DnsFailSet(const string& domain); //13000

		string RedirectGet(const string& url);
		bool RedirectSet(const string& url,const string& urlRedirect); //13001

		string DnsARecordGet(const string& domain);
		bool DnsARecordSet(const string& domain,const string& ips); //13002

		string UrlInfoGet(const string& domain);
		bool UrlInfoSet(const string& domain,const string& url,const int urlParseTag); //13003

		string BlackGet(const string& data);
		bool BlackSet(const string& data,const int keyCheckCode); // 13004

		void RedirectParse(); 	// (1)
		void DnsParse(); 		// (2)
		void UrlContentParse(); // (3)
		int BlackListCheck(); 	// (4)
};

#endif /*CURLCHECK_H*/
