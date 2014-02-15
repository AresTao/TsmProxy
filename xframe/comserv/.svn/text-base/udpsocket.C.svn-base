/*************************************************************************

 * Copyright (c) 2005-2008, by Beijing TeleStar Network Technology Company Ltd.(MT2)
 * All rights reserved.

 * FileName：       udpsocket.C
 * System：         UniFrame
 * SubSystem：      Common
 * Author：         Long Xiangming
 * Date：           2006.06.03
 * Version：        1.0
 * Description：
		CUdpSocket实现。UDP协议的Socket调用。
 *
 * Last Modified:
    2006.06.03, 完成初始版本
        By Long Xiangming


**************************************************************************/
#include "udpsocket.h"
#include "func.h"
#include "info.h"

CUDPSocket::CUDPSocket()
{
	m_localFd    = -1;
	m_remoteFd   = -1;
	m_socketFd   = -1;
	m_localPort  = 0;
	m_remotePort = 0;

	m_socketState= CLOSE;
	m_addrReuseFlag = 0;     //缺省为“地址不重用”

	//在Win32下，必须使用WSAStartup，否则socket()调用始终不成功
#ifdef WIN32
	WORD wVersionRequested = MAKEWORD(2,0);
	WSADATA wsaData;
	WSAStartup( wVersionRequested, &wsaData );
#endif
	m_addrLen = sizeof(struct sockaddr);
}

CUDPSocket::~CUDPSocket()
{
	if(m_socketState==OPEN)
		closeSocket();
}

void CUDPSocket::clientInfo(CStr* s)
{
	s->fCat("fd=%d, server=%s:%d", getFd(), inet_ntoa(m_remoteAddr.sin_addr), ntohs(m_remoteAddr.sin_port));
}
void CUDPSocket::serverInfo(CStr* s)
{
	s->fCat("fd=%d, port=%d", getFd(), m_localPort);
}

//打开服务端socket
//-1: 失败
// 1: 成功
INT CUDPSocket::openServer(CHAR* localAddr, INT localPort)
{
	//if(localAddr == NULL) return -1;
	if(localPort <=0    ) return -1;

	m_localAddr.sin_family = AF_INET;
	m_localAddr.sin_port = htons(localPort);
	if(localAddr==NULL)
		m_localAddr.sin_addr.s_addr = INADDR_ANY;
	else
	{
		//added by Long Xiangming. 2007.11.07
		if(localAddr[0]==0 || strcmp(localAddr,"0")==0 || strcmp(localAddr,"0.0.0.0")==0)
			m_localAddr.sin_addr.s_addr = INADDR_ANY;
		else
			m_localAddr.sin_addr.s_addr=inet_addr(localAddr);
	}

	memset(&(m_localAddr.sin_zero), '\0', 8);
	m_localPort = localPort;

  	m_localFd = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_localFd<0) {
	    perror("open socket server");
	    //exit(1);
	}

	if (setsockopt(m_localFd, SOL_SOCKET, SO_REUSEADDR, (CHAR*)&m_addrReuseFlag, sizeof(m_addrLen)) == -1) {
		perror("setsockopt");
		return -1;
		//exit(1);
	}

	if (bind(m_localFd, (struct sockaddr *)&m_localAddr, sizeof(struct sockaddr)) != 0) {
	    perror("bind");
		return -1;
	    //exit(1);
	}
	m_socketState = OPEN;
	m_socketFd = m_localFd;

	return 1;
}


void CUDPSocket::setRemoteSockAddr(const char* host, int port)
{
	m_remoteAddr.sin_port = htons(port);
	m_remoteAddr.sin_addr.s_addr = inet_addr(host);
}

void CUDPSocket::setRemoteSockAddr(const char* addr)
{
	const char* p = strstr(addr, ":");
	if(p==NULL)
		return;
	CStr host;
	host.set(addr, p-addr);
	setRemoteSockAddr(host.c_str(), atoi(p+1));
}

//打开客户端socket
BOOL  CUDPSocket::openClient(CHAR* remoteAddr,INT remotePort)
{
	//if(remoteAddr == NULL) return FALSE;
	if(remotePort <=0    ) return FALSE;
	CHAR remoteAddress[256];
	if(remoteAddr == NULL)
	{
		strcpy(remoteAddress,"127.0.0.1");
	}
	else
	{
		if(remoteAddr[0] == 0 || strcmp(remoteAddr,"0")==0 || strcmp(remoteAddr,"0.0.0.0")==0)
			strcpy(remoteAddress,"127.0.0.1");
		else
			strcpy(remoteAddress,remoteAddr);
	}

	m_remoteAddr.sin_family = AF_INET;
	m_remoteAddr.sin_port = htons(remotePort);
	m_remoteAddr.sin_addr.s_addr=inet_addr(remoteAddress);

	memset(&(m_remoteAddr.sin_zero), '\0', 8);
	m_remotePort = remotePort;

	m_remoteFd = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_remoteFd<0)
	{
		perror("open socket client");
	    //exit(1);
		return FALSE;
	}

	m_socketFd = m_remoteFd;
	m_socketState = OPEN;
	return TRUE;
}

//打开客户端socket
BOOL  CUDPSocket::openClient()
{
	m_remoteFd = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_remoteFd<0)
	{
		perror("open socket client");
	    //exit(1);
		return FALSE;
	}

	m_socketFd = m_remoteFd;
	m_socketState = OPEN;
	return TRUE;
}

//发送消息
//-1：写socket失败
//-3: socket关闭
//1 ：成功
INT CUDPSocket::sendCode(CCode &code)
{
	if(m_socketState == CLOSE) return -3;
	if(code.length <=0 || code.content == NULL) return -1;
	INT rt = sendto(m_socketFd, code.content, code.length, 0, (struct sockaddr *)&m_clientAddr, sizeof(struct sockaddr));
	if(rt>0)
		return 1;
	else return rt;

}

INT CUDPSocket::sendCode(CCode &code,CHAR* remoteAddr,INT remotePort)
{
	if(m_socketState == CLOSE) return -3;
	if(code.length <=0 || code.content == NULL) return -1;

	m_remoteAddr.sin_family = AF_INET;
	m_remoteAddr.sin_addr.s_addr=inet_addr(remoteAddr);
	m_remoteAddr.sin_port = htons(remotePort);
	memset(&(m_remoteAddr.sin_zero), '\0', 8);

	INT rt = sendto(m_socketFd, code.content, code.length, 0, (struct sockaddr *)&m_remoteAddr, sizeof(struct sockaddr));
	if(rt>0)
		return 1;
	else return rt;

}

//接收消息
//-1: 读Socket失败
//-3: socket关闭
//-4: code.content==NULL，
//    调用Recv()之前未给code.content分配空间
// 1: 成功
INT CUDPSocket::recvCode(CCode& code)
{
	if(m_socketState == CLOSE) return -3;
	if(code.content == NULL) return -4;
	//sockaddr tempAddr;
	INT rt =recvfrom(m_socketFd, code.content, MaxRecvBufSize, MSG_DONTWAIT, (struct sockaddr *)&m_clientAddr, &m_addrLen);

	if(rt>0)
	{
		code.length = rt;
		return 1;
	}
	else return rt;

}

//关闭socket
BOOL   CUDPSocket::closeSocket()
{
	if(m_socketState == CLOSE) return FALSE;
	BOOL rt = FALSE;
	if(m_remoteFd>0)
	{
		if(close_socket(m_remoteFd))
		{
			m_remoteFd = -1;
			m_socketState = CLOSE;  //socket状态
			rt = TRUE;
		}
		else
			rt = FALSE;
    }

	if(m_localFd>0)
	{
		if(close_socket(m_localFd))
		{
			m_localFd = -1;
			m_socketState = CLOSE;  //socket状态
			rt = TRUE;
		}
		else
			rt = FALSE;
	}
  	return rt;
}

void CUDPSocket::setRecvBufSize(INT iRecvBufSize)
{

	if (iRecvBufSize>0 && m_socketFd>0)
	{
		m_socketRecvBufSize = iRecvBufSize;

		if (setsockopt(m_socketFd, SOL_SOCKET, SO_RCVBUF, (CHAR*)&iRecvBufSize,
				sizeof(iRecvBufSize))<0)
		{
			UniERROR("set UDP receive buffer size error: %d.", iRecvBufSize);
			perror("setRecvBufSize:");
		}
		else
			UniINFO("Set UDP socket receive buffer size to %d(bytes) succeed.",
					iRecvBufSize);
	}
}

//这里使用默认取值进行设置。
void CUDPSocket::setRecvBufSize()
{
	m_socketRecvBufSize = MaxRecvBufSize;
	setRecvBufSize(m_socketRecvBufSize);
}

void CUDPSocket::setSendBufSize(INT iSendBufSize)
{

	if (iSendBufSize>0 && m_socketFd>0)
	{
		m_socketSendBufSize = iSendBufSize;

		if (setsockopt(m_socketFd, SOL_SOCKET, SO_SNDBUF, (CHAR*)&iSendBufSize,
				sizeof(iSendBufSize))<0)
		{
			UniERROR("set UDP send buffer size error: %d.", iSendBufSize);
			perror("setSendBufSize:");
		}
		else
			UniINFO("Set UDP socket send buffer size to %d(bytes) succeed.",
					iSendBufSize);
	}
}

//这里使用默认取值进行设置。
void CUDPSocket::setSendBufSize()
{
	m_socketSendBufSize = MaxSendBufSize;
	setSendBufSize(m_socketSendBufSize);
}

