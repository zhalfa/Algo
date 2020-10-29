#include "kitchen.h"
#include <iostream> 

#define PASS_TEST \
    { std::cout<< "test pass:" << __FUNCTION__ << std::endl; };

class messageOutput: public commonMessagerReceiver {

public:
    messageOutput(bool enable = false ): m_enable(enable){}

    virtual void onMessage( messageID id, const string& str){

        if (m_enable){
            std::cout <<"message id : " << id << std::endl;
            std::cout << str;
        }
    }
private:
    bool m_enable;
};
