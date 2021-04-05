class resolution{

typedef unsigned long long DATA_TYPE;

public: DATA_TYPE total_3x_5x( DATA_TYPE upper ){

        if (upper < 3 ) return 0;

        if ( (upper < 5) &&(upper>=3) ) return 3;

        DATA_TYPE total_3x = total_nx( 3, upper);
        DATA_TYPE total_5x = total_nx(5, upper);

        DATA_TYPE total_15x = total_nx(15, upper);

        return total_3x + total_5x - total_15x;

    }

private:
      DATA_TYPE totoal_nx( DATA_TYPE atom, DATA_TYPE upper){

            if (atom > upper) return 0;

            DATA_TYPE cnt = upper/atom;

            total = (1 + cnt)* cnt ;

            total >>= 1;

            total *= atom;

            return total;

        } 


}


;


class find_max_total{

public:
        int get_max( tree* p ){

            if ( p == NULL ) return 0;

            val = p-> vaule;

            int sub_max = std::max ( get_max( p->left), get_max(p->right) );

            return val + sub_max;
            
        }

}

;







class cache{

public:

    struct cache_item{

        UUID key;
        Time time_last_accessed;

        list_item * actual_postion;
        
        void* m_actual_data;
    };

    cache(unsigned long capacity){

        m_capacity = capacity;
    }

    bool put( UUID key, void* p){

        //check key exist already

        if (table.find( key ) == table.end() ){

            cache_item * pItem = new cache_item();

            table.add( key, pItem);

            pItem->key = key;
            pItem->time_last_accessed = current_system_system();

            auto iterator = m_LRU.addtail(pItem);

            pItem->actual_postion = iterator;

            //priority_queue


        }

    };


    bool update( UUID key, void* p );
    bool get( UUID key, void** p){

        if ( table.find( key ) != table.end() ){

            catchem_item *pTemp = get_from_hashtale ();

            if (pTemp ){
                auto iterator = pTemp->actual_postion;

                m_LRU.remove(iterator);

                m_LRU.add_tail(iterator);        
    
                if (p){
                    *p = pTemp->m_actual_data;
                    return true;
                }
            }

        }
        return false;

    }


prviate:

    std::hastable< UUID,  cache_itme*> table;

    unsigned long m_capacity;

    std::list<cachem_item*> m_LRU;



};




