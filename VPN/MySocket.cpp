// MySocket.cpp: implementation of the MySocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySocket.h"
#include <afxtempl.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL MySocket::m_gIsInitialSocket = FALSE;

MySocket::MySocket()
{
	m_hsk = INVALID_SOCKET;	
	m_lpnRefCount = NULL;
}

MySocket::MySocket(SOCKET hsocket)
{
		m_hsk = hsocket;
		m_lpnRefCount = new INT(1);
}

MySocket::MySocket(MySocket& obj)
{
	m_hsk = obj.m_hsk;
	m_lpnRefCount = obj.m_lpnRefCount;

	//�������ü���
	int nCOunt = *m_lpnRefCount;
	*m_lpnRefCount = ++nCOunt;
}

MySocket::~MySocket()
{
	Release();
}

BOOL MySocket::operator==(SOCKET hsocket)
{
		if (m_hsk == hsocket)
			return TRUE;
		else
			return FALSE;
}

BOOL MySocket::operator==(MySocket& obj)
{
		if (m_hsk == obj.m_hsk)
			return TRUE;
		else
			return FALSE;
}



MySocket& MySocket::operator=(MySocket& obj)
{
	Release();

	m_hsk = obj.m_hsk;
	m_lpnRefCount = obj.m_lpnRefCount;

	int nCount = *m_lpnRefCount;
	*m_lpnRefCount = ++nCount;

	return *this;
}

BOOL MySocket::gWSAStartup()
{
	//�Ѿ���ʼ��ֱ�ӷ���
	if (m_gIsInitialSocket)
		return TRUE;

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	BOOL nRet = FALSE;
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		AfxMessageBox("Winsocket ��ʼ�� ʧ��,�޷�����WinSock.dll!!!");

		//��ʼʧ��!
		m_gIsInitialSocket = FALSE;
		return FALSE;
	}
	
	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		AfxMessageBox("Winsocket ��ʼ�� ʧ��,winsocket �汾����!!!");           
		WSACleanup();
		//��ʼʧ��!
		m_gIsInitialSocket = FALSE;
		return FALSE;
	}
	
	/* The WinSock DLL is acceptable. Proceed. */
	//��ʼ�ɹ�!
	m_gIsInitialSocket = TRUE;
	return TRUE;
}

BOOL MySocket::gWSACleanup()
{
	m_gIsInitialSocket = FALSE;
	if(WSACleanup() == SOCKET_ERROR)
	{
		gWSAShowLasterr();
		return FALSE;
	}	
	return TRUE;
}

VOID MySocket::gWSAShowLasterr()
{
	DWORD ErrID = WSAGetLastError();
	if (m_gIsInitialSocket)
	{
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			ErrID,
			0, // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
			);

		//����Ƶ̫��,�ȹ����⼸���!
// 		if (ErrID != WSAEINTR &&
// 			  ErrID != WSAECONNRESET )
// 			MessageBox( NULL, (LPCTSTR)lpMsgBuf, "���:", MB_OK | MB_ICONINFORMATION );

		// Free the buffer.
		LocalFree( lpMsgBuf );
		return;
	}
	else
	{
		AfxMessageBox("winsokcet û�г�ʼ��, �����õ�������!");
		return;
	}

	/*
				 * All Windows Sockets error constants are biased by WSABASEERR from

			#define WSABASEERR              10000

			  * Windows Sockets definitions of regular Microsoft C error constants	
				#define WSAEINTR                (WSABASEERR+4)
				#define WSAEBADF                (WSABASEERR+9)
				#define WSAEACCES               (WSABASEERR+13)
				#define WSAEFAULT               (WSABASEERR+14)
				#define WSAEINVAL               (WSABASEERR+22)
				#define WSAEMFILE               (WSABASEERR+24)

	
				 * Windows Sockets definitions of regular Berkeley error constants
	
				#define WSAEWOULDBLOCK          (WSABASEERR+35)
				#define WSAEINPROGRESS          (WSABASEERR+36)
				#define WSAEALREADY             (WSABASEERR+37)
				#define WSAENOTSOCK             (WSABASEERR+38)
				#define WSAEDESTADDRREQ         (WSABASEERR+39)
				#define WSAEMSGSIZE             (WSABASEERR+40)
				#define WSAEPROTOTYPE           (WSABASEERR+41)
				#define WSAENOPROTOOPT          (WSABASEERR+42)
				#define WSAEPROTONOSUPPORT      (WSABASEERR+43)
				#define WSAESOCKTNOSUPPORT      (WSABASEERR+44)
				#define WSAEOPNOTSUPP           (WSABASEERR+45)
				#define WSAEPFNOSUPPORT         (WSABASEERR+46)
				#define WSAEAFNOSUPPORT         (WSABASEERR+47)
				#define WSAEADDRINUSE           (WSABASEERR+48)
				#define WSAEADDRNOTAVAIL        (WSABASEERR+49)
				#define WSAENETDOWN             (WSABASEERR+50)
				#define WSAENETUNREACH          (WSABASEERR+51)
				#define WSAENETRESET            (WSABASEERR+52)
				#define WSAECONNABORTED         (WSABASEERR+53)
				#define WSAECONNRESET           (WSABASEERR+54)
				#define WSAENOBUFS              (WSABASEERR+55)
				#define WSAEISCONN              (WSABASEERR+56)
				#define WSAENOTCONN             (WSABASEERR+57)
				#define WSAESHUTDOWN            (WSABASEERR+58)
				#define WSAETOOMANYREFS         (WSABASEERR+59)
				#define WSAETIMEDOUT            (WSABASEERR+60)
				#define WSAECONNREFUSED         (WSABASEERR+61)
				#define WSAELOOP                (WSABASEERR+62)
				#define WSAENAMETOOLONG         (WSABASEERR+63)
				#define WSAEHOSTDOWN            (WSABASEERR+64)
				#define WSAEHOSTUNREACH         (WSABASEERR+65)
				#define WSAENOTEMPTY            (WSABASEERR+66)
				#define WSAEPROCLIM             (WSABASEERR+67)
				#define WSAEUSERS               (WSABASEERR+68)
				#define WSAEDQUOT               (WSABASEERR+69)
				#define WSAESTALE               (WSABASEERR+70)
				#define WSAEREMOTE              (WSABASEERR+71)

				#define WSAEDISCON              (WSABASEERR+101)


				 * Extended Windows Sockets error constant definitions

				#define WSASYSNOTREADY          (WSABASEERR+91)
				#define WSAVERNOTSUPPORTED      (WSABASEERR+92)
				#define WSANOTINITIALISED       (WSABASEERR+93)

	
				 * Error return codes from gethostbyname() and gethostbyaddr()
				 * (when using the resolver). Note that these errors are
				 * retrieved via WSAGetLastError() and must therefore follow
				 * the rules for avoiding clashes with error numbers from
				 * specific implementations or language run-time systems.
				 * For this reason the codes are based at WSABASEERR+1001.
				 * Note also that [WSA]NO_ADDRESS is defined only for
				 * compatibility purposes.


				#define h_errno         WSAGetLastError()

				 Authoritative Answer: Host not found 
				#define WSAHOST_NOT_FOUND       (WSABASEERR+1001)
				#define HOST_NOT_FOUND          WSAHOST_NOT_FOUND

				 Non-Authoritative: Host not found, or SERVERFAIL 
				#define WSATRY_AGAIN            (WSABASEERR+1002)
				#define TRY_AGAIN               WSATRY_AGAIN

				/* Non recoverable errors, FORMERR, REFUSED, NOTIMP 
				#define WSANO_RECOVERY          (WSABASEERR+1003)
				#define NO_RECOVERY             WSANO_RECOVERY

				/* Valid name, no data record of requested type 
				#define WSANO_DATA              (WSABASEERR+1004)
				#define NO_DATA                 WSANO_DATA

				/* no address, look for MX record 
				#define WSANO_ADDRESS           WSANO_DATA
				#define NO_ADDRESS              WSANO_ADDRESS

				/*
				 * Windows Sockets errors redefined as regular Berkeley error constants.
				 * These are commented out in Windows NT to avoid conflicts with errno.h.
				 * Use the WSA constants instead.
				 
				#if 0
				#define EWOULDBLOCK             WSAEWOULDBLOCK
				#define EINPROGRESS             WSAEINPROGRESS
				#define EALREADY                WSAEALREADY
				#define ENOTSOCK                WSAENOTSOCK
				#define EDESTADDRREQ            WSAEDESTADDRREQ
				#define EMSGSIZE                WSAEMSGSIZE
				#define EPROTOTYPE              WSAEPROTOTYPE
				#define ENOPROTOOPT             WSAENOPROTOOPT
				#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
				#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
				#define EOPNOTSUPP              WSAEOPNOTSUPP
				#define EPFNOSUPPORT            WSAEPFNOSUPPORT
				#define EAFNOSUPPORT            WSAEAFNOSUPPORT
				#define EADDRINUSE              WSAEADDRINUSE
				#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
				#define ENETDOWN                WSAENETDOWN
				#define ENETUNREACH             WSAENETUNREACH
				#define ENETRESET               WSAENETRESET
				#define ECONNABORTED            WSAECONNABORTED
				#define ECONNRESET              WSAECONNRESET
				#define ENOBUFS                 WSAENOBUFS
				#define EISCONN                 WSAEISCONN
				#define ENOTCONN                WSAENOTCONN
				#define ESHUTDOWN               WSAESHUTDOWN
				#define ETOOMANYREFS            WSAETOOMANYREFS
				#define ETIMEDOUT               WSAETIMEDOUT
				#define ECONNREFUSED            WSAECONNREFUSED
				#define ELOOP                   WSAELOOP
				#define ENAMETOOLONG            WSAENAMETOOLONG
				#define EHOSTDOWN               WSAEHOSTDOWN
				#define EHOSTUNREACH            WSAEHOSTUNREACH
				#define ENOTEMPTY               WSAENOTEMPTY
				#define EPROCLIM                WSAEPROCLIM
				#define EUSERS                  WSAEUSERS
				#define EDQUOT                  WSAEDQUOT
				#define ESTALE                  WSAESTALE
				#define EREMOTE                 WSAEREMOTE
				#endif
					
	
	*/

}

VOID MySocket::gShowLasterr()
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		WSAGetLastError(),
		0, // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, "���:", MB_OK | MB_ICONINFORMATION );
	// Free the buffer.
	LocalFree( lpMsgBuf );
	return;
}

//����һ���׽��� ����:�׽�������,Э��
BOOL MySocket::Socket(int type, int protocol)
{
	//һ������ֻ����һ���׽���,����׽Ӵ���,ֱ�ӷ���
	if (INVALID_SOCKET != m_hsk)
	{
	//	AfxMessageBox("MySocket �����Ѿ�����һ���׽���!��ֱ�ӷ��أ�C++������socket���ֲ���!");
		return TRUE; 
	}

	m_hsk = ::socket(AF_INET, type, protocol);
	if (INVALID_SOCKET == m_hsk)
	{
		MySocket::gWSAShowLasterr();
		return FALSE;
	}
	
	if (NULL == m_lpnRefCount)
		m_lpnRefCount = new int(1);
	else
	{
		AfxMessageBox("MySocket::Socket ���ü������������ڣ�����Ʋ����������!");
	}
	return TRUE;
}


MySocket& MySocket::operator=(SOCKET hsocket)
{
	Release();
	m_hsk = hsocket;
	m_lpnRefCount = new INT(1);
	return *this;
}

BOOL MySocket::Bind(char* lpszIp,u_short nPort)
{
	sockaddr_in ski = {0};
	ski.sin_family = AF_INET;
	ski.sin_port   = ::htons(nPort);

	ski.sin_addr.s_addr = ::inet_addr(lpszIp);

	int nRet = ::bind(m_hsk, (sockaddr*)&ski, sizeof(ski));

	if (SOCKET_ERROR == nRet)
	{
		MySocket::gWSAShowLasterr();
		return FALSE;
	}

	return TRUE;
}



u_short MySocket::gntohs(u_short netshort)
{
	return ::ntohs(netshort);
}

u_short MySocket::ghtons(u_short hostshort)
{
	return ::htons(hostshort);
}

u_long MySocket::gntohl( u_long netlong)
{
	return ::ntohl(netlong);
}

u_long MySocket::ghtonl( u_long hostlong )
{
	return ::htonl(hostlong);
}

//�ַ���IPת�������ֽ�IP , ���� 0 ʧ�ܣ� 
unsigned long MySocket::ginet_addr(const char* lpszIPaddr)
{
	unsigned long uRet = 0;

	uRet = ::inet_addr(lpszIPaddr);
	if (0 == uRet || INADDR_NONE == uRet)
	{
		AfxMessageBox("inet_addr��IP��ַת��ʧ��!");
		return 0;
	}

	return uRet;
}


char* MySocket::ginet_ntoa(in_addr IP)
{
	char* lpszIP = NULL;
	lpszIP = ::inet_ntoa(IP);
	if (NULL == lpszIP)
	{
		AfxMessageBox("ginet_ntoa��IP��ַת��ʧ��!");
	}

	return lpszIP;
}

char* MySocket::ginet_ntoa(u_long uIP)
{
	char* lpszIP = NULL;
	in_addr IP;
	IP.s_addr = uIP;
	lpszIP = ::inet_ntoa(IP);

	if (NULL == lpszIP)
	{
		AfxMessageBox("ginet_ntoa��IP��ַת��ʧ��!");
	}
	
	return lpszIP;
}

//������������(backlog:�ȴ������������,�������ˣ���������ֱ�Ӿܾ�)
BOOL MySocket::Listen(int backlog)
{
	if (SOCKET_ERROR == ::listen(m_hsk, backlog))
	{
		MySocket::gWSAShowLasterr();
		return FALSE;
	}

	return TRUE;
}

//INVALID_SOCKETʧ��:  ��������(addr:��ַ�����Ϣ)
SOCKET MySocket::Accept(OUT sockaddr_in *lpAddr)
{
		SOCKET hNewsk = INVALID_SOCKET;		
		
		int  AddrSize = sizeof(sockaddr);
		hNewsk = ::accept(m_hsk, (sockaddr*)lpAddr, &AddrSize);
		
		if (INVALID_SOCKET == hNewsk)
			gWSAShowLasterr();

		return hNewsk;
}

//�Ѹ��Ƶ�ϵͳ�������ֽ���,ʧ��SOCKET_ERROR: ���� (buf: Ҫ���͵��û�������, len:buf��С, ����ģʽ)
int MySocket::Send(char *buf, int len, int flags)
{
		int nRet = 0;
		
		nRet = ::send(m_hsk, buf, len, flags);
		
		if (SOCKET_ERROR  == nRet)
		{
			gWSAShowLasterr();
		}
		return nRet;
}

//���յ��ֽ���,ʧ��SOCKET_ERROR: ���� (buf: ���յ��û�������, len:buf��С, ����ģʽ)
int MySocket::Recv(char* buf, int len, int flags)
{
		int nRet = 0;
		nRet = ::recv(m_hsk, buf, len, flags);
		if (SOCKET_ERROR == nRet || nRet == 0)
		{
			gWSAShowLasterr();
		}
		
		return nRet;
}


//�ر��׽���,������ü�����������socket ��invalid_socket
BOOL MySocket::CloseSocket()
{
		if (INVALID_SOCKET == m_hsk)
			return TRUE;

		int nRet = ::shutdown(m_hsk, SD_BOTH);
// 		if (SOCKET_ERROR == nRet)
// 		{
// 			gWSAShowLasterr();
// 		}
		
		nRet = ::closesocket(m_hsk);
// 		if (SOCKET_ERROR == nRet)
// 		{
// 			gWSAShowLasterr();
// 			//return FALSE;
// 		}
		
		m_hsk = INVALID_SOCKET;
		if (m_lpnRefCount != NULL)
			delete m_lpnRefCount;
		m_lpnRefCount = NULL;

		return TRUE;
}

//����     (lpDestIp: Ŀ��IP����nDestPort��Ŀ��˿�)
BOOL MySocket::Connect(char* lpDestIp,u_short nDestPort)
{
	sockaddr_in ski = {0};
	ski.sin_family = AF_INET;
	ski.sin_port   = ::htons(nDestPort);
	ski.sin_addr.s_addr = ::inet_addr(lpDestIp);
		
	if ( SOCKET_ERROR == ::connect(m_hsk, (sockaddr*)&ski, sizeof(sockaddr)) )
	{
		gWSAShowLasterr();
		return FALSE;
	}
	
	return TRUE;
}

//����     (lpDestIp: Ŀ��IP����nDestPort��Ŀ��˿�)
BOOL MySocket::Connect(u_long uDestIp,u_short nDestPort)
{
	sockaddr_in ski = {0};
	ski.sin_family = AF_INET;
	ski.sin_port   = ::htons(nDestPort);
	ski.sin_addr.s_addr = uDestIp;
	
	if ( SOCKET_ERROR == ::connect(m_hsk, (sockaddr*)&ski, sizeof(sockaddr)) )
	{
		gWSAShowLasterr();
		return FALSE;
	}
	
	return TRUE;
}


void MySocket::Release()
{
	if (m_hsk == INVALID_SOCKET)
	{
		return ;
	}
	
	//�������ü���
	if (m_lpnRefCount)
	{
		int nRefCount = (*m_lpnRefCount) - 1;
		*m_lpnRefCount = nRefCount;

		if (nRefCount <= 0)
			this->CloseSocket();
	}
	else
	{
		m_hsk = INVALID_SOCKET;
		return;
	}

}

SOCKET MySocket::GetSafeHandle()
{
	return m_hsk;
}


//�ڴ�ϵͳ���ջ�������һ����ȡһ������UDP��,���Բ������ճ������,len��С�����.
//���յ��ֽ���,ʧ��SOCKET_ERROR: UDP���� (buf: ���յ��û�������, len:buf��С, from:�ͻ��˵ĵ�ַ��Ϣ)
int MySocket::RecvFrom(IN char* buf, IN int len, OUT struct sockaddr_in *from)
{
	int nAddrSize = sizeof(sockaddr);
	int nRet = 
		::recvfrom(m_hsk, buf, len, 0, (sockaddr*)from, &nAddrSize);
	
	if (0 == nRet || SOCKET_ERROR == nRet)
	{
		gWSAShowLasterr();
	}
	
	return nRet;
}

//�ѷ��͵��ֽ���,ʧ��SOCKET_ERROR: UDP���� (buf: ���յ��û�������, len:Length of the data in buf. , from:�ͻ��˵ĵ�ַ��Ϣ)
//�ѷ����ֽ������ܻ�С��ָ���ĳ���
int MySocket::SendTo(IN char* buf, IN int len, IN sockaddr_in *to)
{
	int nRet = 
		::sendto(m_hsk, buf, len, 0, (sockaddr*)to, sizeof(sockaddr));
	
	if (SOCKET_ERROR == nRet)
	{
		gWSAShowLasterr();
	}
	
	return nRet;
}

//�ѷ��͵��ֽ���,ʧ��SOCKET_ERROR: UDP���� (buf: ���յ��û�������, len:Length of the data in buf. , lpDestIp: Ŀ��IP nDestPort��Ŀ��˿�)
//�ѷ����ֽ������ܻ�С��ָ���ĳ���
int MySocket::SendTo(IN char* buf, IN int len, IN char* lpDestIp, IN u_short nDestPort)
{
	
	sockaddr_in to = {0};
	to.sin_family = AF_INET;
	to.sin_port = ::htons(nDestPort);
	to.sin_addr.s_addr = ::inet_addr(lpDestIp);

	int nRet = 
		::sendto(m_hsk, buf, len, 0, (sockaddr*)&to, sizeof(sockaddr));
	
	if (SOCKET_ERROR == nRet)
	{
		gWSAShowLasterr();
	}
	
	return nRet;
}
//�����׽������ͺ���

//The WSAAsyncSelect function requests Windows message-based notification of network events for a socket.
//SOCKET_ERRORʧ�� (�𶨵Ĵ���, �ĸ���Ϣ֪ͨ, ʲô�����¼�����֪ͨ)
int MySocket::WSAAsyncSelect(HWND hWnd, u_int wMsg, long lEvent)
{
	int nRet = 0;
	
	nRet = ::WSAAsyncSelect(m_hsk, hWnd, wMsg, lEvent);
	if (SOCKET_ERROR == nRet)
	{
		gWSAShowLasterr();
	}
	
	return nRet;
}
//����: 0��ʱ�˳�,SOCKET_ERROR���� ,���򷵻ط��������¼��������׽����������ܺ�
//����:�ɵȴ�64��socket�ź��¼�(readfds�ɶ�?, writefds ��д? ,nMillisecond: ��ʱʱ�� ���뵥λ )
/*
FD_ZERO(fd_set   *fdset)�����fdsetf,������ʹ��ǰ���ǿյġ�       
FD_SET(int   fd,   fd_set   *fdset): ����׽��ֵ�����       
FD_CLR(int   fd,   fd_set   *fdset)���Ӽ�����ɾ���ᶨ�׽���      
FD_ISSET(int   fd,   fdset   *fdset)�����s�ǲ��Ǽ��ϵĳ�Ա,����Ƿ���TRUE   
*/
int MySocket::gSelect(fd_set FAR *readfds, fd_set FAR *writefds, fd_set FAR *exceptfds, int nMillisecond)
{
	timeval timeout;
	timeout.tv_sec = nMillisecond / 1000;
	timeout.tv_usec = (nMillisecond % 1000)*1000 ;

	int nRet = 0;
	int nfds = 0;	//����,����Ϊ����Berkeley�׽��ּ���
						
	nRet = ::select(nfds, readfds, writefds, exceptfds, &timeout);
	if (SOCKET_ERROR == nRet )
	{
	 gWSAShowLasterr();
	 return nRet;
	}

	return nRet;
}

//CheckSum:����У��͵��Ӻ��� 
USHORT PROTOCOL_HEAD::CheckSum(USHORT *buffer, int size) 
{ 
	unsigned long cksum=0; 
	while(size >1) 
	{ 
		cksum+=*buffer++; 
		size -=sizeof(USHORT); 
	} 
	if(size ) 
	{ 
		cksum += *(UCHAR*)buffer; 
	} 
	
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16); 
	return (USHORT)(~cksum); 
}