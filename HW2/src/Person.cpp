#include "Person.h"
#include "Utils.h"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <format>

Person::Person(const std::string &name, size_t age, const std::string &gender, const std::string &fingerprint, size_t socioeconomic_rank, bool is_alive) :
    name(name),
    age(age),
    gender(gender),
    hashed_fingerprint(Hash(fingerprint)),
    socioeconomic_rank(socioeconomic_rank),
    is_alive(is_alive) {
    if (gender != "Male" && gender != "Female")
        throw std::invalid_argument("invalid gender");

    if (socioeconomic_rank == 0 || socioeconomic_rank > 10)
        throw std::invalid_argument("invalid socioeconomic_rank");
}

std::string Person::get_name() const {
    return name;
}

size_t Person::get_age() const {
    return age;
}

std::string Person::get_gender() const {
    return gender;
}

size_t Person::get_hashed_fingerprint() const {
    return hashed_fingerprint;
}

size_t Person::get_socioeconomic_rank() const {
    return socioeconomic_rank;
}
bool Person::get_is_alive() const {
    return is_alive;
}

bool Person::set_age(size_t age) {
    if (age == 0 || age > 200)
        return false;
        // throw std::invalid_argument("invalid age");

    this->age = age;
    return true;
}

bool Person::set_socioeconomic_rank(size_t rank) {
    if (rank == 0 || rank > 10)
        // return false;
        throw std::invalid_argument("invalid rank");
    this->socioeconomic_rank = rank;
    return true;
}

bool Person::set_is_alive(bool is_alive) {
    this->is_alive = is_alive;
    return true;
}

std::strong_ordering Person::operator<=>(const Person& other) const {
    return hashed_fingerprint <=> other.hashed_fingerprint;
}

void Person::get_info(std::optional<std::string> file_name) const {
    if (file_name.has_value()) {
        std::ofstream os(file_name.value());
        os << std::format("name : {:<7} | ", name)
           << std::format("age : {:<3} | ", age)
           << std::format("gender : {:<6} | ", gender)
           << std::format("hashed_fingerprint : {:<10} | ", hashed_fingerprint)
           << std::format("socioeconomic_rank : {:<2} | ", socioeconomic_rank)
           << std::format("is_alive : {} ", is_alive) << std::endl;
    } else {
        std::cout << std::format("name : {:<7} | ", name)
           << std::format("age : {:<3} | ", age)
           << std::format("gender : {:<6} | ", gender)
           << std::format("hashed_fingerprint : {:<10} | ", hashed_fingerprint)
           << std::format("socioeconomic_rank : {:<2} | ", socioeconomic_rank)
           << std::format("is_alive : {} ", is_alive) << std::endl;
    }
}