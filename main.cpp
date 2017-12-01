// Rc6.cpp : Defines the entry point for the console application.
//

#include <random>
#include <ctime>
#include "RC6algorithm.h"


int main()
{
    std::cout << "Do you want generate random key? Y/N \n";
    std::string key;
    std::cin >> key;
    if (key == "N"){
        std::cout << "Write key \n";
        std::cin >> key;
    } else {
        key.resize(32);
        int rand;
        srand((unsigned int)time(nullptr));
        rand = std::rand() % 256;
        for (char &c: key) {
            rand = std::rand() % 256;
            c = (char) (rand);
        }
        std::cout << key<<"\n";
    }

    Rc6_alg rc(key);
    std::cout << "Use @en \"InputFileName\" \"OutputFileName\" for encryption \n"
            "Use @de \"InputFileName\" \"OutputFileName\" for decryption \n"
            "Use @co \"InputFileName\" \"OutputFileName\" for correlation \n"
            "Use @help for help \n"
            "Use @end  for end \n";
    while (true) {

        std::string str1;
        std::string str2;
        std::string str3;
        std::cin >> str1;

        if(str1 == "@end"){
            break;
        }


        std::cin >> str2;
        std::cin >> str3;

        if(str1 == "@en"){
            rc.makeEncryption(str2,str3);
        }

        if(str1 == "@de"){
            rc.makeDecryption(str2,str3);
        }

        if(str1 == "@co"){
            rc.makeCorrelation(str2,str3);
        }

        if(str1 == "@help"){
            std::cout << "Use @en \"InputFileName\" \"OutputFileName\" for encryption \n"
                    "Use @de \"InputFileName\" \"OutputFileName\" for decryption \n"
                    "Use @co \"InputFileName\" \"OutputFileName\" for correlation \n"
                    "Use @help for help \n"
                    "Use @end  for end \n";
        }
    }

    return 0;
}

