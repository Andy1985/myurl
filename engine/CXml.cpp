#include "CXml.h"
#include "Common.h"
#include <ctime>
#include <cstring>

#include <iostream>
using std::cerr;
using std::endl;

CXml::CXml ()
{
    myDoc = new TiXmlDocument ();
}

CXml::~CXml ()
{
    delete myDoc;
}

bool CXml::ParseUrlXml (const char* xml,vector<string>& urls)
{

	urls.clear();

    const char* p = myDoc->Parse(xml);
    if( p == NULL ) { return false;}

    TiXmlElement *RootElement = myDoc->RootElement();
    if( NULL == RootElement ) {return false;}

    TiXmlElement *UrlCheck = RootElement->FirstChildElement();
    TiXmlElement *UrlParam = UrlCheck->NextSiblingElement(  );
	TiXmlElement *Rcpt = UrlParam->FirstChildElement();
    while( Rcpt )
    {
        TiXmlAttribute* NameVaule = Rcpt->FirstAttribute(  );
		if (NameVaule == NULL) {return false;}

		urls.push_back(NameVaule->Value());
        Rcpt = Rcpt->NextSiblingElement( );
    }

	return true;
}

bool CXml::BuildSocketResult (const int keyCheckCode,const string url,string& result)
{
    TiXmlDeclaration *decl = new TiXmlDeclaration ("1.0", "GB2312", "yes");

    if (NULL == decl) { return false;}

    if (false == myDoc->LinkEndChild (decl)) {return false;}

    TiXmlElement *root = new TiXmlElement ("Msg");
    if (NULL == root) {return false;}
    root->SetAttribute ("TaskType", "URLCHECK");

    if (false == myDoc->LinkEndChild (root)) {return false;}

	TiXmlElement *UrlCheck = new TiXmlElement("UrlCheck");
	if (UrlCheck == NULL) {return false;}
	UrlCheck->SetAttribute("TaskType","Rsp");
	UrlCheck->SetAttribute("TaskId","1");
	UrlCheck->SetAttribute("MachineId","1");
	UrlCheck->SetAttribute("Result",my_int2str(keyCheckCode).c_str());
	UrlCheck->SetAttribute("Url",url.c_str());

	if (false == root->LinkEndChild(UrlCheck)) {return false;}
    myDoc->ToString (result);

    return true;
}
