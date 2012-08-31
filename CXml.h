#ifndef _CXMLCONFIG_H_
#define _CXMLCONFIG_H_

#include "tinyxml.h"
#include <string>
#include <vector>

using std::vector;
using std::string;

class CXml
{
  public:
    CXml ();
    ~CXml ();

    bool BuildSocketResult (const int keyCheckCode,const string url,string& result);
    bool ParseUrlXml (const char* xml,vector<string>& urls);

  private:
    TiXmlDocument * myDoc;
};

#endif /*_CXMLCONFIG_H_*/
