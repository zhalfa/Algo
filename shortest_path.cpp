#include <stdio.h>
int main(){

enum{
    a=0,
    b,
    c,
    d,
    e,
    t,
    M=1<<20
}type;  

unsigned int size = t-a + 1;

printf("%d\n",size);
printf("%d\n", M);

signed int g[size][size]={

    {0, -4,  M,  M,  M, -3},
    {M,  0,  M, -1, -2,  M},
    {M,  8,  0,  M,  M,  3},
    {6,  M,  M,  0,  M,  4},
    {M,  M, -3,  M,  0,  2},
    {M,  M,  M,  M,  M,  0}

};
unsigned edges= size;
signed int opt[size][edges];

    for(int i=0; i < edges; i++){
        opt[t][i]= 0;
    }
}
