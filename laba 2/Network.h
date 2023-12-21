#pragma once


#include <functional>
#include "Layer.h"
class Network {
    public:
    double step = 1e-2;
    std::vector<std::function<double(double)>> Fs;
    std::vector<std::string> F_name;
    std::vector<std::vector <double>> Images;
    std::vector<std::vector <double>> Images_pred;
    std::vector<std::vector <double>> Images_train;
    std::vector<std::vector <double>> Images_pred_train;
    std::function<double(double, std::function<double(double)>)> f_derevative;
    Network(std::vector<std::function<double(double)>> fs, std::vector<std::string> f_name, 
    std::vector<std::vector <double>> images, std::vector<std::vector <double>> images_pred,
    std::vector<std::vector <double>> images_train, std::vector<std::vector <double>> images_pred_train){
        Fs = fs;
        F_name = f_name;
        Images = images;
        Images_pred = images_pred;
        Images_train = images_train;
        Images_pred_train = images_pred_train;
    };
    void NetworkRun();
    double runTrain(std::vector<Layer> Layers, std::function<double(double)> f_act);
};