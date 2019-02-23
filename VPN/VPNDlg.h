
// vpnDlg.h : ͷ�ļ�
//
#if !defined(AFX_VPNCLIENTDLG_H__E3DF5BD6_0E0A_4409_A50D_B1025AECCF43__INCLUDED_)
#define AFX_VPNCLIENTDLG_H__E3DF5BD6_0E0A_4409_A50D_B1025AECCF43__INCLUDED_

#include "TrayIcon.h"
#include "afxwin.h"
#include "Util.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class VpnInfo
{
public:
	int  id;
	char ip[50];
	char location[256];
	char user[50];
	char pass[50];
	char remark[256];
};

// CvpnDlg �Ի���
class CvpnDlg : public CDialogEx
{
// ����
public:
	CvpnDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VPN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	CTrayIcon m_TrayIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ͼƬ��ַ
	CStatic m_ctrlpic;
	void ReadConfig(CString filepath);
	void SaveConfig();
	// ��ʼ������
	void InitData(void);
	CString GetPosition(char* lpIPAddr);
	// ��Ӽ�¼
	int Add(LPSTR lpHostName, LPSTR lpUser, LPSTR lpPass, LPSTR lpRemark);
	// ɾ����¼
	int Del(int rowid);
	//��������
	bool VpnWebConfig();

	afx_msg void OnBnClickedMfclink1();
	afx_msg void OnBnClickedButton1();
	CComboBox m_ip;
	CString m_username;
	CString m_password;
	TCHAR	m_szConfigFile[MAX_PATH];
	afx_msg void OnBnClickedButton2();
	// ��ס����
	CButton m_remember;
	CString configFile;
protected:
	afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
public:
	// ���õ�ǰ״̬
	void SetStatus(CString StatusStr);
	afx_msg void OnCbnSelchangeCombo();
};

#endif // !defined(AFX_VPNCLIENTDLG_H__E3DF5BD6_0E0A_4409_A50D_B1025AECCF43__INCLUDED_)
