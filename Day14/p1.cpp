#include <cmath>
#include <algorithm>
#include <functional>
#include <regex>
#include <print>
#include <array>
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
    std::array<int, 5> quad {};

    while(true) {
        std::string input_line;
        std::getline(input_file, input_line);

        if(input_file.eof()) break;

        auto pos = get_inputs(input_line, entry_regex);

        pos.first += pos.second * 100;
        pos.first.x = emod(pos.first.x, width);
        pos.first.y = emod(pos.first.y, height);

        ++quad[get_quad(pos.first, width, height)];

    }
    std::println("{}", quad[0]*quad[1]*quad[2]*quad[3]);

    return 0;
}

