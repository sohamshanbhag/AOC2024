#include <cassert>
#include <fstream>
#include <print>
#include <vector>
#include <string>
#include <algorithm>
#include <string_view>

struct Vector2D {
    int x {}, y {};

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
    void print() {
        std::print("   ");
        for(auto i = 0uz; i < width; ++i) {
            std::print("{} ", i);
        }
        std::println();
        for(auto i = 0uz; i < height; ++i) {
            std::print("{}, ", i);
            for(auto j = 0uz; j < width; ++j) {
                std::print("{} ", at(i, j));
            }
            std::println();
        }
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
            input_line.at(player_pos) = 'X';
        }
        grid.addRow(input_line);
    }


    while(true) {
        if(!grid.inBounds(position + direction)) break;
        if(grid.at(position + direction) == '#') direction.rotate90();

        position += direction;

        grid.at(position) = 'X';
    }

    std::println("{}", grid.getNumXs());

    return 0;
}
