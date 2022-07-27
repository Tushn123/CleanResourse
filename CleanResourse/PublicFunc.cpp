#include "stdafx.h"
#include "PublicFunc.h"
#include <io.h>
#include <fstream>
#include <Psapi.h>
#include "RegisterEx.h"

#pragma comment(lib,"Psapi.lib")


//获取本机MAC信息
bool GetMacByGetAdaptersInfo(std::string& macOUT)
{
	bool ret = false;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	if(pAdapterInfo == NULL)
		return false;
	// Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
	if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
		if (pAdapterInfo == NULL) 
			return false;
	}

	if(GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
	{
		for(PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
		{
			// 确保是以太网
			if(pAdapter->Type != MIB_IF_TYPE_ETHERNET)
				continue;
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if(pAdapter->AddressLength != 6)
				continue;
			//确保是物理网卡
			if (strstr(pAdapter->Description, "PCI") <= 0)
				continue;
			char acMAC[32];
			sprintf(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
				int (pAdapter->Address[0]),
				int (pAdapter->Address[1]),
				int (pAdapter->Address[2]),
				int (pAdapter->Address[3]),
				int (pAdapter->Address[4]),
				int (pAdapter->Address[5]));
			macOUT = acMAC;
			ret = true;
			break;
		}
	}

	free(pAdapterInfo);
	return ret;
}
//	函数功能：获取挂载信息
int WINAPI GetMountedInfo(wchar_t * pMountedPath, int& iPercent, int& iFreespace)
{
	DWORD val;
	BOOL bFlag;
	TCHAR outPathBuf[MAX_PATH];

	unsigned __int64 i64FreeBytesToCaller;
	unsigned __int64 i64TotalBytes;
	unsigned __int64 i64FreeBytes;
	float total = 0.0f;
	float free = 0.0f;

	if(pMountedPath == NULL)
	{
		return 2; //目录错误
	}

	// 	int dwNum = MultiByteToWideChar (CP_ACP, 0, pMountedPath, -1, NULL, 0);
	// 	wchar_t *pwText;
	// 	pwText = new wchar_t[dwNum];
	// 	if(!pwText)
	// 	{
	// 		return 4; //内存不足
	// 	}
	// 
	// 	MultiByteToWideChar (CP_ACP, 0, pMountedPath, -1, pwText, dwNum);

	bFlag = GetVolumeNameForVolumeMountPoint(
		pMountedPath,    //输入挂载点或目录
		outPathBuf,          //输出卷名
		MAX_PATH);
	//delete []pwText;

	if(bFlag == TRUE)
	{
		bFlag = GetDiskFreeSpaceExW (
			outPathBuf,
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);
		if(bFlag == TRUE)
		{
			total = (float)i64TotalBytes/1024/1024;
			free = (float)i64FreeBytesToCaller/1024/1024;
			iPercent = (int)(100-free / total * 100);
			iFreespace = (int)free;
		}
		else
		{
			OutputDebugString(TEXT("GetMountedInfo--GetDiskFreeSpaceExW False"));
			val = GetLastError();
			if(val == 3)
			{
				return 3; //挂载的分区已丢失
			}
		}
	}
	else
	{
		OutputDebugString(TEXT("GetMountedInfo--GetVolumeNameForVolumeMountPoint False"));
		val = GetLastError();
		if(val == 4390)
		{
			return 1; //当前目录没有挂载
		}
		else if(val == 123 || val == 2)
		{
			return 2; //目录错误
		}
	}
	return 0;
}
bool CheckFolderExist(CString strPath)
{
	WIN32_FIND_DATA  wfd;
	bool rValue = false;
	HANDLE hFind = FindFirstFile(strPath + L"\\*.*", &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;
	}
	FindClose(hFind);
	return rValue;
}

BOOL DoRemoveDirectory1(CString chrDirName)
{
	CString strDirName = chrDirName;

	BOOL result;
	HANDLE Handle;
	WIN32_FIND_DATA fData;
	CString strTemp;
	DWORD errorcode;

	Handle = FindFirstFile(strDirName + L"\\*.*", &fData);

	if (Handle == INVALID_HANDLE_VALUE)
	{
		FindClose(Handle);
		return FALSE;
	}

	do {
		errorcode = GetLastError();
		if((fData.cFileName[0] == '.')||(fData.cFileName[0] == '..'))
		{
			continue;
		}
		if (fData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strDirName[strDirName.GetLength() - 1] != '\\')
			{
				TCHAR chA[MAX_PATH];
				CString strA = strDirName + '\\' + fData.cFileName;
				lstrcpy(chA, strA);
				DoRemoveDirectory(chA);
			}
			else
			{
				TCHAR chB[MAX_PATH];
				CString strB = strDirName + fData.cFileName;
				lstrcpy(chB, strB);
				DoRemoveDirectory(chB);

			}
			strTemp = strDirName + L"\\" + fData.cFileName;
			SetFileAttributes(strTemp, ~FILE_ATTRIBUTE_READONLY);
			if (!RemoveDirectory(strTemp))
			{
				result = FALSE;
			}
			else
			{
				result = TRUE;
			}
		}
		else
		{
			strTemp = strDirName + L"\\" + fData.cFileName;
			BOOL bl = SetFileAttributes(strTemp, ~FILE_ATTRIBUTE_READONLY);
			if (!DeleteFile(strTemp))
			{
				result = FALSE;
			}
			else
			{
				result = TRUE;
			}
		}
	} while (FindNextFile(Handle, &fData));

	errorcode = GetLastError();
	if (errorcode == ERROR_NO_MORE_FILES)//空目录
	{
		::RemoveDirectory(strDirName);
		result = TRUE;
	}
	if (Handle)
	{
		FindClose(Handle);
	}
	return result;
}
BOOL DoRemoveDirectory(CString chrDirName)
{
	CString strDirName = chrDirName;

	BOOL result;
	HANDLE Handle;
	WIN32_FIND_DATA fData;
	CString strTemp;
	DWORD errorcode;

	Handle = FindFirstFile(strDirName + L"\\*.*", &fData);

	if (Handle == INVALID_HANDLE_VALUE)
	{
		FindClose(Handle);
		return FALSE;
	}

	do {
		errorcode = GetLastError();

		if (fData.cFileName[0] == '.')
		{
			continue;
		}
		if (fData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strDirName[strDirName.GetLength() - 1] != '\\')
			{
				TCHAR chA[MAX_PATH];
				CString strA = strDirName + '\\' + fData.cFileName;
				lstrcpy(chA, strA);
				DoRemoveDirectory(chA);
			}
			else
			{
				TCHAR chB[MAX_PATH];
				CString strB = strDirName + fData.cFileName;
				lstrcpy(chB, strB);
				DoRemoveDirectory(chB);

			}
			strTemp = strDirName + L"\\" + fData.cFileName;
			SetFileAttributes(strTemp, ~FILE_ATTRIBUTE_READONLY);
			if (!RemoveDirectory(strTemp))
			{
				result = FALSE;
			}
			else
			{
				result = TRUE;
			}
		}
		else
		{
			strTemp = strDirName + L"\\" + fData.cFileName;
			BOOL bl = SetFileAttributes(strTemp, ~FILE_ATTRIBUTE_READONLY);
			if (!DeleteFile(strTemp))
			{
				result = FALSE;
			}
			else
			{
				result = TRUE;
			}
		}
	} while (FindNextFile(Handle, &fData));

	errorcode = GetLastError();
	if (errorcode == ERROR_NO_MORE_FILES)//空目录
	{
		::RemoveDirectory(strDirName);
		result = TRUE;
	}
	if (Handle)
	{
		FindClose(Handle);
	}
	return result;
}
//BOOL DoRemoveDirectory(CString chrDirName)
//{
//	CString strDirName = chrDirName;
//
//	TCHAR tmp[10];
//	BOOL result;
//	HANDLE Handle;
//	WIN32_FIND_DATA fData;
//	CString strTemp;
//	DWORD errorcode;
//
//	SYSTEMTIME systime;
//	GetLocalTime(&systime);
//	int datIndex = chrDirName.ReverseFind('\\');
//	CString dayDir = chrDirName.Mid(datIndex + 1);
//	int index = dayDir.GetLength();
//	CString day = dayDir.Left(index - 1);
//	WORD i_day = _ttoi(day);
//
//	int icont = chrDirName.Find(L'2020年12月');
//	CString mouth = chrDirName.Mid(icont + 5);
//	mouth = mouth.Left(2);
//	if (mouth == L"12")
//	{
//		return FALSE;
//	}
//
//	Handle = FindFirstFile(strDirName + L"\\*.*", &fData);
//
//	if (Handle == INVALID_HANDLE_VALUE)
//	{
//		return FALSE;
//	}
//
//	do {
//		errorcode = GetLastError();
//
//		if (fData.cFileName[0] == '.')
//		{
//			continue;
//		}
//		if (fData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
//		{
//			if (strDirName[strDirName.GetLength() - 1] != '\\')
//			{
//				TCHAR chA[MAX_PATH];
//				CString strA = strDirName + '\\' + fData.cFileName;
//				wcsncpy_s(tmp, fData.cFileName, 3);
//				if ((!wcscmp(tmp, TEXT("02时")) || !wcscmp(tmp, TEXT("03时"))) && (i_day == systime.wDay)) continue;
//				lstrcpy(chA, strA);
//				DoRemoveDirectory(chA);
//			}
//			else
//			{
//				TCHAR chB[MAX_PATH];
//				CString strB = strDirName + fData.cFileName;
//				wcsncpy_s(tmp, fData.cFileName, 3);
//				if ((!wcscmp(tmp, TEXT("02时")) || !wcscmp(tmp, TEXT("03时"))) && (i_day == systime.wDay)) continue;
//				lstrcpy(chB, strB);
//				DoRemoveDirectory(chB);
//
//			}
//			strTemp = strDirName + L"\\" + fData.cFileName;
//			SetFileAttributes(strTemp, ~FILE_ATTRIBUTE_NORMAL);
//			if (!::RemoveDirectory(strTemp))
//			{
//				result = FALSE;
//			}
//			else
//			{
//				result = TRUE;
//			}
//		}
//		else
//		{
//			strTemp = strDirName + L"\\" + fData.cFileName;
//			BOOL bl = SetFileAttributes(strTemp, ~FILE_ATTRIBUTE_NORMAL);
//			if (!DeleteFile(strTemp))
//			{
//				result = FALSE;
//			}
//			else
//			{
//				result = TRUE;
//			}
//		}
//	} while (FindNextFile(Handle, &fData));
//
//	errorcode = GetLastError();
//	if (errorcode == ERROR_NO_MORE_FILES)//空目录
//	{
//		::RemoveDirectory(strDirName);
//		result = TRUE;
//	}
//	if (Handle)
//	{
//		FindClose(Handle);
//	}
//	return result;
//}
//写注册表
DWORD WINAPI SetstrValueToRegedit(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwSize, LPCTSTR strValue)
{
	CRegisterEx Regedit;

	CString strTemp = TEXT("");

	DWORD ret = -1;

	ret = Regedit.SetRegValue(HKEY_LOCAL_MACHINE, lpSubKey, lpValueName, REG_SZ, dwSize, (PVOID)strValue);

	return ret;
}


CString WINAPI GetstrValueFromRegedit(LPCTSTR lpSubKey, LPCTSTR lpValueName)
{
	CRegisterEx Regedit;

	CString strTemp = TEXT("");
	DWORD TypeOfKey;
	DWORD SizeOfKey;
	PVOID pValueOfKey;

	int ret;

	ret = Regedit.GetRegValue(HKEY_LOCAL_MACHINE, lpSubKey, lpValueName, &TypeOfKey, &SizeOfKey, &pValueOfKey);

	if (ret == ERROR_SUCCESS)
	{
		strTemp = (PWCHAR)pValueOfKey;
	}

	return strTemp;
}


int WINAPI RemoveFolder(WCHAR* dir)
{
	WIN32_FIND_DATAW ffd;
	WCHAR *find_string = NULL;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	find_string = (WCHAR *)malloc((wcslen(dir) + 10 + MAX_PATH) * sizeof(WCHAR));
	*find_string = 0;
	wcscpy(find_string,dir);
	wcscat(find_string,L"\\*");

	hFind = FindFirstFileW(find_string, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) 
	{
		OutputDebugString(L"FindFirstFileW failed \n");

		return 0;
	} 

	do
	{
		if(wcscmp(ffd.cFileName,L".") == 0 || 0 == wcscmp(ffd.cFileName,L".."))
			continue;

		wcscpy(find_string,dir);
		wcscat(find_string,L"\\");
		wcscat(find_string,ffd.cFileName);

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			RemoveFolder(find_string);
			RemoveDirectory(find_string);

		}
		else
		{
			DeleteFile(find_string);
		}

	}while (FindNextFileW(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		//遍历失败 ？？
	}

	FindClose(hFind);

	RemoveDirectory(dir);

	if(find_string)
		free(find_string);
	return dwError;
}

//获取EXE路径
CString GetExePath()
{
	CString strExePath;
	TCHAR szFilePath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	strExePath = szFilePath;
	return strExePath;
}

//写日志(WS信息以及WEBAPI信息)
void Log(CString strType, CString strMsg, CString strLogType )
{
	CString strPath;
	strPath= GetExePath()+L"\\Log\\";
	CreateDirectory(strPath, NULL);
	strPath.Append(L"CleanResourseLog");
	fstream _file;
	_file.open(strPath, ios::in);
	if (!_file)
	{
		CreateDirectory(strPath, NULL);//创建c:/lgftp
	}
	//增加日期
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	char date[100];
	char time[100];
	sprintf_s(date, "%d%02d%02d", st.wYear, st.wMonth, st.wDay);
	sprintf_s(time, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);

	USES_CONVERSION;
	CString strDate;
	strDate = A2T(date);

	if (strLogType == TEXT("RecordServer"))
	{
		strPath.Append(L"\\录播");
		strPath.Append(strDate);
		strPath.Append(L".log");
	}
	if (strLogType == TEXT("Conference"))
	{
		strPath.Append(L"\\会议");
		strPath.Append(strDate);
		strPath.Append(L".log");
	}
	if (strLogType == TEXT("Monitor"))
	{
		strPath.Append(L"\\监控");
		strPath.Append(strDate);
		strPath.Append(L".log");
	}
	if (strLogType == TEXT("Error"))
	{
		strPath.Append(L"\\Error");
		strPath.Append(strDate);
		strPath.Append(L".log");
	}
	if (strLogType == TEXT("RtmpServer"))
	{
		strPath.Append(L"\\资源删除");
		strPath.Append(strDate);
		strPath.Append(L".log");
	}

	CStringA strPathEx(strPath);
	const char *cPath;
	cPath = strPathEx.GetString();

	CStringA strTypeEx(strType);
	CStringA strMsgEx(strMsg);
	const char *cType;
	cType = strTypeEx.GetString();
	const char *cMsg;
	cMsg = strMsgEx.GetString();

	FILE *m_pfLogFile = NULL;
	if (NULL != m_pfLogFile)
	{
		fclose(m_pfLogFile);
	}
	errno_t err;
	if (err = fopen_s(&m_pfLogFile, cPath, "at+"))
	{
		return;
	}

	RcLogInfo rl;
	if (strlen(time) + strlen(cType) + strlen(cMsg) > sizeof(rl.m_cInfo))
	{
		return;
	}
	rl.SetLogFile(m_pfLogFile);

	sprintf_s(rl.m_cInfo, "%s %s%s\n", time, cType, cMsg);

	if (rl.WriteLogInfo(rl.m_cInfo) == 0)
	{
		fclose(m_pfLogFile);
	}
}
