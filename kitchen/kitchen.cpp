#include "kitchen.h"
#include <iostream> 


boost::chrono::milliseconds gTimeInterval(200);
double gOrderAgeInc = 200.0f/1000; 

size_t order::s_ordersCnt = 0;

temperature convertToTemperature(string s ){

    if ( s == "hot" ){
        return hot;
    }else if( s == "cold" ){
        return cold;
    }else if ( s == "frozen"){
        return frozen;
    }else{
        return unknown;
    }
}

string convertTemperatureToString(temperature temp){

    switch(temp){
        case hot:    return "hot   ";
        case cold:   return "cold  ";
        case frozen: return "frozen";
        case any:    return "any   ";
        default:     return "      ";
    }
}

