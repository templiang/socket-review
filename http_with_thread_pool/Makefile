CC=g++
FLAG=-o $@ $^ -std=c++11 
LIB=-lpthread
SERVER=http_server
ALL=http_server

.PHONY:all
all:$(ALL)
$(SERVER):$(SERVER).cc 
	$(CC) $(FLAG) $(LIB)
# tcp_client:tcp_client.cc 
# 	$(CC) $(FLAG) $(LIB)
.PHONY:clean
clean:
	rm -f $(ALL)