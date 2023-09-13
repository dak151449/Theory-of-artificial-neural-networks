#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Func_image.h"


float step;

//typedef float (*func)(float a);

float f_lin(float a) {
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

float f_sigmoid_der(float a) {
    return f_sigmoid(a) * (1 - f_sigmoid(a));
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

float f_derivative(float a, func F) {
    float delta = 0.01;
    return (F(a + delta) - F(a-delta))/(2*delta);
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

std::vector<float> activation_func(std::vector<float> S, func F) {
    std::vector<float> out;
   // out = new std::vector<float>;
    for (size_t i = 0; i < S.size(); i++)
    {
        out.push_back(F(S[i]));
    }
    
    return out;
}

std::vector<float> Err(std::vector<float> Y, std::vector<float> Z) {
    std::vector<float> out;
    for (int i = 0; i < Y.size(); i++) {
        out.push_back(-2*(Y[i]-Z[i]));
    }
    return out;
}

std::map<std::string, std::vector<float>> forvard(std::vector<int> X, std::vector<std::vector<float>> W, func F, std::vector<float> Y_pred) {

    std::map<std::string, std::vector<float>> out;
    std::vector<float> Y = Summ(X, W);
    std::vector<float> Z = activation_func(Y, F);
    
    //int Y_pred = vector_sum(Z);
    out["SUM"] = Y;
    out["PRED"] = Z;
    out["ERR"] = Err(Y_pred, Z);
    return out;
}

void backvard(std::map<std::string, std::vector<float>> out, std::vector<std::vector<float>>& W, std::vector<int> X, func F) {

    

    for (size_t i = 0; i < W[0].size(); i++) // количество нейронов 
    {
        float dL_d_out = out["ERR"][i];
        for (size_t j = 0; j < W.size(); j++) // количество весов к одному нейрону i - index нейрона 
        {   
            // W[j][i];
            // float d_out_dwji = X[j] * f_sigmoid_der(out["SUM"][i]);
            float d_out_dwji = X[j] * f_derivative(out["SUM"][i], F);
            float d_L_d_wji = dL_d_out * d_out_dwji;
            W[j][i] -= step * d_L_d_wji;
           // std::cout << "d_L_d_wji: " <<  f_derivative(out["SUM"][i], F) << std::endl;
        }
        
    }
    
}


int main() {
    std::vector<int> image_1 = {0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1};
    std::vector<float> image_1_pred = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int w, h, nrChanel;

    std::vector<std::vector <int>> images;
    std::vector<std::vector <float>> images_pred;

    for(int im = 0; im < 10; im++) {
        std::vector<int> image_test;
        std::cout << "training_data/" + std::to_string(im) + ".bmp" << std::endl;
        uint8_t *data  = stbi_load(("training_data/" + std::to_string(im) + ".bmp").c_str(), &w, &h, &nrChanel, 4);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j+=1) {
                if (static_cast<int>(data[i*16 + j*4 + 0]) == 0) {
                    image_test.push_back(1);
                    std::cout << 1;
                } else {
                    image_test.push_back(0);
                    std::cout << 0;
                }
            }
            std::cout << std::endl;
        }
        images.push_back(image_test);
        std::vector<float> image_pred_test;
        for (int i = 0; i < 10; i++) {
            if (i == im) {
                image_pred_test.push_back(1);
            } else {
                image_pred_test.push_back(0);
            }
        }
        images_pred.push_back(image_pred_test);
    }

    

    // images.push_back(image_1);
    // images_pred.push_back(image_1_pred);

   
    // sig gipthan step = 0.1 
    // lin step = 0.05
   
    std::vector<func> Fs =  {&f_sigmoid, &f_lin, &f_relu, &f_gipthan};
    std::vector<std::string> F_name = {"sigmoid", "lin", "relu", "gipthan"};
    for (int i = 0; i < Fs.size(); i++) {
        std::vector<std::vector<float>> W;
    
        int M = 10; // количество нейронов
        for (size_t i = 0; i < images[0].size(); i++)
        {
            W.push_back({});
            for (size_t j = 0; j < M; j++)
            {
                W[i].push_back(0);
            }
            
        }
        if (F_name[i] == "sigmoid" || F_name[i] == "gipthan") {
            step = 0.1;
            std::cout << "+++++++++++++++++++++++++++++++++++\n";
        } else if (F_name[i] == "lin" || F_name[i] == "relu"){
            step = 0.1; 
            std::cout << "-----------------------------------\n";
        }
        std::vector<float> loss;
        std::vector<float> epochs;
        int epocha = 10000;
        int EP = epocha;
        func F = Fs[i];
        
        while (epocha) {
            float S = 0;
            int len = 0;
            for (int i = 0; i < images.size(); i++)
            {   
                auto out = forvard(images[i], W, F, images_pred[i]);
                backvard(out, W, images[i], F);
                len = out["ERR"].size();
                for (int err = 0; err < out["ERR"].size(); err++) {
                    // if (max < (out["ERR"][err]*out["ERR"][err] / 4) ) {
                    //     max = out["ERR"][err]*out["ERR"][err] / (float)4;
                    //     //std::cout << max << std::endl;
                    // }
                    float a = out["ERR"][err]*out["ERR"][err] / (float)4;
                    S += a*a;
                }
            }
            epochs.push_back(EP - epocha);
            loss.push_back(sqrt(S));
            epocha--;

        }

        std::cout << F_name[i] << std::endl;
        for(auto elem: loss) {
            std::cout << elem << " ";
        }

        // std::cout << std::endl << std::endl;

        // for(int j = 0; j < W.size(); j++) {
        //     for (auto elem: W[j])
        //     {
        //         std::cout << elem << " ";
        //     }
        //     std::cout << std::endl;
        // }
        std::vector<std::vector<float>> in;
        in.push_back(epochs);
        in.push_back(loss);
        Func_image::plot_image(in, F_name[i], i+1);

        std::cout << std::endl;
        for(int number = 0; number < 10; number++) {
            auto out = forvard(images[number],  W, F, images_pred[number]);
            std::cout << number << ": "; 
            for (size_t i = 0; i < out["PRED"].size(); i++)
            {
                std::cout <<  out["PRED"][i] << " ";
            }   
            std::cout << std::endl;
        }
    }
    Func_image::show();
    return 0;
}