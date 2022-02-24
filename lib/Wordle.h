#ifndef WORDLE_H
#define WORDLE_H

#include <vector>
#include <string>
#include <memory>

#include "WordBank.h"

enum hint {INVALID, WRONG_POS, VALID};

class Wordle {
     public:  
        Wordle()=delete;
        Wordle(WordBank& words);    // Wordle with random word from word bank
        Wordle(const std::string word);             // Wordle with specific word (for testing)

        bool isCorrectWord(const std::string& attempt);
        bool isGameOver();
        const std::vector<int>& getHint();
        const std::string getAnswer();
    
    private:
        const std::string _answer;
        std::vector<int> _last_hint;
        const int maxAttempts;
        int attemptNum;
};

#endif