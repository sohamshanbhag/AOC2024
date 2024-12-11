#include <cmath>
#include <cstdlib>
#include <unordered_map>
#include <print>
#include <fstream>

size_t get_num_digits(long long int num) {
    size_t num_digits = 0;
    while(num >= 1) {
        num /= 10;
        ++num_digits;
    }
    return num_digits;
}

int main(int argc, char** argv) {
    if(argc != 3) {
        std::println("Usage: {} <input file> <number of reps>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    size_t num_reps = atoi(argv[2]);

    std::unordered_map<long long int, long long int> numbers;
    size_t temp_val {};

    while(true) {
        long long int input;
        input_file >> input;
        if(input_file.eof()) break;
        ++numbers[input];
    }

    for(auto i = 0uz; i < num_reps; ++i) {
        std::unordered_map<long long int, long long int> numbers_temp;

        for(const auto& [key, elem]: numbers) {
            if(key == 0) numbers_temp[1] += numbers[key];
            else if((temp_val = get_num_digits(key)) % 2 == 0) {
                auto pow_10 = static_cast<long long int>(std::pow(10, temp_val/2));
                numbers_temp[key / pow_10] += numbers[key];
                numbers_temp[key % pow_10] += numbers[key];
            } else {
                numbers_temp[2024*key] += numbers[key];
            }
        }
        std::swap(numbers, numbers_temp);
    }

    long long int num_elem = 0;
    for(auto elem: numbers) num_elem += elem.second;
    std::println("{}", num_elem);

    return 0;
}

