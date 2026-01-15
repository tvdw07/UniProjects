#include <data_structures/hashtable.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <random>
#include <cstdint>

using namespace data_structures;

// --- Hashfunktionen h0 - h4 ---
uint64_t h0(const std::string &s) {
    return std::hash<std::string>{}(s);
}

uint64_t h1(const std::string &s) {
    return s.length();
}

uint64_t h2(const std::string &s) {
    return s.empty() ? 0 : static_cast<uint64_t>(s[0]);
}

uint64_t h3(const std::string &s) {
    if (s.empty()) return 0;
    uint64_t sum = 0;
    for (const char c : s) sum += static_cast<uint64_t>(c);
    return sum / s.length();
}

uint64_t h4(const std::string &s) {
    uint64_t sum = 0;
    for (const char c : s) sum += static_cast<uint64_t>(c);
    return sum;
}

// Generiert count zufällige Strings mit Längen zwischen min_len und max_len (ki generiert)
std::vector<std::string> generate_test_strings(const size_t count, const size_t min_len=3, const size_t max_len=8) {
    std::vector<std::string> strings;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> len_dist(min_len, max_len);
    std::uniform_int_distribution<char> char_dist('a', 'z');

    for (size_t i = 0; i < count; ++i) {
        const size_t len = len_dist(rng);
        std::string s;
        for (size_t j = 0; j < len; ++j)
            s += char_dist(rng);
        strings.push_back(s);
    }
    return strings;
}


void test_hash_function(const std::string &name, std::function<size_t(const std::string&)> hashfkt,
                        const std::vector<std::string> &test_strings) {
    std::cout << "=== Test " << name << " ===\n";
    Hashtable table(hashfkt);

    for (const auto &s : test_strings) {
        table.insert(s);
    }

    std::cout << "Anzahl Elemente: " << table.size() << "\n";
    std::cout << "Max bucket size: " << table.max_bucket_size() << "\n\n";
}

int main() {
    // Testmenge generieren
    const auto test_strings = generate_test_strings(1000); // 1000 zufällige Strings

    // Teste alle Hashfunktionen
    test_hash_function("h0 (std::hash)", h0, test_strings);
    test_hash_function("h1 (length)", h1, test_strings);
    test_hash_function("h2 (first char)", h2, test_strings);
    test_hash_function("h3 (average char)", h3, test_strings);
    test_hash_function("h4 (sum of chars)", h4, test_strings);

    return 0;
}
