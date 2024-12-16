CC=g++-14
CXXFLAGS=--std=c++23 -Wall -Werror -ggdb -D_GLIBCXX_DEBUG

all: p1 p2
opt: p1_opt p2_opt

p1: p1.cpp grid_vect.hpp
	$(CC) $(CXXFLAGS) p1.cpp -o p1

p2: p2.cpp grid_vect.hpp
	$(CC) $(CXXFLAGS) p2.cpp -o p2

p1_opt: p1.cpp grid_vect.hpp
	$(CC) --std=c++23 -O3 p1.cpp -o p1_opt

p2_opt: p2.cpp grid_vect.hpp
	$(CC) --std=c++23 -O3 p2.cpp -o p2_opt
