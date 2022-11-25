CC=g++
FLAG=-o $@ $^
ALL=server client

.PHONY:all
all:$(ALL)
client:client.cc
	$(CC) $(FLAG)
server:server.cc
	$(CC) $(FLAG)
.PHONY:clean
clean:
	rm -f $(ALL)
