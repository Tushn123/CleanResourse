////////////////////////////////////////////////////////////////////////////////////
// 

#include "stdafx.h"
#include <shlwapi.h>
#include "RegisterEx.h"


CRegisterEx::CRegisterEx(void)
{
}

CRegisterEx::~CRegisterEx(void)
{
}

DWORD WINAPI CRegisterEx::GetRegValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName,DWORD *pdwType,DWORD *pdwSize,PVOID *ppValue,bool bNew)
{
	DWORD ret = ERROR_SUCCESS;			//函数返回值
	HKEY hOpenKey;

	*pdwSize = 0;

	PVOID    achValue = *ppValue;
	DWORD	 cchValue = MAX_VALUE_NAME; 

	if((!bNew) && (achValue == NULL))
	{
		return 0;
	}

	ret = RegOpenKeyEx(hKey,lpSubKey,REG_OPTION_NON_VOLATILE,KEY_READ,&hOpenKey);
	if(ret == ERROR_SUCCESS)
	{
		if(bNew)
		{
			*ppValue = NULL;
			achValue = (TCHAR*)GlobalAlloc(GMEM_ZEROINIT,MAX_VALUE_NAME);
		}
		cchValue = MAX_VALUE_NAME; 	
		((BYTE *)achValue)[0] = '\0'; 
		ret = ::RegQueryValueEx(hOpenKey,lpValueName,0,pdwType,(BYTE *)achValue,&cchValue);
		if(ret == ERROR_SUCCESS)
		{
			*ppValue = 	achValue;
			*pdwSize = cchValue;
		}
		else if(bNew)
		{
			GlobalFree(achValue);
		}

		CloseHandle(hOpenKey);
	}

	return ret;
}

DWORD WINAPI CRegisterEx::SetRegValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName,DWORD dwType,DWORD dwSize,PVOID pValue)
{
	DWORD ret = ERROR_SUCCESS;			//函数返回值
	HKEY hOpenKey;
	DWORD dwCreateOROpen;

	ret = RegCreateKeyEx(hKey,lpSubKey,0,_T("Lange"),REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hOpenKey,&dwCreateOROpen);
	if(ret == ERROR_SUCCESS)
	{
		ret = ::RegSetValueEx(hOpenKey,lpValueName,0,dwType,(BYTE *)pValue,dwSize);

		CloseHandle(hOpenKey);
	}

	return ret;	
}

DWORD WINAPI CRegisterEx::RegDeleteKeyValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName)
{
	DWORD ret = ERROR_SUCCESS;			//函数返回值
	HKEY hOpenKey;

	ret = RegOpenKeyEx(hKey,lpSubKey,0,KEY_ALL_ACCESS ,&hOpenKey);
	if(ret == ERROR_SUCCESS)
	{
		ret = ::RegDeleteValue(hOpenKey,lpValueName);

		CloseHandle(hOpenKey);
	}

	return ret;	
}

DWORD WINAPI CRegisterEx::RegReplaceKeyValue(HKEY hKey,LPCTSTR lpSubKeyNew,LPCTSTR lpSubKeyOld)
{
	HKEY hOpenKeyNew,hOpenKeyOld;			//注册表键的句柄
	DWORD ret;			//函数返回值
	CString csValue;

	TCHAR    *achKey = NULL;   // buffer for subkey name
	DWORD    cbName = MAX_KEY_LENGTH;  // size of name string 
	TCHAR    *achValue = NULL;
	DWORD	 cchValue = MAX_VALUE_NAME; 
	DWORD    dwType = REG_SZ, dwRET = 0;
	int i = 0;

	ret=RegOpenKeyEx(hKey,lpSubKeyNew,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,&hOpenKeyNew);
	if(ret==ERROR_SUCCESS)
	{
		ret=RegOpenKeyEx(hKey,lpSubKeyOld,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,&hOpenKeyOld);
		if(ret==ERROR_SUCCESS)
		{
			achValue = (TCHAR*)GlobalAlloc(GMEM_ZEROINIT,MAX_VALUE_NAME);; 
			dwType = REG_SZ;
			i = 0;

			while(1)
			{
				cchValue = MAX_VALUE_NAME; 	
				achValue[0] = '\0'; 
				ret = RegEnumValue(hOpenKeyNew, i, 
					achValue, 
					&cchValue, 
					NULL, 
					NULL,
					NULL,
					NULL);
				if(ret != ERROR_SUCCESS) break;

				csValue = achValue;

				cchValue = MAX_VALUE_NAME; 	
				achValue[0] = '\0'; 
				ret = ::RegQueryValueEx(hOpenKeyNew,csValue,0,&dwType,(BYTE *)achValue,&cchValue);
				if(ret == ERROR_SUCCESS)
				{
					ret = ::RegSetValueEx(hOpenKeyOld,csValue,0,dwType,(BYTE *)achValue,cchValue);
				}

				i++;
			}

			GlobalFree(achValue);
			CloseHandle(hOpenKeyOld);
		}


		CloseHandle(hOpenKeyNew);
	}

	return 0;
}

DWORD WINAPI CRegisterEx::RegDeleteAllKeyValue(HKEY hKey,LPCTSTR lpSubKey)
{
	HKEY hOpenKey;			//注册表键的句柄
	size_t ret;			//函数返回值
	CString csValue;

	TCHAR    *achKey = NULL;   // buffer for subkey name
	DWORD    cbName = MAX_KEY_LENGTH;  // size of name string 
	TCHAR    *achValue = NULL;
	DWORD	 cchValue = MAX_VALUE_NAME; 
	DWORD    dwType = REG_SZ, dwRET = 0;
	int i = 0;

	ret=RegOpenKeyEx(hKey,lpSubKey,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,&hOpenKey);
	if(ret==ERROR_SUCCESS)
	{
		achValue = (TCHAR*)GlobalAlloc(GMEM_ZEROINIT,MAX_VALUE_NAME);; 
		dwType = REG_SZ;
		i = 0;

		while(1)
		{
			cchValue = MAX_VALUE_NAME; 	
			achValue[0] = '\0'; 
			ret = RegEnumValue(hOpenKey, i, 
				achValue, 
				&cchValue, 
				NULL, 
				NULL,
				NULL,
				NULL);
			if(ret != ERROR_SUCCESS) break;

			//csValue = achValue;
			//csValue.Trim();
			dwRET = ::RegDeleteValue(hOpenKey,achValue);
			if(dwRET == 0) continue; //删除成功

			i++;
		}

		GlobalFree(achValue);
		CloseHandle(hOpenKey);
	}

	return 0;
}

DWORD WINAPI CRegisterEx::RegDeleteAllSubKey(HKEY hKey,LPCTSTR lpSubKey)
{
	HKEY hOpenKey;			//注册表键的句柄
	size_t ret;			//函数返回值
	CString csSubKey,csValue,csKey;

	TCHAR    *achKey = NULL;   // buffer for subkey name
	DWORD    cbName = MAX_KEY_LENGTH;  // size of name string 
	TCHAR    *achValue = NULL;
	DWORD	 cchValue = MAX_VALUE_NAME; 
	DWORD    dwType = REG_SZ, dwRET = 0;;

	int i = 0;

	achKey = (TCHAR*)GlobalAlloc(GMEM_ZEROINIT,MAX_KEY_LENGTH);   	
	ret=RegOpenKeyEx(hKey,lpSubKey,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,&hOpenKey);
	if(ret==ERROR_SUCCESS)
	{		
		dwType = REG_SZ;
		i = 0;

		while(1)
		{
			cbName = MAX_KEY_LENGTH; 
			ret = RegEnumKeyEx(hOpenKey, i, achKey, &cbName, NULL, NULL, NULL, NULL);
			if(ret != ERROR_SUCCESS) break;

			csKey = achKey;

			ret = SHDeleteKey(hOpenKey,csKey);
			if(ret == ERROR_SUCCESS)
			{
				continue;
			}

			i++;
		}

		CloseHandle(hOpenKey);
	}

	return 0;
}

bool CRegisterEx::GetRegDwordValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName, DWORD &dwInitValue)
{
	size_t ret;
	PVOID    achValue = NULL;
	DWORD	 dwValueSize = MAX_VALUE_NAME; 
	DWORD    dwType = REG_SZ;

	ret = GetRegValue(hKey,
		lpSubKey,
		lpValueName,
		&dwType,&dwValueSize,&achValue);
	if(ret==ERROR_SUCCESS)
	{
		dwInitValue = ((DWORD*)achValue)[0];

		GlobalFree(achValue);

		return true;
	}

	return false;
}
//
bool CRegisterEx::GetRegSzValue(HKEY hKey,LPCTSTR lpSubKey,LPCTSTR lpValueName, CString &csInitValue)
{
	size_t ret;
	PVOID    achValue = NULL;
	DWORD	 dwValueSize = MAX_VALUE_NAME; 
	DWORD    dwType = REG_SZ;

	ret = GetRegValue(hKey,
		lpSubKey,
		lpValueName,
		&dwType,&dwValueSize,&achValue);
	if(ret == ERROR_SUCCESS)
	{
		csInitValue = (PWCHAR)achValue;

		GlobalFree(achValue);

		return true;
	}

	return false;
}

