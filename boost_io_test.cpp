
#include "w32api/_mingw.h"

#include "w32api/_bsd_types.h"

//with __LP64__ this head will fail on InterlockedIncrement
#include <boost/asio.hpp>

//without __LP64__ this head will fail
//#include "w32api/ws2tcpip.h"


namespace asio = boost::asio;

int main(){

    asio::io_service service;

}
