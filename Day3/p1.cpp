#include <fstream>
#include <print>
#include <string>

size_t update_match_pos(char present, const std::string_view match, size_t match_pos) {
    if(match[match_pos] == '0') {
        if (std::isdigit(present)) return match_pos;
        else if(present == match[match_pos+1]) return match_pos+2;
    }
    if(match[match_pos] == present) return match_pos+1;

    return 0;
}

int main() {

    // auto input_file = std::ifstream("example.txt");
    auto input_file = std::ifstream("input.txt");

    std::string match {"mul(0,0)"};
    size_t match_pos = 0;
    std::string temp {};
    long long int sum = 0;

    while(true) {
        char c;
        input_file.get(c);
        if(input_file.eof()) break;
        match_pos = update_match_pos(c, match, match_pos);
        if(match_pos == 0 && !temp.empty()) temp.clear();
        if(match_pos != 0) temp.push_back(c);
        if(match_pos == match.size()) {
            auto comma_pos = temp.find(',');
            int first = std::stoi(temp.substr(4, comma_pos-4));
            int second = std::stoi(temp.substr(comma_pos+1, temp.size() - comma_pos-1));
            sum += first * second;

            match_pos = 0;
            temp.clear();
        }


    }
    std::println("{}", sum);


    return 0;
}
