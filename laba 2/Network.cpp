#include "Network.h"

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

double f_sigmoid(double a) {
    return 1.0/(1.0 + exp(-a));
}

int maxIndex(std::vector<double> pred) {
    double maximum = pred[0];
    int index = 0;
    for (int i = 0; i < pred.size(); i++) {
        if (maximum < pred[i]) {
            index  = i;
            maximum = pred[i];
        }
    }
    return index;
}

double Network::runTrain(std::vector<Layer> Layers, std::function<double(double)> f_act) {
    int correct = 0;
    for (int im = 0; im < Images_train.size() ; im++){
        double layer_err = 0;
        for (int lay = 0; lay < Layers.size() - 1; lay++) {
            if (lay == 0) {
                Layers[lay].set0layer(Images_train[im], f_act);
            } else {
                Layers[lay].forvardInput(Layers[lay-1].Out, f_sigmoid);
            }
            
        }
        Layers[Layers.size() - 1].forvardOutput(Layers[Layers.size() - 1 -1].Out, f_act, Images_pred_train[im]);
        auto index = maxIndex(Layers[Layers.size() - 1].Out);
        int number  = 0;
        for (int n = 0; n < Images_pred_train[im].size(); n++) {
            if (Images_pred_train[im][n] > 0.5) {
                number = n;
                break;
            }
        }
   
        if (index == number) {
            correct++;
        }
    }
    //std::cout << "Процент верных: " << double(correct)/double(Images_train.size()) << std::endl;
    return double(correct)/double(Images_train.size());
}

void Network::NetworkRun() {
    //std::vector<std::function<double(double)>> Fs = {f_sigmoid, f_lin, f_relu, f_gipthan};
    //std::vector<std::string> F_name = {"sigmoid", "lin", "relu", "gipthan"};
    std::cout << "Network Run" << std::endl;
    std::vector<Layer> Layers;

    for (int i = 0; i < 3; i++) {
        Layers.push_back(Layer());
        Layers[i].Nerons = {};
        if (i == 0 ){
            for (int j = 0; j < Images[0].size(); j++) {
                Layers[i].Nerons.push_back(Neron(Images[0].size(), f_derevative, Images[j][0]));
            }
        } else {
            for (int j = 0; j < 10; j++) {
                Layers[i].Nerons.push_back(Neron(Layers[i-1].Nerons.size(), f_derevative));
            }   
        }
    }
    std::cout << "Генерация сети закончилась" << std::endl;
    int epochs = 10*10;
    double err = 0;
    long img_len = Images.size();
    //img_len = 1;
    std::function<double(double)> f_act = f_relu;
    while (epochs)
    {   
        err = 0;
        for (int im = 0; im < img_len; im++){
            double layer_err = 0;
            Layers[0].set0layer(Images[im], f_act);
            for (int lay = 1; lay < Layers.size() - 1; lay++) {
                    Layers[lay].forvardInput(Layers[lay-1].Out, f_sigmoid);                
            }
            Layers[Layers.size() - 1].forvardOutput(Layers[Layers.size() - 1 - 1].Out, f_act, Images_pred[im]);
            Layers[Layers.size() - 1].backvardOutput(f_act);
            auto errors = Layers[Layers.size() - 1].getError();
            
            for (int i = 0; i < errors.size(); i++) {
                layer_err += errors[i];
            }
            layer_err = layer_err / double(errors.size());


           // Layers[Layers.size() - 1].forvardOutput(Layers[Layers.size() - 1 -1].Out, f_relu, Images_pred[im]);

            for (int lay = Layers.size() - 1 - 1; lay > 0; lay--) {
                Layers[lay].setError(Layers[lay+1], f_sigmoid);    
            }
            for (int lay = Layers.size() - 1 - 1; lay > 0; lay--) {
                Layers[lay].backvardInput(f_sigmoid); 
            }
            err += layer_err;
            //std::cout << "layer_err: " << layer_err << std::endl;
        }
        err = err / double(Images.size());
        epochs--;
        std::cout << "Epoch: " << epochs << " Error: " << runTrain(Layers, f_act) << std::endl;
    }
    
    runTrain(Layers, f_act);
}