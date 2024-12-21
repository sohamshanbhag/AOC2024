#include <print>
#include <fstream>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#include "grid_vect.hpp"

std::vector<int> get_route(const Grid<char>& grid, const Vector2D& start_pos, const Vector2D& goal_pos) {
    std::unordered_set<int> boundary {grid.get_index(start_pos)};

    Grid<int> values (grid.getWidth(), grid.getHeight(), 100);
    values.at(start_pos) = 0;
    auto goal_idx = grid.get_index(goal_pos);

    std::unordered_set<int> visited;
    Vector2D temp_pos = {};

    while(boundary.size() > 0) {
        std::unordered_set<int> temp_boundary {};
        for(auto elem: boundary) {
            if(visited.contains(elem)) continue;
            visited.insert(elem);

            auto present = grid.get_xy(elem);

            auto present_val = values.at(elem);

            temp_pos = present + Vector2D{1, 0};
            if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#') && !visited.contains(grid.get_index(temp_pos))) {
                temp_boundary.insert(grid.get_index(temp_pos));
                values.at(temp_pos) = present_val + 1;
            }
            temp_pos = present + Vector2D{-1, 0};
            if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#') && !visited.contains(grid.get_index(temp_pos))) {
                temp_boundary.insert(grid.get_index(temp_pos));
                values.at(temp_pos) = present_val + 1;
            }
            temp_pos = present + Vector2D{0, 1};
            if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#') && !visited.contains(grid.get_index(temp_pos))) {
                temp_boundary.insert(grid.get_index(temp_pos));
                values.at(temp_pos) = present_val + 1;
            }
            temp_pos = present + Vector2D{0, -1};
            if(grid.inBounds(temp_pos) && (grid.at(temp_pos) != '#') && !visited.contains(grid.get_index(temp_pos))) {
                temp_boundary.insert(grid.get_index(temp_pos));
                values.at(temp_pos) = present_val + 1;
            }
        }
        std::swap(boundary, temp_boundary);
        if(boundary.contains(goal_idx)) break;
    }

    std::vector<int> route = {goal_idx};
    auto present = goal_pos;
    Vector2D next_pos;
    while(present != start_pos) {
        if(values.at(next_pos = present + Vector2D{0, 1}) == values.at(present) - 1)       route.push_back(grid.get_index(next_pos));
        else if(values.at(next_pos = present + Vector2D{0, -1}) == values.at(present) - 1) route.push_back(grid.get_index(next_pos));
        else if(values.at(next_pos = present + Vector2D{1, 0}) == values.at(present) - 1)  route.push_back(grid.get_index(next_pos));
        else if(values.at(next_pos = present + Vector2D{-1, 0}) == values.at(present) - 1) route.push_back(grid.get_index(next_pos));
        present = next_pos;
    }

    // Grid grid2 = grid;
    // for(auto elem: route) grid2.at(elem) = 'O';
    // grid2.print();

    return route;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    std::string input {};

    int ypos = 0;
    Grid<char> grid;
    Vector2D start_pos, goal_pos;

    while(input_file.peek() != EOF) {
        std::getline(input_file, input);
        grid.addRow(input);
        size_t xpos = input.find('S');
        if(xpos != std::string::npos) start_pos = {static_cast<int>(xpos), ypos};
        xpos = input.find('E');
        if(xpos != std::string::npos) goal_pos = {static_cast<int>(xpos), ypos};
        ++ypos;
    }

    auto route = get_route(grid, start_pos, goal_pos);
    std::unordered_set<int> route_set {};
    for(auto elem: route) route_set.insert(elem);
    std::unordered_map<int, int> time_saved {};
    int temp_idx;
    // std::println("{}", route.size());
    // grid.print();
    // for(auto elem: route) grid.get_xy(elem).println();

    std::vector<Vector2D> directions = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

    for(auto i = 0uz; i < route.size(); ++i) {
        auto pos = grid.get_xy(route[i]);

        for(auto elem: directions) {
            temp_idx = grid.get_index(pos + elem * 2);
            if(route_set.contains(temp_idx) && grid.at(pos + elem) == '#') {
                size_t listpos = std::ranges::find(route, temp_idx) - route.begin();
                // std::print("{} {}, ", i, listpos);
                // pos.print();
                // elem.println();
                if(listpos > i) ++time_saved[listpos - i - 2];
            }
        }
    }

    size_t num_greater_100 {};
    for(auto elem: time_saved) if(elem.first >= 100) num_greater_100 += elem.second;

    std::println("{}", num_greater_100);
    // std::println("{} {}", elem.first, elem.second);

    return 0;
}

