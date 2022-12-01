
// InfTex_Task1.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CInfTexTask1App:
// Сведения о реализации этого класса: InfTex_Task1.cpp
//

class CInfTexTask1App : public CWinApp
{
public:
	CInfTexTask1App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CInfTexTask1App theApp;
