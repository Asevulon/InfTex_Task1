#pragma once

#include <afxwin.h>
#include <vector>
#include<algorithm>
using namespace std;


class Drawer
{
	// Прямоугольник области рисования.
	CRect frame;
	// Указатель на объект окна, обалсти рисования.
	CWnd * wnd;
	// Контекст рисования, привязанный к области рисования.
	CDC * dc;
	// Контекст рисования, привязанный к памяти.
	CDC memDC;
	// Память для контекста рисования memDC.
	CBitmap bmp;
	// Флаг для отслеживания состояния инициализации класса.
	bool init;

protected:
	double convert_range(double data, double outmax, double outmin, double inmax, double inmin);
	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin);


public:
	// Проинициализировать объект класса на основе HWND.
	void Create(HWND hWnd);
	
	void DrawGraph(vector<double>& signal, double fd);
	void DrawSpectr(vector<double>& signal, double fd);
	void DrawOverGraph(vector<double>& signal, double fd, int r = 0, int g = 255, int b = 0);
	void DrawOverSpectr(vector<double>& signal, double fd, int r = 0, int g = 255, int b = 0);
	void DrawTwoSignals(vector<double>& signal1, vector<double>& signal2, double fd);
	void DrawTwoSpecters(vector<double>& signal1, vector<double>& signal2, double fd);


	void Clear();
};
