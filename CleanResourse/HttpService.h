
#include <afxinet.h>
#include <string>
using namespace std;

#include "UrlCode.h"
#include "json/json.h"
#include "xml/tinyxml2.h"

#define  IE_AGENT  _T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)")

#define		SUCCESS				0			// 操作成功
#define		FAILURE				1			// 操作失败
#define		OUTTIME				2			// 操作超时 

#define		BUFFER_SIZE			1024

class  CHttpService {
public:
	CHttpService(LPCTSTR strAgent = IE_AGENT);
	virtual ~CHttpService(void);  
	// TODO: 在此添加您的方法。

	int HttpGet(LPCTSTR strUrl, char* cResponse);
	int HttpGet(LPCTSTR strUrl, std::string& sResponse);
	
	long ParseResponse(char* buf, Json::Value& json_data);
	void ConvertElement(tinyxml2::XMLElement* node, Json::Value& json_data, bool root_is_array);

private:
	int ExecuteRequest(LPCTSTR strMethod, LPCTSTR strUrl, LPCTSTR strPostData, string &strResponse);
	void Clear();

private:
	CInternetSession	*m_pSession;
	CHttpConnection		*m_pConnection;
	CHttpFile			*m_pFile;

public:
	CUrlCode			m_urlcode;
};

//int  WriteLog(char *fmt, ...);

void g_charToTCHAR(const char* strIn, TCHAR* strOut);			//char转TCHAR
void g_TcharToChar(const TCHAR* tcStr, char* cStr);				//TCHAR转char
void g_StringToChar(std::string sStr, char* cStr);				//String转char
CString StringToCString(std::string sStr);						//String转CString