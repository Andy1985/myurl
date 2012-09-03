#include "CSocket.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <iostream>
using std::cerr;
using std::endl;

CSocket::CSocket (string ip, unsigned int port, int timeout)
{
    this->ip = ip;
    this->port = port;
    this->timeout.tv_sec = timeout;
}

CSocket::CSocket (string ip, unsigned int port)
{
    this->ip = ip;
    this->port = port;
    this->timeout.tv_sec = 10;
}

CSocket::~CSocket ()
{

}

bool CSocket::Connect ()
{
    struct hostent *host;

    if ((host = gethostbyname (ip.c_str ())) == NULL)
    {
        cerr << "gethostbyname error!" << endl;
        return false;
    }

    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cerr << "socket create error!" << endl;
        return false;
    }

    if (setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout,
            sizeof (timeout)) == -1)
    {
        cerr << "setsockopt connect timeout error!" << endl;
        return false;
    }

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons (port);
    serv_addr.sin_addr = *((struct in_addr *) host->h_addr);
    bzero (&(serv_addr.sin_zero), 8);

    if (connect (sockfd, (struct sockaddr *) &serv_addr,
            sizeof (struct sockaddr)) == -1)
    {
        if (errno == EINPROGRESS)
        {
            cerr << "connect timeout!" << endl;
            return false;
        }

        cerr << "connect error!" << endl;
        return false;
    }

    if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout,
            sizeof (struct timeval)) < 0)
    {
        cerr << "setsockopt recv timeout error!" << endl;
        Close ();
    }

    struct linger linger;

    linger.l_onoff = 1;
    linger.l_linger = 0;
    if (setsockopt (sockfd, SOL_SOCKET, SO_LINGER, (char *) &linger,
            sizeof (struct linger)) < 0)
    {
        cerr << "setsockopt linger error!" << endl;
        Close ();
    }

    return true;
}

void CSocket::Close ()
{
    close (sockfd);
}

bool CSocket::Send (string data)
{
    if (send (sockfd, data.c_str (), data.size (), 0) == -1)
    {
        cerr << "Send error!" << endl;
        return false;
    }

    return true;
}

bool CSocket::Receive (string & data)
{
    char buffer[RECV_MAX_SIZE] = { '\0' };
    int recvbytes = 0;

    if ((recvbytes = recv (sockfd, buffer, RECV_MAX_SIZE, 0)) == -1)
    {
        if (errno == EINPROGRESS)
        {
            cerr << "recv timeout" << endl;
            return false;
        }

        cerr << "recv error!" << endl;
        return false;
    }

    data = string (buffer);

    return true;
}
