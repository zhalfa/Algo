#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string>
#include <stdlib.h>

//#define USING_STL
//#define SHOW_DETAIL

#ifndef USING_STL
int* g_src = nullptr;

int compare (const void * a, const void * b){

    return ( g_src[*(int*)a] - g_src[*(int*)b] );
}
#endif

int find_first(const int* src, int begin, int end, int val, const int* act){

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

                if (data[i] > data[k]*2){

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
        find_first( index, 1, size-1, data[index[0]]*2, data); 

        int cnt = 0;
        for(int i = 0; i < size; i++){

            int c = index[i];
            int val = data[c];
            
            int res = find_first( index, i+1, size-1, val*2, data);

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

int test_data[] = {183, 86, 377, 115, 193, 335, 186, 92, 249, 221, 362, 27, 290, 59, 163, 326, 140, 226, 372, 136, 11, 168, 167, 29, 182, 330, 62, 323, 67, 335, 329, 202, 22, 258, 269, 167, 193, 56, 211, 42, 229, 173, 21, 119, 184, 137, 398, 324, 315, 370, 13, 326, 91, 180, 356, 273, 62, 370, 196, 81, 305, 125, 284, 327, 336, 105, 46, 129, 113, 257, 124, 295, 382, 145, 14, 167, 234, 364, 43, 150, 287, 8, 76, 378, 188, 384, 203, 251, 354, 399, 332, 260, 76, 168, 139, 12, 226, 186, 94, 339, 395, 170, 234, 378, 267, 201, 97, 102, 117, 92, 252, 356, 101, 280, 286, 241, 265, 89, 44, 219, 40, 329, 31, 117, 97, 171, 81, 275, 309, 127, 167, 256, 297, 353, 186, 165, 106, 283, 219, 224, 328, 71, 132, 29, 303, 19, 270, 168, 108, 315, 340, 149, 196, 323, 218, 245, 46, 251, 121, 355, 379, 288, 164, 228, 241, 350, 393, 300, 234, 164, 124, 114, 187, 256, 143, 91, 227, 365, 259, 336, 232, 151, 37, 28, 75, 207, 274, 121, 58, 395, 29, 37, 235, 193, 218, 28, 143, 211, 328, 329, 376, 4, 43, 163, 213, 138, 206, 40, 104, 18, 328, 288, 169, 317, 317, 196, 124, 143, 270, 183, 90, 299, 172, 325, 44, 390, 305, 139, 154, 186, 69, 82, 142, 64, 197, 307, 155, 4, 348, 211, 22, 228, 99, 143, 146, 368, 340, 222, 111, 210, 5, 201, 61, 130, 78, 105, 120, 336, 244, 226, 122, 265, 308, 216, 282, 58, 124, 37, 62, 24, 200, 36, 252, 299, 179, 350, 268, 71, 173, 331, 281, 130, 133, 294, 260, 163, 399, 381, 99, 196, 159, 173, 13, 68, 390, 295, 126, 66, 284, 140, 90, 84, 176, 342, 336, 307, 245, 156, 379, 18, 87, 212, 148, 172, 59, 9, 336, 10, 342, 387, 206, 101, 113, 172, 121, 55, 19, 199, 121, 304, 339, 211, 340, 67, 105, 228, 327, 350, 384, 258, 320, 24, 22, 69, 196, 81, 30, 84, 92, 372, 72, 250, 25, 185, 22, 99, 240, 42, 298, 313, 298, 190, 124, 190, 209, 181, 19, 136, 132, 355, 394, 4, 379, 369, 73, 176, 50, 55, 260, 142, 379, 284, 393, 5, 21, 367, 104, 213, 361, 354, 126, 259, 144, 202, 2, 306, 384, 21, 42, 68, 328, 389, 72, 308, 358, 98, 36, 8, 153, 248, 103, 133, 133, 48, 90, 154, 367, 146, 368, 329, 100, 46, 188, 197, 249, 190, 103, 233, 163, 97, 253, 92, 86, 325, 352, 396, 375, 388, 357, 129, 236, 60, 214, 321, 60, 304, 28, 27, 50, 348, 356, 102, 394, 97, 299, 243, 239, 2, 28, 3, 100, 281, 47, 138, 159, 351, 135, 134, 339, 92, 215, 127, 104, 29, 49, 164, 285, 29, 143, 335, 377, 100, 38, 371, 149, 289, 167, 388, 292, 195, 343, 344, 29, 390, 82, 140, 341, 169, 226, 232};

int main(){

    std::vector<int> vct; 

    int cnt = 0;

    for ( auto t: test_data ){

        vct.push_back( t );
        if( ++cnt == 8 ) break;
    }

    Solution sol;

    int res = sol.reversePairs( vct );
    std::cout << "res = " << res << std::endl;

    res = sol.reversePairs2( vct );
    std::cout << "res = " << res << std::endl;

    return 0;
}


