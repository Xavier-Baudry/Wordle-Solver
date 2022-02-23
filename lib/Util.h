#ifndef UTIL_H
#define UTIL_H

#define DEBUG
//#define VERBOSE_DEBUG

#include <sstream>
#include <string>
#include <iostream>

template<class t>
std::string iteratableToString(const t& iter){
    if(iter.empty()){
        return "";
    }

    std::stringstream s;
    for(const auto& it : iter){
        s << it;
    }
    return s.str();
}

template<class t>
std::string iteratableToString(const t& iter, char seperator){
    if(iter.empty()){
        return "";
    }

    std::stringstream s;
    for(auto it= iter.begin(); it != iter.end(); ++it){
        if(it != iter.begin()){
            s << seperator;
        }
        s << *it;
    }

    return s.str();
}

#endif