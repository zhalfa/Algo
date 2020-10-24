#pragma once

#include<string>

using std::string;

enum temperature {

    hot,
    cold,
    fronze
};


class order{

public:
    int onDecay();

private:
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

private:
    unsigned int m_capacity;
};

class kitchen{

public: 
    int onOrder();
    int onCourier();
    int onTime();
private:

};
