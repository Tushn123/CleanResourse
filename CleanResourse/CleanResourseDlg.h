
// CleanResourseDlg.h : 头文件
//
#include <vector>


#pragma once
#include "PublicFunc.h"

// CCleanResourseDlg 对话框
class CCleanResourseDlg : public CDialogEx
{
// 构造
public:
	CCleanResourseDlg(CWnd* pParent = NULL);	// 标准构造函数
	int m_classNum;
	long DeleteTimeoutDir(CString strPartitionDirName);
	void DeleteResourse();
	bool DeleteOutMemoryDir(CString strPartitionDirName);
	void SetProfile(CString strAppName, CString strKeyName, CString strValue, CString strFileName);
	CString GetProfile(CString strAppName, CString strKeyName, CString strFileName);
	bool DeleteOffline(CString strID);
	bool DeleteMonth(CString strPath);
	bool DeleteDay(CString strPath);

	long GetSubSystemServerInfo(IN char* ip_port, IN char* code, OUT char* service_ip_port);
	long Exp_GetDir(IN TCHAR* tcMethod, IN TCHAR* tcParams);
	long Exp_DeleteDir(IN TCHAR* tcMethod, IN TCHAR* tcParams);
	long Exp_GetMemoryDir(IN TCHAR* tcMethod, IN TCHAR* tcMethod2, IN TCHAR* tcMac, IN TCHAR* tcResFlag,int Type);
	long GetDir(IN TCHAR* tcMethod, IN TCHAR* tcMethod2, IN TCHAR* tcMac, IN TCHAR* tcResFlag, int Type);
	long Exp_DeleteMemoryDir(IN TCHAR* tcMethod, IN TCHAR* tcMac, IN TCHAR* tcResFlag);
	int  BrowseCurrentDir(CString strDir, CArray<CString, CString&>& strDirInfo);
	void GetResourse();
	CString m_strMac;   //本机mac
	CString m_strDiskPath;	//挂载文件夹的路径
	CString m_strServerIP;
	CString strRspath;
	CString FilePath;
	std::string strPath;
	CString isOutTime;
	CString isMemoryOut;
	CArray<CString, CString&> dirInfo;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLEANRESOURSE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
