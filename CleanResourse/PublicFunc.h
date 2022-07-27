#include <afxsock.h>
#include <afxtempl.h>
#include "imagehlp.h"
#include "HttpService.h"
#include "Logger.h"

#pragma comment(lib,"imagehlp.lib")  
#pragma comment(lib,"iphlpapi.lib")

#define RIGISTER_KEY _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths")

#define GLOG_NO_ABBREVIATED_SEVERITIES

//ʹ�� Windows �� HeapAlloc �������ж�̬�ڴ����
#define myheapalloc(x) (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x))
#define myheapfree(x) (HeapFree(GetProcessHeap(), 0, x))
typedef BOOL (WINAPI *SetSecurityDescriptorControlFnPtr)(
	IN PSECURITY_DESCRIPTOR pSecurityDescriptor,
	IN SECURITY_DESCRIPTOR_CONTROL ControlBitsOfInterest,
	IN SECURITY_DESCRIPTOR_CONTROL ControlBitsToSet);
typedef BOOL (WINAPI *AddAccessAllowedAceExFnPtr)(
	PACL pAcl,
	DWORD dwAceRevision,
	DWORD AceFlags,
	DWORD AccessMask,
	PSID pSid
	);


//��ȡ����MAC��ַ
bool GetMacByGetAdaptersInfo(std::string& macOUT);
DWORD WINAPI SetstrValueToRegedit(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwSize, LPCTSTR strValue);
//��ע���
CString WINAPI GetstrValueFromRegedit(LPCTSTR lpSubKey, LPCTSTR lpValueName);

//��ȡ������Ϣ
int WINAPI GetMountedInfo(wchar_t * pMountedPath, int& iPercent, int& iFreespace);

BOOL DoRemoveDirectory(CString chrDirName);
BOOL DoRemoveDirectory1(CString chrDirName);
bool  CheckFolderExist(CString strPath);

//ɾ���ļ��м����ļ�
int WINAPI RemoveFolder(WCHAR* dir);

//����д��־
void Log(CString strType, CString strMsg, CString strLogType);//д��־(WS��Ϣ�Լ�WEBAPI��Ϣ)	 

//��ȡEXE·��
CString GetExePath();
