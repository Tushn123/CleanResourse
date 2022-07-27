#include "stdafx.h"
#include "UrlCode.h"
//#include "HttpService.h"
#include "PublicFunc.h"

#include <time.h>
#include <sys/stat.h>
#include <atlstr.h>
#include <io.h>
#include <fcntl.h>

#define  NORMAL_CONNECT             INTERNET_FLAG_KEEP_CONNECTION
#define  SECURE_CONNECT             NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define  NORMAL_REQUEST             INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE 
#define  SECURE_REQUEST             NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID


void g_TcharToChar(const TCHAR* tcStr, char* cStr)
{
	int ilength ;
	ilength = WideCharToMultiByte(CP_ACP, 0, tcStr, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, tcStr, -1, cStr, ilength, NULL, NULL);
	cStr[ilength] = '\0';
}

void g_charToTCHAR(const char* strIn, TCHAR* strOut)			//char转TCHAR
{
	int len = MultiByteToWideChar(CP_ACP,0,strIn, -1,NULL,0);   
	MultiByteToWideChar(CP_ACP,0,strIn,-1,strOut,len);   
	strOut[len]='\0';   
}

void g_StringToChar(std::string sStr, char* cStr)
{
	int ilength;
	ilength = sStr.size();
	memcpy(cStr, sStr.c_str(), ilength);
	cStr[ilength] = '\0';
}

CString StringToCString(std::string sStr)
{
	CString wStr;
	int ilength;
	ilength= ::MultiByteToWideChar(CP_ACP, NULL, sStr.c_str(), sStr.size(), NULL, 0);

	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[ilength + 1];

	::MultiByteToWideChar(CP_ACP, NULL, sStr.c_str(), sStr.size(), wszString, ilength);
	wszString[ilength] = '\0';            // UNICODE字串

	wStr = wszString;
	delete[] wszString;
	return wStr;
}

CHttpService::CHttpService(LPCTSTR strAgent)
{
	m_pSession = new CInternetSession(strAgent);
	m_pConnection = NULL;
	m_pFile = NULL;
}


CHttpService::~CHttpService(void)
{
	Clear();
	if(NULL != m_pSession)
	{
		m_pSession->Close();
		delete m_pSession;
		m_pSession = NULL;
	}
}

void CHttpService::Clear()
{
	if(NULL != m_pFile)
	{
		m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}

	if(NULL != m_pConnection)
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}
}

int CHttpService::ExecuteRequest(LPCTSTR strMethod, LPCTSTR strUrl, LPCTSTR strPostData, string &strResponse)  
{  
	CString strServer;  
	CString strObject;  
	DWORD dwServiceType;  
	INTERNET_PORT nPort;  
	strResponse = "";  
	AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);  

	if(AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)  
	{  
		return FAILURE;  
	}  

	try  
	{  
		m_pConnection = m_pSession->GetHttpConnection(strServer,  
			dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT,  
			nPort);  
		m_pFile = m_pConnection->OpenRequest(strMethod, strObject,   
			NULL, 1, NULL, NULL,   
			(dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_REQUEST : SECURE_REQUEST));  
		m_pFile->AddRequestHeaders(_T("Accept: *, */*")); 
		m_pFile->AddRequestHeaders(_T("Accept-Language: zh-cn"));   
		m_pFile->AddRequestHeaders(_T("Content-Type: text/xml; charset=utf-8"));  
		m_pFile->AddRequestHeaders(_T("Content-Type: application/x-www-form-urlencoded"));  
		m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)strPostData, strPostData == NULL ? 0 : _tcslen(strPostData));  

		char szChars[BUFFER_SIZE + 1] = {0};  
		string strRawResponse = "";  
		UINT nReaded = 0;  
		while ((nReaded = m_pFile->Read((void*)szChars, BUFFER_SIZE)) > 0)  
		{  
			szChars[nReaded] = '\0';  
			strRawResponse += szChars;  
			memset(szChars, 0, BUFFER_SIZE + 1);  
		}  

		strResponse = strRawResponse;  

		Clear();  
	}  
	catch (CInternetException* e)  
	{  
		Clear();  
		DWORD dwErrorCode = e->m_dwError;  
		e->Delete();  

		DWORD dwError = GetLastError();  

		Log(TEXT("ExecuteRequest: 获取基础平台数据库数据错误 "),TEXT(""),TEXT("Error"));  

		if (ERROR_INTERNET_TIMEOUT == dwErrorCode)  
		{  
			return OUTTIME;  
		}  
		else  
		{  
			return FAILURE;  
		}  
	}  
	return SUCCESS;  
}
int CHttpService::HttpGet(LPCTSTR strUrl, char* cResponse)
{
	string strResponse;
	int iRet = ExecuteRequest(_T("GET"), strUrl, NULL, strResponse);

	int nCount = 0;
	for (; iRet != 0; )
	{
		iRet = ExecuteRequest(_T("GET"), strUrl, NULL, strResponse);
		if (++nCount >= 2)
		{
			nCount = 0;
			Log(TEXT("基础平台，E00数据库获取错误！"), TEXT(""), TEXT("Error"));
			return -1;
		}
		Sleep(1000);
	}

	string cOut;
	cOut = m_urlcode.UrlUTF8Decode(strResponse.c_str());

	if (cOut.size() > 1024 * 1024 - 1)
	{
		return -1;
	}
	strcpy(cResponse, cOut.c_str());
	char cLog[1024] = "";
	int iLen = cOut.length();
	if (iLen > 1023)
	{
		iLen = 1023;
	}
	memcpy(cLog, cOut.c_str(), iLen);
	
	return iRet;
}
int CHttpService::HttpGet(LPCTSTR strUrl, std::string& sResponse)
{
	int iRet = ExecuteRequest(_T("GET"), strUrl, NULL, sResponse);
	int nCount = 0;
	for (;iRet != 0 ; )
	{
		iRet = ExecuteRequest(_T("GET"), strUrl, NULL, sResponse);
		if (++nCount >= 2)
		{
			nCount = 0;
			Log(TEXT("基础平台，D00数据库获取错误！"),TEXT(""),TEXT("Error"));
			return -1;
		}
		Sleep(1000);
	}
	string cOut;
	cOut = m_urlcode.UrlUTF8Decode(sResponse.c_str());
	sResponse = cOut;
	//USES_CONVERSION;
	//CString log;
	//log = A2T(sResponse.c_str());
	
	///*size_t iLen = cOut.length();
	//char cLog[1024];
	//memset(cLog, 0x00, 1024);
	//memcpy(cLog, sResponse.c_str(), iLen);
	//strcat(cLog, "\0");*/
	//Log(TEXT("HttpGet: 返回Json解析后为："), log,TEXT("RtmpServer"));
	
	return iRet;


}


long CHttpService::ParseResponse(char* buf, Json::Value& json_data)
{
	tinyxml2::XMLDocument doc;
	json_data = Json::Value::null;
	if (doc.Parse(buf) != tinyxml2::XML_SUCCESS) {
		return -1;
	}

	// 首节点不为数组
	ConvertElement(doc.RootElement(), json_data, false);

	return 0;
}
void CHttpService::ConvertElement(tinyxml2::XMLElement* node, Json::Value& json_data, bool root_is_array) {
	if (!node) {
		json_data = Json::Value::null;
		return;
	}

	const char* node_name = node->Name();
	tinyxml2::XMLElement* child_node = node->FirstChildElement();
	if (child_node) {
		// 比较前两个节点名称，判断其子节点是否为数组
		std::string child_node_name = child_node->Name();
		tinyxml2::XMLElement* next_node = child_node->NextSiblingElement();
		if (next_node) {
			bool is_array = false;
			if (child_node_name == next_node->Name()) {
				is_array = true;
			}

			unsigned int index = 0;
			tinyxml2::XMLElement* i_node = node->FirstChildElement();
			for (; i_node != NULL; i_node = i_node->NextSiblingElement()) {
				const char* i_node_name = i_node->Name();
				if (is_array) {
					ConvertElement(i_node, json_data[node_name][i_node_name][index++],
						true);
				}
				else {
					if (root_is_array) {
						ConvertElement(i_node, json_data, false);
					}
					else {
						ConvertElement(i_node, json_data[node_name], false);
					}
				}
			}
		}
		else {
			// 该node只有一个子节点
			if (root_is_array) {
				ConvertElement(child_node, json_data, false);
			}
			else {
				ConvertElement(child_node, json_data[node_name], false);
			}
		}
	}
	else {
		// 独立节点，无子节点
		if (root_is_array) {
			if (node->GetText()) json_data = node->GetText();
			/*				json_data =
			(node->GetText() != NULL) ? node->GetText() : Json::Value::null;*/
		}
		else {
			if (node->GetText()) json_data[node_name] = node->GetText();
			//json_data[node_name] =
			//	(node->GetText() != NULL) ? node->GetText() : Json::Value::null;
		}
	}
}
