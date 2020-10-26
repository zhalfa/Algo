#include"kitchen.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include <boost/date_time.hpp>
//#include <boost/foreach.hpp>
#include <fstream>


using std::ifstream;

class jsonProcessor{

public:
    jsonProcessor(string path): m_path(path), m_hasErr(false), m_cnt(0){

    }

    bool prepare(){

        ifstream input(m_path);       
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

void test_orderfile_not_exist(){

    jsonProcessor orderNoExist("/home/zhangl/_orders.json");
    orderNoExist.prepare();
}

void test_getOder(){

    jsonProcessor orderSrc("/home/zhangl/orders.json"); 
    orderSrc.prepare();
    order* p = orderSrc.getOrder();

    if (p) delete p;
}


int main(){

    //test_orderfile_not_exist();
    test_getOder();

    kitchen *pKitchen;
    courierDispatcher *pDispatcher;

    orderIngester ingester(pKitchen, pDispatcher);
#if 0
    ingester.setFile();
    ingester.setRate();

    ingester.run();

    if ( pDispatcher->empty()){

        //pDispatcher join

        if (pKitchen->close()){

            //pKitchen->join
        }
    }
#endif
    return 0;
}
