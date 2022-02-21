#include <string>
#include <unordered_set>

#include "WordBank.h"
#include "Wordle.h"
#include "Solver.h"
#include "Util.h"

bool Solver::solve(Wordle* wordle){
    reset();

    while(!wordle->isGameOver()){
        auto attempt = nextAttempt();
        bool isSolved = wordle->isCorrectWord(attempt);

        auto hint = wordle->getHint();
        saveHint(hint, attempt);

        #ifdef DEBUG
            std::cout << "ATTEMPT #" << attemptCount << " - Trying word: " << attempt << " | hint: " << iteratableToString(hint) << " | currentMatches: " << currentMatches << " | mustHave: " << iteratableToString(mustHave, ',') << " | cannotHave: " << iteratableToString(cannotHave, ',')  << std::endl;
        #endif

        if(isSolved){
            return true;
        }

        attemptCount++;
    }

    return false;
}

const std::string Solver::nextAttempt(){
    // Using cones and trials for the first two moves has a high probability of creating at least 1+ match
    if(attemptCount == 0){
        return "cones";
    }else if(attemptCount == 1){
        // Only try 'trial' if it is still a potential match after trying 'cones'
        if(wordIsPotentialMatch("trial")){
            return "trial";
        }
    }  
    
    std::vector<std::string> potentialMatches;

    // Find potential matches using the currently known data (confirmed letters, confirmed positions and invalid letters)
    for(auto word : words.all()) {
        if(wordIsPotentialMatch(std::move(word))){
            potentialMatches.push_back(word);
        }
    }

    if(potentialMatches.size() > 0){
        return potentialMatches[0];
    }

    // If somehow we have no potential match, return a random word
    return words.random();
}

void Solver::reset() {
    currentMatches = "-----";
    attemptCount = 0;
    mustHave.clear();
    cannotHave.clear();
}

void Solver::saveHint(const std::vector<int>& hint, const std::string attempt) {
    for(int i = 0; i < 5; i++){
        switch(hint[i]){
            case VALID: 
                currentMatches[i] = attempt[i];

                // Remove certain match from mustHave to speed up search in next attempts
                if(mustHave.count(attempt[i]) > 0){
                    mustHave.erase(attempt[i]);
                }
                break;
            case WRONG_POS:
                if(mustHave.count(attempt[i]) == 0){
                    mustHave.insert(attempt[i]);
                }
                break;
            case INVALID:
                if(cannotHave.count(attempt[i]) == 0){
                    cannotHave.insert(attempt[i]);
                }
                break;
        }
    }
}

bool Solver::wordIsPotentialMatch(const std::string&& word){
    #ifdef VERBOSE_DEBUG
        std::cout << "checking potential word: " << word << std::endl;
    #endif

    for(auto i = 0; i < 5; ++i){
        // Invalid position match
        if(currentMatches[i] != '-' && word[i] != currentMatches[i]){
            #ifdef VERBOSE_DEBUG
                std::cout << "invalid - position mismatch at " << i << std::endl;
            #endif
            return false;
        }

        // Has invalid letter
        if(cannotHave.count(word[i]) > 0){
            #ifdef VERBOSE_DEBUG
                std::cout << "invalid - contains " << word[i] << std::endl;
            #endif
            return false;
        }
    }

    for(auto c : mustHave){
        if(word.find(c) == std::string::npos){
            #ifdef VERBOSE_DEBUG
                std::cout << "invalid - is missing " << c << std::endl;
            #endif
            return false;
        }
    }

    return true;
}