#include <print>
#include <algorithm>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "hash.hpp"

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    std::string input {};
    std::unordered_map<std::string, std::unordered_set<std::string>> map;

    while(input_file.peek() != EOF) {
        std::getline(input_file, input);
        std::string first = input.substr(0, 2);
        std::string second = input.substr(3, 2);
        map[first].insert(second);
        map[second].insert(first);
    }
    using triple_type = std::tuple<std::string, std::string, std::string>;
    std::unordered_set<triple_type, hash_tuple::hash<triple_type>> triples;

    for(auto [elem, val]: map) {
        if(elem[0] != 't') continue;
        for(std::string item: val) {
            for(auto item2: map[item]) {
                if(val.contains(item2)) {
                    std::array<std::string, 3> triple {elem, item, item2};
                    std::ranges::sort(triple);
                    triples.emplace(triple[0], triple[1], triple[2]);
                }
            }
        }
    }

    std::println("{}", triples.size());
    return 0;
}

