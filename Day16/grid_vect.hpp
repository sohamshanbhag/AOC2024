#pragma once
#include <cassert>
#include <limits>
#include <vector>

struct Vector2D {// {{{
    int x {}, y {};

    Vector2D operator+(const Vector2D& other) const { return {x + other.x, y + other.y}; }
    Vector2D operator-(const Vector2D& other) const { return {x - other.x, y - other.y}; }
    Vector2D operator*(const int a) const { return {x * a, y * a}; }
    Vector2D operator/(const int a) const { return {x / a, y / a}; }
    void operator+=(const Vector2D& other) { x += other.x; y += other.y; }
    void operator-=(const Vector2D& other) { x -= other.x; y -= other.y; }
    bool operator==(const Vector2D& other) const { return x == other.x && y == other.y; }
    Vector2D rotate90anti() const {
        int tempx = x;
        return {y, -tempx};
    }
    Vector2D rotate90clock() const {
        int tempx = x;
        return {-y, tempx};
    }
    void print() const { std::print("{} {}, ",  x, y); }
    void println() const { std::println("{} {}",  x, y); }
};
// }}}

template <typename T>
class Grid {// {{{
private:
    size_t width {};
    size_t height {};
    std::vector<T> grid {};

public:
    Grid() { }
    Grid(size_t _width, size_t _height, T _value) 
        : width(_width)
        , height(_height)
        , grid(width*height, _value)
    { }

    size_t getWidth() const { return width; }
    size_t getHeight() const { return height; }

    void addRow(const std::string_view line) {
        if(height == 0) width = line.size();
        else {
            assert(width == line.size());
        }
        height += 1;
        grid.insert(grid.end(), line.cbegin(), line.cend());
    }

    inline int get_index(const Vector2D& pos) const { return pos.x + pos.y*width; }

    inline Vector2D get_xy(const size_t& pos) const {
        return {static_cast<int>(pos % width), static_cast<int>(pos / width)};
    }

    T& at(const Vector2D& pos) { return grid[get_index(pos)]; }
    T at(const Vector2D& pos) const { return grid[get_index(pos)]; }
    T at(const size_t pos) const { return grid[pos]; }
    T& at(const size_t pos) { return grid[pos]; }

    inline bool inBounds(const size_t pos) const {
        auto temp = get_xy(pos);

        return (temp.y < static_cast<int>(height))
        && (temp.x < static_cast<int>(width))
        && (temp.y >= 0)
        && (temp.x >= 0);
    }

    inline bool inBounds(const Vector2D& pos) const {
        return (pos.y < static_cast<int>(height))
        && (pos.x < static_cast<int>(width))
        && (pos.y >= 0)
        && (pos.x >= 0);
    }

    void print() const {
        std::print("   ");
        for(auto i = 0uz; i < width; ++i) {
            std::print("{:6} ", i);
        }
        std::println();
        for(auto i = 0uz; i < height; ++i) {
            std::print("{:6}, ", i);
            for(auto j = 0uz; j < width; ++j) {
                auto value = at({static_cast<int>(j), static_cast<int>(i)});
                if(value == std::numeric_limits<int>::max()) std::print("#######");
                else std::print("{:6} ", value);
            }
            std::println();
        }
    }

    long long int get_gps() const {
        long long int sum = 0;
        for(auto i = 0uz; i < grid.size(); ++i) {
            if(at(i) != 'O') continue;
            Vector2D pos = get_xy(i);
            sum += 100 * pos.y + pos.x;
        }
        return sum;
    }
};
// }}}

