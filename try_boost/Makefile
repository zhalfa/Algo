boost:= /usr/inclued

link:= -lpthread -lboost_coroutine
flag:= -std=c++11 -g

all: boost_asio
    
clean:
	rm boost_asio    

boost_asio: boost_asio_try.cpp
	g++ $(flag) $< -o $@  $(link)

