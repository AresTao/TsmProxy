/*************************************************************************

 * Copyright (c) 2005-2008, by Beijing TeleStar Network Technology Company Ltd.(MT2)
 * All rights reserved.

 * FileName：       udpsocket.h
 * System：         UniFrame 
 * SubSystem：      Common
 * Author：         Long Xiangming
 * Date：           2006.06.03
 * Version：        1.0
 * Description：
		CUDPSocket定义。UDP协议的Socket调用。
		其主要接口与CSocket类(TCP)相同。
 *
 * Last Modified:
    2006.06.03, 完成初始版本
        By Long Xiangming


**************************************************************************/
#ifndef __UDPSOCKET_H
#define __UDPSOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //include inet_addr()  
#include <netdb.h>
#include <unistd.h>
#else
#include <winsock2.h> //include timeval
#endif

#include <fcntl.h>
#include <time.h>

#include "comtypedef.h"
#include "comtypedef_vchar.h"

#ifndef _WIN32
	#define SOCKLEN_T socklen_t
#else
	#define SOCKLEN_T int
#endif

const INT MaxRecvBufSize = 8096;
const INT MaxSendBufSize = 8096;

_CLASSDEF(CUDPSocket)
class CUDPSocket 
{
public:
		CUDPSocket();
		virtual ~CUDPSocket();

		//注意，server和client用的是相同的fd。即发送和接收用使用相同的fd,避免干扰。
		//如果既需要收，也需要发，那么openServer即可。如果只需要发，则openClient。不能同时openServer和openClient

		//打开服务端socket
		INT    openServer(CHAR* localAddr, INT localPort); 
		//打开客户端socket
		BOOL   openClient(CHAR* remoteAddr,INT remotePort); 
		

		//Added by Lxm. 2009-8-10
		//打开客户端socket, 不指定对端地址。（每次发送时， 填写地址，以发往不同地址)
		BOOL   openClient();

		//发送消息
		//-1：写socket失败
		//-3: socket关闭
		// 1：成功
		INT     sendCode(CCode &code);   

		INT     sendCode(CCode &code,CHAR* remoteAddr,INT remotePort); //指定对方地址和端口发送  

		//接收消息
		//-1: 读Socket失败
		//-3: socket关闭
		//-4: code.content==NULL，
		//    调用Recv()之前未给code.content分配空间
		// 1: 成功
		INT     recvCode(CCode& code);   

		//关闭socket
		BOOL    closeSocket();
		
		INT     getFd() { return m_socketFd;} //最近一次调用open获得的fd

		//得到客户端地址,在调用recvCode之后再调用本方法。
		//Added by Lxm. 2009-08-10
		sockaddr_in& getClientSockAddr() {	return m_clientAddr;	}
		sockaddr_in& getRemoteSockAddr() { return m_remoteAddr;	}

		// format of addr: xxx.xxx.xxx.xxx:port
		void setRemoteSockAddr(const char* addr);
		void setRemoteSockAddr(const char* host, int port);
		void clientInfo(CStr*);
		void serverInfo(CStr*);
	
		void    setAddrReuse(BOOL isAddrReuse) { m_addrReuseFlag = isAddrReuse;}

		//added by Long Xiangming. 2010.1.27
		//socket buff 可以设置。目前仅使用setRecvBufSize
		//注意，这两个方法需要显式的调用而不会隐含在openServer或openClient方法中，(在调用openServer方法之后调用)
		//因为对于心跳等使用udpSocket的地方，并不需要设置buffSize
		void setRecvBufSize(INT iRecvBufSize);//设置接收缓冲区大小
		void setRecvBufSize();//设置接收缓冲区大小. 使用默认

		void setSendBufSize(INT iSendBufSize);//设置发送缓冲区大小
		void setSendBufSize();//设置发送缓冲区大小,使用默认

protected:
		struct  sockaddr_in m_localAddr;
		struct  sockaddr_in m_remoteAddr;
		struct  sockaddr_in m_clientAddr;
		INT					m_localFd;
		INT					m_remoteFd;
		INT                 m_socketFd;
		INT					m_localPort;
		INT					m_remotePort;
		SOCKLEN_T			m_addrLen;
		
		TCommState          m_socketState;

		BOOL				m_addrReuseFlag; //地址重用标记。 //added by Long Xiangming.2006.06.30
										     //缺省为0

		//added by Long Xiangming. 2010.1.27
		//socket buff 可以设置。目前仅使用recvBufSize
		UINT m_socketSendBufSize;
		UINT m_socketRecvBufSize;
};


#endif



