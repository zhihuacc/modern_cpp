
CC=clang
CFLAGS=-g -O0 -std=c++14
# -lc++ is necessary to solve linker errors
LFLAGS=-stdlib=libc++ -lc++

EXE=list_ts_test
OBJ=$(EXE:%=%.o)

$(EXE): %: %.o
	$(CC) $(LFLAGS)  $< -o $@

$(OBJ): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -v $(OBJ) $(EXE)
