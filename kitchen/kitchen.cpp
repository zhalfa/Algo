#include"kitchen.h"

int main(){
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
