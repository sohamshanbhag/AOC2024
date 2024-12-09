#include <unistd.h>
#include <print>
#include <fstream>
#include <string>
#include <vector>
#include <ranges>

int main(int argc, char** argv) {

    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};

    std::string input;
    std::getline(input_file, input);

    std::vector<int> parsed_input {};

    bool is_block = true;
    long int block_num = 0;
    for(const auto& ch: input) {
        size_t reps = ch - '0';
        if(is_block) {
            for(auto i = 0uz; i < reps; ++i) parsed_input.push_back(block_num);
            ++block_num;
        } else {
            for(auto i = 0uz; i < reps; ++i) parsed_input.push_back(-1);
        }
        is_block = !is_block;
    }

    for(auto low = 0uz, high = parsed_input.size()-1; low < high; ++low) {
        if(parsed_input[low] == -1) {
            std::swap(parsed_input[high], parsed_input[low]);
            while(parsed_input[high] == -1) --high;
        }
    }

    long long int sum2 = 0;
    for(const auto [idx, val]: parsed_input | std::views::enumerate) {
        if(val == -1) break;
        sum2 += idx * val;
    }
    std::println("{}", sum2);

    return 0;
}

