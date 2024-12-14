#include <cmath>
#include <functional>
#include <algorithm>
#include <limits>
#include <ranges>
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

double variance(const auto& pos) {
    auto mean = std::ranges::fold_left(pos, (Vector2D){0, 0}, std::plus<>()) / pos.size();
    auto variance = std::ranges::fold_left(
        std::ranges::transform_view(pos, [&mean](const auto& val) {
            return (Vector2D){(val.x - mean.x) * (val.x - mean.x), (val.y - mean.y) * (val.y - mean.y)};
        })
      , (Vector2D){0, 0}, std::plus<>()) / pos.size();
    return variance.x + variance.y;
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
    int min_iter = 0;
    double min_var = std::numeric_limits<double>::max();

    while(true){
        iter++;
        if(iter > width * height) break;
        for(auto& pos: robots) {
            pos.first += pos.second;
            pos.first.x = emod(pos.first.x, width);
            pos.first.y = emod(pos.first.y, height);
        }

        auto var = variance(std::views::keys(robots));
        if (var < min_var) {
            min_iter = iter;
            min_var = var;
        }
    }
    std::println("Tree found, iter: {}", min_iter);

    return 0;
}

