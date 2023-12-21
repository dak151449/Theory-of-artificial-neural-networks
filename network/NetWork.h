#pragma once
#include "ActivateFunction.h"
#include "Matrix.h"
#include <fstream>
#include <vector>
using namespace std;
struct data_NetWork {
	int L;
	int* size;
};
class NetWork
{
	int L;
	int* size;
	ActivateFunction actFunc;
	Matrix* weights;
	double** bios; 
	double** neurons_val, ** neurons_err;
	double* neurons_bios_val;
	std::vector<std::vector<double>> neurons_D_back;
public:
	void Init(data_NetWork data);
	void PrintConfig();
	void SetInput(double* values);

	double ForwardFeed();
	int SearchMaxIndex(double* value);
	void PrintValues(int L);

	void BackPropogation(double expect);
	void WeightsUpdater(double lr);

	void SaveWeights();
	void ReadWeights();

	double CE(double expect, double* pred);
	double SoftMax(double yi, double* y);
};

