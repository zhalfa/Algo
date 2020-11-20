#include <vector>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>

using std::vector;

class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {

        range a(0, nums1.size()-1, nums1);
        range b(0, nums2.size()-1, nums2);

        int mid =  nums1.size() + nums2.size();
        if (mid%2 == 1){
            mid = mid/2;

            int val = findtheKth(a, b, mid);

            std::cout << " " << val << std::endl;
            return val;

        }else{
            mid = mid/2;
            int val1 = findtheKth(a, b, mid);
            mid --;
            int val2 = findtheKth(a, b, mid);

            std::cout << " " << val1 << std::endl;
            std::cout << " " << val2 << std::endl;

            return (val1+val2)/2.0f;
        }

        return 0;        
    }
    
    struct range {

        range(int b, int e, vector<int>& s ): begin(b), end(e), src(s){

            split = -1;
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

#if 0
            int i;
            for( i = begin; i<= end; i++){
                
                if (src[i] > val ){

                    split = i - 1;
                    break;
                }
                    
            }
            if (i > end) split = end;
#else
            int* vec = src.data();
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

        int split;
        int begin;
        int end;
        vector<int>& src;

    };

    int findtheKth( range a, range b, size_t k ){
        int ret = -1;
        
        if (a.size()== 0 || b.size()== 0) {

            if (a.size() ) ret = a.at(k);
            if (b.size() ) ret = b.at(k);
            return ret;
        }

        if ((a.size()==1) &&(b.size()==1)){

            if ((k>=0)&& (k<2)){

                if (k==0) return std::min(a.at(0), b.at(0));
                if (k==1) return std::max(a.at(0), b.at(0));
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
                range new_a(pBig->begin, pBig->mid(), pBig->src);
                range new_b(pSmall->begin, pSmall->split, pSmall->src);

                ret = findtheKth( new_a, new_b, k);
            }else{
                ret = pBig->at(k);
            }

        }else{

            range new_a(pBig->mid()+1, pBig->end, pBig->src);

            if (small_back){
                range new_b((pSmall->split < pSmall->begin)? pSmall->begin : pSmall->split + 1, pSmall->end, pSmall->src);

                ret = findtheKth( new_a, new_b, k - halfBig - small_front);
            }else{
                if (new_a.size()) ret = new_a.at(k - halfBig - small_front);
            }
        }
        return ret;
    }
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
