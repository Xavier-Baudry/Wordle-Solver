#ifndef UTIL_H
#define UTIL_H

//#define DEBUG
//#define VERBOSE_DEBUG

#include <sstream>
#include <string>
#include <iostream>

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
    for(auto it= iter.begin(); it != iter.end(); it++){
        s << *it;
        if(it!= last){
            s << seperator;
        }
    }

    return s.str();
}

#endif