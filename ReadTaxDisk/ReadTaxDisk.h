
// ReadTaxDisk.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CReadTaxDiskApp:
// �йش����ʵ�֣������ ReadTaxDisk.cpp
//

class CReadTaxDiskApp : public CWinApp
{
public:
	CReadTaxDiskApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CReadTaxDiskApp theApp;