#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <ranges>

int main() {
    std::ifstream input ("input.txt", std::ios::in);
    std::vector<int> first = {};
    std::vector<int> second = {};

    for (size_t i = 0;; ++i){
        int val;
        input >> val;
        if (input.eof()) break;
        if (i % 2) second.push_back(val);
        else first.push_back(val);
    }

    auto dist = [](const int first, const int second) { return std::abs(first - second); };

    std::ranges::sort(first);
    std::ranges::sort(second);

    std::cout << std::ranges::fold_left(
        std::views::zip_transform(dist, first, second),
        0, std::plus<>()) << '\n';


    return 0;
}
