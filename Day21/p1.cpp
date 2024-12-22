#include <print>
#include <ranges>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <utility>

std::pair<int, int> get_main_quot_rem(char a) {// {{{
    if(a == '0') return {0, 1};
    if(a == 'A') return {0, 2};
    int present = a - '0' + 2;
    return {present / 3, present % 3};
}
// }}}

std::vector<std::string> get_possible_main_keypad_paths(char present, char next) {// {{{
    std::vector<std::string> res {};

    auto [present_quot, present_rem] = get_main_quot_rem(present);
    auto [next_quot, next_rem] = get_main_quot_rem(next);

    std::string temp_str {};
    if(next_rem > present_rem)        temp_str.append(next_rem - present_rem  , '>');
    else if(next_rem < present_rem)   temp_str.append(present_rem - next_rem  , '<');
    if(next_quot > present_quot)      temp_str.append(next_quot - present_quot, '^');
    else if(next_quot < present_quot) temp_str.append(present_quot - next_quot, 'v');
    temp_str.append(1, 'A');
    res.push_back(temp_str);
    temp_str = {};
    if(next_quot > present_quot)      temp_str.append(next_quot - present_quot, '^');
    else if(next_quot < present_quot) temp_str.append(present_quot - next_quot, 'v');
    if(next_rem > present_rem)        temp_str.append(next_rem - present_rem  , '>');
    else if(next_rem < present_rem)   temp_str.append(present_rem - next_rem  , '<');
    temp_str.append(1, 'A');
    res.push_back(temp_str);

    if(present_quot == 0 && next_rem == 0) {
        res.erase(res.begin());
    } else if(present_rem == 0 && next_quot == 0) {
        res.erase(res.begin() + 1);
    }

    return res;
}
// }}}

std::pair<int, int> get_other_quot_rem(char a) {// {{{
    switch(a) {
        case '<': return {0, 0};
        case 'v': return {0, 1};
        case '>': return {0, 2};
        case '^': return {1, 1};
        case 'A': return {1, 2};
        default:
            std::println("UNREACHABLE: {}", a);
        assert(0 && "UNREACHABLE");
    }
}
// }}}

std::vector<std::string> get_possible_other_keypad_paths(char present, char next) {// {{{
    // std::vector<std::string> res {};

    // auto [present_quot, present_rem] = get_other_quot_rem(present);
    // auto [next_quot, next_rem] = get_other_quot_rem(next);



    if(present == '>' && next == '>') return {"A"};
    else if(present == '<' && next == '<') return {"A"};
    else if(present == 'v' && next == 'v') return {"A"};
    else if(present == '^' && next == '^') return {"A"};
    else if(present == 'A' && next == 'A') return {"A"};
    else if(present == 'A' && next == '<') return {"v<<A"};
    else if(present == '^' && next == '<') return {"v<A"};
    else if(present == '>' && next == '<') return {"<<A"};
    else if(present == 'v' && next == '<') return {"<A"};
    else if(present == '<' && next == 'A') return {">>^A"};
    else if(present == '<' && next == '^') return {">^A"};
    else if(present == '<' && next == '>') return {">>A"};
    else if(present == '<' && next == 'v') return {">A"};
    else if(present == '>' && next == 'A') return {"^A"};
    else if(present == '>' && next == 'v') return {"<A"};
    else if(present == 'v' && next == '>') return {">A"};
    else if(present == 'v' && next == '^') return {"^A"};
    else if(present == '^' && next == 'v') return {"vA"};
    else if(present == '^' && next == 'A') return {">A"};
    else if(present == 'A' && next == '^') return {"<A"};
    else if(present == 'A' && next == '>') return {"vA"};
    else if(present == 'A' && next == 'v') return {"<vA", "v<A"};
    else if(present == '>' && next == '^') return {"<^A", "^<A"};
    else if(present == 'v' && next == 'A') return {"^>A", ">^A"};
    else if(present == '^' && next == '>') return {">vA", "v>A"};

    return {};
}
// }}}

char get_other(std::pair<int, int> pos) {// {{{
    switch(pos.first) {
        case 0: switch(pos.second) {
            case 0: return '<';
            case 1: return 'v';
            case 2: return '>';
            default: assert(0 && "UNREACHABLE");
        }
        case 1: switch(pos.second) {
            case 1: return '^';
            case 2: return 'A';
            default: assert(0 && "UNREACHABLE");
        }
        default: assert(0 && "UNREACHABLE");
    }
}
// }}}

std::string generate_other_str(const std::string& order) {// {{{
    std::pair<int, int> pos = {1, 2};
    std::string res {};
    for(auto ch: order) {
        switch(ch) {
            case '^': { pos.first = pos.first + 1; continue; }
            case '>': { pos.second = pos.second + 1; continue; }
            case 'v': { pos.first = pos.first - 1; continue; }
            case '<': { pos.second = pos.second - 1; continue; }
            case 'A': { res.push_back(get_other(pos)); continue; }
            default: assert(0 && "UNREACHABLE");
        }
    }

    return res;
}
// }}}

std::vector<std::string> get_all_paths(const std::string& input) {// {{{

    auto res = get_possible_other_keypad_paths('A', input[0]);
    for(size_t i = 1; i < input.size(); ++i) {
        std::vector<std::string> temp_res {};
        auto paths = get_possible_other_keypad_paths(input[i-1], input[i]);
        for(auto elem: res) {
            for(auto path: paths) {
                std::string temp_str = elem;
                temp_str.append(path);
                temp_res.push_back(temp_str);
            }
        }
        std::swap(temp_res, res);
    }
    auto lengths = std::ranges::min(std::ranges::views::transform(res, [](auto i){return i.size();}));
    std::erase_if(res, [lengths](auto i) {return i.size() > lengths;});
    return res;
}
// }}}

std::unordered_map<std::string, int> get_lengths(size_t reps) {
    std::vector possible_chars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A'};
    std::unordered_map<std::string, int> lengths = {};
    for(char a: possible_chars) {
        for(char b: possible_chars) {
            std::string temp_str {};
            temp_str.push_back(a);
            temp_str.push_back(b);
            if(a == b) {
                lengths[temp_str] = 1;
                continue;
            }
            auto paths = get_possible_main_keypad_paths(a, b);

            for(auto i = 0uz; i < reps; ++i) {
                std::vector<std::string> temp_paths {};
                for(auto elem: paths) {
                    auto all_paths = get_all_paths(elem);
                    for(auto path: all_paths) temp_paths.push_back(path);
                }
                std::swap(paths, temp_paths);
                auto min_len = std::ranges::min(std::ranges::views::transform(paths, [](auto i){return i.size();}));
                std::erase_if(paths, [min_len](auto i){return i.size() > min_len;});
            }

            lengths[temp_str] = std::ranges::min(std::ranges::views::transform(paths, [](auto i){return i.size();}));
        }
    }

    return lengths;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};

    auto lengths = get_lengths(2);
    long long int complexity = 0;

    while(input_file.peek() != EOF) {
        std::string input {};
        std::getline(input_file, input);

        long long int length = 0;
        std::string temp_str = "A";
        temp_str.push_back(input[0]);
        length += lengths[temp_str];
        for(auto i = 1uz; i < input.size(); ++i) {
            temp_str = input[i-1];
            temp_str.push_back(input[i]);
            length += lengths[temp_str];
        }
        complexity += length * std::stoi(input.substr(0, input.size()-1));

        // res.append(get_main_keypad_next('A', input[0]));
        // for(auto i = 1uz; i < input.size(); ++i) {
        //     res.append(get_main_keypad_next(input[i-1], input[i]));
        // }
        // // std::println("{}", res);
        // std::string res2 {};
        // res2.append(get_other_keypad_next('A', res[0]));
        // for(auto i = 1uz; i < res.size(); ++i) {
        //     res2.append(get_other_keypad_next(res[i-1], res[i]));
        // }
        // // std::println("{}", res2);
        // std::string res3 {};
        // res3.append(get_other_keypad_next('A', res2[0]));
        // for(auto i = 1uz; i < res2.size(); ++i) {
        //     res3.append(get_other_keypad_next(res2[i-1], res2[i]));
        // }
        // // std::println("{}", res3);
        // complexity += res3.size() * std::stoi(input.substr(0, input.size()-1));
    }
    std::println("{}", complexity);



    return 0;
}

