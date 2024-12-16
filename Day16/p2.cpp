#include <limits>
#include <print>
#include <fstream>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <unistd.h>
#include <queue>
#include <unordered_set>

#include "grid_vect.hpp"

struct State {
    Vector2D pos, dir {};
};

int get_other_dir_idx(const Vector2D& dir) {
    if(dir == (Vector2D){0, 1}) return 3;
    if(dir == (Vector2D){1, 0}) return 2;
    if(dir == (Vector2D){-1, 0}) return 1;
    if(dir == (Vector2D){0, -1}) return 0;
    assert("UNREACHABLE" && false);
    return -1;
}

int get_dir_idx(const Vector2D& dir) {
    if(dir == (Vector2D){0, 1})  return 0;
    if(dir == (Vector2D){1, 0})  return 1;
    if(dir == (Vector2D){-1, 0}) return 2;
    if(dir == (Vector2D){0, -1}) return 3;
    assert("UNREACHABLE" && false);
    return -1;
}

size_t get_cost(const Grid<char>& grid, const State& start_state, const Vector2D& goal_pos) {
    auto compare_func = [](const auto& a, const auto& b) { return std::get<1>(a) > std::get<1>(b); };
    using T = std::tuple<State, int>;
    std::priority_queue<T, std::vector<T>, decltype(compare_func)> boundary {compare_func};

    std::unordered_map<int, std::unordered_set<int>> visited {};
    std::unordered_set<int> visited_keys {};
    boundary.emplace(start_state, 0);
    int minval = std::numeric_limits<int>::max();

    auto gridsize = grid.getWidth() * grid.getHeight();

    while(boundary.size() != 0) {

        auto [present, present_val] = boundary.top();
        boundary.pop();

        if(present.pos == goal_pos) {
            if(minval >= present_val) {
                minval = present_val;
            }
        }

        if(present_val > minval) break;

        auto present_idx = grid.get_index(present.pos) + get_dir_idx(present.dir)*gridsize;

        if(visited_keys.contains(present_idx)) {
            continue;
        } else {
            visited_keys.insert(present_idx);
        }

        auto temp_pos = present.pos + present.dir;
        if(grid.at(temp_pos) != '#') {
            boundary.emplace((State){temp_pos, present.dir}, present_val + 1);
            auto temp_idx = grid.get_index(temp_pos) + get_dir_idx(present.dir)*gridsize;
            visited[temp_idx].insert(visited[present_idx].begin(), visited[present_idx].end());
            visited[temp_idx].insert(temp_idx);
        }
        {
            auto temp_dir = present.dir.rotate90clock();
            boundary.emplace((State){present.pos, temp_dir}, present_val + 1000);
            auto temp_idx = grid.get_index(present.pos) + get_dir_idx(temp_dir)*gridsize;
            visited[temp_idx].insert(visited[present_idx].begin(), visited[present_idx].end());
            visited[temp_idx].insert(temp_idx);
        }
        {
            auto temp_dir = present.dir.rotate90anti();
            boundary.emplace((State){present.pos, temp_dir}, present_val + 1000);
            auto temp_idx = grid.get_index(present.pos) + get_dir_idx(temp_dir)*gridsize;
            visited[temp_idx].insert(visited[present_idx].begin(), visited[present_idx].end());
            visited[temp_idx].insert(temp_idx);
        }
    }

    std::unordered_set<int> best_spots {};
    for(auto elem: visited[grid.get_index(goal_pos)]) best_spots.insert(elem % gridsize);
    for(auto elem: visited[grid.get_index(goal_pos) + 1 * gridsize]) best_spots.insert(elem % gridsize);
    for(auto elem: visited[grid.get_index(goal_pos) + 2 * gridsize]) best_spots.insert(elem % gridsize);
    for(auto elem: visited[grid.get_index(goal_pos) + 3 * gridsize]) best_spots.insert(elem % gridsize);
    auto num_best_spots = best_spots.size();

    return num_best_spots;

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

