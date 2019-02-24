
// vpnDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "vpn.h"
#include "vpnDlg.h"
#include "RasDial.h"
#include "afxdialogex.h"
#include "MySocket.h"
#include "sqlite3.h"
#include "CppSQLite3.h"
#include "IpFinder.h"
#pragma comment(lib,"rasapi32.lib")
#pragma comment(lib,"sqlite3.lib")
#include <fstream>
#include <iostream>
using namespace std;

#define WM_ICON_NOTIFY WM_USER+10

#if((_WIN32_WINNT >= 0x0400) || (WINVER >= 0x0500))
#define WM_MOUSEHOVER                   0x02A1
#define WM_MOUSELEAVE                   0x02A3
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define  MAX_CONFIG_VPN_NUM 200
HANDLE	g_hDisconnect[100]; // 100���¼�����
HANDLE g_hconnectThread[MAX_CONFIG_VPN_NUM];

#pragma comment(lib, "Iphlpapi.lib")
/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//Ĭ������
MIB_IPFORWARDROW gDefaultGetwayItem = { 0 };
//vpn����
MIB_IPFORWARDROW gVPNGetwayItem = { 0 };
//����IP
extern CDialog* lpMainDlg;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CvpnDlg �Ի���

CvpnDlg::CvpnDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CvpnDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CvpnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDCLOSE, m_ctrlpic);
	//  DDX_Control(pDX, IDC_CHECK1, m_remember);
	//  DDX_Control(pDX, IDC_BUTTON1, m_btnLogin);
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_ip);
	DDX_Text(pDX, IDC_EDIT2, m_username);
	DDX_Text(pDX, IDC_EDIT3, m_password);
	//  DDX_Check(pDX, IDC_CHECK1, m_remember);
	DDX_Control(pDX, IDC_CHECK1, m_remember);

}

BEGIN_MESSAGE_MAP(CvpnDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CvpnDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CvpnDlg::OnBnClickedButton2)
	ON_MESSAGE(WM_ICON_NOTIFY, &CvpnDlg::OnTrayNotification)
	ON_CBN_SELCHANGE(IDC_COMBO, &CvpnDlg::OnCbnSelchangeCombo)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CvpnDlg::OnNMClickList2)
END_MESSAGE_MAP()


// CvpnDlg ��Ϣ�������

void ctow(wchar_t* wchar, const char *str)
{
	int wcslen = ::MultiByteToWideChar(CP_ACP, NULL, str, strlen(str), NULL, 0);
	::MultiByteToWideChar(CP_ACP, NULL, str, strlen(str), wchar, wcslen);
	wchar[wcslen] = 0;
}

CListCtrl *staticlist = NULL;
CMap<int, int, VpnInfo*, VpnInfo*> infos;

BOOL CvpnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			//pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//���ؽ���Ƥ��
	SkinH_Attach();


	//��ȡ���ڷ��;
	LONG lStyle = GetWindowLong(this->m_list.m_hWnd, GWL_STYLE);
	//���ô��ڷ��;
	SetWindowLong(this->m_list.m_hWnd, GWL_STYLE, lStyle | LVS_REPORT);
	//������;����ѡ��;
	this->m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	this->m_list.InsertColumn(0, _T("���"), 0, 50, -1);
	this->m_list.InsertColumn(1, _T("IP��ַ"), 0, 130, -1);
	this->m_list.InsertColumn(2, _T("����λ��"), 0, 130, -1);
	this->m_list.InsertColumn(3, _T("�ӳ�"), 0, 50, -1);
	this->m_list.InsertColumn(4, _T("��ע"), 0, 150, -1);

	staticlist = &this->m_list;

	m_ip = _T("");
	m_username = _T("");
	m_password = _T("");

	//����˻�
	InitData();

	::GetCurrentDirectory(MAX_PATH - 1, m_szConfigFile);
	_tcscat(m_szConfigFile, TEXT("\\Parameter.ini"));

	MySocket::gWSAStartup();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CvpnDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	/*if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
	}*/
	
	if (nID == SC_MINIMIZE)
	{
		m_TrayIcon.Create(this,WM_ICON_NOTIFY,"VPNClient��������,  ˫����ͼ����ʾ������ ...", m_hIcon, IDR_MAINFRAME, TRUE); //����
		m_TrayIcon.SetTooltipText("Running ....");
		ShowWindow(SW_HIDE);
	}
	else if (nID==SC_CLOSE)
	{
		if (MessageBox("ȷ���˳���","�˳�",MB_YESNO)==IDYES)
		{
			if (NULL != g_TerminalEvent)
			{
				AfxMessageBox("��Ͽ����VPN���ٹرճ���!");
				return;
			}

			g_bIsAppExit = TRUE;

			int iItemCount = this->m_list.GetItemCount();
			VpnInfo *pInfo = NULL;
			for (int i = 0; i < iItemCount; i++)
			{			
				pInfo = (VpnInfo *)(this->m_list.GetItemData(i));
				if (pInfo != NULL)
				{
					delete pInfo;
				}
			}

			for (int i = 3; i > 0; i--)
			{
				char szText[200];
				wsprintf(szText, "����������Դ...  %d ���˳�����.", i);
				SetWindowText(szText);
				Sleep(1000);
			}

			//��������
			if (NULL != g_hDisconnect[0])
			{
				SetEvent(g_hDisconnect[0]);
				CloseHandle(g_hDisconnect[0]);
			}

			MySocket::gWSACleanup();
			CDialog::OnClose();
			//exit(-1);
		}
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CvpnDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CvpnDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CvpnDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(lParam) ==WM_LBUTTONDOWN)
	{
		ShowWindow(SW_SHOW);
		return TRUE;
	}
	else if (LOWORD(lParam)==WM_MOUSEHOVER)
	{
		MessageBox("aaa");
		m_TrayIcon.SetTooltipText(GetConnStatus());
		return TRUE;
	}
	return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

void CvpnDlg::OnBnClickedMfclink1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ע��
}


void CvpnDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int	nItem = m_list.GetSelectionMark();
	if (nItem == -1)
	{
		CString statusstr = "û���ҵ��κο��õ�VPN��������";
		OutputString(statusstr.GetBuffer(0));
		UpdateData(FALSE);
		return;
	}

	if (NULL != g_TerminalEvent)
	{
		AfxMessageBox("�Ѿ�����һ�������!");
		return;
	}
	else
	{
		if ((g_TerminalEvent = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
		{
			OutputString("CreateEvent failed with error %d\n", GetLastError());
			return;
		}

	}

	CString statusstr="���������У�";
	OutputString(statusstr.GetBuffer(0));

	OnSysCommand(SC_MINIMIZE,NULL); //��С��������
	//��½
	int isremember=m_remember.GetCheck(); //0:�������� 1:��ס����
	if (isremember==1)
	{
		//д��config.ini
		//SaveConfig();
	}
	try
	{
		g_hDisconnect[0] = CreateEvent(NULL, FALSE, FALSE, NULL);

		ResetEvent(g_hDisconnect[0]); // �����¼�����
		VPNPARAMS VPNParams;
		VPNParams.hTerminalEvent = g_hDisconnect[0];
		CString vpnname="myvpn";
		UpdateData(TRUE);
		lstrcpy(VPNParams.szServer, m_ip.GetBuffer(0));
		lstrcpy(VPNParams.szUserName, m_username.GetBuffer(0));
		lstrcpy(VPNParams.szPassword, m_password.GetBuffer(m_password.GetLength()));
		lstrcpy(VPNParams.szDescription, vpnname.GetBuffer(0));
		lstrcpy(VPNParams.szDomain, "");
		VPNParams.lpMain = this;
		UpdateData(FALSE);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConenectVPN, (LPVOID)&VPNParams, 0, NULL);
		Sleep(500); // �ȴ������������ !!!!!
	}
	catch(...)
	{

	}
}


void CvpnDlg::OnBnClickedButton2()
{
	//�Ͽ�
	SetEvent(g_hDisconnect[0]);
	CloseHandle(g_hDisconnect[0]);
	m_TrayIcon.SetTooltipText("�Ͽ�����!");
}

void CvpnDlg::ReadConfig(CString filepath)
{
	//char chHost[40], chUserName[40], chPassWord[40], chDescription[40];
	try
	{
		ifstream file;
		char line[256];
		file.open(filepath);
		if(!file)
		{
			return;
		}
		while(file.getline(line,sizeof(line)))
		{
			CString str;
			str.Format("%s",line);
			//ip��ʼ��λ��
			size_t ip= str.Find("|");
			m_ip = str.Left(ip);
			CString str1=str.Right(str.GetLength()-ip-1);  //��ȡʣ�µ��ַ���
			//username��ʼ��λ��
			size_t uname=str1.Find("|");
			m_username=str1.Left(uname);
			CString str2=str1.Right(str1.GetLength()-uname-1);
			//password��ʼ��λ��
			size_t pass=str2.Find("|");
			m_password=str2.Left(pass);
		}
	}
	catch(...)
	{

	}
}

void CvpnDlg::SaveConfig()
{

	try{
		CString	filepath = "config.ini";

		CStdioFile	file(filepath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		UpdateData(TRUE);
		CString strip;
		((CComboBox*)GetDlgItem(IDC_COMBO))->GetWindowText(strip);
		file.WriteString(strip + "|");
		file.WriteString(m_username + "|");
		file.WriteString(m_password+"|");
		file.Close();
	}
	catch(...)
	{
	
	}
}

// ��ʼ������
void CvpnDlg::InitData(void)
{
	//CString ip = "103.230.241.24";
	//CString username = "testvpn";
	//CString password = "123456";
	//CString remark = "����";
	//int result = 0;
	//result = this->Add(ip.GetBuffer(), username.GetBuffer(), password.GetBuffer(), remark.GetBuffer());
	//if (result > 0)
	//{
	//	OutputString("��ӳɹ���\n");
	//}
	//else
	//{
	//	OutputString("���ʧ�ܣ�\n");
	//}

	//result = this->Del(2);
	//if (result > 0)
	//{
	//	OutputString("ɾ���ɹ���\n");
	//}
	//else
	//{
	//	OutputString("ɾ��ʧ�ܣ�\n");
	//}

	this->m_list.DeleteAllItems();
	CppSQLite3DB db;
	db.open("VPN.db");
	CppSQLite3Query query = db.execQuery("select rowid,* from vpninfo where 1=1 order by rowid");
	int i = 0;
	while (!query.eof())
	{
		this->m_list.InsertItem(i, query.getStringField("rowid"));
		this->m_list.SetItemText(i, 1, query.getStringField("ip"));
		this->m_list.SetItemText(i, 2, GetPosition((char *)query.getStringField("ip")));
		this->m_list.SetItemText(i, 4, (char *)query.getStringField("remark"));
		VpnInfo *pVpnInfo = new VpnInfo;
		ZeroMemory(pVpnInfo, sizeof(VpnInfo));
		pVpnInfo->id = atoi(query.getStringField("rowid"));
		sprintf(pVpnInfo->ip, "%s", query.getStringField("ip"));
		sprintf(pVpnInfo->user, "%s", query.getStringField("user"));
		sprintf(pVpnInfo->pass, "%s", query.getStringField("pass"));
		this->m_list.SetItemData(i, (DWORD)pVpnInfo);
		query.nextRow();
		//Ĭ��ֵ
		if (i == 0)
		{
			//ip
			m_ip = pVpnInfo->ip;
			//user
			m_username = pVpnInfo->user;
			//password
			m_password = pVpnInfo->pass;

			SetDlgItemText(IDC_EDIT1, m_ip);
			SetDlgItemText(IDC_EDIT2, m_username);
			SetDlgItemText(IDC_EDIT3, m_password);
		}
		i++;
	}
	query.finalize();
	db.close();
}


CString CvpnDlg::GetPosition(char* lpIPAddr)
{
	CIpFinder m_ipfind;
	if (!m_ipfind.Open("qqwry.dat"))
	{
		return CString(_T("qqwry.dat������"));
	}
	std::string country, location;
	m_ipfind.GetAddressByIp(lpIPAddr, country, location);
	m_ipfind.Close();
	CString result;
	result.Format(_T("%s %s"), country.c_str(), location.c_str());
	return result;
}

// ��Ӽ�¼
int CvpnDlg::Add(LPSTR lpHostName, LPSTR lpUser, LPSTR lpPass, LPSTR lpRemark)
{
	char *sql = "insert into vpninfo (ip,user,pass,remark) values ('%s','%s','%s','%s')";
	char *lpSql = new char[4096];
	ZeroMemory(lpSql, 4096);
	sprintf(lpSql, sql, lpHostName, lpUser, lpPass, lpRemark);
	int result = 0;
	CppSQLite3DB db;
	db.open("VPN.db");
	result = db.execDML(lpSql);
	delete[]lpSql;
	db.close();
	return result;
}

// ɾ����¼
int CvpnDlg::Del(int rowid)
{
	char *lpSql = new char[4096];
	ZeroMemory(lpSql, 4096);
	sprintf(lpSql, "delete from vpninfo where rowid=%d", rowid);
	int result = 0;
	CppSQLite3DB db;
	db.open("VPN.db");
	result = db.execDML(lpSql);
	delete[]lpSql;
	db.close();
	return result;
}


#define WEBCONFIG_KEY	TEXT("http")
static int writer(char *data, size_t size, size_t nmemb, string *writeData)
{
	if (NULL == writeData)
		return 0;
	int len = (int)(size * nmemb);
	writeData->append(data, len);

	return len;
}

static BOOL GetReturnBase_Post(string strURL, const char* content, string& strtreturn)
{
	CURL *curl = curl_easy_init();

	if (NULL == curl)
		return 0;

	char errorBuffer[CURL_ERROR_SIZE];

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
	curl_easy_setopt(curl, CURLOPT_HEADER, false);
	curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, true);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10000);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strtreturn);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, &content);

	if (CURLE_OK == curl_easy_perform(curl))
	{
		curl_easy_cleanup(curl);
		return TRUE;
	}

	curl_easy_cleanup(curl);
	return FALSE;
}

static BOOL GetReturnBase_Get(string strURL, const char* content, string& strreturn)
{
	CURL *curl = curl_easy_init();

	if (NULL == curl)
		return 0;

	char errorBuffer[CURL_ERROR_SIZE];

	strURL += "?";
	strURL += content;
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
	curl_easy_setopt(curl, CURLOPT_HEADER, false);
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10000);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strreturn);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, &content);

	if (CURLE_OK == curl_easy_perform(curl))
	{
		curl_easy_cleanup(curl);
		return TRUE;
	}

	curl_easy_cleanup(curl);
	return FALSE;
}

//��ҳ��������
bool CvpnDlg::VpnWebConfig()
{
	TCHAR	szTBuffer1[MAX_PATH] = { 0 };
	TCHAR	szTBuffer2[MAX_PATH] = { 0 };
	TCHAR	szTBuffer3[MAX_PATH] = { 0 };
	GetPrivateProfileString(WEBCONFIG_KEY, TEXT("vpn"), TEXT("/VPN/riskmanage"), szTBuffer1, sizeof(szTBuffer1), m_szConfigFile);
	GetPrivateProfileString(WEBCONFIG_KEY, TEXT("weburl"), TEXT("/VPN/riskmanage"), szTBuffer2, sizeof(szTBuffer2), m_szConfigFile);
	GetPrivateProfileString(WEBCONFIG_KEY, TEXT("webkey"), TEXT("/VPN/riskmanage"), szTBuffer3, sizeof(szTBuffer3), m_szConfigFile);

	std::string strReturn;
	std::string strHttp = CTranscoding::TcharToChar(szTBuffer2);
	strHttp += CTranscoding::TcharToChar(szTBuffer1);

	//std::string strmd5 = "cyvpn2019" + strMachineID + CTranscoding::TcharToChar(szTBuffer3);

	char szMD5[128] = { 0 };
	//GetMD5Hash(szMD5, (unsigned char*)strmd5.c_str(), strmd5.length());

	char szcontent[1024];
	//_snprintf_s(szcontent, sizeof(szcontent) - 1, "deviceToken=%s&machineid=%s&sign=%s", strDeviceToken.c_str(), strMachineID.c_str(), szMD5);

	GetReturnBase_Get(strHttp.c_str(), szcontent, strReturn);

	std::string httphead = CTranscoding::LuaString_Procedure(strHttp.c_str());
	std::string httpcontent = CTranscoding::LuaString_Procedure(szcontent);
	std::string httpreturn = CTranscoding::LuaString_Procedure(strReturn.c_str());

	//TraceRequest(httphead, httpcontent, httpreturn);

	Json::Reader json;
	Json::Value root;
	json.parse(strReturn, root);

	if (root.isMember("info") && root.isMember("status"))
	{
		std::string strinfo;

		if (root["info"].isString())
			strinfo = root["info"].asString();

		std::string info = CTranscoding::LuaString_Procedure(strinfo.c_str());

		if (root["status"].isNumeric() && root["status"].asInt() != 1)
		{
			//g_pCServiceUnits->TraceWebLog(TEXT("����aliapiʧ��: �豸��:%s,������:%s, ��������:%s"),
			//	pRecv->szDeviceToken, pRecv->szMachineID, info.c_str());

			return false;
		}
		else
		{
			//g_pCServiceUnits->TraceWebLog(TEXT("����aliapi�ɹ�: �豸��:%s,������:%s, ��������:%s"),
			//	pRecv->szDeviceToken, pRecv->szMachineID, info.c_str());

			return true;

		}
	}

	return false;
}

void CvpnDlg::OnCbnSelchangeCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iPos;
	CString  strTemp;
	iPos = ((CComboBox*)GetDlgItem(IDC_COMBO))->GetCurSel();//��ǰѡ�е��С�
	((CComboBox*)GetDlgItem(IDC_COMBO))->SetCurSel(iPos);//���õ�n������Ϊ��ʾ�����ݡ�
	((CComboBox*)GetDlgItem(IDC_COMBO))->GetWindowText(strTemp);

}


void CvpnDlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (this->m_list.GetSelectedCount() < 1)
	{
		return;
	}
	POSITION pos = this->m_list.GetFirstSelectedItemPosition();
	VpnInfo *pInfo = NULL;
	while (pos)
	{
		int nIdx = -1;
		nIdx = this->m_list.GetNextSelectedItem(pos);
		if (nIdx >= 0 && nIdx < this->m_list.GetItemCount())
		{
			pInfo = (VpnInfo *)(this->m_list.GetItemData(nIdx));
			if (pInfo != NULL)
			{

				//this->m_hrascon = NULL;
				//int result = ConenectVPN("YVPN����", pInfo->user, pInfo->pass, pInfo->ip, &this->m_hrascon);
				//pInfo = NULL;
				//if (result != 0)
				//{
				//	char *temp = new char[1024];
				//	ZeroMemory(temp, 1024);
				//	sprintf(temp, "����ʧ�ܣ�������룺%d ...", result);
				//	ShowInfo(temp);
				//	delete temp;
				//}

				{
					//ip
					m_ip = pInfo->ip;
					//user
					m_username = pInfo->user;
					//password
					m_password = pInfo->pass;

					SetDlgItemText(IDC_EDIT1, m_ip);
					SetDlgItemText(IDC_EDIT2, m_username);
					SetDlgItemText(IDC_EDIT3, m_password);
				}

			}
		}
	}

	*pResult = 0;
}

void CvpnDlg::OnMenuAdd()
{
	// TODO: �ڴ���������������
	//AddItem dlg;
	//if (dlg.DoModal() == 2)
	//{
	//	return;
	//}
	//int result = 0;
	//result = this->Add(dlg.m_host.GetBuffer(), dlg.m_user.GetBuffer(), dlg.m_pass.GetBuffer(), dlg.m_remark.GetBuffer());
	//if (result > 0)
	//{
	//	ShowInfo("��ӳɹ���");
	//	InitData();
	//	return;
	//}
	//ShowInfo("���ʧ�ܣ�");
}




void CvpnDlg::OnMenuDel()
{
	// TODO: �ڴ���������������
	////this->Del(15);
	//if (this->m_list.GetSelectedCount() < 1)
	//{
	//	return;
	//}
	//if (MessageBox("�Ƿ����ִ�д˲�����", "��ʾ", MB_YESNO) == IDNO)
	//{
	//	return;
	//}
	//POSITION pos = this->m_list.GetFirstSelectedItemPosition();
	//Info *pInfo = NULL;
	//while (pos)
	//{
	//	int nIdx = -1;
	//	nIdx = this->m_list.GetNextSelectedItem(pos);
	//	if (nIdx >= 0 && nIdx < this->m_list.GetItemCount())
	//	{
	//		pInfo = (Info *)(this->m_list.GetItemData(nIdx));
	//		if (pInfo != NULL)
	//		{
	//			int result = 0;
	//			result = this->Del(pInfo->id);
	//			if (result > 0)
	//			{
	//				ShowInfo("ɾ���ɹ���");
	//				InitData();
	//			}
	//			else
	//			{
	//				ShowInfo("ɾ��ʧ�ܣ�");
	//			}
	//		}
	//	}
	//}
}

