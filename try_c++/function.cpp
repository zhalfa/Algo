#define TEST

#ifdef TEST

    #define XXXX -> void
    #define TYPE auto

#else

    #define XXXX  
    #define TYPE void

#endif

// p307 function declaration
//
// deduced return type only available with ‘-std=c++14’ or ‘-std=gnu++14’

struct F{

    [[noreturn]] virtual inline TYPE f(const unsigned long int* const) XXXX const noexcept;

};


int main(){


    return 0;
}
