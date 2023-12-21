#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Func_image.h"
//#include "Neron.h"
#include "Mnist.h"
#include "Network.h"

double step;

double f_relu_der(double a) {
    if (a > 0) {
        return 1;
    }
    return 0;
}




// double f_sigmoid_der(double a) {
//     return f_sigmoid(a) * (1 - f_sigmoid(a));
// }

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

std::vector<std::vector<double>> predict(std::vector<int> y) {
    std::vector<std::vector<double>> out;
    for (size_t i = 0; i < y.size(); i++) {
        std::vector<double> tmp;
        for (size_t j = 0; j < 10; j++) {
            if (j == y[i]) {
                tmp.push_back(1);
            } else {
                tmp.push_back(0);
            }
        }
        out.push_back(tmp);
    }
    return out;
}

int getLabel(std::vector<double> label) {
    for (int i=0; i < label.size(); i++) {
        if(label[i] > 0) {
            return i;
        }
    }
    return 0;
}

void printImages(std::vector<std::vector<double>>& V, std::vector<std::vector<double>> labels) {
    int index = 0;
    for (auto image: V) {
        std::cout << "Number: " << getLabel(labels[index]) << std::endl;
        for (int i = 0; i < V[0].size(); i++) {
            if (i != 0 && i%28==0) {
                std::cout<< std::endl;
            }
            if (image[i] > 0) {
                std::cout << 1 << " ";
            } else {
                std::cout << 0 << " ";
            }
            
        }
        
        std::cout << std::endl;

        if (index == 10)
            return;
        index++;
    }
}

void normolize(std::vector<std::vector<double>>& V) {
    
    for (int i = 0; i < V.size(); i++) {
        for (int j = 0; j < V[0].size(); j++) {
            V[i][j] /=  double(256);
           // std::cout << V[0].size() << std::endl;
        }
    }

    return ;
}

void check(std::vector<std::vector<double>>& V) {
    
    for (int i = 0; i < V.size(); i++) {
        for (int j = 0; j < V[0].size(); j++) {
            if (V[i][j] > 1) {
                std::cout << "V[i][i] " << V[i][j] << std::endl; 
            }
        }
    }

    return ;
}

int main() {
    
    auto test = Mnist::loadTestImages("t10k-images.idx3-ubyte", "t10k-labels.idx1-ubyte");
    std::cout << test.images[0].size() << "   " <<  predict(test.labels)[0].size() << std::endl;
    //return 0;
    normolize(test.images);
    check(test.images);
    //return 0;
    
   // return 0;
    //Mnist::printImages(images);
    
    auto train = Mnist::loadTestImages("train-images.idx3-ubyte", "train-labels.idx1-ubyte");
    std::cout << train.images.size() << "   " << train.labels.size() << std::endl;

    normolize(train.images);
    check(train.images);

    // printImages(train.images, predict(train.labels));
    // return 0;

    Network network({}, {}, train.images, predict(train.labels), test.images, predict(test.labels));
    network.NetworkRun();
    return 0;
  
}