// MySocket.h: interface for the MySocket class.
//ss1
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOCKET_H__AAFE97E6_1447_49BF_B051_FA8C05640E35__INCLUDED_)
#define AFX_MYSOCKET_H__AAFE97E6_1447_49BF_B051_FA8C05640E35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Winsock2.h.>
#pragma comment(lib, "Ws2_32.lib")
#define  OUT
#define  IN

class MySocket  
{
public:
	MySocket();
	MySocket(MySocket&);
	MySocket(SOCKET);

	~MySocket();

	MySocket& operator=(SOCKET hsocket);
	MySocket& operator=(MySocket&);

	BOOL operator==(SOCKET hsocket);
	BOOL operator==(MySocket& );
	
/*	operator UINT()*/;
public:
	static BOOL gWSAStartup();
	static BOOL gWSACleanup();
	//��ʾwinsocket������ԭ��
	static  VOID gWSAShowLasterr();
	//��ʾ������ԭ��
	static  VOID gShowLasterr();
	
	//ushot ����-->����
	static u_short gntohs(u_short netshort);
	//ulong ����-->����
	static u_long gntohl( u_long netlong);

	//ushot ����--> ����
	static u_short ghtons(u_short hostshort);
	//ulong ����--> ����
	static u_long ghtonl( u_long hostlong );

	//�ַ���IPת�������ֽ�IP , ���� 0 ʧ�ܣ� 
	static unsigned long ginet_addr(const char* lpszIPaddr);

	//����IP--���ַ���IP������NULL ʧ��
	static char* ginet_ntoa(in_addr IP);
	//����IP--���ַ���IP������NULL ʧ��
	static char* ginet_ntoa(u_long uIP);

	//����: 0��ʱ�˳�,SOCKET_ERROR���� ,���򷵻ط��������¼��������׽����������ܺ�
	//����:�ɵȴ�64��socket�ź��¼�(readfds�ɶ�?, writefds ��д? ,nMillisecond: ��ʱʱ�� ���뵥λ )
	/*
	FD_ZERO(fd_set   *fdset)�����fdsetf,������ʹ��ǰ���ǿյġ�       
	FD_SET(int   fd,   fd_set   *fdset): ����׽��ֵ�����       
	FD_CLR(int   fd,   fd_set   *fdset)���Ӽ�����ɾ���ᶨ�׽���      
	FD_ISSET(int   fd,   fdset   *fdset)�����s�ǲ��Ǽ��ϵĳ�Ա,����Ƿ���TRUE   
	*/
	static int gSelect(fd_set FAR *readfds, fd_set FAR *writefds, fd_set FAR *exceptfds, int nMillisecond);

public:
	//��¼�Ƿ��Ѿ�������WSAStartup 
	static BOOL m_gIsInitialSocket;
	
	SOCKET m_hsk;
	//���ü���
	PINT  m_lpnRefCount;

public:
	
	SOCKET GetSafeHandle();
	// �ͷ�
	void Release();
	//����һ���׽��� ����:�׽�������,Э��
	BOOL Socket(int type, int protocol);
	//����    ����:ָ��IP�ַ��������˿�;
	BOOL Bind(char* lpszIp,u_short nPort);
	//����     (lpDestIp: Ŀ��IP����nDestPort��Ŀ��˿�)
	BOOL MySocket::Connect(char* lpDestIp,u_short nDestPort);

	//������(backlog:�ȴ������������,�������ˣ���������ֱ�Ӿܾ�)
	BOOL Listen(int backlog);
	
	//INVALID_SOCKETʧ��:  ��������(addr:��ַ�����Ϣ)
	SOCKET MySocket::Accept(OUT sockaddr_in *lpAddr);

	//�Ѹ��Ƶ�ϵͳ�������ֽ���,ʧ��SOCKET_ERROR: ���� (buf: Ҫ���͵��û�������, len:buf��С, ����ģʽ)
	int Send(char *buf, int len, int flags);
	//���յ��ֽ���,ʧ��SOCKET_ERROR: ���� (buf: ���յ��û�������, len:buf��С, ����ģʽ)
	int Recv(char* buf, int len, int flags);

	//�ر��׽���,������ü�����������socket ��invalid_socket
	BOOL CloseSocket();

	//udp -------------------------------------------------------------------------
	//�ڴ�ϵͳ���ջ�������һ����ȡһ������UDP��,���Բ������ճ������,len��С�����.
	//���յ��ֽ���,ʧ��SOCKET_ERROR: UDP���� (buf: ���յ��û�������, len:buf��С, from:�ͻ��˵ĵ�ַ��Ϣ)
	int RecvFrom(IN char* buf, IN int len, OUT struct sockaddr_in *from);
	
	//�ѷ��͵��ֽ���,ʧ��SOCKET_ERROR: UDP���� (buf: ���յ��û�������, len:Length of the data in buf. , from:�ͻ��˵ĵ�ַ��Ϣ)
	//�ѷ����ֽ������ܻ�С��ָ���ĳ���
	int SendTo(IN char* buf, IN int len, IN struct sockaddr_in *to);

	//�ѷ��͵��ֽ���,ʧ��SOCKET_ERROR: UDP���� (buf: ���յ��û�������, len:Length of the data in buf. , lpDestIp: Ŀ��IP nDestPort��Ŀ��˿�)
	//�ѷ����ֽ������ܻ�С��ָ���ĳ���
	int SendTo(IN char* buf, IN int len, IN char* lpDestIp, IN u_short nDestPort);

	//�����׽������ͺ���

	//The WSAAsyncSelect function requests Windows message-based notification of network events for a socket.
	//SOCKET_ERRORʧ�� (�𶨵Ĵ���, �ĸ���Ϣ֪ͨ, ʲô�����¼�����֪ͨ)
	int WSAAsyncSelect(HWND hWnd, u_int wMsg, long lEvent = FD_ACCEPT|
												FD_READ|FD_WRITE|FD_CONNECT|FD_CLOSE);	

	//����     (lpDestIp: Ŀ��IP����nDestPort��Ŀ��˿�)
BOOL Connect(u_long uDestIp,u_short nDestPort);

};


#pragma pack (push)
#pragma pack (1)


struct PROTOCOL_HEAD
{

	// ����IP�ײ� 
	typedef struct ip_hdr 
	{ 
		unsigned char h_verlen;				// 4λ�ײ�����,4λIP�汾�� 
		unsigned char tos;					// 8λ��������TOS 
		unsigned short total_len;			// 16λ�ܳ��ȣ��ֽڣ� 
		unsigned short ident;				// 16λ��ʶ 
		unsigned short frag_and_flags;		// 3λ��־λ 
		unsigned char ttl;					// 8λ����ʱ�� TTL 
		unsigned char proto;				// 8λЭ�� (TCP, UDP ������) 
		unsigned short checksum;			// 16λIP�ײ�У��� 
		unsigned int sourceIP;				// 32λԴIP��ַ 
		unsigned int destIP;				// 32λĿ��IP��ַ 
	}IPHEADER,*LPIPHEADER; 
	

	// ����ICMP�ײ�
	typedef struct icmp_hdr
	{
		unsigned char  i_type;           // ����
		unsigned char  i_code;           // ����
		unsigned short i_cksum;          // У����
		//---------�����û�ʹ���ֶ�-------------------------
		unsigned short i_id;             // �Ǳ�׼��ICMP�ײ�  
		unsigned short i_seq;
		unsigned long  timestamp;
	}ICMPHEADER,*LPICMPHEADER;
	
	// ����UDP�ײ�
	// The UDP packet is lick this. Took from RFC768.
	//                  0      7 8     15 16    23 24    31  
	//                 +--------+--------+--------+--------+ 
	//                 |     Source      |   Destination   | 
	//                 |      Port       |      Port       | 
	//                 +--------+--------+--------+--------+ 
	//                 |                 |                 | 
	//                 |     Length      |    Checksum     | 
	//                 +--------+--------+--------+--------+ 
	//                 |                                     
	//                 |          data octets ...            
	//                 +---------------- ...       
	
	typedef struct udp_hdr  // 8 Bytes
	{
		unsigned short uh_sport;         
		unsigned short uh_dport;
		unsigned short uh_len;
		unsigned short uh_sum;
	} UDPHEADER,*LPUDPHEADER;

	/*UDP/TCP��ͷ�е�У��͵ļ���Ƚϸ��ӵģ�Ҫ�õ� UDP/TCPα�ײ�����Ҫ���α�ײ������ֶΣ�
	Ȼ���ٽ�UDP/TCP��ͷ�Ժ󣨰�����ͷ�������ݸ��ӵ�α�ײ��ĺ��棬
	(��PSDHEADER + UDPHEADER) 
	�ٶ�α�ײ�ʹ������У��ͼ��㣬���õ���ֵ����UDP/TCP��ͷ���ֵ�У���,α�ײ����������µĽṹ���ʾ��*/

	// ����TCP/UDPα�ײ� 
	typedef struct tsd_hdr{

		ULONG sourceip;    //ԴIP��ַ
		ULONG destip;      //Ŀ��IP��ַ
		BYTE mbz;           //�ÿ�(0)
		BYTE ptcl;          // Э������ UDP��Э������Ϊ17��TCPΪ6 
		USHORT plen;        //TCP/UDP���ݰ��ĳ���(����TCP/UDP��ͷ�������ݰ������ĳ��� ��λ:�ֽ�)

	}PSDHEADER,*LPPSDHEADER; 



	/*
	һ��ipv4��tcp��udp��У���  
		IP/ICMP/IGMP/TCP/UDP��Э���У����㷨������ͬ�ģ����õĶ��ǽ���������Ϊ16λ�����������ظ����Ӽ��㡣
		Ϊ�˼������ͣ����ȰѼ�����ֶ���Ϊ0��Ȼ�󣬶���Ч���ݷ�Χ����ÿ��16λ���ж����Ʒ�����ͣ�������ڼ�����ֶ��У�
		
		������ݳ���Ϊ������һ�ֽ�0�����յ����ݺ�ͬ������Ч���ݷ�Χ��ÿ��16λ�����ж����Ʒ������͡�
		���ڽ��շ��ڼ�������а����˷��ͷ������ײ��еļ���ͣ���ˣ�����ײ��ڴ��������û�з����κβ��
		��ô���շ�����Ľ��Ӧ��Ϊȫ0��ȫ1(���忴ʵ����,����һ��) ������������ȫ0��ȫ1����ô��ʾ���ݴ���

		�����㷨ԭ���Ǹ���,������Ҫ!!!
		����IPv4���е�У���ֻ����IPv4ͷ���֣��������ϲ�Э��ͷ��Ӧ�ò����ݣ�У����Ǳ������ġ�
		����ICMP/IGMPУ��ͼ��㷶ΧΪ��ICMP/IGMP��ʼ�����ݽ�����������IPͷ���֣�У����Ǳ������ġ�
		����TCP��UDP��ļ���ͼ����е����⣬����������ݷ�Χ���˰���TCP/UDPͷ��ʼ�����ݽ����⣬��Ҫ����һ��IPαͷ���֣�
		��ναͷ��ֻ��12�ֽ����ݣ�����Դ��ַ(4�ֽ�)��Ŀ�ĵ�ַ(4�ֽ�)��Э��(2�ֽڣ���һ�ֽڲ�0)��TCP/UDP����(2�ֽ�)��
		TCP��У����Ǳ���ģ���UDP��У����ǿ�ѡ�ģ����UDP��У����ֶ�Ϊ0����ʾ������У��ͼ��㣬
		��˶���UDPЭ�����ݵ��޸ĺ���͵���Ļ�ֱ�ӽ�У�����Ϊ0 �Ϳ����ˡ�
		�ο�:http://hi.baidu.com/ffsdaihuiping/blog/item/7a65a6c35dc84345b219a839.html
	*/
	static USHORT CheckSum(USHORT *buffer, int size);	

};



typedef struct _DATAPACK
{
	enum{
		//�ϴβ���������˾ܾ� �ͻ��˲������		����˲������
		SC_OPERATE_REJECT_,	 CS_OPERATE_FINISH_, SC_OPERATE_FINISH_,

		//��¼��
		CS_LOGIN_, SC_LOGIN_WELCOME_, CS_LOGOUT_,

		//��ȡ�û���Ϣ		//�����û���Ϣ
		CS_USER_INFO_GET_, SC_USER_INFO_POST_,

		//�����ļ�
		CS_FILE_GET_, SC_FILE_DATA_POST_, CS_FILE_DATA_GET_,

		//������󳤶�					������������󳤶� = ������󳤶� - ͷ������
		PCAK_TOTAL_MAX_SIZE_ = 0X2000, PACK_DATA_SIZE_ = PCAK_TOTAL_MAX_SIZE_ - 5*4};

	//---------------���ĸ�ʽ-------------
		//����ͷ��						
	struct _HEAD{								
		UINT PackLen;		//�������ܳ���,��У��Ͱ�!����
		UINT PackType;		//����������	
		UINT CurPack;		//����: ��ǰ�ǵڼ�����
		UINT TotalPacks;    //����: �ܹ��ж��ٰ�
		UINT DataLen;		//���ݳ���
	}HEAD;
		//����������
	union _DATA
	{
		//ռ�ط�
		char DataBuffer[PACK_DATA_SIZE_];

		//����״̬
		struct _SC_OPERATE_REJECT{
			char RejectInfo[MAXBYTE];			//�������ܾ��ͻ��˷��ص���Ϣ,���Դ�ӡ����
		};
		struct _CS_OPERATE_FINISH{};			//�ͻ��˸��߷�����һ��������� [cs]
		struct _SC_OPERATE_FINISH{};			//���������߿ͻ���һ��������� [sc ������ͬ]
		
		//��¼��
		struct _CS_LONGIN{				
			char UserName[MAXBYTE]; 
		};
		struct _SC_LOGIN_WELCOME{};
		struct _CS_LOGOUT{};

		//�û���Ϣ��
		struct _CS_USER_INFO_ONLINE_GET{
			enum{USERINFO_ADDR_			   = 0x0001,		//��ȡָ���û�ip��ַ��Ϣ
				 USERINFO_ONLINE_USER_NAME_= 0x0002,		//��ȡ���������û�
				 USERINFO_SHARE_FILE_	   = 0x0004,		//��ȡָ���û��Ĺ����ļ�

				 USERINFO_ALL_ = USERINFO_ADDR_ |			//��ȡ������Ϣ
								USERINFO_ONLINE_USER_NAME_ |
								USERINFO_SHARE_FILE_};					
			DWORD Flag;										//��ȡ��־	
			char UserName[MAXBYTE];							//����USERINFO_ADDR_ �� USERINFO_SHARE_FILE_ ��Ч
		};	
											
		struct _SC_USER_INFO_ONLINE_POST{	
			union{

				struct _USER_NAMES{							//��������ʱָ�� USERINFO_NAME_ ��Ч
					UINT nUserNums;							//���� ���û�����		ע��:nUserNums* MAXBYTE <= PACK_DATA_SIZE_
					char Name[1][MAXBYTE];					//�û���		
				};
				sockaddr_in Addr;							//��������ʱָ�� USERINFO_ADDR_ ��Ч

				struct _SHARE_FILES{						//��������ʱָ�� USERINFO_SHARE_FILE_ ��Ч
					UINT nShareFileNums;					//���� ���û������ļ�����  ע��:nShareFileNums* MAXBYTE <= PACK_DATA_SIZE_
					char FileName[1][MAXBYTE];				//ÿ���ļ����̶�����:MAXBYTE
				};
			};			
		};

		//�ļ���
		struct _CS_FILE_GET_{					//��Ҫ���ĸ��û���ȡʲô�ļ� 
			char UserName[MAXBYTE];				// �û�	"SERVER"��ʾ�ӷ������ϻ�ȡ
			char FileName[MAXBYTE];				// �ļ���
		};
		struct _CS_FILE_DATA_GET{
			UINT GetNextPack;					//�ͻ�����Ҫ��ȡ����һ����������,��ǰ�����ʾ�Ѿ�ȷ���յ�!
		};
		struct _SC_FILE_DATA_POST{				//��������ͻ��˷� �ļ����� ��
			char FileData[PACK_DATA_SIZE_];		//�ļ���������
		};
	}DATA;
	//------------------------------------




}DATAPACK,*LPDATAPACK;
#pragma pack (pop)























#endif // !defined(AFX_MYSOCKET_H__AAFE97E6_1447_49BF_B051_FA8C05640E35__INCLUDED_)

/*
�����ֽ�˳���������ֽ�˳���ת������  

	ntohs =net to host short int 16λ

	htons=host to net short int 16λ

	ntohl =net to host long int 32λ

	htonl=host to net   long int   32λ
	
	//�ַ���IPת�������ֽ�IP
	 sockaddr_in.sinv_addr.s_addr =   unsigned long 
									 inet_addr("132.241.5.10");

	//�������ֽ�˳ ת�� �ַ���IP
	printf("%s",inet_ntoa(ina.sin_addr));

*/


// 	typedef void (_DATAPACK::* LPDOFUN)();
// 	LPDOFUN lpFuns[4];
// 	
// 	_DATAPACK()
// 	{
// 		lpFuns[CS_CMD_] = DoCmd;
// 	}
// 	
// 	void Do()
// 	{
// 		(this->*lpFuns[HEAD.PackType])();
// 	}
// 	
// 	void DoCmd()
// 	{
// 		AfxMessageBox("����Cmd��");
// 	}

//����  sock����			 ���ݳ���        ������         Ҫ���͵�����               Ŀ���ַ��Ϣ
// 	int SendTo(IN MySocket& obj, IN UINT nDataLen, IN DWORD dPackType, IN CHAR* lpDataBuf, sockaddr_in* lpTo)
// 	{	
// 		if (nDataLen > DATABUFF_MAX)
// 		{
// 			AfxMessageBox("�������ݹ���");
// 			return 0;
// 		}
// 		m_nDataLen = nDataLen;
// 		m_PackType = dPackType;
// 		memcpy(m_DataBuf, lpDataBuf, m_nDataLen);
// 		
// 		m_PackLen = 4 + 4 + 4 + m_nDataLen; //������ ���ݳ���  ���� ������    
// 		
// 		int nRet = obj.SendTo((char *)&m_PackLen, m_PackLen,lpTo);
// 		if (0 == nRet || SOCKET_ERROR == nRet)
// 			return nRet;
// 
// 		return m_PackLen;
// 	}
