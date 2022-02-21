#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <unordered_set>

//#define DEBUG
//#define VERBOSE_DEBUG

enum hint {INVALID, WRONG_POS, VALID};

std::vector<std::string> words;

void buildWordList(const std::string fileName){
    std::cout << "Input file:" << fileName << std::endl;

    std::ifstream data(fileName);
    std::string word;

    if(data.is_open()){
        while(getline(data, word)){
            words.push_back(word);
        }
    }

    data.close();

    std::cout << "Loaded " << words.size() << " words." << std::endl;
}

std::string randomWord(){
    return words[rand()%words.size()-1];
}

template<class t>
std::string iteratableToString(t& iter){
    std::stringstream s;
    for(auto it : iter){
        s << it;
    }
    return s.str();
}

template<class t>
std::string iteratableToString(t& iter, char seperator){
    if(iter.size() == 0){
        return "";
    }

    std::stringstream s;

    auto last = iter.begin();
    std::advance(last, iter.size()-1);

    for(auto it = iter.begin(); it != iter.end(); ++it){
        s << *it;
        if(it != last){
            s << seperator;
        }
    }
    return s.str();
}

class Wordle {
    public:
        Wordle(const std::string word): maxAttempts(6), attemptNum(0), answer(word) {
            #ifdef DEBUG
                std::cout << "Wordle is: " << answer << std::endl;
            #endif
        }
        Wordle(): maxAttempts(6), attemptNum(0), answer(randomWord()) {
            #ifdef DEBUG
                std::cout << "Wordle is: " << answer << std::endl;
            #endif
        }

        bool isCorrectWord(const std::string& attempt){
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


        const std::vector<int>& getHint(){
            return hint;
        }

        std::string hintAsString(){
            return iteratableToString(hint);
        }


        bool isGameOver(){
            return attemptNum > maxAttempts;
        }
    private:
        const std::string answer;
        std::vector<int> hint = {INVALID,INVALID,INVALID,INVALID,INVALID};
        const int maxAttempts;
        int attemptNum;
};

class WordleSolver {
    public:
        WordleSolver() {};

        void reset(){
            currentMatches = "-----";
            attemptCount = 0;

            mustHave.clear();
            cannotHave.clear();
        }

        bool solve(Wordle* wordle){
            reset();

            while(!wordle->isGameOver()){
                auto attempt = nextAttempt(attemptCount);
                bool isSolved = wordle->isCorrectWord(attempt);
                saveHint(wordle->getHint(), attempt);

                #ifdef DEBUG
                    std::cout << "ATTEMPT #" << attemptCount << " - Trying word: " << attempt << " | hint: " << wordle->hintAsString() << " | currentMatches: " << currentMatches << " | mustHave: " << iteratableToString(mustHave, ',') << " | cannotHave: " << iteratableToString(cannotHave, ',')  << std::endl;
                #endif

                if(isSolved){
                    return true;
                }

                attemptCount++;
            }

            return false;
        }

    private:
        std::unordered_set<char> cannotHave;
        std::unordered_set<char> mustHave;

        std::string currentMatches;
        int attemptCount;

        void saveHint(const std::vector<int>& hint, const std::string attempt){
            for(int i = 0; i < 5; i++){
                switch(hint[i]){
                    case VALID: 
                        currentMatches[i] = attempt[i];

                        // remove certain match from mustHave to speed up search in next attempts
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

        const std::string nextAttempt(int attemptCount) {
            // Using cones and trials for the first two moves has a high probability of creating at least 1+ match
            if(attemptCount == 0){
                return "cones";
            }else if(attemptCount == 1){
                // If we found any match in 'cones' that would make 'trial' impossible, then don't waste an attempt.
                if(isPotentialMatch("trial")){
                    return "trial";
                }
            }  
            
            std::vector<std::string> potentialMatches;

            // Find potential matches using the currently known data (confirmed letters, confirmed positions and invalid letters)
            for(auto word : words) {
                if(isPotentialMatch(std::move(word))){
                    potentialMatches.push_back(word);
                }
            }

            if(potentialMatches.size() > 0){
                return potentialMatches[0];
            }

            // If somehow we have no potential match, return a random word
            return "words";
        }

        bool isPotentialMatch(const std::string&& word){
            int mustHaveMatch = 0;

            
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
};

int main(int argc, char** argv){
    if(argc == 1){
        std::cout << "Input file is missing!" << std::endl;
        return 0;
    }

    if(argc < 3){
        std::cout << "Number of games is missing" << std::endl;
        return 0;
    }

    int gameCount = std::atoi(argv[2]);
    std::cout << "Simulating using " << gameCount << " games." << std::endl;


    srand(time(NULL));
    buildWordList(argv[1]);

    int succesfulGame = 0;
    auto solver = WordleSolver();

    for(auto i = 0; i < gameCount; ++i){
        auto game = Wordle();

        if(solver.solve(&game)){
            succesfulGame++;
        }
    }

    std::cout << "Succesfully solved " << succesfulGame << " out of " << gameCount << " games. (Win Rate "; printf("%.2f", float(succesfulGame)/float(gameCount)*100); std::cout << "%)" << std::endl;
}