#include <cassert>
#include <fstream>
#include <print>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>
#include <string_view>

struct Vector2D {
    int x {}, y {};

    bool operator==(const Vector2D& other) { return (x == other.x) && (y == other.y); }
    Vector2D operator+(const Vector2D& other) { return {x + other.x, y + other.y}; }
    void operator+=(const Vector2D& other) { x += other.x; y += other.y; }
    void rotate90() {
        int tempx = x;
        int tempy = y;
        x = tempy;
        y = -tempx;
    }
    void print() { std::println("{} {}",  x, y); }
};

class Grid {
private:
    std::vector<char> grid {};
    size_t width {};
    size_t height {};

public:
    Grid() { }

    size_t getWidth() { return width; }
    size_t getHeight() { return height; }

    void addRow(const std::string_view line) {
        if(height == 0) width = line.size();
        else {
            assert(width == line.size());
        }
        height += 1;
        grid.insert(grid.end(), line.cbegin(), line.cend());
    }

    char& at(size_t i, size_t j) {
        return grid.at(i*width + j);
    }

    char& at(const Vector2D& pos) {
        return at(pos.x, pos.y);
    }

    bool inBounds(const Vector2D& pos) {
        return
            (pos.y < static_cast<int>(height))
        && (pos.x < static_cast<int>(width))
        && (pos.y >= 0)
        && (pos.x >= 0);
    }

    bool is_visited(const Vector2D& pos, const Vector2D& dir) {
        if (dir.x == 1  && dir.y == 0 )      return ((at(pos) - '.') & 0b0001) == 0b0001;
        else if (dir.x == -1 && dir.y == 0 ) return ((at(pos) - '.') & 0b0010) == 0b0010;
        else if (dir.x == 0  && dir.y == 1 ) return ((at(pos) - '.') & 0b0100) == 0b0100;
        else if (dir.x == 0  && dir.y == -1) return ((at(pos) - '.') & 0b1000) == 0b1000;
        return false;
    }

    void visit(const Vector2D& pos, const Vector2D& dir) {
        if (dir.x == 1  && dir.y == 0 ) at(pos) += 0b0001;
        else if (dir.x == -1 && dir.y == 0 ) at(pos) += 0b0010;
        else if (dir.x == 0  && dir.y == 1 ) at(pos) += 0b0100;
        else if (dir.x == 0  && dir.y == -1) at(pos) += 0b1000;
    }

    void print() {
        std::println("------------------------------------------");
        std::print("   ");
        for(auto i = 0uz; i < width; ++i) {
            std::print("{:3} ", i);
        }
        std::println();
        std::println("==========================================");
        for(auto i = 0uz; i < height; ++i) {
            std::print("{}| ", i);
            for(auto j = 0uz; j < width; ++j) {
                std::print("{:3} ", (int)(at(i, j) - '.'));
            }
            std::println();
        }
        std::println("------------------------------------------");
    }

    size_t getNumXs() {
        return std::ranges::count(grid, 'X');
    }
};

int main() {

    // std::ifstream input_file {"example.txt"};
    std::ifstream input_file {"input.txt"};

    Grid grid;
    Vector2D position;
    Vector2D direction = {-1, 0};

    while(true) {
        std::string input_line {};
        std::getline(input_file, input_line);
        if(input_file.eof()) break;
        auto player_pos = input_line.find('^');
        if(player_pos != input_line.npos) {
            position = {
                static_cast<int>(grid.getHeight()),
                static_cast<int>(player_pos),
            };
            input_line.at(player_pos) = '.';
        }
        grid.addRow(input_line);
    }
    grid.visit(position, direction);
    Grid orig_grid = grid;
    Vector2D orig_position = position;
    Vector2D orig_direction = direction;

    std::vector<Vector2D> visited_positions = {};
    while(true) {
        if(!grid.inBounds(position + direction)) break;
        if(grid.at(position + direction) == '#') direction.rotate90();
        position += direction;
        visited_positions.push_back(position);
        grid.visit(position, direction);
    }

    size_t obstacle_pos = 0;
    std::unordered_set<int> loop_points {};
    auto width = grid.getWidth();

    while(obstacle_pos != visited_positions.size()) {
        grid = orig_grid;
        position = orig_position;
        direction = orig_direction;
        grid.at(visited_positions.at(obstacle_pos)) = '#';
        bool is_loop = false;
        while(true) {
            if(!grid.inBounds(position + direction)) break;
            if(grid.at(position + direction) == '#') {
                direction.rotate90();
                continue;
            }

            position += direction;

            if(grid.is_visited(position, direction)) {
                is_loop = true;
                break;
            }
            grid.visit(position, direction);
        }
        if(is_loop) {
            auto point = visited_positions.at(obstacle_pos);
            loop_points.insert(point.x*width + point.y);
        }
        ++obstacle_pos;
    }

    std::println("{}", loop_points.size());

    return 0;
}
