CC = g++
CFLAGS = -std=c++11 -Wall -g 
targets = netutil epoll

.PHONY : all clean
all : $(targets)
netutil : Netutil_test.cpp ../Netutil.cpp
	$(CC) $^ -o $@ $(CFLAGS)
epoll : Epoll_test.cpp ../Epoll.cpp ../base/log/thread.cpp
	$(CC) $^ -o $@ $(CFLAGS) -lpthread

clean:
	-rm -rf $(targets)
	


