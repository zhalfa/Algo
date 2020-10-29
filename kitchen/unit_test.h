#include "kitchen.h"
#include <iostream> 

class messageOutput: public commonMessagerReceiver {
public:
    virtual void onMessage( messageID id, const string& str){
        //std::cout <<"message id" << id << std::endl;
        //std::cout << str;
    }
};
