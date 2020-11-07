#include <vector>
#include <cassert>

using std::vector;

class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {

        range a(0, nums1.size()-1, nums1);
        range b(0, nums2.size()-1, nums2);

        int mid =  nums1.size() + nums2.size();
        if (mid%2 == 1){
            mid = mid/2;

            findtheKth(a, b, mid);

        }else{
            mid = mid/2;
            findtheKth(a, b, mid);
            mid --;
            findtheKth(a, b, mid);

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

        size_t front(){

            if ((split >= begin) &&(split <= end)) {

                return split - begin + 1;
            }
            return 0;
        }

        bool splitByVal(int val){

            for( auto i = begin; i<= end; i++){
                
                if ((src[i] > val) &&(i < end)){

                    split = i - 1;

                }else if (i == end) {
                    
                    split = end;
                }
            }

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
            return 0; //be carefull
        }

        int split;
        int begin;
        int end;
        vector<int>& src;

    };

    void findtheKth( range a, range b, size_t k ){

        if (a.size()== 0 || b.size()== 0) {

            if (a.size() ) a.at(k);
            if (b.size() ) b.at(k);
            return ;
        }
    
        range *pBig, *pSmall;
    
        if(a.size() > b.size()){

            pBig = &a;
            pSmall = &b;

        }else{

            pBig = &b;
            pSmall = &a;
        }

        int midVal = pBig->midVal();

        size_t sizeBig = pBig->size();
        size_t halfBig = sizeBig/2;

        pSmall->splitByVal( midVal);

        // if pos is invalid, logic is wrong    

        size_t small_front = pSmall->front(); 
        size_t small_back = pSmall->size() - small_front;

        if ( (halfBig + small_front) >= (k+1) ){

            if (small_front){
                range new_a(pBig->begin, pBig->mid(), pBig->src);
                range new_b(pSmall->begin, pSmall->split, pSmall->src);

                findtheKth( new_a, new_b, k);
            }else{
                pBig->at(k);
            }

        }else{

            range new_a(pBig->mid()+1, pBig->end, pBig->src);

            if (small_back){
                range new_b(pSmall->split, pSmall->end, pSmall->src);

                findtheKth( new_a, new_b, k - halfBig - small_front);
            }else{
                if (new_a.size())new_a.at(k - halfBig - small_front);
            }
        }

    }
};

#include <cstdlib>
#include <algorithm>
#include <iostream>

void fillVector(std::vector<int>& vct, size_t vctSize){

    using std::rand;

    int size = std::rand()% vctSize;
    if (size == 0) size =2;

    for ( auto i=0; i< size; i++){
        vct.push_back( std::rand()%20);
    }

    std::sort(vct.begin(), vct.end(), std::less<int>());
    for (auto a : vct) {
        std::cout << " "<< a << " , ";
    }
    std::cout << std::endl;
}

int main(){

    std::vector<int> A;
    std::vector<int> B;

    fillVector(A, 5);
    fillVector(B, 5);

    Solution sol;
    sol.findMedianSortedArrays(A, B);

}
