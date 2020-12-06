#include <string>
#include <vector>

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
    
    TCPSocket(TCPSocket& that){

        m_socket_fd = that.m_socket_fd;
        that.m_socket_fd = -1;
    }

private:

    int m_socket_fd;

    //TCPSocket(const TCPSocket&);
    TCPSocket& operator = (const TCPSocket&);
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
    performIO(socket);    

    return 0;
}

