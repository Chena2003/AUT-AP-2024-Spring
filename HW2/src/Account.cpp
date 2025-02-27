#include "Account.h"
#include "Bank.h"
#include "Person.h"
#include "Utils.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <format>

Account::Account(Person *const owner, const Bank *const bank, const std::string &password) :
    owner(owner),
    bank(bank),
    account_number(gen_account_number()),
    balance(0),
    account_status(true),
    CVV2(gen_CVV2()),
    password(password),
    exp_date("30-02") {

}

const Person* Account::get_owner() const {
    return owner;
}
double Account::get_balance() const {
    return balance;
}

std::string Account::get_account_number() const {
    return account_number;
}

bool Account::get_status() const {
    return account_status;
}

std::string Account::get_CVV2(const std::string& owner_fingerprint) const {
    if (owner->get_hashed_fingerprint() != Hash(owner_fingerprint))
        throw std::invalid_argument("Input fingerprint doesn't match owner fingerprint");

    return CVV2;
}

std::string Account::get_password(const std::string& owner_fingerprint) const {
    if (owner->get_hashed_fingerprint() != Hash(owner_fingerprint))
        throw std::invalid_argument("Input fingerprint doesn't match owner fingerprint");

    return password;
}

std::string Account::get_exp_date(const std::string& owner_fingerprint) const {
    if (owner->get_hashed_fingerprint() != Hash(owner_fingerprint))
        throw std::invalid_argument("Input fingerprint doesn't match owner fingerprint");

    return exp_date;
}

bool Account::set_password(const std::string &password, const std::string &owner_fingerprint) {
    if (owner->get_hashed_fingerprint() != Hash(owner_fingerprint))
        throw std::invalid_argument("Input fingerprint doesn't match owner fingerprint");

    this->password = password;
    return true;
}

std::strong_ordering Account::operator<=>(const Account& other) const {
    return account_number <=> other.account_number;
}

void Account::get_info(std::optional<std::string> file_name) const {
    if (file_name.has_value()) {
        // owner->get_info(file_name);
//        bank->get_info(file_name);
        std::ofstream os(file_name.value());
        os << std::format("account number: {} | ", account_number)
           << std::format("balance: {} | ", balance)
           << std::format("exp_date: {} | ", exp_date)
           << std::format("account_status: {}", account_status) << std::endl;
    } else {
        // owner->get_info(std::nullopt);
//        bank->get_info(std::nullopt);
        std::cout << std::format("account number: {} | ", account_number)
           << std::format("balance: {} | ", balance)
           << std::format("exp_date: {} | ", exp_date)
           << std::format("account_status: {}", account_status) << std::endl;
    }
}

std::string Account::gen_account_number() {
    std::stringstream ss("");

    for (size_t i = 0; i < 16; ++i)
        ss << Rand(0, 9);

    return ss.str();
}
std::string Account::gen_CVV2() {
    std::stringstream ss("");

    ss << Rand(0, 9) << Rand(0, 9) << Rand(0, 9) << Rand(0, 9);

    return ss.str();
}
