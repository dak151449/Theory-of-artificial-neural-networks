#include "NetWork.h"
void NetWork::Init(data_NetWork data) {
	actFunc.set();
	srand(time(NULL));
	L = data.L;
	size = new int[L];
	for (int i = 0; i < L; i++)
		size[i] = data.size[i];

	weights = new Matrix[L - 1];
	bios = new double* [L - 1];
	for (int i = 0; i < L - 1; i++) {
		weights[i].Init(size[i + 1], size[i]);
		bios[i] = new double[size[i + 1]];
		weights[i].Rand();
		for (int j = 0; j < size[i + 1]; j++) {
			bios[i][j] = ((rand() % 50)) * 0.06 / (size[i] + 15);
		}
	}
	neurons_val = new double* [L]; neurons_err = new double* [L];
	for (int i = 0; i < L; i++) {
		neurons_val[i] = new double[size[i]]; neurons_err[i] = new double[size[i]];
	}
	neurons_bios_val = new double[L - 1];
	for (int i = 0; i < L - 1; i++)
		neurons_bios_val[i] = 1;
}
void NetWork::PrintConfig() {
	cout << "***********************************************************\n";
	cout << "NetWork has " << L << " layers\nSIZE[]: ";
	for (int i = 0; i < L; i++) {
		cout << size[i] << " ";
	}
	cout << "\n***********************************************************\n\n";
}
void NetWork::SetInput(double* values) {
	for (int i = 0; i < size[0]; i++) {
		neurons_val[0][i] = values[i];
	}
}

double NetWork::ForwardFeed() {
	for (int k = 1; k < L; ++k) {
		Matrix::Multi(weights[k - 1], neurons_val[k - 1], size[k - 1], neurons_val[k]);
		Matrix::SumVector(neurons_val[k], bios[k - 1], size[k]);
		actFunc.use(neurons_val[k], size[k]);
	}
	int pred = SearchMaxIndex(neurons_val[L - 1]);
	return pred;
}
int NetWork::SearchMaxIndex(double* value) {
	double max = value[0];
	int prediction = 0;
	double tmp;
	for (int j = 1; j < size[L - 1]; j++) {
		tmp = value[j];
		if (tmp > max) {
			prediction = j;
			max = tmp;
		}
	}
	return prediction;
}
void NetWork::PrintValues(int L) {
	for (int j = 0; j < size[L]; j++) {
		cout << j << " " << neurons_val[L][j] << endl;
	}
}

double NetWork::CE(double expect, double* pred) {
	// double lce = 0.0;
	// for (int i = 0; i < size[L - 1]; i++) {
	// 	if (i == int(expect)) {
	// 		lce = -1.0 * log2(pred[i]);
	// 	}
	// }
	return 1.0 * log2(pred[int(expect)]);
}

double NetWork::SoftMax(double yi, double* y) {
	double sum = 0.0;
	for (int i = 0; i < size[L - 1]; i++) {
		sum += exp(y[i]);
	}
	return exp(yi) / sum;
}

std::vector<std::vector<double>> getB(std::vector<std::vector<double>> new_D,std::vector<std::vector<double>> back_D) {
	std::vector<std::vector<double>> B;
	
}

void NetWork::BackPropogation(double expect) {
	
	for (int i = 0; i < size[L - 1]; i++) {
		
		if (i != int(expect))
			neurons_err[L - 1][i] = -neurons_val[L - 1][i] * actFunc.useDer(neurons_val[L - 1][i]) / double(size[L-1]);
		else
			neurons_err[L - 1][i] = (1.0 - neurons_val[L - 1][i]) * actFunc.useDer(neurons_val[L - 1][i]) / double(size[L-1]);
		
		//neurons_err[L - 1][i] = CE(expect, neurons_val[L - 1]) * actFunc.useDer(neurons_val[L - 1][i]);
		// if (i != int(expect)) {
		// 	neurons_err[L - 1][i] = 0; //(1.0/(neurons_val[L - 1][i])) * actFunc.useDer(neurons_val[L - 1][i]);
		// } else {
		// 	neurons_err[L - 1][i] = (1.0 / neurons_val[L - 1][i]) * actFunc.useDer(neurons_val[L - 1][i]);
		// 	// neurons_err[L - 1][i] = -log2(SoftMax(neurons_val[L - 1][int(expect)], neurons_val[L - 1])) * actFunc.useDer(neurons_val[L - 1][i]);
		// }
	}

	for (int k = L - 2; k > 0; k--) {
		Matrix::Multi_T(weights[k], neurons_err[k + 1], size[k + 1], neurons_err[k]);
		for (int j = 0; j < size[k]; j++)
			neurons_err[k][j] *= actFunc.useDer(neurons_val[k][j]);
	}
	std::vector<std::vector<double>> neurons_D_new;
	for (int i = 0; i < L - 1; i++) {
		neurons_D_new.push_back({});
		for (int j = 0; j < size[i + 1]; j++) {
			neurons_D_new[i].push_back(neurons_err[i + 1][j]);
		}
	}

}
void NetWork::WeightsUpdater(double lr) {
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			for (int k = 0; k < size[i]; ++k) {
				weights[i](j, k) += neurons_val[i][k] * neurons_err[i + 1][j] * lr;
			}
		}
	}
	for (int i = 0; i < L - 1; i++) {
		for (int k = 0; k < size[i + 1]; k++) {
			bios[i][k] += neurons_err[i + 1][k] * lr;
		}
	}
}
void NetWork::SaveWeights() {
	ofstream fout;
	fout.open("Weights.txt");
	if (!fout.is_open()) {
		cout << "Error reading the file";
		system("pause");
	}
	for (int i = 0; i < L - 1; ++i)
		fout << weights[i] << " ";

	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			fout << bios[i][j] << " ";
		}
	}
	cout << "Weights saved \n";
	fout.close();
}
void NetWork::ReadWeights() {
	ifstream fin;
	fin.open("Weights.txt");
	if (!fin.is_open()) {
		cout << "Error reading the file";
		system("pause");
	}
	for (int i = 0; i < L - 1; ++i) {
		fin >> weights[i];
	}
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			fin >> bios[i][j];
		}
	}
	cout << "Weights readed \n";
	fin.close();
}