#include <print>
#include <fstream>
#include <vector>
#include <cassert>

struct Vector2D {// {{{
    int x {}, y {};

    Vector2D operator+(const Vector2D& other) { return {x + other.x, y + other.y}; }
    Vector2D operator-(const Vector2D& other) { return {x - other.x, y - other.y}; }
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
            if(at(i) != 'O') continue;
            Vector2D pos = get_xy(i);
            sum += 100 * pos.y + pos.x;
        }
        return sum;
    }
};
// }}}

Vector2D find_next(const Grid& grid, const Vector2D& pos, const Vector2D& move_direction) {
    auto temp_pos = pos;

    while(grid.at(temp_pos) != '.' && grid.at(temp_pos) != '#') {
        temp_pos += move_direction;
    }

    return temp_pos;
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

    Vector2D end_pos = find_next(grid, pos, move_direction);

    if(grid.at(end_pos) == '#') return;

    while(!(end_pos == pos)) {
        std::swap(grid.at(end_pos), grid.at(end_pos-move_direction));
        end_pos -= move_direction;
    }

    pos += move_direction;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};

    Grid grid;

    size_t xpos = 0;
    Vector2D player_pos;
    while(true) {
        std::string input {};
        std::getline(input_file, input);
        if(input_file.eof()) break;

        size_t ypos = input.find('@');
        if(ypos != std::string::npos) {
            player_pos.x = xpos;
            player_pos.y = ypos;
        }
        ++xpos;
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

