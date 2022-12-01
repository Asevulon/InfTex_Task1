
// MyDlg.h: файл заголовка
//

#pragma once
#include"SourceSignal.h"

// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INFTEX_TASK1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	BOOL isNCorect();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();


	SourceSignal Ssign;
	CEdit A1ctrl;
	CEdit f1ctrl;
	CEdit y1ctrl;
	CEdit A2ctrl;
	CEdit f2ctrl;
	CEdit y2ctrl;
	CEdit A3ctrl;
	CEdit f3ctrl;
	CEdit y3ctrl;
	CEdit Nctrl;
	CEdit fdctrl;
	afx_msg void OnBnClickedButton1();
	double A1;
	double f1;
	double y1;
	double A2;
	double f2;
	double y2;
	double A3;
	double f3;
	double y3;
	int N;
	double fd;
	double NG;
	double NMax;
	CEdit estimatectrl;
};
