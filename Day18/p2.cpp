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

bool can_reach_exit(const Grid<char>& grid, const Vector2D& start_pos, const Vector2D& goal_pos) {
    auto compare_func = [](const auto& a, const auto& b) { return a.second > b.second; };
    using T = std::pair<Vector2D, long long int>;
    [[maybe_unused]] std::priority_queue<T, std::vector<T>, decltype(compare_func)> boundary {compare_func};

    boundary.emplace(start_pos, 0);
    std::unordered_set<int> visited;
    Vector2D temp_pos = {};

    while(true) {
        if(boundary.empty()) return false;
        auto [present, present_val] = boundary.top();
        boundary.pop();

        auto idx = grid.get_index(present);
        if(visited.contains(idx)) continue;
        visited.insert(idx);

        if(present == goal_pos) return true;

        temp_pos = present + Vector2D{1, 0};
        if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#')) boundary.emplace(temp_pos, present_val + 1);
        temp_pos = present + Vector2D{-1, 0};
        if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#')) boundary.emplace(temp_pos, present_val + 1);
        temp_pos = present + Vector2D{0, 1};
        if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#')) boundary.emplace(temp_pos, present_val + 1);
        temp_pos = present + Vector2D{0, -1};
        if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#')) boundary.emplace(temp_pos, present_val + 1);

    }

    return false;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    constexpr int grid_size = 70;
    std::ifstream input_file {argv[1]};
    std::regex entry_regex("([-]?\\d+)");

    Grid<char> grid(grid_size+1, grid_size+1, '.');
    Vector2D start_pos = {0, 0};
    Vector2D goal_pos = {grid_size, grid_size};
    std::string input {};
    [[maybe_unused]] bool path_exists = true;
    size_t low = 0;
    std::vector<Vector2D> obstacles {};

    while(std::getline(input_file, input)) {
        obstacles.emplace_back(get_inputs(input, entry_regex));
    }

    size_t high = obstacles.size();

    while(true) {
        grid.reset();
        size_t present = (low + high) / 2;
        for(auto i = 0uz; i < present; ++i) {
            grid.at(obstacles[i]) = '#';
        }

        path_exists = can_reach_exit(grid, start_pos, goal_pos);
        if(path_exists) {
            if(low == present) break;
            low = present;
        } else {
            high = present;
        }
    }

    std::println("{},{}", obstacles[low].x, obstacles[low].y);

    return 0;
}

