#include <iostream>
#include <fstream>
#include "ProcessingData.h"


std::string ProcessingData::makeStr(std::vector <uint32_t>& outputData)
{
    std::string res;
    for (size_t i = 0; i < 4; i++)
    {
        uint32_t tmp = outputData[i] << 24;
        res.push_back((char)(tmp>>24));

        tmp = outputData[i] << 16;
        res.push_back((char)(tmp >> 24));

        tmp = outputData[i] << 8;
        res.push_back((char)(tmp >> 24));

        res.push_back((char)(outputData[i] >> 24));

    }
    return res;
}

std::vector<unsigned char> ProcessingData::loadData(std::string fileName) {
    std::ifstream file(fileName,std::ios_base::binary);
    std::vector<unsigned char> data;
    if (!file.is_open()) {
        std::cout << "File doesn't exist!\n";
        return data;
    }


    int c;
    while((c = file.get()) != EOF){
        if(c == 10 && data[data.size()-1] == '\r')
            data[data.size()-1] = (unsigned char)c;
        else
            data.push_back((unsigned char)c);
    }
    file.close();

    return data;
}
