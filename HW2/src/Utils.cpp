#include "Utils.h"

#include <random>
#include <functional>
#include <iostream>
#include <fstream>
#include <optional>

size_t Rand(size_t min, size_t max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(min, max);

    return dist(gen);
};

size_t Hash(const std::string& str) {
    return std::hash<std::string>{}(str);
};
