#include "unit_test.h"

//unit test 
void test_orderfile_not_exist(){

    jsonProcessor orderNoExist("_orders.json");
    orderNoExist.prepare();

    PASS_TEST;
}

void test_getOrder(){

    jsonProcessor orderSrc("orders.json"); 
    orderSrc.prepare();
    order* p = orderSrc.getOrder();
    assert(p);

    if (p) delete p;

    PASS_TEST;
}

void test_overflowShelf(){

    jsonProcessor orderSrc("orders.json"); 
    orderSrc.prepare();

    const size_t capacity = 30;
    overflowShelf overflow(capacity,2);

    size_t orderCnt= 0; 
    size_t discardCnt = 0;

    order* p = NULL;
    while(p= orderSrc.getOrder()){

        assert(p->getTemperature() != unknown);

        orderCnt++;

        order* pDiscard;

        if( overflow.addOrder(p, &pDiscard) ){

        }
        if(pDiscard){
            delete pDiscard;
            discardCnt++;
        }
    }
    assert((orderCnt-discardCnt)==capacity);
    
    std::list<order*> rm_list;
    while(overflow.getOrdersCnt() ){

        overflow.decay(rm_list);
    }
    assert(rm_list.size()==capacity);    

    std::list<order*>::iterator it = rm_list.begin();
    std::list<order*>::iterator end = rm_list.end();

    while(it != end ){
        order* p = *it;
        assert(p != NULL);
        delete p;
        it++;
    }

    PASS_TEST;
}

void test_storeShelf(){

    jsonProcessor orderSrc("orders.json"); 
    orderSrc.prepare();

    const size_t capacity = 10;
    storeShelf shelf( cold, capacity, 1);

    size_t orderCnt= 0; 
    size_t discardCnt = 0;

    order* p = NULL;
    while(p= orderSrc.getOrder()){

        assert(p->getTemperature() != unknown);

        orderCnt++;

        order* pDiscard;

        if( shelf.addOrder(p) ){

        }else{
            delete p;
            discardCnt++;
        }
    }
    assert((orderCnt-discardCnt)==capacity);

    std::list<order*> rm_list;
    while(shelf.getOrdersCnt() ){

        shelf.decay(rm_list);
    }
    assert(rm_list.size()==capacity);    

    std::list<order*>::iterator it = rm_list.begin();
    std::list<order*>::iterator end = rm_list.end();

    while(it != end ){
        order* p = *it;
        assert(p && (p->getTemperature() == cold));
        delete p;
        it++;
    }

    PASS_TEST;
}

void test_kitchen(){
    
    jsonProcessor orderSrc("orders.json"); 
    orderSrc.prepare();

    kitchen kit_test;
    bool res = kit_test.init();
    assert(res);

    size_t orderCnt= 0; 

    order* p = NULL;
    while(p= orderSrc.getOrder()){

        assert(p->getTemperature() != unknown);

        orderCnt++;

        kit_test.onOrder(p);
    }
        
    //std::cout <<"json orders count:"<< orderCnt << std::endl;

    size_t cnt = kit_test.getOrdersCnt();
    assert(cnt==45);

    while(cnt = kit_test.getOrdersCnt()){
        
        //std::cout <<"kitchen orders count:"<< cnt << std::endl;        
        kit_test.update();
    }
    
    cnt = kit_test.getWasteCnt();
    assert(cnt == orderCnt);

    PASS_TEST;
}

void test_courierDispatcher(){
    
    courierDispatcher dispatcher;
    
    dispatcher.start();
    assert(dispatcher.isEmpty());

    dispatcher.onCourier(courier(3,NULL));
    dispatcher.onCourier(courier(3,NULL));
    dispatcher.onCourier(courier(2,NULL));
    dispatcher.onCourier(courier(2,NULL));
    dispatcher.onCourier(courier(1,NULL));
    dispatcher.onCourier(courier(1,NULL));

    while( !dispatcher.isEmpty())
        boost::this_thread::sleep_for(boost::chrono::seconds(2));

    dispatcher.stop();
    assert(dispatcher.isEmpty());

    PASS_TEST;
}

void test_kitchen_thread(){
    
    double save = gOrderAgeInc; 
    gOrderAgeInc = 2.0f;

    jsonProcessor orderSrc("orders.json"); 
    orderSrc.prepare();
    messageOutput log;

    kitchen kit_test;
    bool res = kit_test.init(&log);
    assert(res);

    kit_test.start();

    size_t orderCnt= 0; 

    order* p = NULL;
    while(p= orderSrc.getOrder()){

        assert(p->getTemperature() != unknown);

        orderCnt++;

        kit_test.onOrder(p);
    }
        
    size_t cnt = kit_test.getOrdersCnt();
    
    while( !kit_test.isEmpty())
        boost::this_thread::sleep_for(boost::chrono::seconds(4));

    kit_test.stop();
    cnt = kit_test.getWasteCnt();
    assert(cnt == orderCnt);

    gOrderAgeInc = save;

    PASS_TEST;
}

void test_ingester(){

    kitchen kit_test;
    messageOutput log;

    bool res = kit_test.init(&log);
    assert(res);

    kit_test.start();

    courierDispatcher dispatcher(&kit_test);
    dispatcher.start();

    orderIngester ingester(&kit_test, &dispatcher);

    string path ="orders.json";
    ingester.setFile(path);
    ingester.setRate(400);
    
    ingester.run();

    while( !kit_test.isEmpty())
        boost::this_thread::sleep_for(boost::chrono::seconds(4));

    kit_test.stop();
    dispatcher.stop();

    assert(dispatcher.isEmpty());

    size_t cnt = kit_test.getWasteCnt();
    assert(order::s_ordersCnt == (132+1));

    PASS_TEST;
}

int main(){

    test_ingester();

    test_kitchen_thread();    

    test_courierDispatcher();
    
    test_kitchen();

    test_storeShelf();
    test_overflowShelf();//~overflowShelf need more

    test_getOrder();
    test_orderfile_not_exist();

    return 0;
}
