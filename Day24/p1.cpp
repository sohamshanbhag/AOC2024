#include <cassert>
#include <format>
#include <print>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ranges>
#include <algorithm>
#include <unordered_set>

struct Relation {
    std::string first, second, op, res;
    bool done {false};
};

int operate(int first, int second, const std::string& op) {
    if(op == "OR") { return first || second; }
    else if(op == "AND") { return first && second; }
    else if(op == "XOR") { return first != second; }
    else {
    assert(0 && "UNREACHABLE");
    return 0;
    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    std::unordered_map<std::string, long long int> known_wires {};
    std::unordered_set<std::string> unknown_wires {};
    std::vector<Relation> relations {};
    size_t num_z_wires {};
    size_t temp_z {};

    std::string input {"."};

    while(input != "") {
        std::getline(input_file, input);
        if(input == "") break;
        known_wires[input.substr(0, 3)] = input.back() - '0';
    }

    while(input_file.peek() != EOF) {
        std::getline(input_file, input);
        // std::println("{}", input);
        auto first = input.substr(0, 3);
        input = input.substr(4);
        auto pos = input.find(" ");
        auto operation = input.substr(0, pos);
        input = input.substr(pos+1);
        auto second = input.substr(0, 3);
        input = input.substr(7);
        auto result = input;
        relations.emplace_back(first, second, operation, result);
        if(!known_wires.contains(first)) unknown_wires.insert(first);
        if(!known_wires.contains(second)) unknown_wires.insert(second);
        if(!known_wires.contains(result)) unknown_wires.insert(result);
        if(result.starts_with('z') && (temp_z = std::stoi(input.substr(1, 2))) > num_z_wires) num_z_wires = temp_z;
    }

    while(unknown_wires.size() > 0) {
        for(auto& elem: relations) {
            if(known_wires.contains(elem.first) && known_wires.contains(elem.second)) {
                elem.done = true;
                known_wires[elem.res] = operate(known_wires[elem.first], known_wires[elem.second], elem.op);
                unknown_wires.erase(elem.res);
            }
        }
        std::erase_if(relations, [](const auto& a) { return a.done; });
    }

    long long int result {};
    for(auto i = 0uz; i < num_z_wires+1; ++i) {
        result += known_wires["z" + std::format("{:02}", i)] << i;
    }
    std::println("{}", result);


    return 0;
}

