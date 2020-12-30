#define TEST

#ifdef TEST

    #define TYPE auto
    #define XXXX -> void

    // fails because of surfix : const and noexcept

#else

    #define TYPE void
    #define XXXX  

#endif

// p307 function declaration
//
// deduced return type only available with ‘-std=c++14’ or ‘-std=gnu++14’

struct F{

    [[noreturn]] virtual inline TYPE f_name(const unsigned long int* const) XXXX const noexcept {};

};

#include <string>

using std::string;

constexpr auto convert(int) -> string;

int main(){


    return 0;
}
