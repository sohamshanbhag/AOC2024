#include <algorithm>
#include <ranges>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

bool is_valid(const std::vector<int>& list, bool second_iter=0) {
    bool ascending = list.at(0) < *(list.end()-1);
    auto values = std::views::adjacent_transform<2>(list, [ascending](int a, int b) {
        if(ascending) {
            return b - a <= 3 && b - a >= 1 ;
        } else {
            return a - b <= 3 && a - b >= 1 ;
        }
        });

    auto num_invalid = std::ranges::find(values, false) - values.begin();

    if(num_invalid == values.end() - values.begin()) return true;
    if(second_iter) return false;


    std::vector list1 = list;
    list1.erase(list1.begin()+num_invalid+1);
    std::vector list2 = list;
    list2.erase(list2.begin()+num_invalid);
    return is_valid(list1, true) || is_valid(list2, true);
}

int main() {
    std::ifstream input {"input.txt"};
    // std::ifstream input {"example.txt"};
    std::string line;
    int num_valid = 0;

    while(true){
        std::getline(input, line, '\n');
        if(input.eof()) break;
        std::stringstream iss(line);
        std::string val;
        std::vector<int> values (( std::istream_iterator<int>( iss ) ), ( std::istream_iterator<int>() ));
        auto value = is_valid(values);

        num_valid += value;
    }
    std::cout << num_valid << '\n';


    return 0;
}
