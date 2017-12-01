#ifndef PREPARE_H
#define PREPARE_H


#include <string>
#include<vector>



class ProcessingData
{
private:

public:
    std::string makeStr(std::vector <uint32_t>& outputData);
    std::vector<unsigned char> loadData(std::string fileName);
};

#endif