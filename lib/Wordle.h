#ifndef WORDLE_H
#define WORDLE_H

#include <vector>
#include <string>

enum hint {INVALID, WRONG_POS, VALID};

class Wordle {
     public:  
        Wordle();                           // Wordle with random word
        Wordle(const std::string word);     // Wordle with specific word (for testing)

        bool isCorrectWord(const std::string& attempt);
        bool isGameOver();
        const std::vector<int>& getHint();
    
    private:
        const std::string answer;
        std::vector<int> hint;
        const int maxAttempts;
        int attemptNum;
};

#endif