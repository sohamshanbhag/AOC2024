#include <fstream>
#include <string>
#include <print>

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
    std::string do_match {"do()"};
    std::string dont_match {"don't()"};
    size_t match_pos = 0;
    size_t do_match_pos = 0;
    size_t dont_match_pos = 0;
    std::string temp {};
    long long int sum = 0;
    bool domul = true;

    while(true) {
        char c;
        input_file.get(c);
        if(input_file.eof()) break;

        if(domul){
            dont_match_pos = update_match_pos(c, dont_match, dont_match_pos);
            if(dont_match_pos == dont_match.size()) {
                domul = false;
            }
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
        } else {
            do_match_pos = update_match_pos(c, do_match, do_match_pos);
            if(do_match_pos == do_match.size()) {
                domul = true;
            }
        }


    }
    std::println("{}", sum);


    return 0;
}
