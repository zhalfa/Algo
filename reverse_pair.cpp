#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <cassert>

//#define USING_STL
#define SHOW_DETAIL

#ifndef USING_STL
int* g_src = nullptr;

int compare (const void * a, const void * b){

    long m = g_src[*(int*)a];
    long n = g_src[*(int*)b];
    return ( m - n );
}
#endif

int find_first(const int* src, int begin, int end, long val, const int* act){

    const int* vec = src;
    int s = begin;
    int e = end;

    int ret = -1;
    while(s <= e){

        int mid = (s+e)/2;

        if ( val < act[ vec[mid] ] ){

            e = mid - 1;

        }else if (val == act[ vec[mid] ] ){

            while((mid <= end) && (val == act[ vec[mid] ])){

                ++mid;
            }
            ret = mid;
            break;

        }else{

            s = mid + 1;
        }

        if (s > e){

            if (val < act[ vec[mid]])
                ret = mid;
            else
                ret = mid + 1;
        }
    }

    if ( ret > end ) ret = -1;

    return ret;
}

class Solution {

public:

#ifdef USING_STL
    struct helper{

        helper(std::vector<int>& v): vct(v){};

        bool operator()(int i, int k){

            return vct[i] < vct[k];
        }

        std::vector<int>& vct;
    };
#endif

    //brute force version
    int reversePairs(std::vector<int>& nums) {

        int size = nums.size();
        int *data = nums.data();

        int cnt = 0;
        for(int i = 0; i < size; i++){

            for(int k = i+1; k < size; k++){
            
                long t = data[k];
                t *= 2;
                if (data[i] > t){

                    cnt++;
#ifdef SHOW_DETAIL
                    std::cout << i << " : " <<  k << "------" << data[i] << " : " << data[k] << std::endl;
#endif
                }
            }
        }
        return cnt;
    }

    //optimization version
    int reversePairs2(std::vector<int>& nums) {

        int size = nums.size();
        int *data = nums.data();
        int *index = nullptr;

        if (0 == size) return 0;
#ifdef USING_STL
        std::vector<int> index_v;
#else
        int index_d[size];
#endif
        for( auto i = 0; i < size; i++ ){

#ifdef USING_STL
            index_v.push_back( i );
#else
            index_d[i] = i;
#endif
        }

#ifdef USING_STL
        helper hlp(nums);
        std::sort( index_v.begin(), index_v.end(), hlp );
        index = index_v.data();
#else
        g_src = data;
        qsort( index_d, size, sizeof(int), compare);
        index = index_d;
#endif

        int cnt = 0;
        for(int i = 0; i < size; i++){

            int c = index[i];
            long val = data[c];

            int res;
            if ( val > 0 ){

                res = find_first( index, i+1, size-1, val*2, data);

            }else if ( val < 0 ){

                res = find_first( index, 0, size-1, val*2, data);

            } else {
                int p = i;
                do{
                    ++p;
                    if ( p >= size ) break;

                    val = data[index[p]];
                }while ( val == 0 );

                res = (p < size)? p : -1;
            }

            if (res >= 0) {

                for(int k = res; k < size; k++){

                    if (c > index[k]){

                        cnt++;
#ifdef SHOW_DETAIL
                        std::cout << c << " : " <<  k << "------" << data[c] << " : " << data[index[k]] << std::endl;
#endif
                    }
                }
            }
        }

#ifdef SHOW_DETAIL
        std::string org, sorted, idx;
        for ( auto i = 0; i < size; ++i ){

            org += std::to_string(data[i]); org += " ; ";
            sorted += std::to_string(data[index[i]]); sorted += " ; ";
            idx += std::to_string(index[i]); idx += " ; ";
        }

        std::cout << org << std::endl;
        std::cout << sorted << std::endl;
        std::cout << idx << std::endl;
#endif
        return cnt;        
    }
};

#include "data.cpp"

void test_0(){

    std::vector<int> vct; 

    int cnt = 0;

    for ( auto t: test_data ){

        vct.push_back( t );
        if( ++cnt == 8 ) break;
    }

    Solution sol;

    int res = sol.reversePairs( vct );
    int res2 = sol.reversePairs2( vct );
    assert( res == res2 );
}

void test_1(){

    std::vector<int> vct = {-5, -5}; 

    Solution sol;

    int res = sol.reversePairs( vct );
    int res2 = sol.reversePairs2( vct );
    assert( res == res2 );
}

void test_2(){

    std::vector<int> vct = {8, -2, -5, 0, -5, 0, 0 }; 

    Solution sol;

    int res = sol.reversePairs( vct );
    int res2 = sol.reversePairs2( vct );
    assert( res == res2 );
}

void test_3(){

    std::vector<int> vct = {2147483647,2147483647,2147483647,2147483647,2147483647,2147483647}; 

    Solution sol;

    int res = sol.reversePairs( vct );
    int res2 = sol.reversePairs2( vct );
    assert( res == res2 );
}

void test_4(){

    std::vector<int> vct;

    int cnt = 0;

    for ( auto t: time_test ){

        vct.push_back( t );
    }

    Solution sol;

    int res = sol.reversePairs( vct );
    int res2 = sol.reversePairs2( vct );
    assert( res == res2 );
}

int main(){

    test_4();
    test_3();
    test_2();
    test_1();
    test_0();

    return 0;
}

