#include <fstream>
#include <print>
#include <vector>

bool check_match(char lt, char rt, char lb, char rb) {
    return ((lt == 'M' && rb == 'S') || (lt == 'S' && rb == 'M'))
        && ((lb == 'M' && rt == 'S') || (lb == 'S' && rt == 'M'));
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

    size_t num_x_mases = 0;

    for (size_t row = 1; row < height-1; ++row) {
        for (size_t column = 1; column < width-1; ++column) {
            if(data.at(row).at(column) != 'A') continue;
            if(check_match(
                data.at(row-1).at(column-1),
                data.at(row-1).at(column+1),
                data.at(row+1).at(column-1),
                data.at(row+1).at(column+1)
            )) {
                num_x_mases += 1;
            }
        }
    }

    std::println("{}", num_x_mases);


    return 0;
}
