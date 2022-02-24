#ifndef SOLVER_H
#define SOLVER_H

#include <unordered_set>
#include <string>

#include "Wordle.h"
#include "WordBank.h"
#include <memory>

class Solver {
    public:
        Solver(std::shared_ptr<WordBank> w): words(w), solveAttempts(0) {};

        bool solve(Wordle& wordle);
        void stats();

    private:
        const std::string nextAttempt();
        void reset();
        void saveHint(const std::vector<int>& hint, const std::string attempt);
        bool wordIsPotentialMatch(const std::string&& word);

        std::shared_ptr<WordBank> words;

        // Properties used for each solve attempt
        int attemptCount;
        std::string currentMatches;
        std::unordered_set<char> cannotHave;
        std::unordered_set<char> mustHave;
        
        // General Statistics
        std::vector<std::string> failedWords;
        int solveAttempts;
};

#endif