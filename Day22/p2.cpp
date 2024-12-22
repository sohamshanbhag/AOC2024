#include <print>
#include <ranges>
#include <algorithm>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <tuple>

#include "hash.hpp"

using dtype = long long int;
using group_type = std::tuple<int, int, int, int>;

struct hash_tup {
    size_t operator()(const group_type& item) const {
        return (std::get<0>(item) + 9) + (std::get<1>(item) + 9) * 19 + (std::get<2>(item) + 9) * 19*19 + (std::get<3>(item) + 9) * 19*19*19;
    }
};

dtype mix(dtype a, dtype secret) {
    return a ^ secret;
}

dtype prune(dtype a) {
    return a % 16777216;
}

dtype update_secret(dtype secret) {
    secret = prune(mix(secret * 64, secret));
    secret = prune(mix(secret / 32, secret));
    secret = prune(mix(secret * 2048, secret));
    return secret;
}

group_type get_diff(const group_type& old_diff, int new_elem) {
    return {std::get<1>(old_diff), std::get<2>(old_diff), std::get<3>(old_diff), new_elem};
}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    constexpr size_t num_reps {2000};
    std::ifstream input_file {argv[1]};

    using list_type = std::unordered_map<group_type, dtype, hash_tup>;
    std::vector<list_type> difflist {};

    while(input_file.peek() != EOF) {
        std::string input {};
        std::getline(input_file, input);
        auto secret_num = std::stoll(input);

        list_type temp_difflist;
        group_type old_diff = {0, 0, 0, 0};
        int previous_price = secret_num % 10;

        for(auto i = 0uz; i < num_reps; ++i) {
            secret_num = update_secret(secret_num);
            int price = secret_num % 10;
            auto diff = price - previous_price;
            previous_price = price;
            group_type new_diff = get_diff(old_diff, diff);
            if(i > 2 && !temp_difflist.contains(new_diff)) {
                temp_difflist[new_diff] = price;
            }
            old_diff = new_diff;
        }
        difflist.push_back(temp_difflist);
    }


    list_type master_difflist;
    for(auto list: difflist) {
        for(auto [elem, val]: list) {
            master_difflist[elem] += val;
        }
    }

    auto num_bananas = std::ranges::max(std::ranges::views::values(master_difflist));
    auto temp_list = std::ranges::filter_view(master_difflist, [num_bananas](auto i){ return i.second == num_bananas; });
    std::println("{}", num_bananas);

    return 0;
}

