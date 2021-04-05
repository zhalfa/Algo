class data {};

class data_pipe{


public:

    data_pipe(unsigned long size){
    
        m_data_capacity =     size;
    
        m_head = 0;
        m_tail = 0;
        
        m_is_full = false;
    };

    bool put_data( data* p ){
        
        lock_guard guard(m_data_lock);
        
        //check is full
        if ( m_actual_cnt == m_data_capacity) return false;

        if ( m_ring_buffer.size() < m_data_capacity){

            m_ring_buffer.push_back(p);

            m_actual_cnt ++;

        }else{
            
            if (m_head != m_tail){
            
                m_ring_buffer[m_tail] = p;
                m_tail = (m_tail+1)% m_data_capacity;
                m_data

                if (m_tail == m_head) m_is_full = true;

            }

        }

    };

    bool get_data( data* p);

private:

    unsigned long m_data_capacity;
    unsigned long m_actual_cnt;

    mutex m_data_lock;

    unsigned long m_head;
    unsigned long m_tail;
    
    bool m_is_full;
    

    std::vector <data*> m_ring_buffer;
}

;
