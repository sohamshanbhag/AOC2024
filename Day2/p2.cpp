#include <algorithm>
#include <functional>
#include <ranges>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <print>

bool check_diff(int value) {
    return value <= 3 && value >= 1;
}

bool is_valid(const std::vector<int>& list, bool second_iter=0) {
    if(list.size() == 1) return true;
    if(list.size() == 2) {
        int err1 = std::abs(list.at(1) - list.at(0));
        return check_diff(err1);
    }
    if(list.size() == 3) {
        int err1 = std::abs(list.at(1) - list.at(0));
        int err2 = std::abs(list.at(2) - list.at(0));
        int err3 = std::abs(list.at(2) - list.at(1));
        return check_diff(err1) || check_diff(err2) || check_diff(err3);
    };

    auto ascending_list = list
        | std::views::take(4)
        | std::views::adjacent_transform<2>([](int a, int b) { return a < b; });
    bool ascending = std::ranges::fold_left(ascending_list, 0, std::plus<>()) >= 2;

    auto values = list | std::views::adjacent_transform<2>([ascending](int a, int b) {
        if(ascending) return check_diff(b - a);
        else return check_diff(a - b);
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
    std::println("{}", num_valid);


    return 0;
}
