#include <fstream>
#include <print>
#include <vector>

bool check_match(char a, char b, char c, char d) {
    return (a == 'X' && b == 'M' && c == 'A' && d == 'S')
        || (a == 'S' && b == 'A' && c == 'M' && d == 'X');
}


int main() {

    // auto input_file = std::ifstream("example.txt");
    auto input_file = std::ifstream("input.txt");

    std::vector<std::vector<char>> data = {{}};

    size_t idx = 0;
    while(true) {
        char c;
        input_file.get(c);
        if(input_file.eof()) break;
        if(c == '\n') {
            idx++;
            continue;
        }
        if(data.size() <= idx) data.push_back({});
        data.at(idx).push_back(c);
    }

    const auto height = data.size();
    const auto width = data.at(0).size();

    size_t num_xmases = 0;

    for (size_t row = 0; row < height; ++row) {
        for (size_t column = 0; column < width-3; ++column) {
            if(check_match(data.at(row).at(column)
                , data.at(row).at(column+1)
                , data.at(row).at(column+2)
                , data.at(row).at(column+3))
            ) {
                num_xmases += 1;
            }
        }
    }

    for (size_t row = 0; row < height-3; ++row) {
        for (size_t column = 0; column < width; ++column) {
            if(check_match(data.at(row).at(column),
                data.at(row+1).at(column),
                data.at(row+2).at(column),
                data.at(row+3).at(column))
            ) {
                num_xmases += 1;
            }
        }
    }

    for (size_t row = 3; row < height; ++row) {
        for (size_t column = 0; column < width-3; ++column) {
            if(check_match(data.at(row).at(column)
                , data.at(row-1).at(column+1)
                , data.at(row-2).at(column+2)
                , data.at(row-3).at(column+3))
            ) {
                num_xmases += 1;
            }
        }
    }

    for (size_t row = 0; row < height-3; ++row) {
        for (size_t column = 0; column < width-3; ++column) {
            if(check_match(data.at(row).at(column)
                , data.at(row+1).at(column+1)
                , data.at(row+2).at(column+2)
                , data.at(row+3).at(column+3))
            ) {
                num_xmases += 1;
            }
        }
    }

    std::println("{}", num_xmases);


    return 0;
}
