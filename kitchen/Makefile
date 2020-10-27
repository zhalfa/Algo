
boost:=/home/zhangl/boost_bd
cc:= g++
flag:= -g -c -std=c++11 -I$(boost)/include

boost_static:= -lboost_system -lboost_thread -lboost_log_setup -lboost_chrono -lpthread 
dll:=$(boost)/bin/cygboost_thread.dll $(boost)/bin/cygboost_chrono.dll


test : kitchen.o
	$(cc) -L$(boost)/lib $(boost_static) $(dll) kitchen.o -o test 

.cpp.o:
	$(cc) $(flag) -c $< -o $@

kitchen.o : kitchen.cpp  kitchen.h
	$(cc) $(flag) kitchen.cpp -o kitchen.o

clean:
	rm *.o *.exe
