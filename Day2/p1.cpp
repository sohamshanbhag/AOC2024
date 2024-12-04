#include <algorithm>
#include <ranges>
#include <cstdio>
#include <iterator>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

bool is_valid(const std::vector<int>& list) {
    if(list.size() == 1) return true;
    bool ascending = list.at(1) < list.at(2);

    auto values = std::views::adjacent_transform<2>(list, [ascending](int a, int b) {
        if(ascending) {
            return b - a <= 3 && b - a >= 1 ;
        } else {
            return a - b <= 3 && a - b >= 1 ;
        }
        });
    return std::ranges::all_of(values, [](bool a) {return a;});
}

int main() {
    // std::ifstream input {"example.txt"};
    std::ifstream input {"input.txt"};
    std::string line;
    int num_valid = 0;

    while(true){
        std::getline(input, line, '\n');
        if(input.eof()) break;
        std::stringstream iss(line);
        std::string val;
        std::vector<int> values (( std::istream_iterator<int>( iss ) ), ( std::istream_iterator<int>() ));
        num_valid += is_valid(values);
    }
    std::cout << num_valid << '\n';


    return 0;
}
