#include <iostream>
#include <vector>
#include <map>
#include <math.h>

typedef float (*func)(float a);

float f_lin(float a) {
    return a;
}


float f_(float a) {
    return a;
}

float f_relu(float a) {
    if (a > 0) {
        return a;
    }
    return 0;
}


float f_sigmoid(float a) {
    return 1/(1 + exp(-a));
}

float f_gipthan(float a) {
    return (exp(a) - exp(-a))/(exp(a) + exp(-a));
}

int vector_sum(std::vector<int> in) {
    int s = 0;
    for(auto& elem: in) {
        s += elem;
    }
    return s;
}



std::vector<float> Summ(std::vector<int> X, std::vector<std::vector<float>> W) {
    std::vector<float> out;
    for (size_t i = 0; i < W[0].size(); i++)
    {   
        int s = 0;
        for (size_t j = 0; j < X.size(); j++)
        {
            s += X[j]*W[j][i];
        }
        out.push_back(s);
    }
    
    return out;
}

std::vector<float> activation_func(std::vector<float> S, std::vector<std::vector<float>> W, func F) {
    std::vector<float> out;
    for (size_t i = 0; i < S.size(); i++)
    {
        out.push_back(F(S[i]));
    }
    
    return out;
}

std::vector<float> Err(std::vector<float> Y, std::vector<float> Z) {
    std::vector<float> out;
    for (int i = 0; i < Y.size(); i++) {
        out.push_back((Y[i]-Z[i])*(Y[i]-Z[i]));
    }
}

std::map<std::string, std::vector<float>> forvard(std::vector<int> X, std::vector<std::vector<float>> W, func F, std::vector<float> Y_pred) {

    std::map<std::string, std::vector<float>> out;
    std::vector<float> Y = Summ(X, W);
    std::vector<float> Z = activation_func(Y, W, F);
    
    //int Y_pred = vector_sum(Z);
    out["PRED"] = Z;
    out["ERR"] = Err(Y_pred, Z);
    return out;
}

void backvard() {

}


int main() {
    std::vector<int> image;

    std::vector<std::vector<int>> W;
    std::vector<int> X;
    int M = 10; // количество нейронов

    for (size_t i = 0; i < X.size(); i++)
    {
        W.push_back({});
        for (size_t j = 0; i < M; j++)
        {
            W[i].push_back(1);
        }
        
    }


    


    image = {};
    return 0;
}