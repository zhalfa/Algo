#include <string>
#include <vector>
#include <boost/move/move.hpp>

/*
    constexpr how to use it

https://stackoverflow.com/questions/14116003/difference-between-constexpr-and-const#:~:text=constexpr%20is%20a%20new%20C%2B%2B11%20keyword%20that%20rids,under%20certain%20conditions%2C%20that%20objects%20undergo%20static%20initialization.
*/

class TCPSocket{

public:
    TCPSocket(const std::string& host, const std::string& port){};
    ~TCPSocket(){};
    bool isOpen();

    std::vector<char> read(size_t size){

        return std::vector<char>();
    };
    size_t write(std::vector<char> payload){

        return 0;
    };

#if 0    
    //a workaround

    TCPSocket(TCPSocket& that){

        m_socket_fd = that.m_socket_fd;
        that.m_socket_fd = -1;
    }
#endif

    TCPSocket(TCPSocket&& rvref) noexcept : m_socket_fd(-1) {
    //p503 in book
        std::swap(m_socket_fd, rvref.m_socket_fd);
    }

    TCPSocket(const TCPSocket&) = delete;
    TCPSocket& operator = (const TCPSocket&) = delete;

    void setFd(int val){ m_socket_fd = val; }

private:

    int m_socket_fd;
#if 0
    not copyable usage in c++03
    TCPSocket(const TCPSocket&);
    TCPSocket& operator = (const TCPSocket&);
#endif
};

TCPSocket connectToService(){

    TCPSocket socket("", "");

    return socket;
}

void performIO(TCPSocket socket){

    socket.read(1);
    socket.write(std::vector<char>());
}

//p500 in book

int main(){

    performIO(connectToService());

    TCPSocket socket = connectToService();
    
    socket.setFd(1000);
    //such expressions are called xvalues, the x stands for expired
    performIO(boost::move(socket));

    socket;
    return 0;
}

