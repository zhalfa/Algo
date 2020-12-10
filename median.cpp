#include <vector>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>

#define TRY_FAST
#define RECURSIVE_CALL

using std::vector;

class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {

        range a(0, nums1.size()-1, nums1.data(), 1);
        range b(0, nums2.size()-1, nums2.data(), 2);

        aux detail;

        int mid =  nums1.size() + nums2.size();
        if (mid%2 == 1){
            mid = mid/2;

            int val = findtheKth(a, b, mid, detail);

            std::cout << " " << val << std::endl;
            return val;

        }else{
            mid = mid/2;
            int val1 = findtheKth(a, b, mid, detail);
#ifdef TRY_FAST
            int val2 = findPre(a, b, mid, detail);
#else
            mid --;
            int val2 = findtheKth(a, b, mid, detail);
#endif
            std::cout << " " << val1 << std::endl;
            std::cout << " " << val2 << std::endl;

            return (val1+val2)/2.0f;
        }

        return 0;        
    }
    
    struct aux{

        int origin;
        int pos;
    };

    struct range {

        range(int b, int e, int* s, int org ): begin(b), end(e), src(s), origin(org){

            split = -1;
        }

        range& operator=(const range& rhs){

            begin = rhs.begin;
            end = rhs.end;
            src = rhs.src;
            split = rhs.split;
            origin = rhs.origin;

            return *this;
        }

        size_t size(){
            if (end < begin ) return 0;
            return end - begin + 1;
        }
        size_t mid(){return (begin+end)/2;}
        int midVal(){return src[mid()];}

        size_t firstHalfSize(){return mid()- begin + 1;}

        size_t front(){

            if ((split >= begin) &&(split <= end)) {

                return split - begin + 1;
            }
            return 0;
        }

        bool splitByVal(int val){

#ifndef TRY_FAST
            //a little slower than the other
            int i;
            for( i = begin; i<= end; i++){
                
                if (src[i] > val ){

                    split = i - 1;
                    break;
                }
                    
            }
            if (i > end) split = end;
#else
            int* vec = src;
            int s = begin;
            int e = end;
            while(s <= e){

                int mid = (s+e)/2;

                if ( val < vec[mid] ){

                    e = mid - 1;

                }else if (val == vec[mid]){
                    split = mid;
                    break;

                }else{

                    s = mid + 1;
                }

                if (s > e){

                    if (val < vec[mid] ){
                        split = mid - 1;
                    }else{
                        split = mid;
                    }
                }
            }

#endif
            if ((split >= begin) &&(split <= end)) {
                return true;
            }
            return false;
        }

        int at( int k) {

            if ( (k >= 0) && (k<size()) ){

                return src[begin+k];
            }
            assert(false);
            return -1; //be carefull
        }

#ifdef SHOW_DETAIL 
        void show(){
            std::cout << "info: " << begin << " : " << end << " : " << split << std::endl;
            std::cout << "content: ";
            int i = begin;
            while(i <= end){

                std::cout << src[i++] << " ; ";
            }
            std::cout << std::endl;
        };
#endif

        int split;
        int begin;
        int end;
        int* src;
        int origin;

    };

    int findPre( range a, range b, size_t k, const aux& detail ){

        assert((detail.origin > 0) && (detail.origin < 3));

        range *cur, *another;
        if(detail.origin == 1){

            cur = &a;
            another = &b;

        }else{

            cur = &b;
            another = &a;
        }

        if( detail.pos == k ){

            assert(detail.pos > cur->begin);
            return cur->at(k - 1);
        } 

        if ( detail.pos == cur->begin ){
            
            return another->at(k - 1);
        }

        assert(detail.pos > cur->begin);
        int c1 = cur->at(detail.pos - 1);

        int used = detail.pos - cur->begin + 1;
        int c2 = another->at(k -used);

        return std::max(c1, c2);
    }

    int retValue( range& src, int idx, aux& detail){

        detail.origin = src.origin;
        detail.pos = src.begin + idx;
        return src.at(idx);
    }

#ifdef RECURSIVE_CALL
    int findtheKth( range a, range b, size_t k, aux& detail ){
        int ret = -1;
        
        if (a.size()== 0 || b.size()== 0) {

            if (a.size() ) ret = retValue(a, k, detail);
            if (b.size() ) ret = retValue(b, k, detail);
            return ret;
        }

        if ((a.size()==1) &&(b.size()==1)){

            if ((k>=0)&& (k<2)){

                if (k==0){

                    if(a.at(0)<= b.at(0)){

                        ret = retValue(a, 0, detail);

                    }else{

                        ret = retValue(b, 0, detail);
                    }

                }
                if (k==1){

                    if(a.at(0)>= b.at(0)){

                        ret = retValue(a, 0, detail);

                    }else{

                        ret = retValue(b, 0, detail);
                    }
                }
                return ret;
            }
        }
    
        range *pBig, *pSmall;
        if(a.size() >= b.size()){

            pBig = &a;
            pSmall = &b;

        }else{

            pBig = &b;
            pSmall = &a;
        }

        int midVal = pBig->midVal();

        size_t sizeBig = pBig->size();
        size_t halfBig = pBig->firstHalfSize();

        pSmall->splitByVal( midVal);

        size_t small_front = pSmall->front(); 
        size_t small_back = pSmall->size() - small_front;

        if ( (halfBig + small_front) >= (k+1) ){

            if (small_front){
                range new_a(pBig->begin, pBig->mid(), pBig->src, pBig->origin);
                range new_b(pSmall->begin, pSmall->split, pSmall->src, pSmall->origin);

                ret = findtheKth( new_a, new_b, k, detail);
            }else{
                ret = retValue(*pBig, k, detail);
            }

        }else{

            range new_a(pBig->mid()+1, pBig->end, pBig->src, pBig->origin);

            if (small_back){
                range new_b((pSmall->split < pSmall->begin)? pSmall->begin : pSmall->split + 1, pSmall->end, pSmall->src, pSmall->origin);

                ret = findtheKth( new_a, new_b, k - halfBig - small_front, detail);
            }else{
                if (new_a.size()) ret = retValue( new_a, k - halfBig - small_front, detail);
            }
        }
        return ret;
    }
#else
    int findtheKth( range a, range b, size_t k, aux& detail ){
        int ret = -1;

        for(;;){        

        #ifdef SHOW_DETAIL
            a.show();
            b.show();
            std::cout << "K: " << k << std::endl;
        #endif

            if (a.size()== 0 || b.size()== 0) {

                if (a.size() ) ret = retValue(a, k, detail);
                if (b.size() ) ret = retValue(b, k, detail);
                break;
            }

            if ((a.size()==1) &&(b.size()==1)){

                if ((k>=0)&& (k<2)){

                    if (k==0){

                        if(a.at(0)<= b.at(0)){

                            ret = retValue(a, 0, detail);

                        }else{

                            ret = retValue(b, 0, detail);
                        }

                    }
                    if (k==1){

                        if(a.at(0)>= b.at(0)){

                            ret = retValue(a, 0, detail);

                        }else{

                            ret = retValue(b, 0, detail);
                        }
                    }
                }
                break;
            }

            range *pBig, *pSmall;
            if(a.size() >= b.size()){

                pBig = &a;
                pSmall = &b;

            }else{

                pBig = &b;
                pSmall = &a;
            }

            int midVal = pBig->midVal();

            size_t sizeBig = pBig->size();
            size_t halfBig = pBig->firstHalfSize();

            pSmall->splitByVal( midVal);

            size_t small_front = pSmall->front(); 
            size_t small_back = pSmall->size() - small_front;

            if ( (halfBig + small_front) >= (k+1) ){

                if (small_front){
                    range new_a(pBig->begin, pBig->mid(), pBig->src, pBig->origin);
                    range new_b(pSmall->begin, pSmall->split, pSmall->src, pSmall->origin);

                    a = new_a;
                    b = new_b;
                    //ret = findtheKth( new_a, new_b, k);
                }else{

                    ret = retValue(*pBig, k, detail);
                    break;
                }

            }else{

                range new_a(pBig->mid()+1, pBig->end, pBig->src, pBig->origin);

                if (small_back){
                    range new_b((pSmall->split < pSmall->begin)? pSmall->begin : pSmall->split + 1, pSmall->end, pSmall->src, pSmall->origin);

                    a = new_a;
                    b = new_b;
                    k -=(halfBig + small_front);
                    //ret = findtheKth( new_a, new_b, k - halfBig - small_front);
                }else{
                    int tmp = k - halfBig - small_front;

                    if (new_a.size()){

                        ret = retValue(new_a, tmp, detail);
                    }
                    break;
                }
            }
        }
        return ret;
    }
#endif
};


void fillVector(std::vector<int>& vct, size_t vctSize){

    using std::rand;

    for ( auto i=0; i< vctSize; i++){
        vct.push_back( std::rand()%30000);
    }

    std::sort(vct.begin(), vct.end(), std::less<int>());
    for (auto a : vct) {
    }
}

void test_0(){

    std::vector<int> A = {2, 3, 9};
    std::vector<int> B = {8, 12};

    Solution sol;
    double res = sol.findMedianSortedArrays(A, B);
    assert(8 == res);

    std::cout << __FUNCTION__ << std::endl;
}

void test_1(){

    std::vector<int> A = {2, 15, 19};
    std::vector<int> B = {8, 12};

    Solution sol;
    double res = sol.findMedianSortedArrays(A, B);
    assert(12 == res);

    std::cout << __FUNCTION__ << std::endl;
}

void test_2(){

    std::vector<int> A = {2, 15, 19};
    std::vector<int> B = {8, 17};

    Solution sol;
    double res = sol.findMedianSortedArrays(A, B);
    assert(15 == res);

    std::cout << __FUNCTION__ << std::endl;
}

void test_3(){

    std::vector<int> A = {2, 15, 19};
    std::vector<int> B = {8, 17, 21};

    Solution sol;
    double res = sol.findMedianSortedArrays(A, B);
    assert((15+17)/2.0f == res);

    std::cout << __FUNCTION__ << std::endl;
}

void test_4(){

    std::vector<int> A;
    std::vector<int> B;

    fillVector(A, 9);
    fillVector(B, 5);

    Solution sol;
    double res = sol.findMedianSortedArrays(A, B);

    std::cout << __FUNCTION__ << std::endl;
}

void test_5(){

    std::vector<int> A;
    std::vector<int> B;

    fillVector(A, 1000);
    fillVector(B, 1001);

    Solution sol;
    double res = sol.findMedianSortedArrays(A, B);

    std::vector<int> C;
    std::merge(A.begin(), A.end(), B.begin(), B.end(), std::back_inserter(C));

    int c = C[C.size()/2];
    
    assert(c == res);
    std::cout << __FUNCTION__ << std::endl;
}

void test_6(){
#if 1
    std::vector<int> A = {9};
    std::vector<int> B = {8}; 
    //change to 8, it is ok why ???
    //fix, now change to 8, the same with 9
#else
    std::vector<int> A = { 0 ,  3 ,  6 ,  8 ,  9 ,  13 ,  14 ,  14 ,  20 };
    std::vector<int> B = { 0 ,  8 ,  9 ,  20 ,  28 };
#endif
    Solution sol;
    double res = sol.findMedianSortedArrays(A, B);
    assert((9+8)/2.0f == res);

    std::cout << __FUNCTION__ << std::endl;
}

void test_7(){

    std::vector<int> A = {1,3,4};
    std::vector<int> B = {2,5,6}; 

    Solution sol;
    double res = sol.findMedianSortedArrays(A, B);

    assert((4+3)/2.0f == res);

    std::cout << __FUNCTION__ << std::endl;
}

int main(){

    test_7();
    test_6(); 
    test_5(); 
    test_4(); 
    test_3();
    test_2();
    test_1();
    test_0();
    return 0;
}
