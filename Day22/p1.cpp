#include <print>
#include <fstream>

using dtype = long long int;

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

int main(int argc, char** argv) {
    if(argc != 2) {
        std::println("Usage: {} <input file>", argv[0]);
        std::exit(1);
    }

    [[maybe_unused]] constexpr size_t num_reps {2000};
    std::ifstream input_file {argv[1]};

    dtype sum = 0;

    while(input_file.peek() != EOF) {
        std::string input {};
        std::getline(input_file, input);
        auto secret_num = std::stoi(input);
        for(auto i = 0uz; i < num_reps; ++i) {
            secret_num = update_secret(secret_num);
        }
        sum += secret_num;
    }
    std::println("{}", sum);


    return 0;
}

