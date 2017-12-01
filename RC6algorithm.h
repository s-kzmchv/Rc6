#ifndef ALGOTITHM_H
#define ALGOTITHM_H

//RC6
#define w 32
#define r 20
#define b 256


#define P32   0xB7E15163
#define Q32   0x9E3779B9

#include<vector>
#include<iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdio>


class Rc6_alg
{
private:

    uint32_t tableS [2 * r + 4];
    void generateTableOfKeyWords(std::string keyStr);
    uint32_t rotateLeft(uint32_t value, uint32_t bit);
    uint32_t rotateRight(uint32_t value, uint32_t bit);

    void encrypt(const std::vector <uint32_t> & inputData, std::vector <uint32_t>& outputData);
    void decrypt(const std::vector <uint32_t> & inputData, std::vector <uint32_t>& outputData);

public:
    Rc6_alg(std::string key);
    void makeEncryption(std::string inputFile, std::string outputFile);
    void makeDecryption(std::string inputFile, std::string outputFile);

    void makeCorrelation(std::string inputFile, std::string encryptFile);

};

#endif