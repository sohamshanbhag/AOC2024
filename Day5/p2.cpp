#include <span>
#include <unordered_map>
#include <vector>
#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>

bool is_correct_order(std::span<int> present_list, int item, const auto& parents) {
    for(auto elem: present_list) {
        if(elem != item && !parents.at(item).contains(elem)) return false;
    }
    return true;
}

int main() {
    // std::ifstream input_file {"example.txt"};

    std::ifstream input_file {"input.txt"};

    std::vector<std::pair<int, int>> maps;

    while(true) {
        std::string input;
        std::getline(input_file, input);
        if(input == "") break;
        auto break_pos = input.find("|");
        maps.emplace_back(std::stoi(input.substr(0, break_pos)), std::stoi(input.substr(break_pos+1, input.size())));
    }
    {
        std::unordered_set<int> left_elements;

        for(auto [elem1, elem2]: maps) {
            if(!left_elements.contains(elem1)) left_elements.insert(elem1);
        }
        for(auto elem: left_elements) maps.emplace_back(-1, elem);
    }


    std::unordered_map<int, std::unordered_set<int>> parents;

    for(auto [first, second]: maps) parents[second].insert(first);

    std::vector<std::vector<int>> incorrect_orders = {};

    while(true) {
        std::string input;
        std::getline(input_file, input);
        if(input_file.eof()) break;
        std::stringstream iss {input};

        std::string item_input;
        int item;
        std::vector<int> present_list = {};
        bool order = true;
        while(std::getline(iss, item_input, ',')) {
            item = std::stoi(item_input);
            present_list.push_back(item);
            order &= is_correct_order(present_list, item, parents);
        }
        if(!order) incorrect_orders.push_back(present_list);
    }


    std::vector<std::vector<int>> corrected_orders = {};

    for(auto present_order: incorrect_orders) {

        std::unordered_map<int, std::unordered_set<int>> temp_parents;
        for(auto elem: present_order) {
            temp_parents[elem] = {};
            for(auto elem2: present_order) {
                if(parents.at(elem).contains(elem2)) {
                    temp_parents[elem].insert(elem2);
                }
            }
        }

        std::vector<int> new_order(present_order.size());
        for(auto [elem, val]: temp_parents) {
            new_order.at(val.size()) = elem;
        }

        corrected_orders.push_back(new_order);

    }


    long int sum = 0;
    for(auto elem: corrected_orders) sum += elem.at(elem.size()/2);
    std::println("{}", sum);


    return 0;
}
