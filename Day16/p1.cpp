#include <print>
#include <fstream>
#include <queue>
#include <limits>
#include <unordered_map>
#include <unordered_set>

#include "grid_vect.hpp"

struct State {
    Vector2D pos, dir {};
};

int get_dir_idx(const Vector2D& dir) {
    if(dir == (Vector2D){0, 1}) return 0;
    if(dir == (Vector2D){1, 0}) return 1;
    if(dir == (Vector2D){-1, 0}) return 2;
    if(dir == (Vector2D){0, -1}) return 3;
    assert("UNREACHABLE" && false);
    return -1;
}

long long int get_cost(const Grid<char>& grid, const State& start_state, const Vector2D& goal_pos) {
    auto compare_func = [](const auto& a, const auto& b) { return a.second > b.second; };
    using T = std::pair<State, long long int>;
    [[maybe_unused]] std::priority_queue<T, std::vector<T>, decltype(compare_func)> boundary {compare_func};

    boundary.emplace(start_state, 0);
    std::unordered_set<int> visited;
    const auto gridsize = grid.getWidth() * grid.getHeight();

    while(true) {
        auto [present, present_val] = boundary.top();
        boundary.pop();
        auto posidx = grid.get_index(present.pos);
        int diridx = get_dir_idx(present.dir);
        auto idx = posidx + gridsize * diridx;
        if(visited.contains(idx)) continue;
        visited.insert(idx);

        if(present.pos == goal_pos) return present_val;

        auto temp_pos = present.pos + present.dir;
        if(grid.at(temp_pos) != '#') boundary.emplace((State){temp_pos, present.dir}, present_val + 1);
        boundary.emplace((State){present.pos, present.dir.rotate90clock()}, present_val + 1000);
        boundary.emplace((State){present.pos, present.dir.rotate90anti()}, present_val + 1000);
    }

    return 0;

}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    Grid<char> grid;

    int ypos = 0;
    Vector2D start_pos;
    Vector2D goal_pos;

    while(true) {
        std::string input {};
        std::getline(input_file, input);
        if(input_file.eof()) break;
        grid.addRow(input);
        size_t xpos = input.find('S');
        if(xpos != std::string::npos) start_pos = {static_cast<int>(xpos), ypos};
        xpos = input.find('E');
        if(xpos != std::string::npos) goal_pos = {static_cast<int>(xpos), ypos};
        ++ypos;
    }
    State start_state = {start_pos, {1, 0}};

    std::println("{}", get_cost(grid, start_state, goal_pos));

    return 0;
}

