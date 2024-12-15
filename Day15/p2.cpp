#include <algorithm>
#include <print>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <cassert>

struct Vector2D {// {{{
    int x {}, y {};

    Vector2D operator+(const Vector2D& other) const { return {x + other.x, y + other.y}; }
    Vector2D operator-(const Vector2D& other) const { return {x - other.x, y - other.y}; }
    void operator+=(const Vector2D& other) { x += other.x; y += other.y; }
    void operator-=(const Vector2D& other) { x -= other.x; y -= other.y; }
    bool operator==(const Vector2D& other) const { return x == other.x && y == other.y; }
    Vector2D operator/(const int a) { return {x / a, y / a}; }
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
    Grid() { }

    size_t getWidth() { return width; }
    size_t getHeight() { return height; }

    void addRow(const std::string_view line) {
        if(height == 0) width = line.size()*2;
        else {
            assert(width == line.size()*2);
        }
        height += 1;
        std::string new_line;
        for(auto ch: line) {
            if(ch == '.') new_line.append("..");
            else if(ch == '#') new_line.append("##");
            else if(ch == '@') new_line.append("@.");
            else if(ch == 'O') new_line.append("[]");
        }
        grid.insert(grid.end(), new_line.cbegin(), new_line.cend());
    }

    inline int get_index(const Vector2D& pos) const { 
        return pos.x + pos.y*width;
    }

    inline Vector2D get_xy(const size_t& pos) const {
        return {static_cast<int>(pos % width), static_cast<int>(pos / width)};
    }

    char& at(const Vector2D& pos) { return grid[get_index(pos)]; }
    char at(const Vector2D& pos) const { return grid[get_index(pos)]; }
    char at(const size_t pos) const { return grid[pos]; }
    char& at(const size_t pos) { return grid[pos]; }

    inline bool inBounds(const Vector2D& pos) const {
        return (pos.y < static_cast<int>(height))
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
                std::print("{} ", at({static_cast<int>(j), static_cast<int>(i)}));
            }
            std::println();
        }
    }

    long long int get_gps() const {
        long long int sum = 0;
        for(auto i = 0uz; i < grid.size(); ++i) {
            if(at(i) != '[') continue;
            Vector2D pos = get_xy(i);
            auto value = 100 * pos.y + pos.x;
            sum += value;
        }
        return sum;
    }
};
// }}}

std::unordered_set<int> find_next(const Grid& grid, const Vector2D& pos, const Vector2D& move_direction) {


    std::unordered_set boundary = {grid.get_index(pos)};
    std::unordered_set<int> region = {grid.get_index(pos)};
    while(true) {
        std::unordered_set<int> temp_boundary = {};
        for(const auto& pos: boundary) {
            auto elem = grid.get_xy(pos);
            auto new_elem = elem + move_direction;
            if(grid.at(elem) == '.') continue;
            else if(grid.at(new_elem) == '#') return {};
            else if(grid.at(new_elem) == '.') continue;
            else {
                temp_boundary.insert(grid.get_index(new_elem));

                if(grid.at(new_elem) == '[') {
                    auto matching = grid.get_index(new_elem + (Vector2D){1, 0});
                    if(!region.contains(matching)) temp_boundary.insert(matching);
                }
                else if(grid.at(new_elem) == ']') {
                    auto matching = grid.get_index(new_elem + (Vector2D){-1, 0});
                    if(!region.contains(matching)) temp_boundary.insert(matching);
                }
            }
        }
        if(temp_boundary.size() == 0) break;
        std::swap(boundary, temp_boundary);
        for(auto elem: boundary) region.insert(elem);
    }
    return region;
}

void move(Grid& grid, char direction, Vector2D& pos) {
    Vector2D move_direction;
    switch(direction) {
        case '^':
            move_direction = {0, -1};
            break;
        case '<':
            move_direction = {-1, 0};
            break;
        case '>':
            move_direction = {1, 0};
            break;
        case 'v':
            move_direction = {0, 1};
            break;
        default:
            assert("UNREACHABLE" && false);
            break;
    }

    auto end_pos = find_next(grid, pos, move_direction);

    if(end_pos.size() == 0) return;

    Grid temp_grid = grid;
    for(auto temp_pos: end_pos) {
        temp_grid.at(temp_pos) = '.';
    }
    for(auto temp_pos: end_pos) {
        temp_grid.at(grid.get_xy(temp_pos) + move_direction) = grid.at(temp_pos);
    }
    std::swap(temp_grid, grid);
    pos += move_direction;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};

    Grid grid;

    size_t ypos = 0;
    Vector2D player_pos;
    while(true) {
        std::string input {};
        std::getline(input_file, input);
        if(input_file.eof()) break;

        size_t xpos = input.find('@');
        if(xpos != std::string::npos) {
            player_pos.x = xpos*2;
            player_pos.y = ypos;
        }
        ++ypos;
        if(input == "") break;
        grid.addRow(input);

    }

    while(true) {
        char direction {};
        input_file.get(direction);
        if(input_file.eof()) break;
        if(direction != '^' && direction != '<' && direction != '>' && direction != 'v') continue;

        move(grid, direction, player_pos);
    }

    std::println("{}", grid.get_gps());

    return 0;
}

