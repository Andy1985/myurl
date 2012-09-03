#ifndef _CSOCKET_H_
#define _CSOCKET_H_

#include <sys/time.h>

#include <string>
using std::string;

#define RECV_MAX_SIZE 1024

class CSocket
{
  public:
    CSocket (string ip, unsigned int port, int timeout);
    CSocket (string ip, unsigned int port);
     ~CSocket ();

    bool Connect ();
    void Close ();
    bool Send (string data);
    bool Receive (string & data);

  private:
    int sockfd;
    string ip;
    unsigned int port;
    struct timeval timeout;
};

#endif /*_CSOCKET_H_*/
