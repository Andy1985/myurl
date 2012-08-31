#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include "CDnsParse.h"

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

CDnsParse::CDnsParse(const string domain_) : domain(domain_)
{
	
}

CDnsParse::~CDnsParse()
{

}

bool CDnsParse::getIps(vector<string>& ips)
{
	ips.clear();

	char** pptr;
	struct hostent *hptr;
	char str[32];

	if ((hptr = gethostbyname(domain.c_str())) == NULL)
	{
		return false;
	}

	switch(hptr->h_addrtype)
	{
		case AF_INET:
		case AF_INET6:
			pptr = hptr->h_addr_list;	
			for (;*pptr != NULL;pptr++)
			{
				ips.push_back(inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str)));
			}
			break;
		default:
			break;
	}

	return true;
}
