#include "Mnist.h"

int reverseInt (int i) 
{
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

uint32_t swap_endian(uint32_t val) {
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}


std::vector<int> read_mnist_labels(std::string full_path, int& number_of_labels) {
    auto reverseInt = [](int i) {
        unsigned char c1, c2, c3, c4;
        c1 = i & 255, c2 = (i >> 8) & 255, c3 = (i >> 16) & 255, c4 = (i >> 24) & 255;
        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
    };

    typedef unsigned char uchar;

    std::ifstream file(full_path, std::ios::binary);

    if(file.is_open()) {
        int magic_number = 0;
        file.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverseInt(magic_number);

        if(magic_number != 2049) {
            std::cout << "Invalid MNIST label file!" << std::endl;
            return {};
        }

        file.read((char *)&number_of_labels, sizeof(number_of_labels)), number_of_labels = reverseInt(number_of_labels);

       std::vector<int> _dataset;
        for(int i = 0; i < number_of_labels; i++) {
            _dataset.push_back(0);
            file.read((char*)&_dataset[i], 1);
        }
        return _dataset;
    } else {
        std::cout << "Invalid MNIST label file!" << std::endl;
        return {};
    }
}

outData Mnist::loadTestImages(std::string filename, std::string labelfilename) {

    std::vector<std::vector<double>> images;
    
    std::ifstream file (filename, std::ios::binary);
    std::ifstream labelfile (labelfilename, std::ios::binary);

    if (file.is_open())
    {   
        std::cout << "File opened  "<< sizeof(int) * 8 << std::endl;
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
 
        file.read((char*)&magic_number,sizeof(magic_number)); 
        magic_number= reverseInt(magic_number);

        file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= reverseInt(number_of_images);

        file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= reverseInt(n_rows);

        file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= reverseInt(n_cols);

        // std::cout << "magic_number: " << magic_number << std::endl;
        // std::cout << "number_of_images: " << number_of_images << std::endl;
        // std::cout << "n_rows: " << n_rows << std::endl;
        // std::cout << "n_cols: " << n_cols << std::endl;

        int label_magic_number = 0;
        labelfile.read((char *)&label_magic_number, sizeof(label_magic_number));
        label_magic_number = reverseInt(label_magic_number);
    //    std::cout << "label_magic_number: " << label_magic_number << std::endl;
        if(label_magic_number != 2049) {
            std::cout << "Invalid MNIST image file!\n";
            return {};
        }

        int number_of_labels = 0;
        labelfile.read((char *)&number_of_labels, sizeof(number_of_labels)), number_of_labels = reverseInt(number_of_labels);
        // std::cout << "number_of_labels: " << number_of_labels << std::endl;
        
        for(int i=0;i<number_of_images;++i)
        {   
            images.push_back(std::vector<double>());
            for(int r=0;r<n_rows;++r)
            {
                for(int c=0;c<n_cols;++c)
                {
                    unsigned char temp=0;
                    file.read((char*)&temp,sizeof(temp));
                    images[i].push_back(temp);
                }
            }

        }

        std::vector<int> _dataset;
        for(int i = 0; i < number_of_labels; i++) {
            double _label;
            labelfile.read((char*)&_label, 1);
            _dataset.push_back(_label);
        }
        std::cout << "Dataset loaded " << _dataset.size()<< std::endl;
        file.close();
        labelfile.close();
        int count;
        return outData{
            images: images,
            labels: read_mnist_labels(labelfilename, count),
        };
    }
    file.close();
    labelfile.close();
    return {};
}

void Mnist::printImages(std::vector<std::vector<double>> images) {
    for (int i = 0; i < images.size(); i++) {
        for (int j = 0; j < images[i].size(); j++) {
            std::cout << images[i][j] << " ";
            // if (j % 28 == 27)
            //     std::cout << std::endl;
        }
        std::cout << "------------------------------" << std::endl;
    }
    std::cout << std::endl;
}
