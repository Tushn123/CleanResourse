
// CleanResourseDlg.h : ͷ�ļ�
//
#include <vector>


#pragma once
#include "PublicFunc.h"

// CCleanResourseDlg �Ի���
class CCleanResourseDlg : public CDialogEx
{
// ����
public:
	CCleanResourseDlg(CWnd* pParent = NULL);	// ��׼���캯��
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
	CString m_strMac;   //����mac
	CString m_strDiskPath;	//�����ļ��е�·��
	CString m_strServerIP;
	CString strRspath;
	CString FilePath;
	std::string strPath;
	CString isOutTime;
	CString isMemoryOut;
	CArray<CString, CString&> dirInfo;
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLEANRESOURSE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
