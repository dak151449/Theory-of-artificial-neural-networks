#include "Layer.h"

Layer::Layer(int size, int prev_size) {
    for (int i = 0; i < prev_size; i++) {
        W.push_back({});
        for (int j = 0; j < size; j++) {
            //W[i].push_back(((rand() % 100)) * 0.03 / double(prev_size + 35));
            W[i].push_back(0.001);
        }
    }
}

Layer::Layer(int size) {
    for (int i = 0; i < size; i++) {
        W.push_back({});
        for (int j = 0; j < 1; j++) {
            W[i].push_back(1);
        }
    }
}

std::vector<double> Layer::Multi(const Layer& m, const std::vector<double>& b) {
    std::vector<double> out;
    for (int i = 0; i < m.W[0].size(); i++) {
        double tmp = 0.0;
        for (int j = 0; j < m.W.size(); j++) {
            tmp += double(m.W[j][i]) * double(b[j]);
        }    
        out.push_back(tmp);
    }
    return out;
}


std::vector<std::vector<double>> Transpon(std::vector<std::vector<double>> m) {
    std::vector<std::vector<double>> out;
    for (int i = 0; i < m[0].size(); i++) {
        out.push_back({});
        for (int j = 0; j < m.size(); j++) {
            out[i].push_back(m[j][i]);
        }
    }
    return out;
}


std::vector<double> Layer::Multi_T(const Layer& m, const std::vector<double>& b) {
    std::vector<double> out;
    auto m_t = Transpon(m.W);
    // for (int i = 0; i < m.W[0].size(); i++) {
    //     double tmp = 0.0;
    //     for (int j = 0; j < m.W.size(); j++) {
    //         tmp += m.W[j][i] * b[j];
    //     }    
    //     out.push_back(tmp);
    // }
    for (int i = 0; i < m_t[0].size(); i++) {
        double tmp = 0.0;
        for (int j = 0; j < m_t.size(); j++) {
            tmp += double(m_t[j][i]) * double(b[j]);
        }    
        out.push_back(tmp);
    }
    return out;
}

std::vector<double> Layer::SumVector(const std::vector<double>& v1, const std::vector<double>& v2) {
    std::vector<double> out;
    for (int i = 0; i < v1.size(); i++) {
        out.push_back(v1[i] + v2[i]);
    }
    return out;
}

void Layer::printW() {
    for (size_t i = 0; i < 20; i++)
    {
        for(size_t j = 0; j < W[0].size(); j++) {
            std::cout << W[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
}