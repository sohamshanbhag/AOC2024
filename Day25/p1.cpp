#include <print>
#include <vector>
#include <ranges>
#include <fstream>

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    std::string input {};
    constexpr size_t width = 5;
    constexpr size_t height = 7;

    bool new_elem = true;

    std::vector<std::array<int, 5>> keys {};
    std::vector<std::array<int, 5>> locks {};

    bool present_is_key = false;

    std::array<int, width> present_config {0};

    while(input_file.peek() != EOF) {
        std::getline(input_file, input);
        if(input == "") {
            if(present_is_key) keys.push_back(present_config);
            else locks.push_back(present_config);

            new_elem = true;
            present_config.fill(0);
            continue;
        }
        if(new_elem) {
            if(input.starts_with('#')) {
                present_is_key = false;
            } else {
                present_is_key = true;
            }
            new_elem = false;
            continue;
        }
        if(!present_is_key) {
            for(auto [idx, elem]: input | std::views::enumerate) {
                if(elem == '#') {
                    ++present_config[idx];
                }
            }
        } else {
            for(auto [idx, elem]: input | std::views::enumerate) {
                if(elem == '.') {
                    ++present_config[idx];
                }
            }
        }
    }
    if(present_is_key) keys.push_back(present_config);
    else locks.push_back(present_config);

    for(auto& elem: keys) {
        for(auto& item: elem) item = height - 2 - item;
    }

    size_t num_matches = 0;
    for(const auto& lock: locks) {
        for(const auto& key: keys) {
            bool matches = true;
            for(auto i = 0uz; i < width; ++i) {
                if(lock[i] + key[i] > 5) {
                    matches = false;
                    break;
                }
            }
            num_matches += matches;
        }
    }

    std::println("{}", num_matches);

    return 0;
}

