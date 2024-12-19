#include <print>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <string>

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

bool is_valid(const std::string& input, const std::unordered_set<std::string>& allowed, size_t max_allowed) {

    if(input.size() == 0) return true;
    auto lim = std::min(max_allowed, input.size());
    for(auto i = 0uz; i < lim; ++i) {
        if(
            allowed.contains(input.substr(0, i+1)) &&
            is_valid(input.substr(i+1), allowed, max_allowed)
        )
            return true;
    }

    return false;
}

using dtype = long long int;

dtype cached_num_variations(std::unordered_map<std::string, dtype>& memo, const std::string& input, const std::unordered_set<std::string>& allowed, size_t allowed_size) {
    if(input.size() == 0) return 1;
    if(memo.contains(input)) return memo[input];
    if(!is_valid(input, allowed, allowed_size)) {
        memo[input] = 0;
        return 0;
    }
    auto loop_lim = std::min(allowed_size, input.size());
    dtype sum = 0;
    for(auto i = 0uz; i < loop_lim; ++i) {
        if(allowed.contains(input.substr(0, i+1))) {
            sum += cached_num_variations(memo, input.substr(i+1), allowed, allowed_size);
        }
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
    size_t max_allowed = 0;
    std::unordered_map<std::string, dtype> memo;
    for(auto elem: allowed) {
        if(max_allowed < elem.size()) max_allowed = elem.size();
    }

    dtype num_var = 0;
    while(input_file.peek() != EOF) {
        std::getline(input_file, input);
        dtype var = cached_num_variations(memo, input, allowed, max_allowed);
        // std::println("{} {}", input, var);
        // std::println("----");
        num_var += var;
    }

    std::println("{}", num_var);

    return 0;
}

