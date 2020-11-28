boost:= /home/zhangl/boost_bd

boost_static:= -lboost_system -lboost_thread -lboost_chrono \
               -lboost_program_options -lpthread -lws2_32

dll:=   $(boost)/bin/cygboost_thread.dll \
		$(boost)/bin/cygboost_chrono.dll \
		$(boost)/bin/cygboost_program_options.dll

extra:= -D_WIN32_WINNT=0x0602 -D__USE_W32_SOCKETS


# __MSABI_LONG found in /usr/include/w32api/_mingw_mac.h

# On 64-bit architectures, long int, according to gcc is at least an int64_t.
# On 32-bit, long int is at least int32_t. 
# With Microsoft compilers, long is always an int32_t, regardless of 32/64-bit. 

# use standard types such as int32_t, uint32_t, int64_t, uint64_t, etc from <stdint.h>
# rather than trying to make assumptions about naked types such as long int

# The 64-bit model for any given platform (e.g. LP64 for most *nix platforms, Mac OS X, etc) 
# is a given, so even if you could convince the compiler to use a different 64-bit model 
# you would probably break any calls to system code, libraries, etc

# this is why compiling /usr/include/w32api/ws2tcpip.h without __LP64__ fails
#

extra+=  

include:= -I$(boost)/include -I/usr/include/w32api


flag:= -std=c++11 

all: boost_asio

boost_asio: boost_io_test.cpp
	g++ $(flag) $(extra) $(include) -L$(boost)/lib $(boost_static) $(dll)\
		boost_io_test.cpp -o boost_asio
