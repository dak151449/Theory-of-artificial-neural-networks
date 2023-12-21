#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Func_image.h"
#include "Neron.h"

double step;

//typedef double (*func)(double a);

double f_lin(double a) {
    return a;
}
double f_lin_der(double a) {
    if (a > 0) {
        return 1;
    }
    return 1;
}
double f_relu(double a) {
    if (a > 0) {
        return a;
    }
    return 0;
}
double f_relu_der(double a) {
    if (a > 0) {
        return 1;
    }
    return 0;
}


double f_sigmoid(double a) {
    return 1.0/(1.0 + exp(-a));
}

double f_sigmoid_der(double a) {
    return f_sigmoid(a) * (1 - f_sigmoid(a));
}

double f_gipthan(double a) {
    return (exp(a) - exp(-a))/(exp(a) + exp(-a));
}

int vector_sum(std::vector<int> in) {
    int s = 0;
    for(auto& elem: in) {
        s += elem;
    }
    return s;
}

double f_derivative(double a, std::function<double(double)> F) {
    double delta = 1e-10;
    return (F(a + delta) - F(a-delta))/((double)2.0*delta);
}


std::vector<double> Summ(std::vector<int> X, std::vector<std::vector<double>> W) {
    std::vector<double> out;
    for (size_t i = 0; i < W[0].size(); i++)
    {   
        double s = 0;
        for (size_t j = 0; j < X.size(); j++)
        {
            s += double(X[j])*W[j][i];
        }
        out.push_back(s);
    }
    
    return out;
}

std::vector<double> activation_func(std::vector<double> S, std::function<double(double)> F) {
    std::vector<double> out;
   // out = new std::vector<double>;
    for (size_t i = 0; i < S.size(); i++)
    {
        out.push_back(F(S[i]));
    }
    
    return out;
}

void forvard_test(std::vector<int> X, std::vector<Neron>& Nerons, std::function<double(double)> F, std::vector<double> Y_true) {
    for(int i = 0; i < Nerons.size(); i++) 
    {
        Nerons[i].Summ(X);
        Nerons[i].Activation(F);
        Nerons[i].Err_der(Y_true[i]);
    }
}

void backvard_test(std::vector<int> X, std::vector<Neron>& Nerons, std::function<double(double)> F) {
    for(int i = 0; i < Nerons.size(); i++) 
    {
        Nerons[i].Update_weights(X, F, step);
    }
}


int main() {
    std::vector<int> image_1 = {0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1};
    std::vector<double> image_1_pred = {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int w, h, nrChanel;

    std::vector<std::vector <int>> images;
    std::vector<std::vector <double>> images_pred;

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
        std::vector<double> image_pred_test;
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
   
    //std::vector<func> Fs =  {&f_lin} ;//, &f_lin, &f_relu, &f_gipthan};
    std::vector<std::function<double(double)>> Fs = {f_sigmoid, f_lin, f_relu, f_gipthan};
    std::vector<std::string> F_name = {"sigmoid", "lin", "relu", "gipthan"};
    for (int i = 0; i < Fs.size(); i++) {
        std::vector<std::vector<double>> W;
    
        int M = 10; // количество нейронов
        std::vector<Neron> Nerons;
        for (int j = 0; j < M; j++) {
            Nerons.push_back(Neron(images[0].size(), f_derivative));
        }
        if (F_name[i] == "sigmoid" || F_name[i] == "gipthan") {
            step = 0.01;
            std::cout << "+++++++++++++++++++++++++++++++++++\n";
        } else if (F_name[i] == "lin"){
            step = 0.01; 
            std::cout << "-----------------------------------\n";
        } else if (F_name[i] == "relu") {
            step = 0.01;
        }
        std::vector<double> loss;
        std::vector<double> epochs;
        int epocha = 100000;
        int EP = epocha;
        std::function<double(double)> F = Fs[i];
        
        while (epocha) {
            double S = 0;
            int len = 0;
            for (int im = 0; im < images.size(); im++)
            {   
                forvard_test(images[im], Nerons, F, images_pred[im]);
                backvard_test(images[im], Nerons, F);
                //len = out["ERR"].size();
                for (int err = 0; err < Nerons.size(); err++) {
                    double a = Nerons[err].err;
                    S += a/10;
                }
                //S += out["loss"][i];
            }
            epochs.push_back(EP - epocha);
            loss.push_back(S/images.size());
            epocha--;
        }

        std::cout << F_name[i] << std::endl;
        for(auto elem: loss) {
            std::cout << elem << " ";
        }

        std::vector<std::vector<double>> in;
        in.push_back(epochs);
        in.push_back(loss);
        Func_image::plot_image(in, F_name[i], i+1);

        // std::cout << std::endl;
        // for(int number = 0; number < 10; number++) {
        //     auto out = forvard(images[number],  W, F, images_pred[number]);
        //     std::cout << number << ": "; 
        //     for (size_t i = 0; i < out["PRED"].size(); i++)
        //     {
        //         std::cout <<  out["PRED"][i] << " ";
        //     }   
        //     std::cout << std::endl;
        // }
    }
    Func_image::show();
    return 0;
}