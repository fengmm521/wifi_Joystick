#ifndef __SocketUtil_H__
#define __SocketUtil_H__

//#include "cocos2d.h"
#ifdef WIN32
#include <winsock.h>
typedef int				socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include "errno.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
typedef int				SOCKET;

//#pragma region define win32 const variable in linux
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
//#pragma endregion
#endif
#define SOCKET_ERROR -1

static uint32_t QueryIP(const  char * szIP)
{
	uint32_t ip;
	ip = inet_addr(szIP);
	if(ip == INADDR_NONE)
	{
		struct hostent *host=NULL;
		host = gethostbyname(szIP);
		if(host != NULL)
			memcpy(&ip, host->h_addr_list[0], host->h_length);
		else
		{
			return INADDR_NONE;
		}
	}
    
	return ip;
}

static int GetSocket(const  char * ip, int port)
{
	int sckLocal = socket( AF_INET , SOCK_STREAM , IPPROTO_IP ) ;
    int timeOutNum = 0;
	
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr= QueryIP(ip);
	
	if(addr.sin_addr.s_addr == INADDR_NONE)
	{
		return 0;
	}
	
	int iTimeOut=5*1000;
	//发送超时设定
    ::setsockopt(sckLocal, SOL_SOCKET, SO_SNDTIMEO, (char*)&iTimeOut, sizeof(iTimeOut));
	//连接
    while ((connect(sckLocal,(struct sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR)) {
        timeOutNum++;
        
        if (timeOutNum > 6) {
            return 0;
        }
    }
	
	return sckLocal;
}

static int tcpsend(int s, char * buff, int len)
{
	int nret = 0, socklen = 0;
    
	while((nret+=socklen) < len)
	{
		socklen = send(s, &buff[nret], len, 0);
		if(0 >= socklen)
		{
			return socklen;
		}
	}
    
	return nret;
}

static int tcprecv(int s, char * buff, int len)
{
	int nret = 0, socklen = 0;
    
	while((nret+=socklen) < len)
	{
		socklen = recv(s, (char*)&buff[nret], len-nret, 0);
		if(0 >= socklen)
		{
			return socklen;
		}
	}
    
	return nret;
}

#endif



