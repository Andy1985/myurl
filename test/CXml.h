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

    bool BuildUrlData (const vector<string>& urls,string& urlData);
    bool ParseSocketResult (const char *result,string& str);

  private:
    TiXmlDocument * myDoc;
};

#endif /*_CXMLCONFIG_H_*/
