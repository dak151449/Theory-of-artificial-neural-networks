#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <math.h>
#include "../matplotlibcpp.h"
namespace plt = matplotlibcpp;


typedef float (*func)(double a);
typedef float (*func2)(float a, float b);

class Func_image {
    public:
        Func_image(){};
        static void plot_image(std::vector<std::vector<double>> image);
        static void plot_image(std::vector<std::vector<double>> image, std::string name, int index);
        static void plot_image(func F);
        static void show();
};