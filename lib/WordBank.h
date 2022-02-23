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
        WordBank() = default;

        void setData(const std::string& filename);
        const std::vector<std::string>& all();
        std::string random() const; 
        
    private: 
        std::vector<std::string> _words;
};

extern WordBank words;

#endif