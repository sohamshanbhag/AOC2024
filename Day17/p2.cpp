#include <cmath>
#include <print>
#include <ranges>
#include <regex>
#include <cassert>
#include <fstream>

using dtype = long long int;

struct Registers {
    dtype A{}, B{}, C {};
};

dtype get_inputs(const std::string& input, const std::regex& reg) {
    auto words_begin = std::sregex_iterator(input.begin(), input.end(), reg);
    auto words_end = std::sregex_iterator();
    std::smatch m = *words_begin;
    dtype val = std::stoi(m.str());
    return val;
}

int get_literal_operand(size_t a) {
    return a;
}

dtype get_combo_operand(size_t a, const Registers& reg) {
    if(a <= 3) return a;
    else if(a == 4) return reg.A;
    else if(a == 5) return reg.B;
    else if(a == 6) return reg.C;
    else {
        assert("UNREACHABLE" && false);
    }
    return -1;
}

void process_operand(int opcode, int operand, Registers& reg, std::vector<int>& output) {
    switch(opcode) {
        case 0:
            reg.A = reg.A / std::pow(2, get_combo_operand(operand, reg));
            return;
        case 1:
            reg.B = reg.B ^ get_literal_operand(operand);
            return;
        case 2:
            reg.B = get_combo_operand(operand, reg) % 8;
            return;
        case 4:
            reg.B = reg.B ^ reg.C;
            return;
        case 5:
            output.push_back(get_combo_operand(operand, reg) % 8);
            return;
        case 6:
            reg.B = reg.A / std::pow(2, get_combo_operand(operand, reg));
            return;
        case 7:
            reg.C = reg.A / std::pow(2, get_combo_operand(operand, reg));
            return;
        case 3:
        default:
            assert("NOT IMPLEMENTED" && false);
            return;
    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};
    std::regex entry_regex("(\\d+)");

    Registers reg = {};

    std::string input {};
    std::getline(input_file, input);
    reg.A = get_inputs(input, entry_regex);
    std::getline(input_file, input);
    reg.B = get_inputs(input, entry_regex);
    std::getline(input_file, input);
    reg.C = get_inputs(input, entry_regex);
    input_file.seekg(10, std::ios_base::cur);

    std::vector<std::pair<int, int>> ops;
    std::vector<int> required {};
    while(input_file.peek() != EOF) {
        char opcode_ch {};
        char operand_ch {};
        input_file.get(opcode_ch);
        input_file.seekg(1, std::ios_base::cur);
        input_file.get(operand_ch);
        input_file.seekg(1, std::ios_base::cur);
        int opcode = opcode_ch - '0';
        int operand = operand_ch - '0';
        ops.emplace_back(opcode, operand);
        required.emplace_back(opcode);
        required.emplace_back(operand);
    }

    auto reg_bak = reg;

    int match_id = required.size()-1;
    dtype aval = 0;
    while(true) {
        reg = {aval, reg_bak.B, reg_bak.C};
        auto instruction_pointer = 0uz;
        std::vector<int> generated = {};
        while(instruction_pointer != ops.size()) {
            auto [opcode, operand] = ops[instruction_pointer];
            if(opcode == 3) {
                if(reg.A != 0) instruction_pointer = get_literal_operand(operand);
                else ++instruction_pointer;
            } else {
                process_operand(opcode, operand, reg, generated);
                ++instruction_pointer;
            }
            if(opcode == 5 && generated.size() > required.size() - match_id) break;
        }
        if(match_id == 0 && std::ranges::equal(generated, required)) break;
        if(std::ranges::equal(generated, std::ranges::views::drop(required, match_id))) {
            aval = aval*8;
            match_id -= 1;
        }
        else ++aval;
    }
    std::println("{}", aval);

    return 0;
}

