#ifndef CURLPARSE_H
#define CURLPARSE_H

#include <string>
using std::string;

#include <curl/curl.h>
#include <curl/types.h>

const unsigned int timeout = 10;

class CUrlParse
{
	public:
		CUrlParse(const string url);
		~CUrlParse();

		bool dns();
		bool getRedirectUrl(string& urlRedirect);

		bool getUrlContent(string& urlContent);

	private:
		string url;
		bool dns_stat;
		char errorBuffer[CURL_ERROR_SIZE];
};
#endif /*CURLPARSE_H*/
