
CC=g++
LINKER=g++

# flags for clang
# LFLAGS=-stdlib=libc++ -lc++
# flags for g++
LFLAGS=-L/usr/lib -lstdc++

# -lc++ is necessary to solve linker errors
server client: server.o client.o ../sock_ops.o ../inet_addr.o 
	$(LINKER) server.o ../sock_ops.o ../inet_addr.o $(LFLAGS) -o server
	$(LINKER) client.o ../sock_ops.o ../inet_addr.o $(LFLAGS) -o client

CFLAGS=-g -Wall -O0 -std=c++14
objects = server.o client.o ../sock_ops.o ../inet_addr.o
$(objects): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -v $(objects) server client
