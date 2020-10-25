dll:=/home/zhangl/boost_bd/bin/cygboost_thread.dll 

test : kitchen.o
	g++ -L/home/zhangl/boost_bd/lib \
		-lboost_system -lboost_thread -lboost_log_setup -lpthread \
		 $(dll) kitchen.o -o test 

kitchen.o : kitchen.cpp  kitchen.h
	g++ -g -std=c++11 -c -I/home/zhangl/boost_bd/include kitchen.cpp -o kitchen.o

clean:
	rm *.o *.exe
