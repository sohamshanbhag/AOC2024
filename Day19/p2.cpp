#include <print>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <string_view>

std::unordered_set<std::string> get_inputs(const std::string& input, const std::regex& reg) {
    auto words_begin = std::sregex_iterator(input.begin(), input.end(), reg);
    auto words_end = std::sregex_iterator();
    std::unordered_set<std::string> allowed = {};
    for(auto word = words_begin; word != words_end; ++word){
        std::smatch m = *word;
        allowed.insert(m.str());
    }
    return allowed;
}

bool is_valid(const std::string_view input, const std::unordered_set<std::string>& allowed) {
    if(input.size() == 0) return true;
    for(auto elem: allowed) {
        if(
            input.substr(0, elem.size()) == elem
            && is_valid(input.substr(elem.size()), allowed)
        )
            return true;
    }

    return false;
}

using dtype = long long int;

dtype cached_num_variations(std::unordered_map<std::string, dtype>& memo, const std::string& input, const std::unordered_set<std::string>& allowed) {
    if(input.size() == 0) return 1;
    if(memo.contains(input)) return memo[input];
    if(!is_valid(input, allowed)) {
        memo[input] = 0;
        return 0;
    }
    dtype sum = 0;
    for(auto elem: allowed) {
        if(elem.size() > input.size()) continue;
        dtype val = (input.substr(0, elem.size()) == elem) * cached_num_variations(memo, input.substr(elem.size()), allowed);
        sum += val;
    }
    memo[input] = sum;

    return sum;
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
    std::unordered_set<std::string> allowed = get_inputs(input, entry_regex);
    // for(auto elem: allowed) std::println("{}", elem);
    std::getline(input_file, input);

    std::unordered_map<std::string, dtype> memo;
    dtype num_var = 0;
    while(input_file.peek() != EOF) {
        std::getline(input_file, input);
        dtype var = cached_num_variations(memo, input, allowed);
        // std::println("{} {}", input, var);
        // std::println("----");
        num_var += var;
    }

    std::println("{}", num_var);

    return 0;
}

