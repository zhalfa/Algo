boost:= /home/zhangl/boost_bd

boost_static:= -lboost_system -lboost_thread -lboost_chrono \
               -lboost_program_options -lpthread -lws2_32

dll:=   $(boost)/bin/cygboost_thread.dll \
		$(boost)/bin/cygboost_chrono.dll \
		$(boost)/bin/cygboost_program_options.dll

extra:= -D__CYGWIN__ -D_WIN32_WINNT=0x0601 -D__USE_W32_SOCKETS

#/usr/include/w32api/winerror.h cannot find __MSABI_LONG
#found in /usr/include/w32api/_mingw_mac.h


all: boost_asio

boost_asio: boost_io_test.cpp
	g++ -std=c++11 $(extra) -I$(boost)/include -L$(boost)/lib $(boost_static) $(dll)\
		boost_io_test.cpp -o boost_asio
