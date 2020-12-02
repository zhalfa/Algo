#include "boost/asio.hpp"
#include "boost/asio/spawn.hpp"
#include "boost/bind.hpp"
#include "boost/date_time.hpp"

#include <iostream>

namespace asio = boost::asio;

void wait_and_print( boost::asio::yield_context yield, boost::asio::io_service service ){

    boost::asio::deadline_timer timer(service);
    timer.expires_from_now(boost::posix_time::seconds(5));
    timer.async_wait(yield);

    std::cout << "Hello, " << std::flush;

    timer.expires_from_now(boost::posix_time::seconds(10));
    timer.async_wait(yield);

    std::cout << "World" << std::endl;
}

void test_coro( boost::asio::yield_context yield ){

    std::cout << "Hello, " << std::flush;
}

int main(){

    try{    
        asio::io_service service;
        asio::ip::tcp::resolver resolver(service);
        asio::ip::tcp::resolver::query query("www.hotmail.com","https");
        asio::ip::tcp::resolver::iterator end; 
        asio::ip::tcp::resolver::iterator iter = resolver.resolve(query); 

        while(iter != end){

            asio::ip::tcp::endpoint endpoint = iter->endpoint();

            std::cout << "Address: " << endpoint.address() << " Port: " << endpoint.port() << std::endl;
            std::cout << iter->host_name() << "  " << iter->service_name() << std::endl;
            iter++;
        }

    }
    catch(std::exception& e){

    }

    {
        asio::io_service service;

        //sample not suit for new boost
        auto t = boost::bind(wait_and_print, ::_1, boost::ref(service));
        //boost::asio::spawn(service, t);

        boost::asio::spawn(service, test_coro);

        service.run();
    }

    return 0;
}
