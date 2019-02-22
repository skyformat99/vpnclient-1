//
//// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
//// ���Ǿ���ʹ�õ��������ĵ�
//// �ض�����Ŀ�İ����ļ�
//
//#pragma once
//
//#ifndef _SECURE_ATL
//#define _SECURE_ATL 1
//#endif
//
//#ifndef VC_EXTRALEAN
//#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
//#endif
//
//#include "targetver.h"
//
//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��
//
//// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
//#define _AFX_ALL_WARNINGS
//
//#include <afxwin.h>         // MFC ��������ͱ�׼���
//#include <afxext.h>         // MFC ��չ
//
//
//#include <afxdisp.h>        // MFC �Զ�����
//
//
//
//#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
//#endif
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
//#endif // _AFX_NO_AFXCMN_SUPPORT
//
//#include "SkinH.h"
//#pragma comment(lib,"SkinH.lib")
//
//#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#if !defined(AFX_STDAFX_H__DB0917CA_805B_4295_AE82_843E03BFC367__INCLUDED_)
#define AFX_STDAFX_H__DB0917CA_805B_4295_AE82_843E03BFC367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifndef _WIN32_IE // ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0500 //Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#define WINVER 0x0501
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <ras.h>
#pragma comment(lib, "rasapi32.lib")

///////////////����Ƥ��///////////////
#include "SkinH.h"
#pragma comment(lib,"SkinH.lib")
////////////////END//////////////////

//////////////////////////////////////////////////////////////////////////////////
//���Ӵ���

#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#ifndef _DEBUG
#pragma comment (lib,"../Lib/LibCurl_X64.lib")
#pragma comment (lib,"../Lib/LibJson_X64.lib")
#else
#pragma comment (lib,"../Lib/LibCurl_X64D.lib")
#pragma comment (lib,"../Lib/LibJson_X64D.lib")
#endif  // _DEBUG
#else
#ifndef _DEBUG
#pragma comment (lib,"../Lib/LibCurl.lib")
#pragma comment (lib,"../Lib/LibJson.lib")
#else
#pragma comment (lib,"../Lib/LibCurlD.lib")
#pragma comment (lib,"../Lib/LibJsonD.lib")
#endif
#endif

#define LUAFORMAT_UTF8
#include "MD5.h"
#include "json.h"
#include "curl.h"
#include "Transcoding.h"

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DB0917CA_805B_4295_AE82_843E03BFC367__INCLUDED_)

