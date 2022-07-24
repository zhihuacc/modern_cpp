
CC=clang
# CFLAGS=-g -O0 -std=c++14
# C++17 is needed for shared_mutex
CFLAGS=-g -O0 -std=c++17
# -lc++ is necessary to solve linker errors
LFLAGS=-stdlib=libc++ -lc++

EXE=unordered_hashmap_ts_test
OBJ=$(EXE:%=%.o)

$(EXE): %: %.o
	$(CC) $(LFLAGS)  $< -o $@

$(OBJ): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -v $(OBJ) $(EXE)
