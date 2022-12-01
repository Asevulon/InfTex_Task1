#pragma once

#include<vector>
#include<fstream>
#include"Drawer.h"
#include"cmplx.h"
#include<thread>
using namespace std;


#define NOISE_NUM 12


static HWND mainwnd;
const double pi = 4 * atan(1);


//структура под параметры синусоид
struct SinParam
{
	double A = 0;
	double f = 0;
	double y = 0;


	SinParam() :A(0), f(0), y(0) {}
	SinParam(double A, double f, double y) :A(A), f(f), y(y) {};
};


//класс, который расчитывает заданный сигнал
class SourceSignal
{
private:
	//параметры синусоид
	SinParam s1;
	SinParam s2;
	SinParam s3;


	//число точек
	int N;
	int fincluded;

	//частота приема точек
	double fd;


	//уровень шума
	double NoiseGate;
	double betta;
	double NoiseMax;


	//исходный сигнал
	vector<cmplx> signal;
	//зашумленный сигнал
	vector<cmplx> noised_signal;
	//очищенный сигнал
	vector<cmplx> purified_signal;
	//частотный спектр исходного сигнала
	vector<cmplx> fspectr;
	//частотный спектр очищенного сигнала
	vector<cmplx> purified_fspectr;
	//амплитудный спектр зашумленного сигнала
	vector<double> Aspectr;
	//амплитудный спектр очищенного сигнала
	vector<double> purified_Aspectr;


	//рисовальщики
	Drawer signal_drv;
	Drawer Aspectr_drv;
	Drawer purified_signal_drv;


protected:
	//считает сигнал в конкретной точке
	double signalCalc(int i);


	double getLowestA();
	double getHighestA();


	void addNoise();
	vector<double> generateNoiseBasics();
	double calcES();
	double calcEN(vector<double>& item);
	double calcES(double* input, int len, int pos);
	

	vector<double>getReal(vector<cmplx>& obj);
	void createPurifiedSpectr();

public:
	void Draw();
	void setDrawerHWND(HWND hWnd);
	void setspecDrawerHWND(HWND hWnd);
	void setsignalNoNoiseDrvHWND(HWND hWnd);
	


	void setS1(SinParam item);
	void setS2(SinParam item);
	void setS3(SinParam item);
	void setN(int N);
	void setfd(double fd);
	void setNoiseGate(double NG);
	void setNoiseMax(double NMax);

	
	//очищает массив отсчетов
	void clear();


	vector<cmplx> getSignal();


	//по переданным параметрам перерасчитывает и возвращает сигнал
	vector<cmplx> getSignal(SinParam _s1, SinParam _s2, SinParam _s3, int _N, double _fd);


	//создает сигнал по заданным параметрам
	void CreateSignal();


	void createSpectr();
	vector<double> getSpectr();


	void RemoveNoise();


	void start();
	
	//возвращает частное среднеквадратичного отклонения и энергии заданного чистого сигнала
	float estimate();
};


