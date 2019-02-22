#include <Ras.h>
#include <Raserror.h>

typedef struct VPNPARAMS
{
    CHAR	szDescription[ RAS_MaxEntryName + 1 ];
    CHAR	szServer[ RAS_MaxPhoneNumber + 1 ];
    CHAR	szUserName[ UNLEN + 1 ];
	CHAR	szPassword[ PWLEN + 1 ];
    CHAR	szDomain[ DNLEN + 1 ];
	HANDLE	hTerminalEvent;
	CDialog* lpMain;
};

BOOL ConenectVPN(LPVOID lparam);
void WINAPI RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);
BOOL CreateRasEntry(const char *pszEntryName, const char *pszServerName, const char *pszUserName,
					const char *pszPassWord);
void OutputString( char *lpFmt, ... );
LPCTSTR GetConnStatus();

BOOL MyPing(u_long DestHost);
//�ڲ�����3048,��Ҫ�������� 
void MyShowDbg(char* lpszFormat, ...);

//��ȡԭʼ����
BOOL GetDefaultGetway();
//��ȡvpn����
BOOL GetVPNGetway();

// ��ԭ·��
BOOL RepairDefaultGetway();
BOOL AddRouteItem(char* lpszIP);
BOOL DelRouteItem(char* lpszIP);
//Ĭ������
extern MIB_IPFORWARDROW gDefaultGetwayItem;
//vpn����
extern MIB_IPFORWARDROW gVPNGetwayItem;

extern BOOL g_bIsAppExit;//����Ҫ�˳�
extern HANDLE g_TerminalEvent;
extern int gItem;//��ǰ�������ӵ���



