#ifndef WORDBANK_H
#define WORDBANK_H

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "Util.h"

class WordBank {
    public:
        WordBank() {}

        void setData(const std::string filename){
            #ifdef DEBUG
                std::cout << "Input file:" << filename << std::endl;
            #endif

            std::ifstream data(filename);
            std::string word;

            if(data.is_open()){
                while(getline(data, word)){
                    _words.push_back(word);
                }
            }

            data.close();

            #ifdef DEBUG
                std::cout << "Loaded " << _words.size() << " words." << std::endl;
            #endif
        }

        const std::vector<std::string>& all(){
            return _words;
        }

        const std::string random() {
            if(_words.size() == 0){
               std::cout << "WARN -- word bank not initialized " << std::endl; 
               return "";
            }

            return _words[rand()%_words.size()-1];
        }
    private: 
        std::vector<std::string> _words;
};

extern WordBank words;

#endif