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
        case msgOrderMoved:     return "OrderMoved    ";
        default:                return "              ";
    }
}

// move some orders into its shelf from overflow area
// when overflow is full. add order to kitchen can triggle 
// this scenario. valid pOrder means the shelf with the
// same temperature is also full
//
// we can also actively call this function routinely with
// invalid pOrder to prevent orders from decaying fast

void kitchen::moveOverflowToShelves(order* pOrder){

    temperature tpr = any;

    if( pOrder) tpr = pOrder->getTemperature(); 

    std::list<baseShelf*> check;

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
        check.clear();

        overflowShelf* pOverflow = (overflowShelf*) m_pOverflow;
        size_t moveCnt = pOverflow->getMoveList(info);

        if (moveCnt){

            for(auto it : info ){

                shelfInfo* pInfo = it.second;
                baseShelf* shelf= pInfo->m_shelf; 

                for( auto pOrder: pInfo->m_list){

                    assert(pOrder->getTemperature()==pInfo->m_tpr); 
                    m_pOverflow->removeOrder(pOrder); 
                    bool res = shelf->addOrder(pOrder, NULL);
                    assert(res);
                }
                pInfo->m_list.clear();
                delete pInfo;
            }
            info.clear();
            if (m_pLog) m_pLog->onMessage(msgOrderMoved, "\n");
        }
    } 
}

size_t overflowShelf::getMoveList(shelvesInfoType& available){
    size_t ret = 0;
    if (available.size()== 0) return ret;

    if(getOrdersCnt()){

        auto end = available.end();
        for( auto pOrder: m_space){

            temperature tpr = pOrder->getTemperature();
            auto it = available.find(tpr);
            if (it == end) continue;

            shelfInfo* pInfo = it->second;
            assert(pInfo->m_tpr == tpr);

            size_t cnt = pInfo->m_shelf->getAvailableCnt();

            if(pInfo->m_list.size()< cnt){
                pInfo->m_list.push_front(pOrder);
                ret++;
            }
        }
    }
    return ret;
}
