#include <vector>

class A {

public:
    A(int a, int b, int c){}
};

int main(){

    //Qualified by a type, create an object of type, initialize with list
    //list used as constructor arguments
    A a{1, 2, 3};

    int b[]= {1, 2, 3};

    //list used to construct an initializer_list object first.
    //std::vector has a initializer-list constructor
    std::vector<int> v= {1, 2, 3};
    

    return 0;
}
