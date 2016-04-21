// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include "OPini.h"

#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")

#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")

#include <iostream>
#include <vector>
#include <map>
using namespace std;

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define PACKET_TRANS_LEN	8192*10
#define PACKET_INFO_LEN		2048
#define MAX_IP_LEN			32

#define PROXY_BASE_PORT		30000

#pragma warning(disable:4305)
#pragma warning(disable:4309)
#pragma warning(disable:4996)


//
// �ͻ���A  --              --  
// �ͻ���B  --  ���������  --  ��ʵ����IP/Port
// �ͻ���C  --				--
//

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//
// �ͻ�����Ϣ
//
typedef struct tagGATEWAYCLIENT
{
	char			m_szSrcIP[MAX_IP_LEN];// ԴIP
	USHORT			m_nSrcPort;			// Դ�˿�
	SOCKADDR_IN		m_addrfrom;			// Դ��ַ

	USHORT			m_nBindPort;		// �󶨶˿�
	SOCKADDR_IN		m_addrto;			// �����ַ
	SOCKET			m_socketProxy;		// ����socket
	HANDLE			m_hThreadRecv;		// ��������߳�
	DWORD			m_nRecvTID;			// ��������߳�ID

	DWORD			m_nTickSend;		// ��������
	DWORD			m_nTickRecv;		// ��������
	DWORD			m_nSizeRecv;		// �ۼƷ���
	DWORD			m_nSizeSend;		// �ۼƽ���

	DWORD			m_nIndex;			// ���
	char			*m_pszRecvData;		// ���ջ���

	tagGATEWAYCLIENT::tagGATEWAYCLIENT()
	{
		Init();
	}
	tagGATEWAYCLIENT::~tagGATEWAYCLIENT()
	{
		if(m_pszRecvData != NULL)
		{
			GlobalFree(m_pszRecvData);
			m_pszRecvData = NULL;
		}
	}
	void Init()
	{
		memset(m_szSrcIP,0x00,MAX_IP_LEN);
		m_nSrcPort = 0;
		ZeroMemory(&m_addrfrom,sizeof(SOCKADDR_IN));
		ZeroMemory(&m_addrto,sizeof(SOCKADDR_IN));
		m_socketProxy = INVALID_SOCKET;
		m_hThreadRecv = INVALID_HANDLE_VALUE;
		m_nRecvTID = 0;
		m_nIndex   = 0;
		m_nBindPort = 0;
		m_nTickSend = m_nTickRecv = 0;
		m_nSizeRecv = m_nSizeSend = 0;
		m_pszRecvData = NULL;
	}
}GATEWAYCLIENT;
#pragma pack()

// client list
typedef std::vector<GATEWAYCLIENT *>			GateWayClientList;
typedef std::vector<GATEWAYCLIENT *>::iterator	GateWayClientPtr;

// white ip list
typedef std::map<ULONG,ULONG>					 ClientWhiteList;


//////////////////////////////////////////////////////////////////////////
// Ini file name
#define ITEM_FILE_CFG_NAME	"QQ277460272.ini"
#define ITEM_FILE_IP_NAME	"whitelst.txt"

#define ITEM_ADVANCED_NODE	"Advance"
#define ITEM_SLEEP_UI		"SleepUI"
#define ITEM_SLEEP_ACTIVE	"SleepActive"
#define ITEM_MAX_ONLINE_SEC	"MaxOnLineSec"

// Valve info
#define ITEM_VALVE_NODE		"Valve"
#define ITEM_LISTEN_PORT	"ProxyPort"
#define ITEM_REAL_IP		"GameIP"
#define ITEM_REAL_PORT		"GamePort"

extern int			MTGetTransSize(DWORD nSize,char *pszTemp,int nLen);
extern void			MTInitCFG();
extern BOOL			MTProxyRemoveClient(GATEWAYCLIENT *pClient/* = NULL*/);
extern DWORD WINAPI ThreadListen(LPVOID lParam);
extern DWORD WINAPI MTThreadProxyActive(LPVOID lParam);

extern void			MTLockGateStop();
extern void			MTLockGateStart();
extern void			MTLockWhiteStart();
extern void			MTLockWhiteStop();
extern BOOL			MTInWhiteList(ULONG nAddr);
extern BOOL			MTLoadWhiteList();
extern void			MTPrintHelp();

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
