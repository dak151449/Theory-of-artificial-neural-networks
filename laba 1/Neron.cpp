#include "Neron.h"

Neron::Neron(int N, std::function<double(double, std::function<double(double)>)> f) {
    f_derivative = f;
    weights = {};
    for (size_t i = 0; i < N; i++)
    {
        weights.push_back(0.0);
    }
    
}

double Neron::Summ(std::vector<int> X) {
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
    err_der = -2.0 * (Y_true - Y);
    err = abs(Y_true - Y);
    return err_der;
}


void Neron::Update_weights(std::vector<int> X, std::function<double(double)> F, double step) {
    double dL_d_out = err_der;
    for (size_t j = 0; j < weights.size(); j++) 
    {
        double d_out_dwji = double(X[j]) * f_derivative(S, F);
        double d_L_d_wji = dL_d_out * d_out_dwji;
        weights[j] -= step * d_L_d_wji;
    }
}