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

    storeOverflow overflow(15);

    order* p = NULL;
    while(p= orderSrc.getOrder()){

        order* pDiscard;

        overflow.addOrder(p, &pDiscard);
        if(pDiscard)
            delete pDiscard;
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
