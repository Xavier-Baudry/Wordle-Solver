#include <string>
#include <vector>

#include "Wordle.h"
#include "Util.h"
#include "WordBank.h"

Wordle::Wordle(): maxAttempts(6), attemptNum(0), answer(words.random()), hint({INVALID,INVALID,INVALID,INVALID,INVALID}) {
    #ifdef DEBUG
        std::cout << "Wordle is: " << answer << std::endl;
    #endif
}

Wordle::Wordle(const std::string word): maxAttempts(6), attemptNum(0), answer(word), hint({INVALID,INVALID,INVALID,INVALID,INVALID}) {
    #ifdef DEBUG
        std::cout << "Wordle is: " << answer << std::endl;
    #endif
}

bool Wordle::isCorrectWord(const std::string& attempt){
    // Always set the hint, even if the game would be complete
    for(auto i = 0; i < 5; ++i){
        if(attempt[i] == answer[i]){
            hint[i] = VALID;
        }else if(answer.find(attempt.substr(i, 1)) != std::string::npos){
            hint[i] = WRONG_POS;
        }else{
            hint[i] = INVALID;
        }
    }

    attemptNum++;

    return attempt == answer;
}

const std::vector<int>& Wordle::getHint(){
    return hint;
}

bool Wordle::isGameOver(){
    return attemptNum >= maxAttempts;
}