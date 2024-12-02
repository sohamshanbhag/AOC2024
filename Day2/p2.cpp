#include <ranges>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

bool is_valid(const std::vector<int>& list, bool second_iter = 0) {
    bool ascending = list.at(0) < *(list.end()-1);
    for(const auto& [idx, value]: list | std::views::adjacent<2> | std::views::enumerate ){

        const auto& [prev, present] = value;
        if(ascending) {
            if(present - prev > 3 || present - prev < 1) {
                if(second_iter) return false;
                else {
                    std::vector list1 = list;
                    list1.erase(list1.begin()+idx+1);
                    std::vector list2 = list;
                    list2.erase(list2.begin()+idx);
                    return is_valid(list1, true) || is_valid(list2, true);
                }
            }
        } else {
            if(prev - present > 3 || prev - present < 1) {
                if(second_iter) return false;
                else {
                    std::vector list1 = list;
                    list1.erase(list1.begin()+idx+1);
                    std::vector list2 = list;
                    list2.erase(list2.begin()+idx);
                    return is_valid(list1, true) || is_valid(list2, true);
                }
            }
        }
    }
    return true;
}

int main() {
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
