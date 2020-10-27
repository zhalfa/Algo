#pragma once

#include<string>
#include<list>
#include<array>
#include<unordered_map>
#include<cstdlib>
#include<cassert>

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
    order(string id, string name, temperature temp, int shelfLife, float decayRate):
        m_id(id), m_name(name), m_temp(temp), m_shelfLife(shelfLife), m_decayRate(decayRate), m_orderAge(0) {
    }

    float decay(size_t modifier){

        m_orderAge++;
        
        float val = m_shelfLife - m_orderAge - m_orderAge*m_decayRate*modifier;
        val/= m_shelfLife;
        return val;
    }

    temperature getTemperature(){ return m_temp;}
private:

    string m_id;
    string m_name;
    temperature m_temp;
    int m_shelfLife;
    float m_decayRate;
    size_t m_orderAge;
};

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>


class jsonProcessor{

public:
    jsonProcessor(string path): m_path(path), m_hasErr(false), m_cnt(0){

    }

    bool prepare(){

        std::ifstream input(m_path);       
        try{

            boost::property_tree::json_parser::read_json(input, m_root);

        }catch(boost::property_tree::json_parser::json_parser_error& e){   

            m_hasErr = true;

            return false;
        }
        
        for(auto it = m_root.begin(); it != m_root.end(); it++ ){

           m_cnt++;
        }

        m_it = m_root.begin();
        return true;
    }

    order* getOrder(){

        order* ret = NULL;

        if (m_it != m_root.end() ) {

            auto it = m_it;

            string id = it->second.get<string>("id");
            string name = it->second.get<string>("name");
            string temp = it->second.get<string>("temp");
            int shelfLife = it->second.get<int>("shelfLife");
            float decayRate = it->second.get<float>("decayRate");

            ret = new order(id, name, convertToTemperature(temp), shelfLife, decayRate);

            m_it++;
        }
        return ret;
    };

private:

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

    string m_path;
    boost::property_tree::ptree m_root;
    boost::property_tree::ptree::iterator m_it;
    bool m_hasErr;
    size_t m_cnt;
};


class storeShelf{

public:
    storeShelf(temperature temp, size_t max): m_temp(temp), m_maxCnt(max), m_cnt(0){

        m_shelfDecayModifier = 1;
    }

    size_t getOrdersCnt(){return m_cnt;}

    temperature getTemperature(){return m_temp;}

    bool hasOrder(order*pOrder){
        
        if (m_index.find(pOrder) == m_index.end()) return false;
        return true;
    }

    bool addOrder(order* pOrder){
    
        bool ret = false;

        if (pOrder && !hasOrder(pOrder) && (pOrder->getTemperature() == m_temp)){
            
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

            m_cnt--;
            ret = pOrder;
        }
        return ret;
    }

    size_t decay(std::list<order*>& rm_list){

        return decayOrClean(rm_list);
    }

private:
    size_t decayOrClean(std::list<order*>& rm_list, bool decay=true){

        if ( m_cnt == 0 ) return 0;

        std::list<order*> list;

        std::list<order*>::iterator it = m_space.begin();
        std::list<order*>::iterator end = m_space.end();
        for ( ; it != end; it++ ){

            order* p = *it;            
            assert(p);

            if (!decay)
                list.push_back(p);

            if (decay && (p->decay(m_shelfDecayModifier)<= 0)){
                
                list.push_back(p);
            }
        }
        
        size_t ret = list.size();

        it = list.begin();
        end = list.end();
        for ( ; it != end; it++ ){

            order* p = *it;
            assert(p);
            removeOrder(p);
            rm_list.push_back(p);
        }

        return ret;
    } 

    typedef list<order*>::iterator ItemIteratorType;

    list<order*> m_space;
    unordered_map<order*, ItemIteratorType> m_index;

    temperature m_temp;
    unsigned int m_maxCnt;
    unsigned int m_cnt;
    unsigned int m_shelfDecayModifier;
};

#include "boost/container/stable_vector.hpp"
#include "boost/unordered_map.hpp"


class storeOverflow{

public:
    storeOverflow(size_t max): m_maxCnt(max), m_cnt(0){

        m_shelfDecayModifier = 2;
    }

    size_t getOrdersCnt(){return m_cnt;}

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
               assert(it != m_space.end());

               MapPairType item(pOrder, it);
               m_index.insert(item);
               *ppDiscard = NULL;
               m_cnt ++;
               ret = true;

           }else{

               size_t size = m_space.size();
               size_t rand = std::rand()%size;// random 0 ~ size-1
               order* p = m_space[rand];
               OrderVectorIteratorType it = m_index.find(p)->second;
               assert(it != m_space.end());
               m_index.erase(p);

               m_space[rand] = pOrder;
               MapPairType item(pOrder, it);
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
            m_cnt--;
            ret = pOrder;
        }
        return ret;
    }
    
    size_t decay(std::list<order*>& rm_list){

        if ( m_cnt == 0 ) return 0;

        size_t size = m_space.size();
        std::list<order*> list;

        for ( auto i = 0; i < size; i++ ){

            order* p = m_space[i];            
            assert(p!=NULL);
            if (p->decay(m_shelfDecayModifier)<= 0){
                
                list.push_back(p);
            }
        }
        
        size_t ret = list.size();

        std::list<order*>::iterator it = list.begin();
        std::list<order*>::iterator end = list.end();
        for ( ; it != end; it++ ){

            order* p = *it;
            assert(p);
            removeOrder(p);
            rm_list.push_back(p);
        }

        return ret;
    } 
    
private:
    
    typedef boost::container::stable_vector<order*> OrderVectorType;
    typedef boost::container::stable_vector<order*>::iterator OrderVectorIteratorType;
    typedef boost::unordered_map<order*, OrderVectorIteratorType>::value_type MapPairType; 

    OrderVectorType m_space;

    boost::unordered_map<order*, OrderVectorIteratorType> m_index;

    unsigned int m_maxCnt;
    unsigned int m_cnt;
    unsigned int m_shelfDecayModifier;
};

class kitchen{

public: 
    kitchen(): m_pOverflow(NULL), m_inUse(false), m_wasteCnt(0){

    }
    ~kitchen(){
        clear();
    }

    bool init(){
        return buildStorage();
    }

    bool onOrder( order* pOrder ){

        if (!m_inUse || !pOrder) return false;
        //lock obj
        if (!m_inUse ) return false;

        return putOrder(pOrder);

    }
    
    size_t getOrdersCnt(){
        return showStatus();
    }
    
    size_t getWasteCnt(){
        return m_wasteCnt;
    }

    void update(){
        decay();    
    }

    int onCourier();
    int onTime();
private:

    typedef boost::container::stable_vector<storeShelf*> ShelvesVectorType;
    typedef boost::container::stable_vector<storeShelf*>::iterator ShelvesVectorIteratorType;
    typedef boost::unordered_map<temperature, ShelvesVectorIteratorType>::value_type MapPairType; 

    //for following mumber functions, caller must be thread safe
    
    storeShelf* findShelf(order* pOrder){
        storeShelf* ret = NULL;

        if(pOrder){
            temperature temp = pOrder->getTemperature();
            ShelvesVectorIteratorType it = m_index.find(temp)->second; 
            ret = *it;
        }
        return ret;
    }
    
    void waste(order*& pOrder){
        m_wasteCnt++;
        delete pOrder;
        pOrder = NULL;
    }

    void destroy_orders(std::list<order*>& list ){

        std::list<order*>::iterator it = list.begin();
        std::list<order*>::iterator end = list.end();
        for ( ; it != end; it++ ){

            order* p = *it;
            waste(p);
        }
        list.clear();
    }

    //if do not discard any existing order, return true
    bool putOrder(order* pOrder){
        bool ret = false;

        storeShelf* shelf = findShelf(pOrder);
        assert(shelf);

        order* discard = NULL;
        if (shelf->addOrder(pOrder)){

            ret = true;

        }else{
            m_pOverflow->addOrder(pOrder, &discard);
            if (discard){

                storeShelf* shelf = findShelf(discard);
                assert(shelf);
                ret = shelf->addOrder(discard);
                assert(!ret);
                if (ret) discard = NULL;

            }else{
                ret = true;
            }
        }
        if (discard) waste(discard);

        return ret;
    }

    void decay(){

        if (!m_inUse) return;

        std::list<order*> rm_list;

        ShelvesVectorIteratorType it = m_shelves.begin();
        ShelvesVectorIteratorType end = m_shelves.end();
        for ( ; it != end; it++ ){

            storeShelf *shelf = *it;
            assert(shelf);
            shelf->decay(rm_list);
        }
        
        m_pOverflow->decay(rm_list);

        destroy_orders(rm_list);
    }

    bool buildStorage(){

        if (m_inUse) return false;

        m_shelves.push_back( new storeShelf(hot, 10) );       
        m_shelves.push_back( new storeShelf(cold, 10) );       
        m_shelves.push_back( new storeShelf(frozen, 10) );       

        ShelvesVectorIteratorType it = m_shelves.begin();
        ShelvesVectorIteratorType end = m_shelves.end();
        for ( ; it != end; it++ ){

            assert(*it);
            temperature temp = (*it)->getTemperature();
            MapPairType item(temp,it);
            m_index.insert(item);
        }

        m_pOverflow = new storeOverflow(15);
        if (m_pOverflow && m_shelves.size()){

            m_inUse = true;
        }          
        return m_inUse;
    }

    void clear(){

        ShelvesVectorIteratorType it = m_shelves.begin();
        ShelvesVectorIteratorType end = m_shelves.end();
        for ( ; it != end; it++ ){
            
            storeShelf *p = *it;
            assert(p);
            delete p;
        }
        m_shelves.clear();
        m_index.clear();

        if (m_pOverflow){
            delete m_pOverflow;
            m_pOverflow = NULL;
        }
        m_inUse = false;
    }

    size_t showStatus(){
        size_t total = 0;

        if (!m_inUse) return total;

        ShelvesVectorIteratorType it = m_shelves.begin();
        ShelvesVectorIteratorType end = m_shelves.end();
        for ( ; it != end; it++ ){

            storeShelf *shelf = *it;
            total += shelf->getOrdersCnt();
        }
        total += m_pOverflow->getOrdersCnt();

        return total;
    }

    ShelvesVectorType m_shelves;
    boost::unordered_map<temperature, ShelvesVectorIteratorType> m_index;
    storeOverflow* m_pOverflow;
    bool m_inUse;
    size_t m_wasteCnt;
};

class courier{

private:
    order* m_pOrder;
};

class courierDispatcher{

public:
    int onCourier();
    int sendCourier();
    void run();
};

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

