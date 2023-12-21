#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <ios>

struct outData {
    std::vector<std::vector<double>> images;
    std::vector<int> labels;
};

class Mnist {
    public:
        Mnist(){};
        static outData loadTestImages(std::string filename, std::string labelfilename);
        static void printImages(std::vector<std::vector<double>> images);
};