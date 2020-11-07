#include <vector>

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

        size_t front_small = pSmall->front(); 
        size_t back_small = pSmall->size() - front_small;

        if ( (halfBig + front_small) >= (k+1) ){

            if (front_small){
                range new_a(pBig->begin, pBig->mid(), pBig->src);
                range new_b(pSmall->begin, pSmall->split, pSmall->src);

                findtheKth( new_a, new_b, k);
            }else{
                pBig->at(k);
            }

        }else{

            range new_a(pBig->mid()+1, pBig->end, pBig->src);

            if (back_small){
                range new_b(pSmall->split, pSmall->end, pSmall->src);

                findtheKth( new_a, new_b, k - halfBig - front_small);
            }else{
                if (new_a.size())new_a.at(k - halfBig - front_small);
            }
        }

    }
};
