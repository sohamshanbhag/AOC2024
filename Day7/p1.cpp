#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <print>
#include <iterator>

using dtype = long long unsigned int;
// using dtype = unsigned __int128;

std::unordered_map<std::string, dtype> calc(const std::span<int> values, size_t pos, const std::unordered_map<std::string, dtype> memo) {
    std::unordered_map<std::string, dtype> output = {};
    for(auto [key, val]: memo) {
        output[key + '+'] = val + values[pos];
        output[key + '*'] = val * values[pos];
    }

    return output;
}

bool is_valid(dtype result, const std::span<int> rhs_values) {
    std::unordered_map<std::string, dtype> memo {{"", rhs_values[0]}};
    for(auto i = 1uz; i < rhs_values.size(); ++i) {
        memo = calc(rhs_values, i, memo);
    }
    for(auto [key, val]: memo) if(val == result) return true;

    return false;
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
        std::println("{}", input_line);

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
