#include "Common.h"
#include "CXml.h"
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

bool CXml::BuildUrlData (const vector<string>& urls,string& str)
{
    char current_time[32] = { 0 };
    time_t now = time (NULL);
    struct tm *ptr = localtime (&now);

    strftime (current_time, 32, "%Y-%m-%d %H:%M:%S", ptr);

    TiXmlDeclaration *decl = new TiXmlDeclaration ("1.0", "GB2312", "yes");

    if (NULL == decl)
        return false;
    if (false == myDoc->LinkEndChild (decl))
        return false;

    TiXmlElement *root = new TiXmlElement ("Msg");

    if (NULL == root)
        return false;
    root->SetAttribute ("TaskType", "URLCHECK");
    if (false == myDoc->LinkEndChild (root))
        return false;
    TiXmlElement *head = new TiXmlElement ("UrlCheck");

    if (NULL == head) return false;
    head->SetAttribute ("TaskType", "Req");
    head->SetAttribute ("TaskId", 1);
    head->SetAttribute ("MachineId", 1);
    head->SetAttribute ("TaskTime", current_time);
    if (false == root->LinkEndChild (head)) return false;

    TiXmlElement *param = new TiXmlElement ("UrlParam");

    if (NULL == param)  return false;
    if (false == root->LinkEndChild (param)) return false;

    for (vector<string>::const_iterator it = urls.begin();it != urls.end(); it++)
    {
        TiXmlElement *recever = new TiXmlElement ("UrlItem");
        if (NULL == recever) return false;
        recever->SetAttribute ("NameValue", it->c_str());
        if (false == param->LinkEndChild (recever)) return false;
    }

    myDoc->ToString (str);

    return true;
}

bool CXml::ParseSocketResult (const char *result,string& str)
{
	str.clear();

    if (NULL == myDoc->Parse (result))
    {
        cerr << "Parse " << result << " error!" << endl;
        return false;
    }

    TiXmlElement *root = myDoc->RootElement ();

    if (NULL == root)
        return false;
    TiXmlElement *AddrGet = root->FirstChildElement ();

    if (NULL == AddrGet)
        return false;
    TiXmlAttribute *att = AddrGet->FirstAttribute ();

    if (NULL == att)
        return false;

    while (NULL != att)
    {
        if (0 == strcmp (att->Name (), "Result"))
		{
			str += "[";
            str += att->Value ();
			str += " ";	
		}

		if (0 == strcmp (att->Name(),"Url"))
		{
			str += att->Value();	
			str += "]";
		}

        att = att->Next ();
    }

    return false;
}
