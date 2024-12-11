CC=g++-14
CXXFLAGS=--std=c++23 -Wall -Werror -ggdb -D_GLIBCXX_DEBUG

all: p1
opt: p1_opt

p1: p1.cpp
	$(CC) $(CXXFLAGS) p1.cpp -o p1

p1_opt: p1.cpp
	$(CC) --std=c++23 -O3 p1.cpp -o p1_opt
