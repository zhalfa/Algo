boost:= /usr/inclued

link:= -lpthread -lboost_coroutine
flag:= -std=c++11 -g

des:= asio_boost rvalue_boost

all: $(des)
    
clean:
	rm $(des) 

asio_boost: boost_asio_try.cpp
	g++ $(flag) $< -o $@  $(link)

rvalue_boost: rvalue_reference.cpp 
	g++ $(flag) $< -o $@  $(link)
