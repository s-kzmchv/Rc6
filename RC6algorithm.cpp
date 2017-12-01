#include "RC6algorithm.h"
#include "ProcessingData.h"


Rc6_alg::Rc6_alg(std::string keyStr)
{    if (keyStr.length() * 8 > b)
    {
        keyStr.resize(b/8);
    }

    generateTableOfKeyWords(keyStr);
}

void Rc6_alg::makeEncryption(std::string inputFile, std::string outputFile)
{
    ProcessingData p;
    std::vector<unsigned char> text = p.loadData(inputFile);
    if(text.empty())
        return;

    int usless_data = (w/2) -  (text.size() % (w/2));
    for(int i = 0; i < usless_data; i++){
        text.push_back(' ');
    }

    std::vector <uint32_t> inputData;
    std::vector <uint32_t> encryptedData;

    std::ofstream fileOutput(outputFile,std::ios_base::binary);
    fileOutput << usless_data << "\n";
    inputData.assign(4, 0);

    for (size_t i = 0; i < text.size(); i += (w/2)) {
        int j = 0;
        for (size_t k = 0; k < (w/2); k += 4)
        {
            inputData[j] = (uint32_t) ( text[i + k] + (text[i + k + 1] << 8) + (text[i + k + 2] << 16) + (text[i + k + 3] << 24));
            j++;
        }
        encrypt(inputData, encryptedData);
        fileOutput << p.makeStr(encryptedData);
        encryptedData.resize(0);
    }
    fileOutput.close();
}



void Rc6_alg::makeDecryption(std::string inputFile, std::string outputFile)
{
    ProcessingData p;
    std::vector<unsigned char> text = p.loadData(inputFile);

    char c_arr [4];
    int i = 0;
    while (text[i] != '\n'){
        c_arr[i] = text[i];
        i++;
    }
    i++;
    int usless_data = atoi(c_arr);

    std::vector <uint32_t> encryptedData;
    std::vector <uint32_t> outputData;

    std::ofstream fileOutput(outputFile,std::ios_base::binary);
    encryptedData.assign(4, 0);
    for (i; i < text.size(); i += (w/2)) {
        int j = 0;
        for (size_t k = 0; k < (w/2); k += 4)
        {
            encryptedData[j] = (uint32_t) ( text[i + k] + (text[i + k + 1] << 8) + (text[i + k + 2] << 16) + (text[i + k + 3] << 24));
            j++;
        }
        decrypt(encryptedData, outputData);
        if(i + (w/2) == text.size() ){
            std::string tmp = p.makeStr(outputData);
            tmp.resize(tmp.size() - usless_data);
            fileOutput << tmp;
        } else
            fileOutput << p.makeStr(outputData);
        outputData.resize(0);
    }
    fileOutput.close();
}




void Rc6_alg::generateTableOfKeyWords(std::string keyStr)
{


    if (keyStr.length() % 4 != 0)
    {
        for (size_t i = 0; i < (4 - keyStr.length() % 4); i++)
        {
            keyStr += " ";
        }
    }

    uint32_t tableL[b / w];

    int k = 0;
    for (size_t i = 0; i < keyStr.length(); i+=4)
    {
        tableL[k] =(uint32_t) (keyStr[i] + (keyStr[i + 1] << 8) + (keyStr[i + 2] << 16) + (keyStr[i + 3] << 24));
        k++;
    }

    for ( k; k < b/w; k ++)
    {
        tableL[k] = 0;
    }

    int c = b / w;

    tableS[0] = P32;

    for (int i = 1; i < (2 * r + 4); i++)
        tableS[i] = tableS[i - 1] + Q32;

    int v;

    if (c < 2 * r + 4)
        v = 2 * r + 4;
    else
        v = c;
    v *= 3;

    int i = 0;
    int j = 0;
    uint32_t A = 0;
    uint32_t B = 0;

    for (k = 0; k < v; k++) {

        tableS[i] = rotateLeft(tableS[i] + A + B, 3);
        A = tableS[i];
        i++;
        i %= 2 * r + 4;

        tableL[j] = rotateLeft(tableL[j] + A + B, ((A + B) % w));
        B = tableL[j];
        j++;
        j %= c;

    }

}

uint32_t Rc6_alg::rotateLeft(uint32_t value, uint32_t bit)
{
    return value << bit | value >> (w - bit);
}

uint32_t Rc6_alg::rotateRight(uint32_t value, uint32_t bit)
{
    return value >> bit | value << (w - bit);
}


void Rc6_alg::encrypt(const std::vector <uint32_t>& inputData, std::vector <uint32_t>& outputData)
{
    uint32_t A = inputData[0];
    uint32_t B = inputData[1];
    uint32_t C = inputData[2];
    uint32_t D = inputData[3];

    uint32_t tmp1;
    uint32_t tmp2;

    B += tableS[0];
    D += tableS[1];

    auto valueLog =(uint32_t) (log(w) / log(2));

    for (int i = 0; i < r; i++)
    {
        tmp1 = rotateLeft(B * (2 * B + 1), valueLog);
        tmp2 = rotateLeft(D * (2 * D + 1), valueLog);

        A = rotateLeft((A ^ tmp1), (tmp2 % w)) + tableS[2*i];
        C = rotateLeft((C ^ tmp2), (tmp1 % w)) + tableS[2 * i + 1];

        tmp1 = A;
        A = B;
        B = C;
        C = D;
        D = tmp1;
    }

    A += tableS[2 * r + 2];
    C += tableS[2 * r + 3];

    outputData.push_back(A);
    outputData.push_back(B);
    outputData.push_back(C);
    outputData.push_back(D);

}



void Rc6_alg::decrypt(const std::vector <uint32_t>& inputData, std::vector <uint32_t>& outputData)
{
    uint32_t A = inputData[0];
    uint32_t B = inputData[1];
    uint32_t C = inputData[2];
    uint32_t D = inputData[3];

    uint32_t tmp1;
    uint32_t tmp2;

    auto valueLog =(uint32_t) (log(w) / log(2));

    C -= tableS[2 * r + 3];
    A -= tableS[2 * r + 2];

    for (int i = r-1; i >=0; i--)
    {
        tmp1 = D;
        D = C;
        C = B;
        B = A;
        A = tmp1;

        tmp2 = rotateLeft(D * (2 * D + 1), valueLog);
        tmp1 = rotateLeft(B * (2 * B + 1), valueLog);

        C = rotateRight((C - tableS[2 * i + 1]), (tmp1 % w)) ^ tmp2;
        A = rotateRight((A - tableS[2 * i]), (tmp2 % w)) ^ tmp1;
    }

    D -= tableS[1];
    B -= tableS[0];

    outputData.push_back(A);
    outputData.push_back(B);
    outputData.push_back(C);
    outputData.push_back(D);

}

void Rc6_alg::makeCorrelation(std::string inputFile, std::string encryptFile) {

    double average_input = 0;
    double average_encrypt = 0;

    ProcessingData p;
    std::vector<unsigned char> encryptedData = p.loadData(encryptFile);
    std::vector<unsigned char> inputData = p.loadData(inputFile);
    int bit;

    for(int i = 0; i < inputData.size(); i++) {
        for(int k = 0; k < 8; k++) {
            bit = ((inputData[i] >> k) & 1);
            average_input += bit;
            bit = ((encryptedData[i] >> k) & 1);
            average_encrypt += bit;
        }
    }
    average_input = average_input / (inputData.size()*8);
    average_encrypt = average_encrypt / (encryptedData.size()*8);

    std::cout << "distribution encrypt \"1\": " << average_encrypt << "\n";
    std::cout << "distribution encrypt \"0\": " << 1 - average_encrypt << "\n";


    double numerator = 0;
    double denominator1 = 0;
    double denominator2 = 0;


    for(int i = 0; i < inputData.size(); i++) {
        for(int k = 0; k < 8; k++) {
            int bit_input = ((inputData[i] >> k) & 1);
            int bit_encrypt = ((encryptedData[i] >> k) & 1);

            numerator +=(bit_input - average_input) * (bit_encrypt - average_encrypt);
            denominator1 += pow(bit_input - average_input, 2);
            denominator2 += pow(bit_encrypt - average_encrypt, 2);
        }
    }

    double  coefficient = numerator / sqrt(denominator1*denominator2);
    std::cout << "Correlation coefficient: " << coefficient << "\n";

}
