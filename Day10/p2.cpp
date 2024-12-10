#include <print>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>

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
    void print() const { std::print("{} {}",  x, y); }
    void println() const { std::println("{} {}",  x, y); }
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

    char at(int i, int j) const { return at({i, j}); }

    char& at(const Vector2D& pos) { return grid[pos.x*width + pos.y]; }

    char at(const Vector2D& pos) const { return grid[pos.x*width + pos.y]; }

    bool inBounds(const Vector2D& pos) const {
        return 
            (pos.y < static_cast<int>(height))
        && (pos.x < static_cast<int>(width))
        && (pos.y >= 0)
        && (pos.x >= 0);
    }

    void print() const {
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

    int get_index(const Vector2D& pos) const {
        return pos.x*width + pos.y;
    }

    std::vector<Vector2D> get_pos(int c) {
        std::vector<Vector2D> pos {};
        for(auto x = 0uz; x < height; ++x) {
            for(auto y = 0uz; y < width; ++y) {
                if(grid[x*width + y] == c) pos.emplace_back(x, y);
            }
        }

        return pos;
    }
};

int get_num_trails(const Grid& grid, Vector2D pos) {
    if(grid.at(pos) == '9') {
        return 1;
    }
    int num_trails {};
    Vector2D new_pos = pos + Vector2D(1, 0);
    if (grid.inBounds(new_pos) && grid.at(new_pos) - grid.at(pos) == 1) {
        num_trails += get_num_trails(grid, new_pos);
    }
    new_pos = pos + Vector2D(-1, 0);
    if (grid.inBounds(new_pos) && grid.at(new_pos) - grid.at(pos) == 1) {
        num_trails += get_num_trails(grid, new_pos);
    }
    new_pos = pos + Vector2D(0, 1);
    if (grid.inBounds(new_pos) && grid.at(new_pos) - grid.at(pos) == 1) {
        num_trails += get_num_trails(grid, new_pos);
    }
    new_pos = pos + Vector2D(0, -1);
    if (grid.inBounds(new_pos) && grid.at(new_pos) - grid.at(pos) == 1) {
        num_trails += get_num_trails(grid, new_pos);
    }
    return num_trails;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    Grid map;

    while(true) {
        std::string input;
        std::getline(input_file, input);
        if(input_file.eof()) break;
        map.addRow(input);
    }

    std::vector trailheads = map.get_pos('0');

    size_t num_trails_tot = 0;
    for(const auto& elem: trailheads) {
        auto num_trails = get_num_trails(map, elem);
        num_trails_tot += num_trails;
    }
    std::println("{}", num_trails_tot);
    return 0;
}

