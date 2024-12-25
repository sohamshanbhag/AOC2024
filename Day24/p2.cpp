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

using dtype = long long int;

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

dtype calculate_result(auto known_wires, auto unknown_wires, auto relations, size_t num_z_wires) {
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

    dtype result {};
    for(auto i = 0uz; i < num_z_wires+1; ++i) {
        result += known_wires["z" + std::format("{:02}", i)] << i;
    }
    return result;


}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    std::unordered_map<std::string, dtype> known_wires {};
    std::unordered_set<std::string> unknown_wires {};
    std::vector<Relation> relations {};
    size_t num_z_wires {};
    size_t temp_z {};
    size_t num_x_wires {};
    size_t temp_x {};
    size_t num_y_wires {};
    size_t temp_y {};

    std::string input {"."};

    while(input != "") {
        std::getline(input_file, input);
        if(input == "") break;
        std::string wire = input.substr(0, 3);
        known_wires[wire] = input.back() - '0';
        if(wire.starts_with('y') && (temp_y = std::stoi(input.substr(1, 2))) > num_y_wires) num_y_wires = temp_y;
        if(wire.starts_with('x') && (temp_x = std::stoi(input.substr(1, 2))) > num_x_wires) num_x_wires = temp_x;
    }

    while(input_file.peek() != EOF) {
        std::getline(input_file, input);
        auto first = input.substr(0, 3);
        input = input.substr(4);
        auto pos = input.find(" ");
        auto operation = input.substr(0, pos);
        input = input.substr(pos+1);
        auto second = input.substr(0, 3);
        input = input.substr(7);
        auto result = input.substr(0, 3);
        relations.emplace_back(first, second, operation, result);
        if(!known_wires.contains(first)) unknown_wires.insert(first);
        if(!known_wires.contains(second)) unknown_wires.insert(second);
        if(!known_wires.contains(result)) unknown_wires.insert(result);
        if(result.starts_with('z') && (temp_z = std::stoi(input.substr(1, 2))) > num_z_wires) num_z_wires = temp_z;
    }

    for(auto& elem: known_wires) elem.second = 0;
    auto res = calculate_result(known_wires, unknown_wires, relations, num_z_wires);
    if(res != 0) std::println("x:0 y:0 z:{}", res);

    for(auto i = 0uz; i < num_x_wires+1; ++i) {
        for(auto& elem: known_wires) elem.second = 0;
        known_wires["x" + std::format("{:02}", i)] = 1;
        dtype input_x = 1LL << i;
        dtype input_y = 0;

        auto res = calculate_result(known_wires, unknown_wires, relations, num_z_wires);
        if(res != input_x + input_y) std::println("{}, x:{} y:{} z:{}", i, input_x, input_y, res);
    }

    for(auto i = 0uz; i < num_y_wires+1; ++i) {
        for(auto& elem: known_wires) elem.second = 0;
        known_wires["y" + std::format("{:02}", i)] = 1;
        dtype input_x = 0LL;
        dtype input_y = 1LL << i;

        auto res = calculate_result(known_wires, unknown_wires, relations, num_z_wires);
        if(res != input_x + input_y) std::println("{}, x:{} y:{} z:{}", i, input_x, input_y, res);
    }

    auto min_wires = num_y_wires < num_x_wires ? num_y_wires: num_x_wires;
    for(auto i = 0uz; i < min_wires+1; ++i) {
        for(auto& elem: known_wires) elem.second = 0;
        known_wires["y" + std::format("{:02}", i)] = 1;
        known_wires["x" + std::format("{:02}", i)] = 1;
        dtype input_y = 1LL << i;
        dtype input_x = 1LL << i;

        auto res = calculate_result(known_wires, unknown_wires, relations, num_z_wires);
        if(res != input_x + input_y) std::println("{}, x:{} y:{} z:{}", i, input_x, input_y, res);
    }

    return 0;
}


