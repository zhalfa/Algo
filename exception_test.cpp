// exception example
// g++ -std=c++11 -g -O0 test.cpp -o test
// use gdb disassemble to investigate further

#include <iostream>       // std::cerr
#include <typeinfo>       // operator typeid
#include <exception>      // std::exception

class Polymorphic {

virtual void member(){}

};

void test_exception(){

    try
    {
        Polymorphic * pb = 0;
        typeid(*pb);  // throws a bad_typeid exception
    }
    catch (std::exception& e)
    {
        std::cerr << "exception caught: " << e.what() << '\n';
    }
}

int main () {

    int cnt = 100;
    while (cnt ){

        test_exception();
        cnt--;
    }
    return 0;
}
