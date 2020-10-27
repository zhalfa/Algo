#include "kitchen.h"
#include <iostream> 

void test_orderfile_not_exist(){

    jsonProcessor orderNoExist("/home/zhangl/_orders.json");
    orderNoExist.prepare();
}

void test_getOrder(){

    jsonProcessor orderSrc("/home/zhangl/orders.json"); 
    orderSrc.prepare();
    order* p = orderSrc.getOrder();
    assert(p);

    if (p) delete p;
}

void test_storeOverflow(){

    jsonProcessor orderSrc("/home/zhangl/orders.json"); 
    orderSrc.prepare();

    const size_t capacity = 30;
    storeOverflow overflow(capacity);

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
}

void test_storeShelf(){

    jsonProcessor orderSrc("/home/zhangl/orders.json"); 
    orderSrc.prepare();

    const size_t capacity = 10;
    storeShelf shelf( cold, capacity);

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
}

void test_kitchen(){
    
    jsonProcessor orderSrc("/home/zhangl/orders.json"); 
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

}

void test_courierDispatcher(){
    
    courierDispatcher dispatcher;
    
    dispatcher.start();

    dispatcher.onCourier(courier(3,NULL));
    dispatcher.onCourier(courier(3,NULL));
    dispatcher.onCourier(courier(2,NULL));
    dispatcher.onCourier(courier(2,NULL));
    dispatcher.onCourier(courier(1,NULL));
    dispatcher.onCourier(courier(1,NULL));

    dispatcher.close();
}

int main(){
    
    test_courierDispatcher();
    
    test_kitchen();

    test_storeShelf();
    test_storeOverflow();//~storeOverflow need more

    test_getOrder();
    test_orderfile_not_exist();

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
