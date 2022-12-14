#pragma once
#include"pch.h"
#include"SourceSignal.h"


void SourceSignal::setS1(SinParam item)
{
	s1 = item;
}
void SourceSignal::setS2(SinParam item)
{
	s2 = item;
}
void SourceSignal::setS3(SinParam item)
{
	s3 = item;
}
void SourceSignal::setN(int _N)
{
	N = _N;
}
void SourceSignal::setfd(double _fd)
{
	fd = _fd;
}
void SourceSignal::setNoiseGate(double NG)
{
	NoiseGate = NG;
}
void SourceSignal::setNoiseMax(double NMax)
{
	NoiseMax = NMax;
}

void SourceSignal::clear()
{
	signal_drv.Clear();
	Aspectr_drv.Clear();
	purified_signal_drv.Clear();


	signal.clear();
	noised_signal.clear();
	purified_signal.clear();
	fspectr.clear();
	purified_fspectr.clear();
	Aspectr.clear();
	purified_Aspectr.clear();
}


vector<cmplx> SourceSignal::getSignal()
{
	return signal;
}


//done
double SourceSignal::signalCalc(int i)
{
	double res = s1.A * sin(2 * pi * s1.f * i / fd + s1.y);
	res+= s2.A * sin(2 * pi * s2.f * i / fd + s2.y);
	res += s3.A * sin(2 * pi * s3.f * i / fd + s3.y);
	return res;
}
//done
void SourceSignal::CreateSignal()
{
	for (int i = 0; i < N; i++)
	{
		cmplx temp;
		temp.re = signalCalc(i);
		temp.im = 0;
		signal.push_back(temp);
	}


	addNoise();
}


vector<cmplx> SourceSignal::getSignal(SinParam _s1, SinParam _s2, SinParam _s3, int _N, double _fd)
{
	s1 = _s1;
	s2 = _s2;
	s3 = _s3;
	N = _N;
	fd = _fd;


	CreateSignal();


	return signal;
}


double SourceSignal::getLowestA()
{
	double res = min(s1.A, s2.A);
	res = min(res, s3.A);
	return res;
}
double SourceSignal::getHighestA()
{
	double res = max(s1.A, s2.A);
	res = max(res, s3.A);
	return res;
}


void SourceSignal::Draw()
{
	
	

	signal_drv.DrawGraph(getReal(signal), fd);
	Aspectr_drv.DrawSpectr(Aspectr, fd);
	purified_signal_drv.DrawGraph(getReal(purified_signal), fd);


	CString str;
	str.Format(L"В восстановленный сигнал вошли %i частот из %i.", fincluded, N);
	MessageBox(mainwnd, str, L"Результат", MB_OK);
}


void SourceSignal::setDrawerHWND(HWND hWnd)
{
	signal_drv.Create(hWnd);
}
void SourceSignal::setsignalNoNoiseDrvHWND(HWND hWnd)
{
	purified_signal_drv.Create(hWnd);
}
void SourceSignal::setspecDrawerHWND(HWND hWnd)
{
	Aspectr_drv.Create(hWnd);
}


vector<double> SourceSignal::getSpectr()
{
	return Aspectr;
}

//done
void SourceSignal::createSpectr()
{
	if (noised_signal.empty())return;


	fspectr = noised_signal;
	


	fourea(N, fspectr, -1);
	
	for (int i = 0; i < N; i++)
	{
		Aspectr.push_back(sqrt(fspectr[i].re * fspectr[i].re + fspectr[i].im * fspectr[i].im));
	}
}


//done
void SourceSignal::addNoise()
{
	vector<double>noise = generateNoiseBasics();

	double Es = calcES();
	double En = calcEN(noise);
	betta = sqrt(NoiseGate * Es / En);


	for (int i = 0; i < N; i++)
	{
		cmplx temp;
		temp.re = noise[i] * betta + signal[i].re;
		noised_signal.push_back(temp);
	}

}

//done
vector<double> SourceSignal::generateNoiseBasics()
{
	srand(time(NULL));


	vector<double> res;
	

	for (int i = 0; i < N; i++)
	{
		res.push_back(0);


		for (int g = 0; g < NOISE_NUM; g++)
		{
			res[i] += -1 + 2. * double(rand()) / RAND_MAX;
		}
	}


	return res;
}


double SourceSignal::calcES()
{
	double ES = 0;
	for (auto& item : signal)ES += item.re * item.re;
	return ES;
}
double SourceSignal::calcES(double* input, int len, int pos)
{
	double ES = 0;
	for (int i = 0; i < pos + 1; i++)
	{
		ES += input[i] * input[i];
		ES += input[N - i - 1] * input[N - i - 1];
	}
	return ES;
}


double SourceSignal::calcEN(vector<double>& item)
{
	double EN = 0;
	for (auto& obj : item)EN += obj * obj;
	return EN;
}


//testing
void SourceSignal::RemoveNoise()
{
	double Enoised = 0;
	for (auto& item : Aspectr)Enoised += item * item;

	Enoised *= NoiseMax;
	double Etemp = 0;
	int l = 0;
	int r = N - 1;
	while (l < r)
	{
		Etemp += Aspectr[l] * Aspectr[l];
		Etemp += Aspectr[r] * Aspectr[r];
		l++;
		r--;
		if (Etemp > Enoised)break;
	}

	if (NoiseMax == 0)l = 0;
	fincluded = 2 * l;


	purified_signal = vector<cmplx>(N, cmplx());
	for (int i = 0; i < l; i++)
	{
			purified_signal[i] = fspectr[i];
			purified_signal[N - 1 - i] = fspectr[N - 1 - i];
	}


	fourea(N, purified_signal, 1);
}



void SourceSignal::start()
{
	CreateSignal();

	createSpectr();
	RemoveNoise();
	createPurifiedSpectr();

	signal_drv.DrawTwoSignals(getReal(signal), getReal(noised_signal), fd);
	purified_signal_drv.DrawTwoSignals(getReal(signal), getReal(purified_signal), fd);
	Aspectr_drv.DrawTwoSpecters(Aspectr, purified_Aspectr, fd);

}



vector<double> SourceSignal::getReal(vector<cmplx>& obj)
{
	vector<double>real;
	for (auto& item : obj)
	{
		real.push_back(item.re);
	}
	return real;
}


void SourceSignal::createPurifiedSpectr()
{
	if (purified_signal.empty())return;


	purified_fspectr = purified_signal;


	fourea(N, purified_fspectr, -1);

	for (int i = 0; i < N; i++)
	{
		purified_Aspectr.push_back(sqrt(purified_fspectr[i].re * purified_fspectr[i].re + purified_fspectr[i].im * purified_fspectr[i].im));
	}
}


float SourceSignal::estimate()
{
	double Es = 0;
	for (auto& item : signal)Es += item.re * item.re;
	double delta = 0;
	for (int i = 0; i < N; i++)delta += (signal[i].re - purified_signal[i].re) * (signal[i].re - purified_signal[i].re);
	return delta / Es;
}