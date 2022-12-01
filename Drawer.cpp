#include"pch.h"
#include"Drawer.h"


void Drawer::Create(HWND hWnd)
{
	// Получаем указатель на окно.
	wnd = CWnd::FromHandle(hWnd);
	// Получаем прямоугольник окна.
	wnd->GetClientRect(frame);
	// Получаем контекст для рисования в этом окне.
	dc = wnd->GetDC();

	// Создаем буфер-контекст.
	memDC.CreateCompatibleDC(dc);
	// Создаем растр для контекста рисования.
	bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
	// Выбираем растр для использования буфер-контекстом.
	memDC.SelectObject(&bmp);
	init = true;
}


double Drawer::convert_range(double data, double outmax, double outmin, double inmax, double inmin)
{
	double k = (outmax - outmin) / (inmax - inmin);
	return  (data - inmin) * k + outmin;
}
vector<double> Drawer::convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin)
{
	vector<double> output = data;
	double k = (outmax - outmin) / (inmax - inmin);
	for (auto& item : output)
	{
		item = (item - inmin) * k + outmin;
	}

	return output;
}


void Drawer::DrawGraph(vector<double>& signal, double fd)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal.size();
	double data_x_min = 0;
	double data_x_max = (N - 1) / fd;
	double data_y_min = *min_element(signal.begin(), signal.end());
	double data_y_max = *max_element(signal.begin(), signal.end());

	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen);


	signal = convert_range(signal, actual_top, actual_bottom, data_y_max, data_y_min);


	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);


		memDC.LineTo(x, signal[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawSpectr(vector<double>& signal, double fd)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal.size();
	double data_x_min = 0;
	double data_x_max = fd - fd / N;
	double data_y_min = 0;
	double data_y_max = *max_element(signal.begin(), signal.end());


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen);


	signal = convert_range(signal, actual_top, actual_bottom, data_y_max, data_y_min);


	double len = fd / N;
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);


		memDC.LineTo(x, signal[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::DrawOverGraph(vector<double>& signal, double fd, int r, int g, int b)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(r, g, b));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal.size();
	double data_x_min = 0;
	double data_x_max = (N - 1) / fd;
	double data_y_min = *min_element(signal.begin(), signal.end());
	double data_y_max = *max_element(signal.begin(), signal.end());

	memDC.SelectObject(&data_pen);


	signal = convert_range(signal, actual_top, actual_bottom, data_y_max, data_y_min);


	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawOverSpectr(vector<double>& signal, double fd, int r, int g, int b)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(r, g, b));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal.size();
	double data_x_min = 0;
	double data_x_max = fd - fd / N;
	double data_y_min = 0;
	double data_y_max = *max_element(signal.begin(), signal.end());


	memDC.SelectObject(&data_pen);


	signal = convert_range(signal, actual_top, actual_bottom, data_y_max, data_y_min);


	double len = fd / N;
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::DrawTwoSignals(vector<double>& signal1, vector<double>& signal2, double fd)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen1(PS_SOLID, 3, RGB(200, 0, 0));
	CPen data_pen2(PS_SOLID, 2, RGB(0, 0, 200));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal1.size();
	double data_x_min = 0;
	double data_x_max = (N - 1) / fd;
	double data_y_min = min(*min_element(signal1.begin(), signal1.end()), *min_element(signal2.begin(), signal2.end()));
	double data_y_max = max(*max_element(signal1.begin(), signal1.end()), *max_element(signal2.begin(), signal2.end()));

	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen1);
	signal1 = convert_range(signal1, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal1[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal1[i]);
	}


	memDC.SelectObject(&data_pen2);
	signal2 = convert_range(signal2, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal2[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal2[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawTwoSpecters(vector<double>& signal1, vector<double>& signal2, double fd)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen1(PS_SOLID, 3, RGB(200, 0, 0));
	CPen data_pen2(PS_SOLID, 2, RGB(0, 0, 200));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal1.size();
	double data_x_min = 0;
	double data_x_max = fd;
	double data_y_min = 0;
	double data_y_max = max(*max_element(signal1.begin(), signal1.end()), *max_element(signal2.begin(), signal2.end()));



	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen1);
	signal1 = convert_range(signal1, actual_top, actual_bottom, data_y_max, data_y_min);
	double len = fd / (N - 1);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal1[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal1[i]);
	}

	memDC.SelectObject(&data_pen2);
	signal2 = convert_range(signal2, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal2[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal2[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::Clear()
{
	memDC.FillSolidRect(frame, RGB(255, 255, 255));
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
