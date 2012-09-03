#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;

#define MAXFDS 256
#define EVENTS 100
#define PORT 5555
#define MAXSIZE 256

bool setNonBlock(int fd)
{
    int flags = fcntl(fd,F_GETFL,0);
    flags |= O_NONBLOCK;

    if (-1 == fcntl(fd,F_SETFL,flags))
    {
        return false;
    }

    return true;
}

int main(int argc,char* argv[],char* evp[])
{
    int listen_fd;
    int nfds;
    int confd;
    int ret;
    int epfd;

    int on = 1;

    char buffer[512];
    char rsp_msg[3] = "OK";

    struct sockaddr_in saddr;
    struct sockaddr_in caddr;
    struct epoll_event ev;
    struct epoll_event events[EVENTS];
    socklen_t clilen;
    
    listen_fd = socket(AF_INET,SOCK_STREAM,0);

    if (-1 == listen_fd)
    {
        cout << "create socket error!" << endl;
        return -1;
    }

    epfd = epoll_create(MAXFDS);

    setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons((short)(PORT));
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (-1 == bind(listen_fd,(struct sockaddr*)&saddr,sizeof(saddr)))
    {
        perror("bind error");
        return -1;
    }

    if (-1 == listen(listen_fd,32))
    {
        perror("listen error");
        return -1;
    }

    ev.data.fd = listen_fd;
    ev.events = EPOLLIN | EPOLLET;

    epoll_ctl(epfd,EPOLL_CTL_ADD,listen_fd,&ev);

    while (true)
    {
        nfds = epoll_wait(epfd,events,MAXFDS,0);

        for (int i = 0; i < nfds; ++i)
        {
            if (listen_fd == events[i].data.fd)
            {
                clilen = sizeof(caddr);
                confd = accept(listen_fd,(struct sockaddr*)&caddr,&clilen);
                if (-1 == confd)
                {
                    perror("accept error");
                    break;
                }

                cout << "accept a require" << endl;
                setNonBlock(confd);
                ev.data.fd = confd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_ADD,confd,&ev);
            }
            else if (events[i].events & EPOLLIN)
            {
                bzero(buffer,sizeof(buffer));
                cout << "ready to read,fd is:" << events[i].data.fd << endl;
                ret = read(events[i].data.fd,buffer,MAXSIZE);
            
                if (ret < 0)
                {
                    perror("recv error");
					epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd, &ev);
					close(events[i].data.fd);
                    //return -1;
                }

                cout << "accept msg is: " << buffer << endl;
                ev.data.fd = events[i].data.fd;
                ev.events = EPOLLOUT | EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_MOD,events[i].data.fd, &ev);

            }
            else if (events[i].events & EPOLLOUT)
            {
                ret = send(events[i].data.fd,rsp_msg,strlen(rsp_msg),1);
                if (ret < 0)
                {
                    perror("send error");
			epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd, &ev);
			close(events[i].data.fd);
                    //return -1;
                }
                
                ev.data.fd = events[i].data.fd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_MOD,events[i].data.fd,&ev);
                
            }
        }
    }
}
