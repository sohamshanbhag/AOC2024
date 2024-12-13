#include <regex>
#include <cstdio>
#include <print>
#include <fstream>

std::pair<int, int> get_inputs(std::string& input, std::regex& reg) {
    auto words_begin = std::sregex_iterator(input.begin(), input.end(), reg);
    auto words_end = std::sregex_iterator();
    std::smatch m = *words_begin++;
    int a1 = std::stoi(m.str());
    m = *words_begin;
    int a2 = std::stoi(m.str());

    return {a1, a2};
}

long long int get_cost(int a1, int a2, int a3, int a4, int b1, int b2) {
    // [a1 a3] [x]   [b1]
    // [a2 a4] [y] = [b2]
    //     1     [a4 -a3] [b1]   [x]
    // (ad - bc) [-a2 a1] [b2] = [y]
    //     1     [a4 * b1 - a3 * b2 ]   [x]
    // (ad - bc) [-a2 * b1 + a1 * b2] = [y]
    //

    int calc1 = a4 * b1 - a3 * b2;
    int calc2 = -a2 * b1 + a1 * b2;
    int denom = a1 * a4 - a2 * a3;

    if(calc1 % denom == 0 && calc2 % denom == 0) {
        return 3 * calc1 / denom + calc2 / denom;
    }
    return 0;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    std::regex entry_regex("(\\d+)");
    std::smatch re_match;
    // std::regex result_regex("Prize: X=(\\d+), Y=(\\d+)");

    long long int cost = 0;
    while(true) {
        std::string input_line1, input_line2, input_line3, newline;
        std::getline(input_file, input_line1);
        std::getline(input_file, input_line2);
        std::getline(input_file, input_line3);
        std::getline(input_file, newline);

        auto [a1, a2] = get_inputs(input_line1, entry_regex);
        auto [a3, a4] = get_inputs(input_line2, entry_regex);
        auto [b1, b2] = get_inputs(input_line3, entry_regex);

        cost += get_cost(a1, a2, a3, a4, b1, b2);

        if(input_file.eof()) break;
    }

    std::println("{}", cost);
    return 0;
}

