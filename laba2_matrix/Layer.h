#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <stdlib.h>


class Layer {
    public:
        Layer(int size, int prev_size);
        Layer(int size);
        std::vector<double> Out;
        std::vector<double> Input;
        std::vector<double> Summ;
        
        double step =  3e-5; // 3e-5 для relu
        double step_input = 1e-3;

        std::vector<std::vector<double>> W;

        static std::vector<double> Multi(const Layer& m, const std::vector<double>& b);
        static std::vector<double> Multi_T(const Layer& m, const std::vector<double>& b);
        static std::vector<double> SumVector(const std::vector<double>& v1, const std::vector<double>& v2);
        void printW();
};