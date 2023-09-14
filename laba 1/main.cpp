#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Func_image.h"


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
    //F = f_sigmoid;
    //std::cout << "f_derivative " << (F(a + delta) - F(a-delta))/((double)2.0*delta) << "    " << f_lin_der(a) << " F: " << F(a) << " Real: " << a << std::endl;
    //return f_lin_der(a);
    //return f_sigmoid_der(a);
    //return f_relu_der(a);
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


double Err_f(double a) {
    return abs(a) / 10.0;
}

std::vector<double> Err(std::vector<double> Y, std::vector<double> Z) {
    std::vector<double> out;
    for (int i = 0; i < Y.size(); i++) {
        out.push_back(double(-2.0)*(Y[i]-Z[i]));
    }
    return out;
}

std::vector<double> Err_abs(std::vector<double> Y, std::vector<double> Z) {
    std::vector<double> out;
    for (int i = 0; i < Y.size(); i++) {
        out.push_back(abs(Y[i]-Z[i]));
    }
    return out;
}

std::map<std::string, std::vector<double>> forvard(std::vector<int> X, std::vector<std::vector<double>> W, std::function<double(double)> F, std::vector<double> Y_pred) {

    std::map<std::string, std::vector<double>> out;
    std::vector<double> Y = Summ(X, W);
    std::vector<double> Z = activation_func(Y, F);
    
    
    //int Y_pred = vector_sum(Z);
    out["SUM"] = Y;
    out["PRED"] = Z;
    out["ERR"] = Err(Y_pred, Z);
    out["loss"] = Err_abs(Y_pred, Z);
    // for(auto elem: out["ERR"]) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;
    return out;
}

void backvard(std::map<std::string, std::vector<double>> out, std::vector<std::vector<double>>& W, std::vector<int> X, std::function<double(double)> F) {

    

    for (size_t i = 0; i < W[0].size(); i++) // количество нейронов 
    {
        double dL_d_out = out["ERR"][i];
        for (size_t j = 0; j < W.size(); j++) // количество весов к одному нейрону i - index нейрона 
        {   
            // W[j][i];
            // double d_out_dwji = X[j] * f_sigmoid_der(out["SUM"][i]);
            double d_out_dwji = double(X[j]) * f_derivative(out["SUM"][i], F);
            double d_L_d_wji = dL_d_out * d_out_dwji;
            W[j][i] -= step * d_L_d_wji;
            //std::cout << "DELTA: " << f_derivative(out["SUM"][i], F) << std::endl;
        }
        
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
        for (size_t i = 0; i < images[0].size(); i++)
        {
            W.push_back({});
            for (size_t j = 0; j < M; j++)
            {
                W[i].push_back(0);
            }
            
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
        int epocha = 1000;
        int EP = epocha;
        std::function<double(double)> F = Fs[i];
        
        while (epocha) {
            double S = 0;
            int len = 0;
            for (int i = 0; i < images.size(); i++)
            {   
                auto out = forvard(images[i], W, F, images_pred[i]);
                backvard(out, W, images[i], F);
                out = forvard(images[i], W, F, images_pred[i]);
                len = out["ERR"].size();
                for (int err = 0; err < out["ERR"].size(); err++) {
                    double a = out["loss"][err];
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

        // std::cout << std::endl << std::endl;

        // for(int j = 0; j < W.size(); j++) {
        //     for (auto elem: W[j])
        //     {
        //         std::cout << elem << " ";
        //     }
        //     std::cout << std::endl;
        // }
        std::vector<std::vector<double>> in;
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