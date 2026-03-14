CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pthread

# One executable per .cpp file (e.g. basic-1.cpp -> basic-1)
SRCS = $(wildcard *.cpp)
TARGETS = $(SRCS:.cpp=)

.PHONY: all clean run

all: $(TARGETS)

$(TARGETS): %: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Usage: make run PROG=basic-1
run: $(PROG)
	./$(PROG)

clean:
	rm -f $(TARGETS)
