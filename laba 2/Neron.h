#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <functional>
#include <math.h>

class Neron {
    public:
    std::vector<double> weights;
    std::vector<double> back_delta;
    //std::function<double(double, std::function<double(double)>)> f_derivative;
    double S; //summ x*w ...
    double Y; //predict
    double err_der;
    double err;
    double delta;
    Neron(int N, std::function<double(double, std::function<double(double)>)> f);
    Neron(int N, std::function<double(double, std::function<double(double)>)> f, double M);
    double Summ(std::vector<double>& X);
    double Err_der(double Y_true);
    double Activation(std::function<double(double)> F);
    void Update_weightsOutput(std::vector<double>& X, std::function<double(double)> F, double step);
    void Update_weightsInput(std::vector<double>& X, std::function<double(double)> F, double step);
};