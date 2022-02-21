#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <sstream>
#include <unordered_set>

#include "lib/Wordle.h"
#include "lib/Util.h"
#include "lib/WordBank.h"
#include "lib/Solver.h"

WordBank words = WordBank();

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
    
    words.setData(argv[1]);
    
    int succesfulGame = 0;
    auto solver = Solver();

    for(auto i = 0; i < gameCount; ++i){
        auto game = Wordle();

        if(solver.solve(&game)){
            succesfulGame++;
        }
    }

    std::cout << "Succesfully solved " << succesfulGame << " out of " << gameCount << " games. (Win Rate "; printf("%.2f", float(succesfulGame)/float(gameCount)*100); std::cout << "%)" << std::endl;
}