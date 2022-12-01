
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "InfTex_Task1.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INFTEX_TASK1_DIALOG, pParent)
	, A1(1)
	, f1(2)
	, y1(3)
	, A2(0)
	, f2(5)
	, y2(6)
	, A3(0)
	, f3(8)
	, y3(9)
	, N(512)
	, fd(200)
	, NG(10)
	, NMax(90)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, A1ctrl);
	DDX_Control(pDX, IDC_EDIT2, f1ctrl);
	DDX_Control(pDX, IDC_EDIT3, y1ctrl);
	DDX_Control(pDX, IDC_EDIT4, A2ctrl);
	DDX_Control(pDX, IDC_EDIT5, f2ctrl);
	DDX_Control(pDX, IDC_EDIT6, y2ctrl);
	DDX_Control(pDX, IDC_EDIT7, A3ctrl);
	DDX_Control(pDX, IDC_EDIT8, f3ctrl);
	DDX_Control(pDX, IDC_EDIT9, y3ctrl);
	DDX_Control(pDX, IDC_EDIT10, Nctrl);
	DDX_Control(pDX, IDC_EDIT11, fdctrl);
	DDX_Text(pDX, IDC_EDIT1, A1);
	DDX_Text(pDX, IDC_EDIT2, f1);
	DDX_Text(pDX, IDC_EDIT3, y1);
	DDX_Text(pDX, IDC_EDIT4, A2);
	DDX_Text(pDX, IDC_EDIT5, f2);
	DDX_Text(pDX, IDC_EDIT6, y2);
	DDX_Text(pDX, IDC_EDIT7, A3);
	DDX_Text(pDX, IDC_EDIT8, f3);
	DDX_Text(pDX, IDC_EDIT9, y3);
	DDX_Text(pDX, IDC_EDIT10, N);
	DDX_Text(pDX, IDC_EDIT11, fd);
	DDX_Text(pDX, IDC_EDIT12, NG);
	DDX_Text(pDX, IDC_EDIT13, NMax);
	DDX_Control(pDX, IDC_EDIT14, estimatectrl);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &MyDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &MyDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	Ssign.setDrawerHWND(GetDlgItem(IDC_SSIGNAL)->GetSafeHwnd());
	Ssign.setspecDrawerHWND(GetDlgItem(IDC_SPECTR)->GetSafeHwnd());
	Ssign.setsignalNoNoiseDrvHWND(GetDlgItem(IDC_NO_NOISE_PICTURE)->GetSafeHwnd());

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	PostQuitMessage(0);
}


void MyDlg::OnBnClickedOk()
{
	Ssign.clear();
	mainwnd = this->GetSafeHwnd();
	
	UpdateData(TRUE);


	if (!isNCorect())return;


	Ssign.setfd(fd);
	Ssign.setN(N);
	Ssign.setS1(SinParam(A1, f1, y1));
	Ssign.setS2(SinParam(A2, f2, y2));
	Ssign.setS3(SinParam(A3, f3, y3));
	Ssign.setNoiseGate(NG / 100.);
	Ssign.setNoiseMax(NMax / 100.);


	Ssign.start();

	CString str;
	float est = 100 * Ssign.estimate();
	str.Format(L"%10.2f", est);
	str += '%';
	estimatectrl.SetWindowTextW(str);
}


void MyDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	Ssign.clear();
}


BOOL MyDlg::isNCorect()
{
	if (log2(N) - int(log2(N)) != 0 || N < 2)
	{
		CString str;

		if (N < 2)
		{
			str.Format(L"Требуется вводить число отсчетов N равным 2 в какой-либо степени. Ближайшее подходящее значение:%i", 2);
		}
		else
		{
			int closest = pow(2, round(log2(N)));
			str.Format(L"Требуется вводить число отсчетов N равным 2 в какой-либо степени. Ближайшее подходящее значение:%i", closest);
		}
		MessageBox(str, L"Ошибка: некорректное значение N");
		return FALSE;
	}
	return TRUE;
}