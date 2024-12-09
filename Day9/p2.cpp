#include <functional>
#include <unistd.h>
#include <print>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <ranges>

int main(int argc, char** argv) {

    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    std::ifstream input_file {argv[1]};

    std::string input;
    std::getline(input_file, input);

    std::vector<std::pair<int, int>> parsed_locs {};

    bool is_block = true;
    long int block_num = 0;
    long int num_char = 0;
    long int num_elem = 0;
    for(const auto& ch: input) {
        size_t reps = ch - '0';
        num_char += reps;
        ++num_elem;
        if(is_block) {
            parsed_locs.emplace_back(reps, block_num);
            ++block_num;
        } else {
            parsed_locs.emplace_back(reps, -1);
        }
        is_block = !is_block;
    }

    std::vector<std::vector<std::reference_wrapper<std::pair<int, int>>>> new_locs(parsed_locs.size());
    for(auto [idx, item]: parsed_locs | std::views::enumerate) {
        new_locs[idx] = {item};
    }

    auto high = parsed_locs.size();
    while(true) {
        --high;
        if(high == 0) break;
        if(parsed_locs[high].second == -1) continue;
        auto num_reps = parsed_locs[high].first;
        size_t low = 0;
        for(low = 0uz; low < high; low++) {
            if (parsed_locs[low].second == -1 && num_reps <= parsed_locs[low].first) break;
        }
        if(low == high) continue;
        new_locs[low].push_back(new_locs[high][0]);
        auto item_size = new_locs[low].size();
        std::swap(new_locs[low][item_size-1], new_locs[low][item_size-2]);
        new_locs[low][item_size-1].get().first -= new_locs[low][item_size-2].get().first;
    }

    std::unordered_set<int> seen_indices = {};
    std::vector<int> final {};
    for(const auto& elem: new_locs) {
        for(const auto& item: elem) {
            auto val = item.get().second;
            if(val != -1 && seen_indices.contains(val)){
                val = -1;
            }
            seen_indices.insert(val);
            for(auto i = 0; i < item.get().first; ++i) {
                final.push_back(val);
            }
        }
    }

    long long int sum = 0;
    for(const auto [idx, val]: final | std::views::enumerate) {
        if(val == -1) continue;
        sum += idx * val;
    }
    std::println("{}", sum);

    return 0;
}

