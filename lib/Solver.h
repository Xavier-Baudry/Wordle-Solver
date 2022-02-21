#ifndef SOLVER_H
#define SOLVER_H

#include <unordered_set>
#include <string>
#include "Wordle.h"

class Solver {
    public:
        Solver() {};

        bool solve(Wordle* wordle);

    private:
        const std::string nextAttempt();
        void reset();
        void saveHint(const std::vector<int>& hint, const std::string attempt);
        bool wordIsPotentialMatch(const std::string&& word);

        int attemptCount;
        std::string currentMatches;
        std::unordered_set<char> cannotHave;
        std::unordered_set<char> mustHave;
        
};

#endif