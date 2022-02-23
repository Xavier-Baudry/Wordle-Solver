#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "WordBank.h"
#include "Util.h"

void WordBank::setData(const std::string& filename){
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

const std::vector<std::string>& WordBank::all(){
    return _words;
}

std::string WordBank::random() const {
    if(_words.size() == 0){
        std::cout << "WARN -- word bank not initialized " << std::endl; 
        return "";
    }

    return _words[rand()%_words.size()-1];
}