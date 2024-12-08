#include <cctype>
#include <print>
#include <fstream>
#include <unordered_map>
#include <ranges>
#include <unordered_set>

struct Vector2D {
    int x {}, y {};

    Vector2D operator+(const Vector2D& other) const { return {x + other.x, y + other.y}; }
    Vector2D operator-(const Vector2D& other) const { return {x - other.x, y - other.y}; }
    void operator+=(const Vector2D& other) { x += other.x; y += other.y; }
    bool operator==(const Vector2D& other) const { return (x == other.x) && (y == other.y); }
    void rotate90() {
        int tempx = x;
        int tempy = y;
        x = tempy;
        y = -tempx;
    }
    void print() const { std::println("{} {}",  x, y); }
};

template<typename T>
void
hash_combine(std::size_t &seed, T const &key) {
  std::hash<T> hasher;
  seed ^= hasher(key) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std {
template<>
  struct hash<Vector2D> {
    std::size_t operator()(Vector2D const &p) const {
      std::size_t seed(0);
      ::hash_combine(seed, p.x);
      ::hash_combine(seed, p.y);
      return seed;
    }
  };
}

bool is_bounded(const Vector2D& pos, int width, int height) {
    return pos.x >= 0 && pos.y >= 0 && pos.x < height && pos.y < width;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};

    std::unordered_map<char, std::unordered_set<Vector2D>> antennas {};

    size_t pos_x = 0;
    size_t pos_y_one = 0;
    size_t width = 0;
    while(true) {
        ++pos_y_one;
        char c ;
        input_file.get(c);
        if(input_file.eof()) break;
        if(c == '\n') { width = pos_y_one-1; pos_y_one = 0; ++pos_x; continue; }
        if(!std::isalnum(c)) { continue; }
        size_t pos_y = pos_y_one - 1;
        antennas[c].emplace(pos_x, pos_y);
    }
    size_t height = pos_x;

    std::unordered_set<Vector2D> antinodes {};

    for(const auto& [key, elem]: antennas) {

        auto cart = std::ranges::views::cartesian_product(elem, elem);

        for(auto [item1, item2]: cart) {
            if(item1 == item2) continue;
            auto diff = item1 - item2;
            auto new_item1 = item1;
            while(is_bounded(new_item1, width, height)) {
                antinodes.insert(new_item1);
                new_item1 = new_item1 + diff;
            }
            new_item1 = item2;
            while(is_bounded(new_item1, width, height)) {
                antinodes.insert(new_item1);
                new_item1 = new_item1 - diff;
            }
        }

    }

    std::println("{}", antinodes.size());

    return 0;
}

