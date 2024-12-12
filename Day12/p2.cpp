#include <print>
#include <fstream>
#include <unordered_set>
#include <ranges>
#include <vector>
#include <cassert>

struct Vector2D {// {{{
    int x {}, y {};

    Vector2D operator+(const Vector2D& other) { return {x + other.x, y + other.y}; }
    Vector2D operator-(const Vector2D& other) { return {x - other.x, y - other.y}; }
    void operator+=(const Vector2D& other) { x += other.x; y += other.y; }
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
                std::print("{} ", at(i, j));
            }
            std::println();
        }
    }

    int get_index(const Vector2D& pos) const {
        return pos.x*width + pos.y;
    }

    Vector2D get_xy(const size_t& pos) const {
        return {static_cast<int>(pos / width), static_cast<int>(pos % width)};
    }
};
// }}}

std::vector<int> get_neighbours(const Grid& grid, const int pos) {
    auto xy = grid.get_xy(pos);
    std::vector<int> neighbours = {};
    auto pos_val = grid.at(xy);
    size_t new_idx;

    Vector2D new_pos = xy + Vector2D{1, 0};
    new_idx = grid.get_index(new_pos);
    if(grid.inBounds(new_pos) && grid.at(new_idx) == pos_val) neighbours.push_back(new_idx);

    new_pos = xy + Vector2D{-1, 0};
    new_idx = grid.get_index(new_pos);
    if(grid.inBounds(new_pos) && grid.at(new_idx) == pos_val) neighbours.push_back(new_idx);

    new_pos = xy + Vector2D{0, 1};
    new_idx = grid.get_index(new_pos);
    if(grid.inBounds(new_pos) && grid.at(new_idx) == pos_val) neighbours.push_back(new_idx);

    new_pos = xy + Vector2D{0, -1};
    new_idx = grid.get_index(new_pos);
    if(grid.inBounds(new_pos) && grid.at(new_idx) == pos_val) neighbours.push_back(new_idx);

    return neighbours;
}

long int get_region_metrics(Grid& grid, int row, int column) {
    std::unordered_set<int> neighbours = {grid.get_index({row, column})};
    std::unordered_set<int> region = {grid.get_index({row, column})};
    size_t vertices = 0;
    size_t area = 0;
    size_t perimeter = 0;

    while(neighbours.size() != 0) {
        std::unordered_set<int> new_neighbours {};
        for(auto elem: neighbours) {
            auto this_neighbours = get_neighbours(grid, elem);

            if(this_neighbours.size() == 0) {
                perimeter += 8;
                vertices += 4;
            } else if(this_neighbours.size() == 1) {
                perimeter += 4;
                vertices += 2;
            } else if(this_neighbours.size() == 2) {
                Vector2D sum_key = grid.get_xy(this_neighbours[0]) + grid.get_xy(this_neighbours[1]) - grid.get_xy(elem);
                if(grid.get_index(sum_key) == elem) {
                } else if(grid.at(sum_key) == grid.at(elem)) {
                    perimeter += 2; vertices += 1;
                } else if(grid.at(sum_key) != grid.at(elem)) {
                    perimeter += 4; vertices += 2;
                }
            } else if(this_neighbours.size() == 3) {
                for(auto i = 0uz; i < this_neighbours.size(); ++i) {
                    for(auto j = i+1; j < this_neighbours.size(); ++j) {
                        Vector2D sum_key = grid.get_xy(this_neighbours[i]) + grid.get_xy(this_neighbours[j]) - grid.get_xy(elem);
                        if(grid.get_index(sum_key) == elem) {
                        } else if(grid.at(sum_key) == grid.at(elem)) {
                        } else if(grid.at(sum_key) != grid.at(elem)) {
                            perimeter += 2; vertices += 1;
                        }
                    }
                }
            } else if(this_neighbours.size() == 4 ) {
                for(auto i = 0uz; i < this_neighbours.size(); ++i) {
                    for(auto j = i+1; j < this_neighbours.size(); ++j) {
                        Vector2D sum_key = grid.get_xy(this_neighbours[i]) + grid.get_xy(this_neighbours[j]) - grid.get_xy(elem);
                        if(grid.get_index(sum_key) == elem) {
                        } else if(grid.at(sum_key) == grid.at(elem)) {
                        } else if(grid.at(sum_key) != grid.at(elem)) {
                            perimeter += 2; vertices += 1;
                        }
                    }
                }
            }

            std::erase_if(this_neighbours, [&region](int elem) {return region.contains(elem);});
            for(auto item: this_neighbours) new_neighbours.insert(item);
            area += 1;
        }
        neighbours.clear();
        for(auto item: new_neighbours) neighbours.insert(item);
        region.merge(new_neighbours);
    }

    for(auto elem: region) grid.at(elem) = '.';
    perimeter -= vertices;



    return area * perimeter;

}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    Grid grid {};

    while(true) {
        std::string input {};
        std::getline(input_file, input);
        if(input_file.eof()) break;
        grid.addRow(input);
    }

    long int sum = 0;
    for(auto row = 0uz; row < grid.getHeight(); ++row) {
        for(auto column = 0uz; column < grid.getWidth(); ++column) {
            if(grid.at(row, column) == '.') continue;

            sum += get_region_metrics(grid, row, column);
        }
    }

    std::println("{}", sum);

    return 0;
}

