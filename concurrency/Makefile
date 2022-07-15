
cc=cc

# -lc++ is necessary to solve linker errors
main: main.o
	$(cc) main.o -stdlib=libc++ -lc++ -o main

objects = main.o
$(objects): %.o: %.cpp
	$(cc) -c -std=c++14 $< -o $@

clean:
	rm -v $(objects) main