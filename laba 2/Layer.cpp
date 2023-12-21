#include "Layer.h"


void Layer::forvardOutput(std::vector<double> input, std::function<double(double)> activation, std::vector<double> Y_true) {
    std::vector<double> out;
    for(int i = 0; i < Nerons.size(); i++) 
    {
        Nerons[i].Summ(input);
        double o;
        o = Nerons[i].Activation(activation);
        out.push_back(o);
        Nerons[i].Err_der(Y_true[i]);
    }
    Input = input;
    Out = out;
}

void Layer::backvardOutput(std::function<double(double)> F) {
    for(int i = 0; i < Nerons.size(); i++) 
    {
        Nerons[i].Update_weightsOutput(Input, F, step);
    }
}

void Layer::setError(Layer& next_layer, std::function<double(double)> F) {
    std::vector<double> err_next = next_layer.getError();
    
    for(int i = 0; i <  Nerons.size(); i++) 
    {   
        double sum_w_err = 0.0;
        for (int j = 0; j < next_layer.Nerons.size(); j++) {
            Neron n_next = next_layer.Nerons[j];
            sum_w_err += n_next.delta*n_next.weights[i];
        }
        
        Nerons[i].delta = sum_w_err*f_derivative(Nerons[i].S, F);
     //   std::cout << "delta: " << Nerons[i].delta << std::endl; // = " << Nerons[i].err << " sum_w_err: "<< sum_w_err <<  "f_derivative(n.S, F): " << f_derivative(n.S, F) << std::endl;
    }
}

void Layer::backvardInput(std::function<double(double)> F) {
    long len_n = Nerons.size();
    for(int i = 0; i < len_n; i++) 
    {   
        Nerons[i].Update_weightsInput(Input, F, step_input);
    }
}

void Layer::forvardInput(std::vector<double>& input, std::function<double(double)> activation) {
    long len_n = Nerons.size();
    std::vector<double> out;
    for(int i = 0; i < len_n; i++) 
    {
        Nerons[i].Summ(input);
        double o = Nerons[i].Activation(activation);
        out.push_back(o);
        //std::cout << "Summ[i] = " <<  Nerons[i].Summ(input) << " Out[i] = " << o << std::endl;
    }
    this->Input = input;
    this->Out = out;
}


std::vector<double> Layer::getError() {
    std::vector<double> err_out;
    long len_n = Nerons.size();
    for (int i = 0; i < len_n; i++) {
        err_out.push_back(Nerons[i].err);
    }
    return err_out;
}

std::vector<double> Layer::getDerError() {
    std::vector<double> err_out;
    long len_n = Nerons.size();
    for (int i = 0; i < len_n; i++) {
        err_out.push_back(Nerons[i].err_der);
    }
    return err_out;
}



double Layer::f_derivative(double a, std::function<double(double)> F) {
    double delta = 1e-5;
    return (F(a + delta) - F(a-delta))/((double)2.0*delta);
}



void Layer::set0layer(std::vector<double>& input,  std::function<double(double)> F) {
    std::vector<double> out;
    long len_n = Nerons.size();
    for (size_t i = 0; i < len_n; i++)
    {
        Nerons[i].S = input[i];
        //auto o = Nerons[i].Activation(F);
        out.push_back(input[i]);
    }
    Input = input;
    Out = out;
}