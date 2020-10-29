#include "kitchen.h"
#include <iostream> 

#define PASS_TEST \
    { std::cout<< "test pass:" << __FUNCTION__ << std::endl; };

class messageOutput: public commonMessagerReceiver {

public:
    messageOutput(bool enable = false ): m_enable(enable) {
        m_pickuped = 0;
    }

    virtual void onMessage( messageID id, const string& str){

        if (m_enable){
            string msg = "Message: ";
            msg += convertMessageToString(id); msg += "\n";
            std::cout << msg; 
            std::cout << str;
        }
        if (id == msgOrderPickuped ) m_pickuped++;
    }
    
    size_t m_pickuped;
private:
    bool m_enable;
};
