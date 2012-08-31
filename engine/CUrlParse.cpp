#include <iostream>
using std::cerr;
using std::endl;
using std::cout;

#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>

#include "CUrlParse.h"
#include "Common.h"

#include "CUrlData.h"

CUrlParse::CUrlParse(const string url_) : url(url_)
{
    curl_global_init(CURL_GLOBAL_ALL);
	memset(errorBuffer,'\0',sizeof(errorBuffer));
	dns_stat = true;
}

CUrlParse::~CUrlParse()
{
    curl_global_cleanup();
}

bool CUrlParse::dns()
{
	return dns_stat;
}

bool CUrlParse::getRedirectUrl(string& urlRedirect)
{
    CURLcode code;
    CURL* conn;

    conn = curl_easy_init(  );
    if( conn == NULL )
    {
        return false;
    }

	curl_easy_setopt( conn, CURLOPT_ERRORBUFFER, errorBuffer );
	curl_easy_setopt(conn,CURLOPT_URL,url.c_str());
	curl_easy_setopt(conn,CURLOPT_WRITEFUNCTION,writer);
	curl_easy_setopt( conn, CURLOPT_WRITEDATA, &urlRedirect);
	curl_easy_setopt( conn, CURLOPT_REDIR_PROTOCOLS, 1 );
	curl_easy_setopt( conn, CURLOPT_HEADER, 1 );
	curl_easy_setopt( conn, CURLOPT_NOBODY, 1 );
    curl_easy_setopt( conn, CURLOPT_TIMEOUT, timeout );
    curl_easy_setopt( conn, CURLOPT_CONNECTTIMEOUT, timeout );
    curl_easy_setopt( conn, CURLOPT_FORBID_REUSE, 1 );
    curl_easy_setopt( conn, CURLOPT_BUFFERSIZE, 1024 * 1024 );

    code = curl_easy_perform( conn );
    if( code != CURLE_OK )
    {
		if (code == CURLE_COULDNT_RESOLVE_HOST)
		{
			dns_stat = false;
			curl_easy_cleanup(conn);

			return false;
		}
    }
	else
	{
    	long http_code = 0;
    	curl_easy_getinfo(conn,CURLINFO_RESPONSE_CODE,&http_code);
		if (http_code > 300 && http_code < 400)
		{
			string::size_type start = urlRedirect.find("Location: ");
			if (start != string::npos)
			{
				start += strlen("Location: ");
				if (start != string::npos)
				{
					string::size_type end = urlRedirect.find("\r\n",start);
					urlRedirect = urlRedirect.substr(start,end-start);
					if (urlRedirect.find("http://") == string::npos && urlRedirect.find("https://") == string::npos)
					{
						CUrlData myUrl(url);
						string buffer = myUrl.getHead();
						buffer += myUrl.getDomain();
						buffer += urlRedirect;
						urlRedirect = buffer;
					}
				}
			}
			else
			{
				urlRedirect = url;
			}
		}
		else
		{
			urlRedirect = url;
		}
	}

    curl_easy_cleanup(conn);

    return true;
}

bool CUrlParse::getUrlContent(string& urlContent)
{
    CURLcode code;
    CURL* conn;

    conn = curl_easy_init(  );
    if( conn == NULL )
    {
        return false;
    }

    curl_easy_setopt( conn, CURLOPT_ERRORBUFFER, errorBuffer );
    curl_easy_setopt(conn,CURLOPT_URL,url.c_str());
    curl_easy_setopt( conn, CURLOPT_FOLLOWLOCATION, 1 );
    curl_easy_setopt( conn, CURLOPT_MAXREDIRS, 3 );
    curl_easy_setopt(conn,CURLOPT_WRITEFUNCTION,writer);
    curl_easy_setopt( conn, CURLOPT_WRITEDATA, &urlContent);
    curl_easy_setopt( conn, CURLOPT_REDIR_PROTOCOLS, 1 );
    //curl_easy_setopt( conn, CURLOPT_HEADER, 1 );
    curl_easy_setopt( conn, CURLOPT_TIMEOUT, timeout );
    curl_easy_setopt( conn, CURLOPT_CONNECTTIMEOUT, timeout );
    curl_easy_setopt( conn, CURLOPT_FORBID_REUSE, 1 );
    curl_easy_setopt( conn, CURLOPT_BUFFERSIZE, 1024 * 1024 );

    code = curl_easy_perform( conn );

    if( code != CURLE_OK )
    {
        if( strstr( errorBuffer, "Empty reply" ) )
        {
            char filename[512];
            char cmd[1024];

            static int i = 0;
            time_t timep;
            struct tm *p;
            time( &timep );
            p = localtime( &timep );

            sprintf( filename,
                     "/tmp/.checklinkkey_tmp.%d%02d%02d%02d%02d%02d.%d.%d.txt",
                     ( 1900 + p->tm_year ), ( 1 + p->tm_mon ), p->tm_mday,
                     p->tm_hour, p->tm_min, p->tm_sec, getpid(  ), i++ );

            sprintf( cmd,
                     "curl -r 0-10000 --location --connect-timeout 5 %s > %s 2>/dev/null",
                     url.c_str(), filename );

            printf( "DEBUG:Empty reply, system( %s )", cmd );

            system( cmd );

            read_tmp_file( filename, &urlContent );
            unlink( filename );
        }
    }

    curl_easy_cleanup(conn);

    return true;

}
