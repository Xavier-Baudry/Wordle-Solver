#include <string>
#include <vector>
#include <memory>

#include "Wordle.h"
#include "Util.h"
#include "WordBank.h"

Wordle::Wordle(WordBank& words): maxAttempts(6), attemptNum(0), _answer(words.random()), _last_hint({INVALID,INVALID,INVALID,INVALID,INVALID}) {
    #ifdef DEBUG
        std::cout << "Wordle is: " << _answer << std::endl;
    #endif
}

Wordle::Wordle(const std::string word): maxAttempts(6), attemptNum(0), _answer(word), _last_hint({INVALID,INVALID,INVALID,INVALID,INVALID}) {
    #ifdef DEBUG
        std::cout << "Wordle is: " << _answer << std::endl;
    #endif
}

bool Wordle::isCorrectWord(const std::string& attempt){
    // Always set the hint, even if the game would be complete
    for(auto i = 0; i < 5; ++i){
        if(attempt[i] == _answer[i]){
            _last_hint[i] = VALID;
        }else if(_answer.find(attempt.substr(i, 1)) != std::string::npos){
            _last_hint[i] = WRONG_POS;
        }else{
            _last_hint[i] = INVALID;
        }
    }

    attemptNum++;

    return attempt == _answer;
}

const std::vector<int>& Wordle::getHint(){
    return _last_hint;
}

const std::string Wordle::getAnswer() {
    return _answer;
}

bool Wordle::isGameOver(){
    return attemptNum >= maxAttempts;
}