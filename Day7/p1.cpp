#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <print>
#include <iterator>

using dtype = long unsigned int;

bool calc(dtype res, const std::vector<int>& values, int pos) {
    auto present = values.at(pos);
    if(pos == 0) return (present == static_cast<int>(res));
    if(res % present == 0) {
        if(calc(res / present, values, pos-1)) return true;
    }
    if(calc(res - present, values, pos-1)) return true;
    return false;
}

bool is_valid(dtype result, const std::vector<int>& rhs_values) {
    return calc(result, rhs_values, rhs_values.size()-1);
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};

    dtype sum {};
    while(true) {
        std::string input_line {};
        std::getline(input_file, input_line);
        if(input_file.eof()) break;

        size_t col_pos = input_line.find(':');
        dtype result = std::stoull(input_line.substr(0, col_pos));

        std::string rhs = input_line.substr(col_pos+1);
        std::stringstream iss(rhs);
        std::vector<int> rhs_values (( std::istream_iterator<int>( iss ) ), ( std::istream_iterator<int>() ));
        if(is_valid(result, rhs_values)) sum += result;
    }

    std::println("{}", sum);



    return 0;

}
