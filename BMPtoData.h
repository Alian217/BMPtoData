
// BMPtoData.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBMPtoDataApp:
// �йش����ʵ�֣������ BMPtoData.cpp
//

class CBMPtoDataApp : public CWinApp
{
public:
	CBMPtoDataApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBMPtoDataApp theApp;