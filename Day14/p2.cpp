#include <cmath>
#include <unistd.h>
#include <cassert>
#include <regex>
#include <print>
#include <string>
#include <fstream>

struct Vector2D {// {{{
    int x {}, y {};

    Vector2D operator+(const Vector2D& other) { return {x + other.x, y + other.y}; }
    Vector2D operator-(const Vector2D& other) { return {x - other.x, y - other.y}; }
    void operator+=(const Vector2D& other) { x += other.x; y += other.y; }
    Vector2D operator/(const int a) const { return {x / a, y / a}; }
    Vector2D operator*(const int a) const { return {x * a, y * a}; }
    void rotate90() {
        int tempx = x;
        int tempy = y;
        x = tempy;
        y = -tempx;
    }
    void print() const { std::print("{} {}",  x, y); }
    void println() const { std::println("{} {}",  x, y); }
};
// }}}

class Grid {// {{{
private:
    size_t width {};
    size_t height {};
    std::vector<char> grid {};

public:
    Grid(size_t _width, size_t _height)
        : width {_width}
        , height {_height}
        , grid(width * height, '.')
    { }

    size_t getWidth() { return width; }
    size_t getHeight() { return height; }
    int get_index(const Vector2D& pos) const { return pos.x + pos.y * width; }
    Vector2D get_xy(const size_t& pos) const { return {static_cast<int>(pos % width), static_cast<int>(pos / width)}; }

    char at(int i, int j) const { return at({i, j}); }

    char& at(const Vector2D& pos) { return grid[get_index(pos)]; }

    char at(const Vector2D& pos) const { return grid[get_index(pos)]; }
    char at(const size_t pos) const { return grid[pos]; }
    char& at(const size_t pos) { return grid[pos]; }

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
                std::print("{} ", at(j, i));
            }
            std::println();
        }
    }

    void mark(const Vector2D& pos) {
        at(pos) = '#';
    }

    int check_cont() {
        size_t len = 10;
        for(size_t i = 0; i < grid.size() - len; ++i) {
            bool is_valid = true;
            for(size_t j = 0; j < len; ++j) {
                if(at(i+j) != '#') {
                    i += j;
                    is_valid = false;
                    break;
                }
            }
            if(is_valid) return true;
        }
        return false;
    }

};
// }}}

std::pair<Vector2D, Vector2D> get_inputs(const std::string& input, const std::regex& reg) {
    auto words_begin = std::sregex_iterator(input.begin(), input.end(), reg);
    auto words_end = std::sregex_iterator();
    std::smatch m = *words_begin++;
    int px = std::stoi(m.str());
    m = *words_begin++;
    int py = std::stoi(m.str());
    m = *words_begin++;
    int vx = std::stoi(m.str());
    m = *words_begin++;
    int vy = std::stoi(m.str());
    return {{px, py}, {vx, vy}};
}

int emod(int x, int base) {
    return ((x % base) + base) % base;
}

int get_quad(const Vector2D& pos, int width, int height) {
    // 0 | 1
    // 2 | 3
    if((pos.x == width/2) || (pos.y == height/2)) return 4;
    else return (pos.x > width/2) + 2 * (pos.y > height/2);
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    constexpr int width = 101;
    constexpr int height = 103;

    std::ifstream input_file {argv[1]};
    std::regex entry_regex("([-]?\\d+)");
    std::smatch re_match;

    std::vector<std::pair<Vector2D, Vector2D>> robots;

    while(true) {
        std::string input_line;
        std::getline(input_file, input_line);

        if(input_file.eof()) break;

        auto pos = get_inputs(input_line, entry_regex);
        robots.push_back(pos);
    }

    int iter = 0;
    while(true){

        iter++;
        if(iter > width * height) break;
        for(auto& pos: robots) {
            pos.first += pos.second;
            pos.first.x = emod(pos.first.x, width);
            pos.first.y = emod(pos.first.y, height);
        }

        Grid grid(width, height);

        for(auto pos: robots) grid.mark(pos.first);
        if(grid.check_cont()) break;
    }
    std::println("Tree found, iter: {}", iter);

    return 0;
}

