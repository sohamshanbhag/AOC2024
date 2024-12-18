#include <print>
#include <fstream>
#include <regex>
#include <queue>
#include <limits>
#include <unordered_map>
#include <unordered_set>

#include "grid_vect.hpp"

Vector2D get_inputs(const std::string& input, const std::regex& reg) {
    auto words_begin = std::sregex_iterator(input.begin(), input.end(), reg);
    auto words_end = std::sregex_iterator();
    std::smatch m = *words_begin++;
    int px = std::stoi(m.str());
    m = *words_begin++;
    int py = std::stoi(m.str());
    m = *words_begin++;
    return {px, py};
}

long long int get_cost(const Grid<char>& grid, const Vector2D& start_pos, const Vector2D& goal_pos) {
    auto compare_func = [](const auto& a, const auto& b) { return a.second > b.second; };
    using T = std::pair<Vector2D, long long int>;
    [[maybe_unused]] std::priority_queue<T, std::vector<T>, decltype(compare_func)> boundary {compare_func};

    boundary.emplace(start_pos, 0);
    std::unordered_set<int> visited;
    Vector2D temp_pos = {};

    while(true) {
        auto [present, present_val] = boundary.top();
        boundary.pop();

        auto idx = grid.get_index(present);
        if(visited.contains(idx)) continue;
        visited.insert(idx);

        if(present == goal_pos) return present_val;

        temp_pos = present + Vector2D{1, 0};
        if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#')) boundary.emplace(temp_pos, present_val + 1);
        temp_pos = present + Vector2D{-1, 0};
        if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#')) boundary.emplace(temp_pos, present_val + 1);
        temp_pos = present + Vector2D{0, 1};
        if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#')) boundary.emplace(temp_pos, present_val + 1);
        temp_pos = present + Vector2D{0, -1};
        if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#')) boundary.emplace(temp_pos, present_val + 1);

    }

    return 0;

}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    constexpr int grid_size = 70;
    constexpr int num_meas = 1024;
    std::ifstream input_file {argv[1]};
    std::regex entry_regex("([-]?\\d+)");
    Grid<char> grid(grid_size+1, grid_size+1, '.');

    [[maybe_unused]] Vector2D start_pos = {0, 0};
    [[maybe_unused]] Vector2D goal_pos = {grid_size, grid_size};
    std::string input {};

    // while(input_file.peek() != EOF) {
    for(auto i = 0uz; i < num_meas; ++i) {
        std::getline(input_file, input);
        grid.at(get_inputs(input, entry_regex)) = '#';
    }

    auto cost = get_cost(grid, start_pos, goal_pos);
    // grid.print();
    std::println("{}", cost);

    return 0;
}

