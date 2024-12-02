#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

int main() {
    std::ifstream input ("input.txt", std::ios::in);
    std::vector<int> first = {};
    std::unordered_map<int, int> hashmap;

    for (size_t i = 0;; ++i){
        int val;
        input >> val;
        if (input.eof()) break;
        if (i % 2) hashmap[val] += 1;
        else first.push_back(val);
    }


    int sum = 0;
    for(int val: first) sum += val * hashmap[val];
    std::cout << sum << '\n';

    return 0;
}
