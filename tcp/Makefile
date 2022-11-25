CC=g++
FLAG=-o $@ $^ -std=c++11 
LIB=-lpthread
ALL=tcp_server tcp_client

.PHONY:all
all:$(ALL)
tcp_server:tcp_server.cc 
	$(CC) $(FLAG) $(LIB)
tcp_client:tcp_client.cc 
	$(CC) $(FLAG) $(LIB)
.PHONY:clean
clean:
	rm -f $(ALL)