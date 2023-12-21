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

double f_derivative(double a, std::function<double(double)> F) {
    double delta = 1e-5;
    return (F(a + delta) - F(a-delta))/((double)2.0*delta);
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

double Network::Forvard() {
    auto out_sum = Layer::Multi(Layers[0], Layers[0].Input);
    Layers[0].Summ = out_sum;
    std::function<double(double)> f_act = &f_lin;
   // std::cout << "Вызод 0: " << Layers[0].Summ.size()  << "   ";
    for (size_t i = 0; i < out_sum.size(); i++)
    {
        out_sum[i] = f_act(out_sum[i]); // надо поменять хардкод функции активации
     //   std::cout << out_sum[i] << " ";
    }
    Layers[0].Out = out_sum;
    //std::cout << std::endl;

    for (int lay = 1; lay < Layers.size(); lay++) {
        Layers[lay].Input = Layers[lay - 1].Out;
        auto out_sum = Layer::Multi(Layers[lay], Layers[lay - 1].Out);
        Layers[lay].Summ = out_sum;
        // std::cout << "Вызод " << lay << ": ";
        for (size_t i = 0; i < out_sum.size(); i++)
        {
            out_sum[i] = f_act(out_sum[i]); // надо поменять хардкод функции активации
            // std::cout << out_sum[i] << " ";
        }
        Layers[lay].Out = out_sum;
        // std::cout << std::endl;
    }

    return maxIndex(Layers[Layers.size() - 1].Out);
}

void Network::BackPropogation(std::vector<double>& pred) {
    errors.clear();//{};
    std::vector<std::vector<double>>().swap(errors);
    for (size_t i = 0; i < Layers.size(); i++) {
        errors.push_back(std::vector<double>());
    }
    std::function<double(double)> f_act = &f_lin;
    for (size_t i = 0; i < Layers[Layers.size() - 1].W[0].size(); i++)
    {
        double a = double(-2.0) * (double(pred[i]) - Layers[Layers.size() - 1].Out[i]);
        double S = Layers[Layers.size() - 1].Summ[i];
        errors[Layers.size() - 1].push_back(a*f_derivative(S, f_act));
        // std::cout << a << " " << f_derivative(S, f_act) << " "<< errors[Layers.size() - 1][i] << " " << Layers[Layers.size() - 1].Out[i] << " " << S << std::endl;
    }

    for (int k = Layers.size() - 2; k >= 0; k--) {
		errors[k] = Layer::Multi_T(Layers[k], errors[k + 1]);  //neurons_err[k]);
        int size_k = Layers[k].W[0].size();
		for (int j = 0; j < size_k; j++)
			errors[k][j] *= f_derivative(Layers[k].Summ[j], f_act);
	}
}

void Network::WeightsUpdater(double step) {

    for (int i = 0; i < Layers.size(); i++) {
        // std::cout << "L " << i << std::endl;
		for (int j = 0; j < Layers[i].W[0].size(); j++) {
            
			for (int k = 0; k < Layers[i].W.size(); k++) {
                double a = Layers[i].W[k][j];
                Layers[i].W[k][j] -= step * errors[i][j] * Layers[i].Input[k]; // +
                // std::cout << "err " << a  << " " << Layers[i].Input[k] << " " << a - step * errors[i][j] * Layers[i].Input[k] << " " << step * errors[i][j] * Layers[i].Input[k] << std::endl;
			}
		}
	}
}


double Network::runTrain(std::vector<Layer> Layers, std::function<double(double)> f_act) {
    int correct = 0;
    for (int im = 0; im < Images_train.size() ; im++){
        double layer_err = 0;
        Layers[0].Input = Images[im];
        int predict = Forvard();
        int number  = 0;
        for (int n = 0; n < Images_pred_train[im].size(); n++) {
            if (Images_pred_train[im][n] > 0.5) {
                number = n;
                break;
            }
        }
        if (predict == number) {
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
    //std::vector<Layer> Layers;
    std::vector<unsigned long> sizes = {256, 10};
    for (int i = 0; i < 2; i++) {
        if (i == 0 ){
            Layers.push_back(Layer(10, Images[0].size()));
        } else {
            Layers.push_back(Layer(10, Layers[i-1].W[0].size()));
        }
    }
    std::cout << "Генерация сети закончилась" << std::endl;
    int epochs = 20;
    double err = 0;
    long img_len = Images.size();
    //img_len = 1;
    std::function<double(double)> f_act = f_relu;
    while (epochs)
    {   
        err = 0;
        for (int im = 0; im < img_len; im++){
            Layers[0].Input = Images[im];
            // if (im > 9999) {
            //     std::cout << "Debug\n";
            // }
            int predict = Forvard();
            BackPropogation(Images_pred[im]);
            WeightsUpdater(step);
         //   std::cout << im << std::endl;

        }

        // for (size_t i = 0; i < Layers.size(); i++)
        // {
        //     Layers[i].printW();
        // }
        

        err = err / double(Images.size());
        epochs--;
       std::cout << "Epoch: " << epochs << " Error: " << runTrain(Layers, f_act) << std::endl;
    }
    
   runTrain(Layers, f_act);
}