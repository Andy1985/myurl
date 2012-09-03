#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;

#include <vector>
using std::vector;

#include <cstdlib>

#include "CXml.h"
#include "CSocket.h"

#include <sstream>
using std::stringstream;

#define FRAME_SIZE 200
#define FRAME_FILL 'A'
#define FRAME_HEAD "Cmd:"
#define RETURN_MIN_SIZE 50

int main(int argc,char* argv[])
{
	const string ip = "192.168.188.16";
	const unsigned int port = 5003;
	const unsigned int timeout = 30;

	vector<string> urls;
	if (NULL != argv[1]) urls.push_back(argv[1]);

	CXml* myXML = new CXml();
	string urlsToSend;
	if (false == myXML->BuildUrlData(urls,urlsToSend))
	{
		cerr << "BuildUrlData error!" << endl;
		delete myXML;
		exit(1);
	}

	stringstream ss ;
    ss << FRAME_HEAD;
    ss << urlsToSend.size (); 

    int count = FRAME_SIZE - ss.str ().length (); 

    for (int i = 0; i < count; i++)
    {   
        ss << FRAME_FILL;
    }   

    ss << urlsToSend;

	
	CSocket* mySock = new CSocket(ip,port,timeout);
	if (false == mySock->Connect())
	{
		cerr << "Connect error!" << endl;
		delete mySock;
		delete myXML;
		exit(1);
	}

	if (false == mySock->Send(ss.str()))
	{
		cerr << "SendUrlData error!" << endl;
		delete mySock;
		delete myXML;
		exit(1);
	}

	string resultData;
	if (false ==  mySock->Receive(resultData))
	{
		cerr << "ReceiveUrlData error!" << endl;
		delete mySock;
		delete myXML;
		exit(1);
	}
	
	string resBuffer;
	CXml *myXML2 = new CXml();
	myXML2->ParseSocketResult(resultData.c_str(),resBuffer);	

	cout << resBuffer << endl;

	mySock->Close();

	delete myXML2;
	delete mySock;
	delete myXML;
	
	exit(0);
}
