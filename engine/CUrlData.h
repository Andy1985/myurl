#ifndef CURLDATA_H
#define CURLDATA_H

#include <string>
using std::string;

#include <vector>
using std::vector;

class CUrlData
{
	public:
		CUrlData();
		CUrlData(const string url_); 
		~CUrlData();

		string getDomain() const;
		string getHead() const;

		string getUrl() const;
		void setUrl(const string url);
		void setDnsFailCount(const int dnsFailCount);
	 	int getDnsFailCount() const;
		void addIps(const string ip);
		void setIps(const vector<string> ips_);
		const vector<string>& getIps() const;
		int getKeyCheckReturn() const;
		void setKeyCheckReturn(const int keyCheckReturn);

	private:
		string url;
		string domain;
		string head;
		int dnsFailCount;
		vector<string> ips;
		void setDomain();
		int keyCheckReturn;

};
#endif /*CURLDATA_H*/
