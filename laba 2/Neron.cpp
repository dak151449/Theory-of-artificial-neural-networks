#include "Neron.h"

double f_derivative(double a, std::function<double(double)> F) {
    double delta = 1e-5;
    return (F(a + delta) - F(a-delta))/((double)2.0*delta);
}

Neron::Neron(int N, std::function<double(double, std::function<double(double)>)> f) {
    //f_derivative = f;
    weights = {};
    back_delta = {};
    for (size_t i = 0; i < N; i++)
    {
        weights.push_back(1.0); ///rand());
        back_delta.push_back(0.0);
    }
    
}

Neron::Neron(int N, std::function<double(double, std::function<double(double)>)> f, double A) {
    weights = {};
    S = A;   
}

double Neron::Summ(std::vector<double>& X) {
    double s = 0.0;
    for (int i = 0; i < X.size(); i++) {
        s += weights[i] * X[i];
    }
    S = s;
    return s;
}

double Neron::Activation(std::function<double(double)> F) {
    Y = F(S);
    return Y;
}

double Neron::Err_der(double Y_true) {
    err_der = double(-2.0) * (Y_true - Y);
    err = abs(Y_true - Y);
    return err_der;
}

void Neron::Update_weightsOutput(std::vector<double>& X, std::function<double(double)> F, double step) {
    double dL_d_out = err_der;
    delta = err_der*f_derivative(S, F);
    std::vector<double> back_d;
    for (size_t j = 0; j < weights.size(); j++) 
    {
        weights[j] -= step * err_der * double(X[j]);
        back_d.push_back(step * err_der * double(X[j]));
        // if (X[j] != 0) 
        //     std::cout << "Weights: " << weights[j] << " delta: " << step * d_L_d_wji << " err_der: " << err_der << " "  << f_derivative(S, F) << "    S: "<< S << " X[j]: "<< X[j] << std::endl;
    }
    back_delta = back_d;
}

void Neron::Update_weightsInput(std::vector<double>& X, std::function<double(double)> F, double step) {
    long len_w = weights.size();
    std::vector<double> back_d;
    for (size_t j = 0; j < len_w; j++) 
    {   
        weights[j] -= step * delta * double(X[j]) + 0,01*back_delta[j];
        back_d.push_back(step * delta * double(X[j]));
        // if (X[j] != 0) 
        //     std::cout << "Weights: " << weights[j] << " delta: " << delta << " "  << double(X[j]) << std::endl;
    }
    back_delta = back_d;
}