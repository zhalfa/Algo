
boost:=/home/zhangl/boost_bd
cc:= g++
flag:= -O3 -c -std=c++11 -I$(boost)/include

boost_static:= -lboost_system -lboost_thread -lboost_chrono -lboost_program_options -lpthread 
dll:=   $(boost)/bin/cygboost_thread.dll \
		$(boost)/bin/cygboost_chrono.dll \
		$(boost)/bin/cygboost_program_options.dll


cli: test main.o
	$(cc) -L$(boost)/lib $(boost_static) $(dll) kitchen.o main.o -o cli 

test : kitchen.o unit_test.o
	$(cc) -L$(boost)/lib $(boost_static) $(dll) kitchen.o unit_test.o -o unit_test 

.cpp.o:
	$(cc) $(flag) -c $< -o $@

kitchen.o : kitchen.cpp  kitchen.h
	$(cc) $(flag) kitchen.cpp -o kitchen.o

unit_test.o : unit_test.cpp  kitchen.h  unit_test.h
	$(cc) $(flag) unit_test.cpp -o unit_test.o

main.o : main.cpp  kitchen.h  unit_test.h
	$(cc) $(flag) main.cpp -o main.o

clean:
	rm *.o *.exe
