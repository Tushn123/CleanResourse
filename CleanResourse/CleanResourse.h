
// CleanResourse.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCleanResourseApp: 
// �йش����ʵ�֣������ CleanResourse.cpp
//

class CCleanResourseApp : public CWinApp
{
public:
	CCleanResourseApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCleanResourseApp theApp;