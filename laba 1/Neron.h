#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <functional>

class Neron {
    public:
    std::vector<double> weights;
    std::function<double(double, std::function<double(double)>)> f_derivative;
    double S; //summ x*w ...
    double Y; //predict
    double err_der;
    double err;
    Neron(int N, std::function<double(double, std::function<double(double)>)> f);
    double Summ(std::vector<int> X);
    double Err_der(double Y_true);
    double Activation(std::function<double(double)> F);
    void Update_weights(std::vector<int> X, std::function<double(double)> F, double step);
};