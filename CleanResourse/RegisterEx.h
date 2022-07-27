////////////////////////////////////////////////////////////////////////////////////
//
#define _CRegisterEx_H_
//#if !defined(_CRegisterEx_H_)


#pragma once

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

class __declspec(dllexport) CRegisterEx
{
public:
	CRegisterEx(void);
	virtual ~CRegisterEx(void);

	DWORD WINAPI GetRegValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName,DWORD *pdwType,DWORD *pdwSize,PVOID *ppValue,bool bNew = true);

	DWORD WINAPI SetRegValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName,DWORD dwType,DWORD dwSize,PVOID pValue);

	DWORD WINAPI RegDeleteKeyValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName);

	DWORD WINAPI RegReplaceKeyValue(HKEY hKey,LPCTSTR lpSubKeyNew,LPCTSTR lpSubKeyOld);

	DWORD WINAPI RegDeleteAllKeyValue(HKEY hKey,LPCTSTR lpSubKey);

	DWORD WINAPI RegDeleteAllSubKey(HKEY hKey,LPCTSTR lpSubKey);

	bool GetRegDwordValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName, DWORD &dwInitValue);

	bool GetRegSzValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName, CString &csInitValue);

private:
};

//#endif // !defined(_CRegisterEx_H_)