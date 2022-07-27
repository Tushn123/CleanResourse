#include <afxsock.h>
#include <afxtempl.h>
#include "imagehlp.h"
#include "HttpService.h"
#include "Logger.h"

#pragma comment(lib,"imagehlp.lib")  
#pragma comment(lib,"iphlpapi.lib")

#define RIGISTER_KEY _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths")

#define GLOG_NO_ABBREVIATED_SEVERITIES

//使用 Windows 的 HeapAlloc 函数进行动态内存分配
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


//获取本机MAC地址
bool GetMacByGetAdaptersInfo(std::string& macOUT);
DWORD WINAPI SetstrValueToRegedit(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwSize, LPCTSTR strValue);
//读注册表
CString WINAPI GetstrValueFromRegedit(LPCTSTR lpSubKey, LPCTSTR lpValueName);

//获取挂载信息
int WINAPI GetMountedInfo(wchar_t * pMountedPath, int& iPercent, int& iFreespace);

BOOL DoRemoveDirectory(CString chrDirName);
BOOL DoRemoveDirectory1(CString chrDirName);
bool  CheckFolderExist(CString strPath);

//删除文件夹及其文件
int WINAPI RemoveFolder(WCHAR* dir);

//分类写日志
void Log(CString strType, CString strMsg, CString strLogType);//写日志(WS信息以及WEBAPI信息)	 

//获取EXE路径
CString GetExePath();
