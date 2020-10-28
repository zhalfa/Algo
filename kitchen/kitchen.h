#pragma once

#include<string>
#include<list>
#include<unordered_map>
#include<cstdlib>
#include<cassert>
#include "boost/chrono.hpp"

boost::chrono::milliseconds gTimeInterval(200);
double gOrderAgeInc = 200.0f/1000; 

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
        case hot: return "hot";
        case cold:return "cold";
        case frozen:return "frozen";
        default: return "";
    }
}

class order{

public:
    order(string id, string name, temperature temp, int shelfLife, float decayRate):
        m_id(id), m_name(name), m_temp(temp), m_shelfLife(shelfLife), m_decayRate(decayRate), m_orderAge(0) {
    }

    float decay(size_t modifier){

        m_orderAge+= gOrderAgeInc;
        
        float val = m_shelfLife - m_orderAge - m_orderAge*m_decayRate*modifier;
        val/= m_shelfLife;
        return val;
    }
    
    temperature getTemperature(){ return m_temp;}

    void getOrderInfo(string& str){

        str+= m_id;
        str+= " : ";
        str+= m_name;
        str+= " : ";
        str+= convertTemperatureToString(m_temp);
        str+= " : ";
        str+= std::to_string(m_shelfLife);
        str+= " : ";
        str+= std::to_string(m_decayRate);
        str+= " : ";
        str+= std::to_string(m_orderAge);
        str+= "\n";
    }
private:

    string m_id;
    string m_name;
    temperature m_temp;
    int m_shelfLife;
    float m_decayRate;
    double m_orderAge;
};

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>


class jsonProcessor{

public:
    jsonProcessor(string path): m_path(path), m_hasErr(false), m_cnt(0){

    }

    bool prepare(string path = ""){
        if (path != "")
            m_path = path;

        std::ifstream input(m_path);       
        try{

            boost::property_tree::json_parser::read_json(input, m_root);

        }catch(boost::property_tree::json_parser::json_parser_error& e){   

            m_hasErr = true;

            return false;
        }
        
        for(auto i : m_root){ m_cnt++; }

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
    
    size_t getOrdersInfo(string& str){
        
        size_t cnt = 0;
        for (auto i: m_space){
            cnt++;        
            i->getOrderInfo(str);
        }
        if (cnt)
            str+= "----------------------\n";
        return cnt;
    }

private:
    size_t decayOrClean(std::list<order*>& rm_list, bool decay=true){

        if ( m_cnt == 0 ) return 0;

        std::list<order*> list;

        for( auto p : m_space ){

            assert(p);

            if (!decay)
                list.push_back(p);

            if (decay && (p->decay(m_shelfDecayModifier)<= 0)){
                
                list.push_back(p);
            }
        }
        
        size_t ret = list.size();

        for( auto p : list ){ 
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

        for ( auto p : list ){

            assert(p);
            removeOrder(p);
            rm_list.push_back(p);
        }

        return ret;
    } 

    size_t getOrdersInfo(string& str){
        size_t cnt = 0;

        for( auto p : m_space ){
            cnt++;
            assert(p);
            p->getOrderInfo(str);
        }
        if (cnt)
            str+= "----------------------\n";
        return cnt;
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

#include "boost/thread/thread.hpp"
#include "boost/thread/mutex.hpp"

typedef boost::lock_guard<boost::mutex> MutexType;

class commonThread{

public:
    void start(){
        MutexType lock(m_mtx);
        m_start = true;

        boost::thread thr([this](){
                
                this->run();
            }
        );
        m_thread = boost::move(thr);
    }
    
    void stop(){
        m_mtx.lock();
        m_start = false;
        m_mtx.unlock();
        m_thread.join();
    }

    bool isEmpty(){
        MutexType lock(m_mtx);
        return checkEmpty();
    }

    void run(){

        while(m_start){
            {
                MutexType lock(m_mtx);
                doWork();
            }   
            boost::this_thread::sleep_for(gTimeInterval);
        }   
    }

private:
    virtual void doWork() = 0; 
    virtual bool checkEmpty() = 0;
protected:
    boost::mutex m_mtx;
    boost::thread m_thread;
    bool m_start;
};


enum messageID{

    msgUnknown = 0,
    msgOrderReceived,
    msgOrderPickuped,
    msgOrderDiscarded,
    msgOrderDecayed
};

class commonMessagerReceiver{
public:
    virtual void onMessage( messageID id, const string& str)=0;
};

#include <iostream>

class messageOutput: public commonMessagerReceiver {
public:
    virtual void onMessage( messageID id, const string& str){
        std::cout <<"message id" << id << std::endl;
        //std::cout << str;
    }
};

struct courier{
    courier() {} 
    courier(size_t t, order* p): m_pickupTime(t), m_pOrder(p) {}

    boost::chrono::milliseconds m_pickupTime; 
    order* m_pOrder;
};

struct compare_courier{

    bool operator()(const courier& left, const courier& right) const {

        return left.m_pickupTime > right.m_pickupTime;
    }
};

class kitchen: public commonThread {

public: 
    kitchen(): m_pOverflow(NULL), m_inUse(false), m_wasteCnt(0){

    }
    ~kitchen(){
        clear();
    }

    bool init(commonMessagerReceiver* pLog = NULL){
        MutexType lock(m_mtx);
        m_pLog = pLog;
        return buildStorage();
    }

    bool onOrder( order* pOrder ){

        if (!m_inUse || !pOrder) return false;
        MutexType lock(m_mtx);
        if (!m_inUse ) return false;

        bool ret = putOrder(pOrder);
        getStatus(true);
        if (m_pLog) m_pLog->onMessage(msgOrderReceived, m_logDetails);
        return ret;
    }
    
    size_t getOrdersCnt(){
        MutexType lock(m_mtx);
        return getStatus();
    }
    
    size_t getWasteCnt(){
        MutexType lock(m_mtx);
        return m_wasteCnt;
    }
    
    boost::chrono::milliseconds getTime(){
        return m_time;
    }

    //single thread unit test support
    void update(){
        MutexType lock(m_mtx);
        m_time += gTimeInterval;
        decay();    
    }

    bool onCourier(courier* pCourier){

        if (!m_inUse || !pCourier) return false;
        MutexType lock(m_mtx);
        if (!m_inUse ) return false;
        order *p = pickUp(pCourier->m_pOrder);
        if (p){
            getStatus(true);
            if (m_pLog) m_pLog->onMessage(msgOrderPickuped, m_logDetails);
        }
        return p;
    }

private:

    //for following mumber functions, their caller must acquire mutex first
    virtual void doWork(){

        m_time += gTimeInterval;
        decay();
    }
    
    virtual bool checkEmpty(){ return (getStatus()==0); }

    typedef boost::container::stable_vector<storeShelf*> ShelvesVectorType;
    typedef boost::container::stable_vector<storeShelf*>::iterator ShelvesVectorIteratorType;
    typedef boost::unordered_map<temperature, ShelvesVectorIteratorType>::value_type MapPairType; 
    
    order* pickUp(order* pOrder){

        order* ret = m_pOverflow->removeOrder(pOrder);
        if (!ret){
            storeShelf *shelf = findShelf(pOrder);
            assert(shelf);
            ret = shelf->removeOrder(pOrder);
        }

        return ret;
    } 

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

        for ( auto p : list ){

            auto tmp = p;
            waste(tmp);
        }
        list.clear();
    }

    //if no existing order is discarded, return true, otherwise return false
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
                //assert(!ret);
                if (ret) discard = NULL;

            }else{
                ret = true;
            }
        }
        if (discard){ 
            waste(discard);
            if (m_pLog) m_pLog->onMessage(msgOrderDiscarded, m_logDetails);
        }
        return ret;
    }

    void decay(){

        if (!m_inUse) return;

        std::list<order*> rm_list;

        for ( auto shelf : m_shelves ){

            assert(shelf);
            shelf->decay(rm_list);
        }
        
        m_pOverflow->decay(rm_list);

        if (rm_list.size()){
            getStatus(true);
            if (m_pLog) m_pLog->onMessage(msgOrderDecayed, m_logDetails);
        }
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

        for ( auto p : m_shelves ){
            
            assert(p->getOrdersCnt() == 0);
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

    size_t getStatus( bool orderDetails = false){
        size_t total = 0;
        if (!m_inUse) return total;

        string& details = m_logDetails;
        if (orderDetails) details.empty();

        for ( auto p: m_shelves){

            total += p->getOrdersCnt();
            if (orderDetails)
                p->getOrdersInfo(details);
        }

        total += m_pOverflow->getOrdersCnt();
        if (orderDetails)
            m_pOverflow->getOrdersInfo(details);

        return total;
    }

    boost::chrono::milliseconds m_time;

    ShelvesVectorType m_shelves;
    boost::unordered_map<temperature, ShelvesVectorIteratorType> m_index;
    storeOverflow* m_pOverflow;
    bool m_inUse;
    size_t m_wasteCnt;
    string m_logDetails;
    commonMessagerReceiver* m_pLog;
};



#include "boost/heap/priority_queue.hpp"

class courierDispatcher: public commonThread {

public:
    bool onCourier(courier cr){
        MutexType lock(m_mtx);
        return putCourier(cr);
    }
    
private:

    virtual void doWork(){

        while(m_space.size()){
            
            courier tmp = getCourier();
            tmp;
        }
    }

    virtual bool checkEmpty(){ return (m_space.size()==0); };

    courier getCourier(){

        courier ret(0,NULL);

        if ( m_space.size()){

            ret = m_space.top();
            m_space.pop();
        }
        return ret;
    }

    bool putCourier(courier cr){
        m_space.push(cr);
        return true;
    }

    boost::heap::priority_queue<courier, boost::heap::compare<compare_courier>> m_space;
};

class orderIngester{
public:
    orderIngester(kitchen* pKitchen, courierDispatcher* pDispatcher): m_pKitchen(pKitchen),
         m_pDispatcher(pDispatcher), m_TimeInterval(500), m_json("") {

    }

    bool setFile(string& path){

        return m_jsonOK = m_json.prepare(path);
    }

    int setRate(unsigned int rate){
        assert(rate < 1000);

        size_t t = 1000/rate; 
        m_TimeInterval = boost::chrono::milliseconds(t); 
        return 0;
    }

    void run(){

        if (!m_jsonOK || !m_pDispatcher || !m_pKitchen ) return ;
        order *p;
        while ( p = m_json.getOrder()){
            
            m_pKitchen->onOrder(p);

            courier tmp;
            tmp.m_pOrder = p;
            //random 2 ~ 6 seconds;
            size_t random = std::rand()%7;
            if (random < 2 ) random = 2;

            tmp.m_pickupTime = m_pKitchen->getTime() + boost::chrono::milliseconds(1000*random);
            m_pDispatcher->onCourier(tmp);

            boost::this_thread::sleep_for(m_TimeInterval);
        }
    }

private:
    int sendOrder(order*);
    int arrangeCourier(courier*);
    kitchen* m_pKitchen;
    courierDispatcher* m_pDispatcher;

    boost::chrono::milliseconds m_TimeInterval;
    jsonProcessor m_json;
    bool m_jsonOK;    
};

