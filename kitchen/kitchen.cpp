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

string convertMessageToString(messageID msg){

    switch(msg){
        case msgOrderReceived:  return "OrderReceived ";
        case msgOrderPickuped:  return "OrderPickuped ";
        case msgOrderDiscarded: return "OrderDiscarded";
        case msgOrderDecayed:   return "OrderDecayed  ";
        case msgOrderMissed:    return "OrderMissed   ";
        default:                return "              ";
    }
}

void kitchen::moveOverflowToShelves(order* pOrder){

    temperature tpr = any;

    if( pOrder) tpr = pOrder->getTemperature(); 

    std::list<store*> check;

    for( auto s: m_shelves){

        temperature t = s->getTemperature();
        if ((t == tpr)||(t==any)) continue;
        if(s->isFull()) continue;

        check.push_front(s);
    }

    typedef std::pair<temperature, shelfInfo*> shelvesInfoItemType; 

    shelvesInfoType info;

    if (check.size()){

        for(auto shelf: check){

            shelfInfo* p = new shelfInfo (shelf->getTemperature(), shelf);
            if (p){
                
                shelvesInfoItemType item( p->m_tpr, p);
                info.insert(item);
            }
        }
    } 
    
    check.clear();

    storeOverflow* pOverflow = (storeOverflow*) m_pOverflow;
    pOverflow->getMoveList(info);
}

void storeOverflow::getMoveList(shelvesInfoType& available){



}
