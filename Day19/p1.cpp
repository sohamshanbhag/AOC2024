#include <print>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <string_view>

std::vector<std::string> get_inputs(const std::string& input, const std::regex& reg) {
    auto words_begin = std::sregex_iterator(input.begin(), input.end(), reg);
    auto words_end = std::sregex_iterator();
    std::vector<std::string> allowed = {};
    for(auto word = words_begin; word != words_end; ++word){
        std::smatch m = *word;
        allowed.push_back(m.str());
    }
    return allowed;
}

bool is_valid(const std::string_view input, const std::vector<std::string>& allowed) {
    for(auto elem: allowed) {
        if(input.size() == 0) return true;
        if(
            input.substr(0, elem.size()) == elem
            && is_valid(input.substr(elem.size()), allowed)
        )
            return true;
    }

    return false;
}


int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    std::regex entry_regex("([a-z]+)");
    std::string input {};
    std::getline(input_file, input);
    std::vector allowed = get_inputs(input, entry_regex);
    // for(auto elem: allowed) std::println("{}", elem);
    std::getline(input_file, input);

    size_t num_valid = 0;
    while(input_file.peek() != EOF) {
        std::getline(input_file, input);
        num_valid += is_valid(input, allowed);
    }

    std::println("{}", num_valid);

    return 0;
}

