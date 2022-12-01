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


//��������� ��� ��������� ��������
struct SinParam
{
	double A = 0;
	double f = 0;
	double y = 0;


	SinParam() :A(0), f(0), y(0) {}
	SinParam(double A, double f, double y) :A(A), f(f), y(y) {};
};


//�����, ������� ����������� �������� ������
class SourceSignal
{
private:
	//��������� ��������
	SinParam s1;
	SinParam s2;
	SinParam s3;


	//����� �����
	int N;
	int fincluded;

	//������� ������ �����
	double fd;


	//������� ����
	double NoiseGate;
	double betta;
	double NoiseMax;


	//�������� ������
	vector<cmplx> signal;
	//����������� ������
	vector<cmplx> noised_signal;
	//��������� ������
	vector<cmplx> purified_signal;
	//��������� ������ ��������� �������
	vector<cmplx> fspectr;
	//��������� ������ ���������� �������
	vector<cmplx> purified_fspectr;
	//����������� ������ ������������ �������
	vector<double> Aspectr;
	//����������� ������ ���������� �������
	vector<double> purified_Aspectr;


	//������������
	Drawer signal_drv;
	Drawer Aspectr_drv;
	Drawer purified_signal_drv;


protected:
	//������� ������ � ���������� �����
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

	
	//������� ������ ��������
	void clear();


	vector<cmplx> getSignal();


	//�� ���������� ���������� ��������������� � ���������� ������
	vector<cmplx> getSignal(SinParam _s1, SinParam _s2, SinParam _s3, int _N, double _fd);


	//������� ������ �� �������� ����������
	void CreateSignal();


	void createSpectr();
	vector<double> getSpectr();


	void RemoveNoise();


	void start();
	
	//���������� ������� ������������������� ���������� � ������� ��������� ������� �������
	float estimate();
};


