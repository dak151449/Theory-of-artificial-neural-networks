#pragma once
#include "Neron.h"

class Layer {
    public:
        std::vector<Neron> Nerons;
        std::vector<double> Out;
        std::vector<double> Input;
        double step =  1e-2; // 3e-5 для relu
        double step_input = 1e-3;
        void forvardOutput(std::vector<double> input, std::function<double(double)> activation, std::vector<double> Y_true);    
        void backvardOutput(std::function<double(double)> F);
        void setError(Layer& next_layer, std::function<double(double)> F);

        void backvardInput(std::function<double(double)> F);
        void forvardInput(std::vector<double>& input, std::function<double(double)> activation); 

        std::vector<double> getError(); 
        std::vector<double> getDerError();     
        double f_derivative(double a, std::function<double(double)> F);

        void set0layer(std::vector<double>& input,  std::function<double(double)> F);
};