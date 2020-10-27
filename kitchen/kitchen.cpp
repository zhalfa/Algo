#include"kitchen.h"


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
    while(overflow.notEmpty() ){

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

int main(){

    test_storeOverflow();

    test_getOder();

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
