#include <functional>
#include <unistd.h>
#include <print>
#include <algorithm>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

// #include "hash.hpp"
//

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

    std::vector<std::vector<std::string>> groups;

    for(auto [elem, val]: map) {
        for(std::string item: val) {
            for(auto item2: map[item]) {
                if(val.contains(item2)) {
                    std::vector triple {elem, item, item2};
                    std::ranges::sort(triple);
                    groups.push_back(triple);
                }
            }
        }
    }
    std::ranges::sort(groups);
    auto [last, _] = std::ranges::unique(groups);
    groups.erase(last, groups.end());

    std::unordered_set<std::string> keys;
    for(auto elem: groups) for(auto item: elem) keys.insert(item);


    size_t size = 3;
    while(true) {
        std::vector<std::vector<std::string>> temp_groups;

        for(std::string elem: keys) {
            for(std::vector<std::string> group: groups) {
                bool add = true;
                for(std::string item: group) {
                    if(!map[elem].contains(item)) add = false;
                }
                if(add) {
                    group.push_back(elem);
                    std::ranges::sort(group);
                    temp_groups.push_back(group);
                }
            }
        }
        std::swap(groups, temp_groups);

        std::ranges::sort(groups);
        auto [last, _] = std::ranges::unique(groups);
        groups.erase(last, groups.end());

        if(groups.size() == 1) break;

        keys = {};
        for(auto elem: groups) for(auto item: elem) keys.insert(item);

        ++size;
        std::println("{} {}", groups.size(), size);
        usleep(1'000'000);
    }

    for(auto elem: groups[0]) std::print("{},", elem);
    std::println();
    return 0;
}

