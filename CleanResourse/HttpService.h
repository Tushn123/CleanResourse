
#include <afxinet.h>
#include <string>
using namespace std;

#include "UrlCode.h"
#include "json/json.h"
#include "xml/tinyxml2.h"

#define  IE_AGENT  _T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)")

#define		SUCCESS				0			// �����ɹ�
#define		FAILURE				1			// ����ʧ��
#define		OUTTIME				2			// ������ʱ 

#define		BUFFER_SIZE			1024

class  CHttpService {
public:
	CHttpService(LPCTSTR strAgent = IE_AGENT);
	virtual ~CHttpService(void);  
	// TODO: �ڴ�������ķ�����

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

void g_charToTCHAR(const char* strIn, TCHAR* strOut);			//charתTCHAR
void g_TcharToChar(const TCHAR* tcStr, char* cStr);				//TCHARתchar
void g_StringToChar(std::string sStr, char* cStr);				//Stringתchar
CString StringToCString(std::string sStr);						//StringתCString