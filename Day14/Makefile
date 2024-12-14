CC=g++-14
CXXFLAGS=--std=c++23 -Wall -Werror -ggdb -D_GLIBCXX_DEBUG

all: p1 p2
opt: p1_opt p2_opt

p1: p1.cpp
	$(CC) $(CXXFLAGS) p1.cpp -o p1

p2: p2.cpp
	$(CC) $(CXXFLAGS) -I./ p2.cpp -L./ -lraylib -o p2

p1_opt: p1.cpp
	$(CC) --std=c++23 -O3 p1.cpp -o p1_opt

p2_opt: p2.cpp
	$(CC) --std=c++23 -O3 p2.cpp -o p2_opt
