
// CleanResourseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CleanResourse.h"
#include "CleanResourseDlg.h"
#include "afxdialogex.h"
#include "json/json.h"
#include "simple_function.h"
#include "deletedir.h"
#include "BaseService.h"

#define SERVICE_SUCCESS		0			//服务器连接成功
#define DATA_NULL			1			//服务器连接成功,返回为空
#define NOT_SERVICE			-1			//服务器连接失败
#define ERROR_VALUE			-2			//传入的值有误
#define NOT_FIND			-3			//没有找到
#define FAIL_OPERAT			-4			//操作失败

#define RIGISTER_KEY _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths")
#define CHECK_TRUE_CONTINUE(result)  if(TRUE == result) {\
	continue;}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CHttpService m_cHttpService;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
CUrlCode encode;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CCleanResourseDlg 对话框



CCleanResourseDlg::CCleanResourseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLEANRESOURSE_DIALOG, pParent)
{
	m_classNum = 200*1024;
	strRspath = TEXT("");
	FilePath = TEXT("");
	strPath = "";
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCleanResourseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCleanResourseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()
bool IsLocalIP(char* serverip)
{
	//1.初始化wsa
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		return false;
	}
	//2.获取主机名
	char hostname[256];
	ret = gethostname(hostname, sizeof(hostname));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	//3.获取主机ip
	HOSTENT* host = gethostbyname(hostname);
	if (host == NULL)
	{
		return false;
	}
	//4.转化为char*并拷贝返回
	char ip[30];
	for (int i = 0; i < host->h_length; i++)
	{
		in_addr addr;
		char *p = host->h_addr_list[i];
		if (p == NULL)
			break;
		memcpy(&addr.S_un.S_addr, p, host->h_length);
		char *szIp = ::inet_ntoa(addr);
		if (strcmp(szIp, serverip) == 0)
		{
			return true;
		}
	}
	return false;
}

// CCleanResourseDlg 消息处理程序
int CCleanResourseDlg::BrowseCurrentDir(CString strDir, CArray<CString, CString&>& strDirInfo)
{
	int iCount = 0;

	if (strDir == _T(""))
	{
		return iCount;
	}
	else
	{
		if (strDir.Right(1) != _T("//"))
			strDir += L"//";
		strDir = strDir + _T("*.*");
	}

	CFileFind finder;
	CString strPath;
	BOOL bWorking = finder.FindFile(strDir);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		strPath = finder.GetFilePath();	//strPath就是所要获取Test目录下的文件夹和文件（包括路径）
		if (finder.IsDirectory() && !finder.IsDots())
		{
			strDirInfo.Add(strPath);
			iCount++;
		}
	}

	return iCount;
}
void CCleanResourseDlg::GetResourse()
{
	Log(TEXT("离线删除: 进入监控目录获取==================================="), TEXT(""), TEXT("RtmpServer"));

	//遍历挂载硬盘
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	wchar_t PathName[MAX_PATH];

	wcscpy(PathName, m_strDiskPath);
	wcscat(PathName, L"\\*");

	hFind = FindFirstFileW(PathName, &ffd);


	if (INVALID_HANDLE_VALUE == hFind)
	{
		CloseHandle(hFind);
		return;
	}
	while (FindNextFileW(hFind, &ffd) != 0)
	{
		if (wcscmp(ffd.cFileName, L".") == 0 || 0 == wcscmp(ffd.cFileName, L".."))
			continue;
		wcscpy(PathName, m_strDiskPath);
		wcscat(PathName, L"\\");
		wcscat(PathName, ffd.cFileName);
		wcscat(PathName, L"\\");
		USES_CONVERSION;
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			int iPercent;
			int iFreespace;
			int iRet = GetMountedInfo(PathName, iPercent, iFreespace);
			if (iRet != 0)
			{
				CString str1;
				str1.Format(_T("%d"), iRet);
				Log(TEXT("CleanResourse: iRet =    GetMountedInfo failed ！"), str1, TEXT("RtmpServer"));
				break;
			}
			CString fileName;
			fileName.Format(TEXT("%s"), ffd.cFileName);
			//GetDir(TEXT("/api/v1.0/resource/3/autodelete/2?macAddr="), L"/api/v1.0/resource/3/autodelete?macAddr=", m_strMac.GetBuffer(), fileName.GetBuffer(), 3);
			//GetDir(TEXT("/api/tempstorage/getDelResByMacAddr?macAddr="), L"/api/tempstorage/getDelResByMacAddr?macAddr=", m_strMac.GetBuffer(), fileName.GetBuffer(), 3);
			GetDir(TEXT("/api/tempstorage/getMemeryOutResource?macAddr="), L"/api/tempstorage/getDelResByMacAddr?macAddr=", m_strMac.GetBuffer(), fileName.GetBuffer(), 3);
		}
	}
	FindClose(hFind);
}
BOOL CCleanResourseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	USES_CONVERSION;
	std::string mac, strip;
	GetMacByGetAdaptersInfo(mac);
	//mac = "ECD68A7FFE7E";
	m_strMac.Format(_T("%s"), A2T(mac.c_str()));
	m_strMac.Replace(L"-", L"");
	
	//获取D00数据库地址
	CString strSubKey;
	CString strIPandPort, strIP, strPort,isMonitor, diskPath;
	int diskNum;
	strSubKey = RIGISTER_KEY + CString(_T("\\CloudClassroom\\WebService"));
	strIP = GetstrValueFromRegedit(strSubKey, L"WebBaseServiceIP");
	strPort = GetstrValueFromRegedit(strSubKey, L"WebBaseServicePort");
	//strIP = "172.16.62.144";
	//strPort = "10102";
	m_strDiskPath = GetstrValueFromRegedit(strSubKey, L"DiskPath");
	isMonitor = GetstrValueFromRegedit(strSubKey, L"blnRecordServer");
	diskPath = GetstrValueFromRegedit(strSubKey, L"DiskPath");
	isOutTime = GetstrValueFromRegedit(strSubKey, L"OverTimeDel");
	isMemoryOut = GetstrValueFromRegedit(strSubKey, L"OverRAMDel");
	diskNum = BrowseCurrentDir(diskPath, dirInfo);
	strIPandPort.Format(TEXT("%s:%s"), strIP.GetString(), strPort.GetString());
	char ip_port[MAX_PATH];
	g_TcharToChar(strIPandPort, ip_port);

	char ip_port1[MAX_PATH];
	long lret = GetSubSystemServerInfo(ip_port,"D01", ip_port1);
	if (lret>0)
	{
		m_strServerIP = A2T(ip_port1);
		SetstrValueToRegedit(strSubKey, L"D00Msg", 2 * m_strServerIP.GetLength(), m_strServerIP);
	}
	else
	{
		m_strServerIP = GetstrValueFromRegedit(strSubKey, L"D00Msg");
	}
	if (m_strServerIP == L"")
	{
		TerminateProcess(GetCurrentProcess(), 10);
	}
	//获取教室数量
	char ip_port2[MAX_PATH];
	GetSubSystemServerInfo(ip_port, "E00", ip_port2);
	CBaseService cbase;
	cbase.m_strServerIP = ip_port2;
	char cIP[MAX_PATH];
	char cPort[MAX_PATH];
	g_TcharToChar(strIP.GetBuffer(), cIP);
	g_TcharToChar(strPort.GetBuffer(), cPort);
	std::list<ElectricClassroomInfo> e_list;
	cbase.GetAllElectricClassroomInfo(cIP, cPort, e_list);
	std::list<ElectricClassroomInfo>::iterator iter;
	bool is_find_classid = false;
	int temp = 0;
	for (iter = e_list.begin(); iter != e_list.end(); ++iter)
	{
		IP str_ip;
		std::string sMsg = iter->recordServerIp;
		ResolveIPLine(sMsg, str_ip);
		CString strRecordIP = A2T(str_ip.strRtmp_ip.c_str());
		CHECK_TRUE_CONTINUE(strRecordIP.IsEmpty());//如果录播服务器为空，则停止录制
		USES_CONVERSION;
		char* liveip;
		liveip = T2A(strRecordIP);
		CHECK_TRUE_CONTINUE(!(IsLocalIP(liveip)));//如果不是该录播服务器对应的教室，则停止录制
		temp++;
	}
	if (temp!=0)
	{
		CString strIDnum = TEXT("");
		strIDnum.Format((L"%d"), temp);
		SetstrValueToRegedit(strSubKey, L"ClassNum", 2 * strIDnum.GetLength(), strIDnum);
	}
	else
	{
		CString strIDnum = TEXT("");
		strIDnum = GetstrValueFromRegedit(strSubKey, L"ClassNum");
		temp = _ttoi(strIDnum);
	}
	if (isMonitor ==L"1")
		//m_classNum = (temp * 48 * 1024 + temp * 32 * 1024)/diskNum;//监控24小时2G*24;录播4G*8节课
		m_classNum = 48 * 1024;//监控24小时2G*24;录播4G*8节课
	else
		//m_classNum = (temp * 32 * 1024)/diskNum;
		m_classNum = 48 * 1024;
	//获取监控目录文件
	GetResourse();
	//DeleteResourse();
	SetTimer(1, 3 * 60 * 1000, NULL);
	SetTimer(2, 3600 * 1000, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCleanResourseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCleanResourseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	CWnd::ShowWindow(SW_HIDE);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCleanResourseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


long CCleanResourseDlg::GetSubSystemServerInfo(IN char* ip_port,IN char* code, OUT char* service_ip_port)
{
	std::string strUrl;
	strUrl = "http://";
	strUrl += ip_port;
	strUrl += "/Base/WS/Service_Basic.asmx/WS_G_GetSubSystemServerInfo?sysID=";
	strUrl += code;
	strUrl += "&subjectID=";

	CString str;
	USES_CONVERSION;
	str = A2T(strUrl.c_str());
	Log(TEXT("GetSubSystemServerInfo: strUrl="), str, TEXT("RtmpServer"));

	TCHAR cUrl[1024 * 5] = L"";
	g_charToTCHAR(strUrl.c_str(), cUrl);
	std::string cResponse;
	int iret = m_cHttpService.HttpGet(cUrl, cResponse);
	if (iret!=0)
	{
		return -1;
	}
	Json::Value out_param;
	if (m_cHttpService.ParseResponse((char*)cResponse.c_str(), out_param) != 0) {
		return -1;
	}

	if (!out_param.empty() && out_param.isMember("ArrayOfString") &&
		out_param["ArrayOfString"].isObject() &&
		out_param["ArrayOfString"].isMember("string") &&
		out_param["ArrayOfString"]["string"].isArray() &&
		out_param["ArrayOfString"]["string"].size() >= 4)
	{
		std::string str_server = out_param["ArrayOfString"]["string"][3].asString();
		std::string ip_port = str_server.substr(7, str_server.size() - 8);

		g_StringToChar(ip_port.c_str(), service_ip_port);
	}
	else {
		return -1;
	}
	return 1;
}

long CCleanResourseDlg::Exp_GetDir(IN TCHAR* tcMethod, IN TCHAR* tcParams)
{ 
	//107B44F0F4DE
	CString strUrl;
	strUrl = TEXT("http://");
	strUrl += m_strServerIP;
	strUrl += tcMethod;
	strUrl += tcParams;
	//strUrl +=L"6CB31152479B";
	Log(TEXT("SendServer: 入参 strUrl="), strUrl, TEXT("RtmpServer"));

	std::string cResponse;
	int iret = m_cHttpService.HttpGet(strUrl, cResponse);

	Json::Value value;
	Json::Reader read;
	if (!read.parse(cResponse, value))
	{
		return FAIL_OPERAT;
	}
	Json::Value val_array = value["data"];
	int iSize = val_array.size();
	for (int nIndex = 0; nIndex < iSize; ++nIndex)
	{
		CString strRspath;
		USES_CONVERSION;
		std::string strPath = val_array[nIndex].asString();
		if ("" == strPath)
		{
			return 0;
		}
		strRspath = A2T(strPath.c_str());
		strRspath.Replace(L"/", L"\\");
		int icont = strRspath.ReverseFind('\\');
		strRspath = strRspath.Left(icont);
		/*for (int i = 0; i < 2; i++)
		{*/
		//DoRemoveDirectory(strRspath);
		DeleteFile(strRspath);
		/*}*/
		Log(TEXT("删除目录成功，strRspath = "), strRspath, TEXT("RtmpServer"));
	}
	return  iSize;
}

long CCleanResourseDlg::Exp_DeleteDir(IN TCHAR* tcMethod, IN TCHAR* tcParams)
{
	CString strUrl;
	strUrl = TEXT("http://");
	strUrl += m_strServerIP;
	strUrl += tcMethod;
	strUrl += tcParams;

	Log(TEXT("删除超时数据库记录，strUrl = "), strUrl, TEXT("RtmpServer"));
	
	std::string url;
	USES_CONVERSION;
	url = T2A(strUrl.GetBuffer());
	
	std::string respone;
	bool iret = net::http::Post(url, "", respone);
	Log(TEXT("删除超时目录数据库返回，respone = "), A2T(respone.c_str()), TEXT("RtmpServer"));
	Json::Value value;
	Json::Reader read;
	if (!read.parse(respone, value))
	{
		return FAIL_OPERAT;
	}
	
	std::string strData = value["msg"].asString();

	if (strData == "success")
	{
		return SERVICE_SUCCESS;
	}
	else
	{
		Log(TEXT("DeleteTimeoutDir: Exp_GetTimeoutDirPath  删除Failed! "), TEXT(""), TEXT("RtmpServer"));
		return FAIL_OPERAT;
	}
	return SERVICE_SUCCESS;
}
CString WINAPI GetLocalIP()
{
	CString strIP = TEXT("");
	//1.初始化wsa  
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		return strIP;
	}

	//2.获取主机名  
	char hostname[256];
	ret = gethostname(hostname, sizeof(hostname));
	if (ret == SOCKET_ERROR)
	{
		return strIP;
	}

	//3.获取主机ip  
	HOSTENT* host = gethostbyname(hostname);
	if (host == NULL)
	{
		return strIP;
	}

	//4.转化为char*并拷贝返回  

	USES_CONVERSION;
	// 	wchar_t wTemp[256];
	// 	ZeroMemory(wTemp,256);
	// 	_tcscpy(wTemp,A2T(inet_ntoa(*(in_addr*)*host->h_addr_list)));
	//strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list));  
	strIP.Format(TEXT("%s"), A2T(inet_ntoa(*(in_addr*)*host->h_addr_list)));
	return strIP;
}
//写配置文件信息
void CCleanResourseDlg::SetProfile(CString strAppName, CString strKeyName, CString strValue, CString strFileName)
{
	CString strFilePath;
	TCHAR szPath[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(), szPath, sizeof(szPath));
	strFilePath = szPath;
	int id = strFilePath.ReverseFind('\\');
	ASSERT(id != -1);
	strFilePath = strFilePath.Left(id);
	strFilePath = strFilePath + _T("\\") + strFileName;
	//strFilePath = strFilePath + _T("\\MoniterIni\\") + strFileName;
	USES_CONVERSION;
	PCSTR cPath = T2A(strFilePath.GetBuffer());
	MakeSureDirectoryPathExists(cPath);
	::WritePrivateProfileString(strAppName, strKeyName, strValue, strFilePath);
}

//获取配置文件信息
CString CCleanResourseDlg::GetProfile(CString strAppName, CString strKeyName, CString strFileName)
{
	CString strFilePath, strValue;
	strValue = TEXT("");
	TCHAR szPath[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(), szPath, sizeof(szPath));
	strFilePath = szPath;
	int id = strFilePath.ReverseFind('\\');
	ASSERT(id != -1);
	strFilePath = strFilePath.Left(id);
	strFilePath = strFilePath + _T("\\") + strFileName;

	WIN32_FIND_DATA wfd;
	bool rValue = false;
	HANDLE hFind = FindFirstFile(strFilePath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) /*&& (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)*/)
	{
		TCHAR buf[500] = { 0 };
		::GetPrivateProfileString(strAppName, strKeyName, _T(""), buf, 500, strFilePath);

		strValue.Format(TEXT("%s"), buf);
	}
	FindClose(hFind);

	return strValue;
}
long CCleanResourseDlg::Exp_GetMemoryDir(IN TCHAR * tcMethod, IN TCHAR* tcMethod2, IN TCHAR * tcMac, IN TCHAR * tcResFlag, int Type)
{
	CString strUrl;
	strUrl = TEXT("http://");
	strUrl += m_strServerIP;
	strUrl += tcMethod;
	strUrl += tcMac;
	// strUrl += TEXT("&partitionDirName=");
	// strUrl += tcResFlag;
	// strUrl += TEXT("&serverIP=");
	// strUrl += GetLocalIP();
	USES_CONVERSION;
	Log(TEXT("获取要删除的资源目录: strUrl="), strUrl, TEXT("RtmpServer"));

	std::string cResponse;
	int iret = m_cHttpService.HttpGet(strUrl, cResponse);
	//Log(TEXT("获取要删除的资源目录:  cResponse="), A2T(cResponse.c_str()), TEXT("RtmpServer"));
	Json::Value value;
	Json::Reader read;
	if (!read.parse(cResponse, value))
	{
		return FAIL_OPERAT;
	}
	Json::Value val_array = value["data"];
	int iSize = val_array.size();
	CString strFile, strID, temp,strNum;
	for (int nIndex = 0; nIndex < iSize; ++nIndex)
	{
		strPath = val_array[nIndex]["phyPath"].asString();
		strRspath = CA2W(strPath.c_str());
		if ("" == strPath)
		{
			continue;
		}
		strRspath = CA2W(strPath.c_str());
		strRspath.Replace(L"/", L"\\");
		int icont = strRspath.ReverseFind('\\');
		strRspath = strRspath.Left(icont);
		//DoRemoveDirectory(strRspath);
		DeleteFile(strRspath);
		Log(TEXT("删除本地目录成功，strRspath = "), strRspath, TEXT("RtmpServer"));
		int icont1 = strRspath.Find(L"LGFTP");
		FilePath = strRspath.Mid(icont1 + 5);
		FilePath.Replace(L"\\", L"/");
		FilePath += L"/";
	}

	CString strUrl1;
	strUrl1 = TEXT("http://");
	strUrl1 += m_strServerIP;
	strUrl1 += tcMethod2;
	strUrl1 += m_strMac;
	// strUrl1 += L"&storagePath=";
	// strUrl1 += FilePath;
	// strUrl1 += L"&serverIP=";
	// strUrl1 += GetLocalIP();

	string url = "";
	string respone = "";
	url = T2A(strUrl1.GetBuffer());
	std::string postData = "";
	Log(TEXT("开始删除数据库记录，url = "), strUrl1, TEXT("RtmpServer"));

	Json::FastWriter json_write;
	string str = json_write.write(val_array);
	string postdata="";
	encode.GB2312ToUTF_8(postdata, (char*)str.c_str(), strlen(str.c_str()));
	std::string str_strUrl1;
	str_strUrl1 = CW2A(strUrl1);
	bool iret2 = net::http::Post(str_strUrl1, postdata, respone);
	Log(TEXT("开始删除数据库记录，respone = "), A2W(respone.c_str()), TEXT("RtmpServer"));
	Json::Value value2;
	Json::Reader read2;
	if (!read2.parse(respone, value2))
	{
		return 0;
	}
	if ((value2["errCode"].asInt() == 0) && (value2["data"].asInt() == 1))
	{
		//Log(TEXT("InsertDatabase: 删除数据库目录成功，文件路径FilePath = "), FilePath, TEXT("RtmpServer"));
		Log(TEXT("InsertDatabase: 删除数据库目录成功"), L"", TEXT("RtmpServer"));
	}
	else {
		Log(TEXT("InsertDatabase: 删除数据库目录失败"), L"", TEXT("RtmpServer"));
	}

	return  iSize;
}

CString UTF82WCS(const char* szU8)
{
	//预转换，得到所需空间的大小;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//转换
	::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//最后加上'\0'
	wszString[wcsLen] = '\0';

	CString unicodeString(wszString);

	delete[] wszString;
	wszString = NULL;

	return unicodeString;

}

long CCleanResourseDlg::GetDir(IN TCHAR * tcMethod, IN TCHAR * tcMethod2, IN TCHAR * tcMac, IN TCHAR * tcResFlag, int Type)
{
	CString strUrl;
	strUrl = TEXT("http://");
	strUrl += m_strServerIP;
	strUrl += tcMethod;
	strUrl += tcMac;
	// strUrl += TEXT("&partitionDirName=");
	// strUrl += tcResFlag;
	// strUrl += TEXT("&serverIP=");
	// strUrl += GetLocalIP();
	USES_CONVERSION;
	Log(TEXT("获取要删除的资源目录: strUrl="), strUrl, TEXT("RtmpServer"));

	std::string cResponse;
	int iret = m_cHttpService.HttpGet(strUrl, cResponse);
	CString cstr_cResponse;
	cstr_cResponse=CA2W(cResponse.c_str());
	Log(TEXT("获取要删除的资源目录:  cResponse="), cstr_cResponse, TEXT("RtmpServer"));

	Json::Value value;
	Json::Reader read;
	if (!read.parse(cResponse.c_str(), value))
	{
		return FAIL_OPERAT;
	}
	Json::Value val_array = value["data"];
	int iSize = val_array.size();

	CString strFile, strID, temp, strNum;
	strNum.Format((L"%d"), iSize);
	strFile = tcResFlag;
	strID = tcResFlag;
	strFile += L".ini";

	int code=value["errCode"].asInt();

	for (int nIndex = 0; nIndex < iSize; ++nIndex)
	{
		strPath = val_array[nIndex]["phyPath"].asString();
		//strRspath = CA2W(strPath.c_str());

		if ("" == strPath)
		{
			continue;
		}
		strRspath = CA2W(strPath.c_str());
		strRspath.Replace(L"/", L"\\");
		int icont = strRspath.ReverseFind('\\');
		strRspath = strRspath.Left(icont);
		strRspath = strRspath.Mid(0, strRspath.FindOneOf(L"年") + 1);
		SetProfile(strID, L"num", strNum, strFile);
		//写入配置文件用于离线删除
		if (Type == 3)
		{
			temp.Format((L"%d"), nIndex);
			SetProfile(strID, temp, strRspath, strFile);
		}
		//-------------
	}
	return  iSize;
}

long CCleanResourseDlg::Exp_DeleteMemoryDir(IN TCHAR * tcMethod, IN TCHAR * tcMac, IN TCHAR * tcResFlag)
{
	CString strUrl;
	strUrl = TEXT("http://");
	strUrl += m_strServerIP;
	strUrl += tcMethod;
	strUrl += tcMac;
	strUrl += TEXT("&resFlag=");
	strUrl += tcResFlag;
	std::string url;
	USES_CONVERSION;
	url = T2A(strUrl.GetBuffer());

	Log(TEXT("删除超内存数据库记录，strUrl = "), strUrl, TEXT("RtmpServer"));

	std::string respone;
	bool iret = net::http::Post(url, "", respone);
	CString cstr_cResponse;
	cstr_cResponse = CA2W(respone.c_str());
	Log(TEXT("删除超时目录数据库返回，respone = "), cstr_cResponse, TEXT("RtmpServer"));

	Json::Value value;
	Json::Reader read;
	if (!read.parse(respone, value))
	{
		return FAIL_OPERAT;
	}

	std::string strData = value["message"].asString();

	if (strData == "Success")
	{
		return SERVICE_SUCCESS;
	}
	else
	{
		Log(TEXT("DeleteTimeoutDir: Exp_GetTimeoutDirPath  删除Failed! "), TEXT(""), TEXT("RtmpServer"));
		return FAIL_OPERAT;
	}
	return SERVICE_SUCCESS;
}

long CCleanResourseDlg::DeleteTimeoutDir(CString strPartitionDirName)
{
	long lret = -1;
	Log(TEXT("开始获取超时目录"), TEXT(""), TEXT("RtmpServer"));
	//lret = Exp_GetMemoryDir(TEXT("/api/v1.0/resource/3/autodelete/1?macAddr="), L"/api/v1.0/resource/3/autodelete?macAddr=", m_strMac.GetBuffer(), strPartitionDirName.GetBuffer(),1);
	lret = Exp_GetMemoryDir(TEXT("/api/tempstorage/getDelResByMacAddr?macAddr="), L"/api/tempstorage/updateResDeleteFlag?macAddr=", m_strMac.GetBuffer(), strPartitionDirName.GetBuffer(),1);

	if (lret <= 0)
	{
		Log(TEXT("获取超时监控目录失败"), TEXT(""), TEXT("RtmpServer"));
		return -1;

	}
	//lret = Exp_GetMemoryDir(TEXT("/api/v1.0/resource/2/autodelete/1?macAddr="), L"/api/v1.0/resource/2/autodelete?macAddr=", m_strMac.GetBuffer(), strPartitionDirName.GetBuffer(),2);
	// lret = Exp_GetMemoryDir(TEXT("/api/tempstorage/getDelResByMacAddr?macAddr="), L"/api/tempstorage/getDelResByMacAddr?macAddr=", m_strMac.GetBuffer(), strPartitionDirName.GetBuffer(),2);
	//
	// if (lret <= 0)
	// {
	// 	Log(TEXT("获取超时录播目录失败"), TEXT(""), TEXT("RtmpServer"));
	// 	return -1;
	// }

	return 1;
}

bool CCleanResourseDlg::DeleteOutMemoryDir(CString strPartitionDirName)
{
	long lret = -1;
	Log(TEXT("开始获取超内存目录"), TEXT(""), TEXT("RtmpServer"));
	//lret = Exp_GetMemoryDir(TEXT("/api/v1.0/resource/3/autodelete/2?macAddr="),L"/api/v1.0/resource/3/autodelete?macAddr=",m_strMac.GetBuffer(), strPartitionDirName.GetBuffer(),3);
	lret = Exp_GetMemoryDir(TEXT("/api/tempstorage/getMemeryOutResource?macAddr="),L"/api/tempstorage/updateResDeleteFlag?macAddr=",m_strMac.GetBuffer(), strPartitionDirName.GetBuffer(),3);

	if (lret <= 0)
	{
		Log(TEXT("获取超内存监控目录失败"), TEXT(""), TEXT("RtmpServer"));
		Log(TEXT("开始离线删除功能=============================="), TEXT(""), TEXT("RtmpServer"));
		DeleteOffline(strPartitionDirName);
	}

	//lret = Exp_GetMemoryDir(TEXT("/api/v1.0/resource/2/autodelete/2?macAddr="), L"/api/v1.0/resource/2/autodelete?macAddr=", m_strMac.GetBuffer(), strPartitionDirName.GetBuffer(),4);
	// lret = Exp_GetMemoryDir(TEXT("/api/tempstorage/getMemeryOutResource?macAddr="), L"/api/tempstorage/getMemeryOutResource?macAddr=", m_strMac.GetBuffer(), strPartitionDirName.GetBuffer(),4);
	// if (lret <= 0)
	// {
	// 	Log(TEXT("获取超内存录播目录失败"), TEXT(""), TEXT("RtmpServer"));
	// 	return false;
	// }
	return true;
}

bool CCleanResourseDlg::DeleteOffline(CString strID)
{
	Log(TEXT("开始离线删除"), TEXT(""), TEXT("RtmpServer"));
	CString strFile = strID + TEXT(".ini");
	CString strNum = GetProfile(strID, L"num", strFile);
	CString strPath,temp;
	for (int i = 0; i < _ttoi(strNum);i++)
	{
		temp.Format((L"%d"), i);
		strPath = GetProfile(strID, temp, strFile);// L"D:/监控资源/507/2022年02月/15日/"
		DeleteMonth(strPath);
	}
	return true;
}

bool CCleanResourseDlg::DeleteMonth(CString strPath)
{
	int icont = strPath.Find(L'年');
	strPath = strPath.Left(icont + 1);
	CString Month, strFile;
	for (int a = 1; a < 13; a++)
	{
		Month.Format((L"%02d"), a);
		strFile = strPath + Month + L"月/";
		if (!CheckFolderExist(strFile))
		{
			continue;
		}
		else
		{
			bool lert = DeleteDay(strFile);
			if (lert)
			{
				return true;
			}
		}
	}
	return false;
}

bool CCleanResourseDlg::DeleteDay(CString strPath)
{
	CString Day, strFile2;
	for (int b = 1; b < 32; b++)
	{
		Day.Format((L"%02d"), b);
		strFile2 = strPath + Day + L"日/";
		if (!CheckFolderExist(strFile2))
		{
			continue;
		}
		else
		{
			Log(TEXT("开始离线删除最久的目录,strFile="), strFile2, TEXT("RtmpServer"));
			bool lret = DoRemoveDirectory(strFile2);
			//bool lret = DeleteFile(strFile2);
			if (lret)
			{
				return true;
			}
		}
	}
	return false;
}

void CCleanResourseDlg::DeleteResourse()
{
	Log(TEXT("CleanResourse: 进入资料删除==================================="), TEXT(""), TEXT("RtmpServer"));

	//========超时处理=======================
	//DeleteTimeoutDir();

	//=========硬盘使用率超出处理=============
	//遍历挂载硬盘
	WIN32_FIND_DATAW ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	wchar_t PathName[MAX_PATH];

	wcscpy(PathName, m_strDiskPath);
	wcscat(PathName, L"\\*");

	hFind = FindFirstFileW(PathName, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		Log(TEXT("CleanResourse: 硬盘 FindFirstFileW failed \n"), TEXT(""), TEXT("RtmpServer"));
		return;
	}

	while (FindNextFileW(hFind, &ffd) != 0)
	{
		if (wcscmp(ffd.cFileName, L".") == 0 || 0 == wcscmp(ffd.cFileName, L".."))
			continue;

		wcscpy(PathName, m_strDiskPath);
		wcscat(PathName, L"\\");
		wcscat(PathName, ffd.cFileName);
		wcscat(PathName, L"\\");
		Log(TEXT("CleanResourse: 当前目录 PathName ="), PathName, TEXT("RtmpServer"));
		 
		USES_CONVERSION;
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			int iPercent;
			int iFreespace;
			int iRet = GetMountedInfo(PathName, iPercent, iFreespace);

			if (iRet != 0)
			{
				CString str1;
				str1.Format(_T("%d"), iRet);
				Log(TEXT("CleanResourse: iRet =    GetMountedInfo failed ！"), str1, TEXT("RtmpServer"));
				break;
			}
			int i = iFreespace / 1024 ;
			CString strFree;
			strFree.Format(TEXT("%d"), i);
			Log(TEXT("CleanResourse资源删除开始检查，剩余空间:  iFreespace = "), strFree, TEXT("RtmpServer"));
			if (m_classNum == 0)
			{
				m_classNum = 1024 * 200;
			}
			CString fileName;
			fileName.Format(TEXT("%s"), ffd.cFileName);
			if (isOutTime==L"1")
			{
				DeleteTimeoutDir(fileName);
			}
			int icont = 0;
			while (iFreespace <= m_classNum)
			{
				bool lret;
				std::vector<CString> strMemDirPath;
				lret = DeleteOutMemoryDir(fileName);
				if( (!lret)||(icont>30))
				{
					Log(TEXT("DeleteOutMemoryDir资源删除失败"), L"", TEXT("RtmpServer"));
					break;
				}
				GetMountedInfo(PathName, iPercent, iFreespace);
				int b = iFreespace / 1024;
				CString strLeft;
				strLeft.Format(TEXT("%d"), b);
				Log(TEXT("CleanResourse资源删除后:  iFreespace = "), strLeft, TEXT("RtmpServer"));
				icont++;
				Sleep(1000);
			}
		}
	}
	FindClose(hFind);
}

void CCleanResourseDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1)
	{
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		if (systime.wHour == 3)
		{
			DeleteResourse();
		}
	}
	else if (nIDEvent == 2)
	{
		SYSTEMTIME systime;
		GetLocalTime(&systime);
		if (systime.wHour == 1)
		{
			TerminateProcess(GetCurrentProcess(), 10);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

