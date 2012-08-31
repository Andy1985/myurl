#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;
using std::cerr;

#include <string>
using std::string;

#include <cstring>
#include "CUrlCheck.h"
#include "CXml.h"

#include "Common.h"

int main(int argc,char* argv[])
{
	if (argc != 2)
	{
		cerr << "Usage: " << argv[0] << " <url>" << endl;
		exit(1);
	}

	CUrlCheck myUrlCheck;

	myUrlCheck.setDnsFailLimit(5);
	myUrlCheck.setUrlGetLimit(5);

	myUrlCheck.setDnsFailMem("|192.168.188.16:13000");
	myUrlCheck.setRedirectMem("|192.168.188.16:13001");
	myUrlCheck.setDnsARecordMem("|192.168.188.16:13002");
	myUrlCheck.setUrlInfoMem("|192.168.188.16:13003");
	myUrlCheck.setBlackMem("|192.168.188.16:13004");

	int checkCode = myUrlCheck.Check(argv[1]);

	CXml myXml;
	string resultBuffer;
	myXml.BuildSocketResult(checkCode,string(argv[1]),resultBuffer);

	cerr << resultBuffer << endl;


	exit(0);
}
