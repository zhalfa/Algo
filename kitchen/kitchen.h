#pragma once

#include<string>
#include<list>
#include<array>
#include<unordered_map>

using std::string;
using std::list;
using std::array;
using std::unordered_map;

enum temperature {

    hot = 0,
    cold,
    fronze
};


class order{

public:
    int onDecay();
    temperature getTemperature(){ return m_temp;}
private:

    unsigned int incAge(){
        
        return ++m_orderAge;
    }
    string m_id;
    string m_name;
    temperature m_temp;
    unsigned int m_decayRate;
    signed   int m_shelfLife;
    unsigned int m_orderAge;
};

class courier{

private:
    string m_orderId;
};

class kitchen;
class courierDispatcher;

class orderIngester{
public:
    orderIngester(kitchen* pKitchen, courierDispatcher* pDispatcher): m_pKitchen(pKitchen), m_pDispatcher(pDispatcher){

    }

    int setFile(string& path){

        m_path = path;
        return 0;
    }
    int setRate(unsigned int rate){

        m_rate = rate;
        return 0;
    }
    int run();
private:
    int sendOrder(order*);
    int arrangeCourier(courier*);
    
    string m_path;
    unsigned int m_rate;

    kitchen* m_pKitchen;
    courierDispatcher* m_pDispatcher;
};

class courierDispatcher{

public:
    int onCourier();
    int sendCourier();
};

class shelf{

public:
    bool hasOrder(order*pOrder){
        
        if (m_index.find(pOrder) == m_index.end()) return false;
        return true;
    }

    bool addOrder(order* pOrder){
    
        bool ret = false;

        if (pOrder && !hasOrder(pOrder)){
            
           shelfInfo& info = m_info[pOrder->getTemperature()]; 

           if (info.cnt < info.maxCnt){
           
                m_space.push_front(pOrder);
                std::pair<order*, ItemType> item(pOrder, m_space.begin());
                m_index.insert(item);
                ret = true;
           }
        }
        return ret;
    } 

    bool removeOrder(order* pOrder){
        bool ret = false;

        if ( hasOrder(pOrder)){
            ItemType it = m_index.find(pOrder)->second;
            m_space.erase(it);
            m_index.erase(pOrder);
            ret = true;
        }
        return ret;
    }

    void decay();
private:
//    unsigned int m_MaxCapacity;
//    temperature m_tempe;
//    unsigned int m_cnt;

    list<order*> m_space;

    typedef list<order*>::iterator ItemType;

    unordered_map<order*, list<order*>::iterator> m_index;

    struct shelfInfo{
        temperature tempe;
        unsigned int maxCnt;
        unsigned int cnt;
    }; 

    std::array<shelfInfo, 3> m_info;
};

class kitchen{

public: 
    int onOrder();
    int onCourier();
    int onTime();
private:
    bool putOrder();
    int buildStorage();
    bool showStatus();
};
