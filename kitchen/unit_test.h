#include "kitchen.h"
#include <iostream> 

#define PASS_TEST \
    { std::cout<< "test pass:" << __FUNCTION__ << std::endl; };

class messageOutput: public commonMessagerReceiver {
public:
    virtual void onMessage( messageID id, const string& str){
#if 0
        std::cout <<"message id" << id << std::endl;
        std::cout << str;
#endif
    }
};
