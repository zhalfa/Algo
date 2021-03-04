#if 0
// Given an array [3, 4, 5, 1, 4, 5, 6, 1, 9], a integer N.
//
// Example: N =5
// Output: [3, 4, 1, 4, 1, 5, 5, 6, 9]

{3, 4, 1, 1, 9, 5, 5, }

{3, 5, 4, 7, 1, 1, 9, 5, 8, 5, 6}

{3, 1, 4, 7, 1, 5, 9, 5, 8, 5, 6}

{3, 1, 4, 1, 7, 5, 9, 5, 8, 5, 6}

{3, 1, 4, 1(r), 5(l), 5, 9, 5(3), 8, 7, 6}


{3, 1, 4,|| 1(r), 5(l),|| 5, 9, 5(3),|| 8, 7, 6}



[...l...ir....]
{|.....|}

{..|...|}

[...l...i...r....]

while (i < r) {
array[i] > N: i <-> r, r--
         = N: i++;
         < N: exchange; l++; i++;
}

#endif

#include <cstddef>

void adjust_sequence_by_n( int n, int a[], int start, int end){

    if (a == NULL) return;
    
    if (start < 0) return;

    if (start >= end) return;

    int less_than_n = start - 1;
    int equal_n = less_than_n;

    int greater_than_n = end + 1; 

    while ((equal_n + 1) != greater_than_n){ //if equal_n adjacent to greater_than_n, finish.

        int current = equal_n + 1; 

        int temp = a[current];

        if ( temp > n) {

            --greater_than_n;

            a[current] = a[greater_than_n];

            a[greater_than_n] = temp;

        }else if (temp < n){

            ++less_than_n; // must adjust synchronously
            ++equal_n;

            if (equal_n != less_than_n){ //equal_n set not empty
        
                a[equal_n] = a[less_than_n];
            }

            a[less_than_n] = temp;

        }else{ //equal n

            ++equal_n;
            a[equal_n] = temp;

        }
    }
}


int main(){

    int a[9]= {3, 4, 5, 1, 4, 5, 6, 1, 9};

    int n = 5;

    adjust_sequence_by_n( n, a, 0, sizeof(a)/sizeof(int) - 1 );

    return 0;
}

