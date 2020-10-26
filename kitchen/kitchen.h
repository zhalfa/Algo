#pragma once

#include<string>
#include<list>
#include<array>
#include<unordered_map>
#include<cstdlib>

using std::string;
using std::list;
using std::array;
using std::unordered_map;
using std::rand;

enum temperature {

    hot = 0,
    cold,
    frozen,
    unknown
};


class order{

public:
    order(string id, string name, temperature temp, size_t shelfLife, float decayRate):
        m_id(id), m_name(name), m_temp(temp), m_shelfLife(shelfLife), m_decayRate(decayRate), m_orderAge(0) {
    }

    int onDecay();
    temperature getTemperature(){ return m_temp;}
private:

    unsigned int incAge(){
        
        return ++m_orderAge;
    }
    string m_id;
    string m_name;
    temperature m_temp;
    size_t m_shelfLife;
    float m_decayRate;
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

class storeShelf{

public:
    bool hasOrder(order*pOrder){
        
        if (m_index.find(pOrder) == m_index.end()) return false;
        return true;
    }

    bool addOrder(order* pOrder){
    
        bool ret = false;

        if (pOrder && !hasOrder(pOrder)){
            
           //shelfInfo& info = m_info[pOrder->getTemperature()]; 

           if (m_cnt < m_maxCnt){
           
                m_space.push_front(pOrder);
                std::pair<order*, ItemIteratorType> item(pOrder, m_space.begin());
                m_index.insert(item);

                m_cnt ++;
                ret = true;
           }
        }
        return ret;
    } 

    order* removeOrder(order* pOrder){
        order* ret = NULL;

        if ( hasOrder(pOrder)){
            ItemIteratorType it = m_index.find(pOrder)->second;
            m_space.erase(it);
            m_index.erase(pOrder);

            shelfInfo& info = m_info[pOrder->getTemperature()]; 
            info.cnt--;
            ret = pOrder;
        }
        return ret;
    }

    void decay(){

    }

private:

    typedef list<order*>::iterator ItemIteratorType;

    list<order*> m_space;

    unordered_map<order*, ItemIteratorType> m_index;

    struct shelfInfo{
        temperature tempe;
        unsigned int maxCnt;
        unsigned int cnt;
    }; 

    std::array<shelfInfo, 3> m_info;
    temperature m_tempe;
    unsigned int m_maxCnt;
    unsigned int m_cnt;
};

#include "boost/container/stable_vector.hpp"
#include "boost/unordered_map.hpp"

class storeOverflow{

public:

    bool hasOrder(order*pOrder){
       
        if (m_index.find(pOrder) == m_index.end() ) return false;
        return true;
    }

    bool addOrder(order* pOrder, order** ppDiscard){
        bool ret = false;
        
        if (pOrder && ppDiscard && !hasOrder(pOrder)){
            
           if ( m_cnt < m_maxCnt){

               m_space.push_back(pOrder);
               unsigned int index = m_space.size()-1;

               OrderVectorIteratorType it = m_space.begin() + index;

               boost::unordered_map<order*, OrderVectorIteratorType>::value_type item(pOrder, it);
               m_index.insert(item);
               *ppDiscard = NULL;
               m_cnt ++;
               ret = true;

           }else{

               size_t size = m_space.size();
               size_t rand = std::rand()%size;//= random 0 ~ size-1
               order* p = m_space[rand];
               OrderVectorIteratorType it = m_index.find(p)->second;
               m_index.erase(p);

               m_space[rand] = pOrder;
               std::pair<order*, OrderVectorIteratorType> item(pOrder, it);
               m_index.insert(item);
               *ppDiscard = p;
               ret = true;
           }
        }
        return ret;
    }

    order* removeOrder(order* pOrder){
        order* ret = NULL;        

        if (hasOrder(pOrder)){

            OrderVectorIteratorType it = m_index.find(pOrder)->second;
            m_index.erase(pOrder);
            m_space.erase(it);
            m_cnt --;
            ret = pOrder;
        }
        return ret;
    }
private:
    typedef boost::container::stable_vector<order*> OrderVectorType;
    typedef boost::container::stable_vector<order*>::iterator OrderVectorIteratorType;
    
    OrderVectorType m_space;

    boost::unordered_map<order*, OrderVectorIteratorType> m_index;

    unsigned int m_maxCnt;
    unsigned int m_cnt;
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
