#include "Bank.h"
#include "Person.h"
#include "Account.h"
#include "Utils.h"
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <format>

Bank::Bank(const std::string& bank_name, const std::string& bank_fingerprint) :
    bank_name(bank_name),
    hashed_bank_fingerprint(Hash(bank_fingerprint)) {

}

Bank::~Bank() {
  for(auto& account : bank_accounts) {
     delete account;
   }
}

Account* Bank::create_account(Person& owner, const std::string& owner_fingerprint, const std::string password) {
  if(owner.get_hashed_fingerprint() != Hash(owner_fingerprint) )
       throw std::invalid_argument("Hashed fingerprint don't match");

  // 没有当前客户
  if(!customer_2_accounts.contains(&owner))
      bank_customers.push_back(&owner);

  // 创建账户
  auto* account = new Account(&owner, this, password);
  bank_accounts.push_back(account);

  account_2_customer[account] = &owner;
  customer_2_accounts[&owner].push_back(account);

  return account;
}

bool Bank::delete_account(Account& account, const std::string& owner_fingerprint) {
  auto owner = account.owner;
  if(owner->get_hashed_fingerprint() != Hash(owner_fingerprint) ) {
        throw std::invalid_argument("Input fingerprint don't match.");
    }

  if(account.get_balance() != 0.0) {
    throw std::invalid_argument("Input fingerprint don't match.");
  }

  std::erase(bank_accounts, &account);
  account_2_customer.erase(&account);
  std::erase(customer_2_accounts[owner], &account);

  return true;
}

bool Bank::delete_customer(Person& owner, const std::string& owner_fingerprint) {
  if(owner.get_hashed_fingerprint() != Hash(owner_fingerprint) || customer_2_unpaid_loan[&owner] != 0.0) {
    throw std::invalid_argument("Input fingerprint don't match.");
  }

  std::erase(bank_customers, &owner);
  for(auto& elem : customer_2_accounts[&owner]) {
    std::erase(bank_accounts, elem);
    account_2_customer.erase(elem);
  }

  customer_2_accounts.erase(&owner);
  customer_2_paid_loan.erase(&owner);
  customer_2_unpaid_loan.erase(&owner);

  return true;
}


bool Bank::deposit(Account& account, const std::string& owner_fingerprint, double amount) {
  auto owner = account.owner;
  if(owner->get_hashed_fingerprint() != Hash(owner_fingerprint) ) {
    throw std::invalid_argument("Input fingerprint don't match.");
  }

  account.balance += amount;
  return true;
}

bool Bank::withdraw(Account& account, const std::string& owner_fingerprint, double amount) {
  auto owner = account.owner;
  if(owner->get_hashed_fingerprint() != Hash(owner_fingerprint) ) {
    throw std::invalid_argument("Input fingerprint don't match.");
  }

  if(account.balance < amount) {
   throw std::invalid_argument("Input fingerprint don't match.");
  }

  account.balance -= amount;
  return true;
}

bool Bank::transfer(Account& source, Account& destination, const std::string& owner_fingerprint,
                    const std::string& CVV2, const std::string& password,
                    const std::string& exp_date, double amount) {
  auto owner = source.owner;
  if(owner->get_hashed_fingerprint() != Hash(owner_fingerprint) ||
     source.CVV2 != CVV2 ||
     source.password != password ||
     source.exp_date != exp_date ||
     source.balance < amount) {
    throw std::invalid_argument("Input fingerprint don't match.");
  }

  source.balance -= amount;
  destination.balance += amount;

  return true;
}

bool Bank::take_loan(Account& account, const std::string& owner_fingerprint, double amount) {
  auto owner = account.owner;
  if(owner->get_hashed_fingerprint() != Hash(owner_fingerprint))
    throw std::invalid_argument("Input fingerprint don't match.");

  size_t rank = owner->get_socioeconomic_rank();
  double total_balance = 0.0;
  for(const auto& elem : customer_2_accounts[owner])
    total_balance += elem->balance;
  double total_loan = static_cast<double>(rank) / 10 * total_balance;

  double current_loan{0.0};
  if (customer_2_unpaid_loan.contains(owner))
    current_loan = customer_2_unpaid_loan[owner];

  if(total_loan < amount + current_loan)
      throw std::invalid_argument("Input fingerprint don't match.");

  if (!customer_2_unpaid_loan.contains(owner)) {
    customer_2_unpaid_loan[owner] = 0.0;
    customer_2_paid_loan[owner] = 0.0;
  }
  double interst = static_cast<double>(amount) / rank / 10;
  customer_2_unpaid_loan[owner] += amount + interst;
  bank_total_loan += amount + interst;
  bank_total_balance += interst;
  return true;
}

bool Bank::pay_loan(Account& account, double amount) {
  auto owner = account.owner;

  customer_2_paid_loan[owner] += amount;
  customer_2_unpaid_loan[owner] -= amount;
  bank_total_loan -= amount;

  size_t rank = owner->get_socioeconomic_rank();
  if(customer_2_paid_loan[owner] > pow(10, rank))
      owner->set_socioeconomic_rank(rank + 1);

  return true;
}

const std::string& Bank::get_bank_name() const {
  return bank_name;
}

size_t Bank::get_hashed_bank_fingerprint() const {
  return hashed_bank_fingerprint;
}

const std::vector<Person*>& Bank::get_bank_customers(std::string& bank_fingerprint) const {
  if(Hash(bank_fingerprint) != hashed_bank_fingerprint)
    throw std::invalid_argument("bank fingerprint don't match.");

  return bank_customers;
}

const std::vector<Account*>& Bank::get_bank_accounts(std::string& bank_fingerprint) const {
  if(Hash(bank_fingerprint) != hashed_bank_fingerprint)
    throw std::invalid_argument("bank fingerprint don't match.");

  return bank_accounts;
}

const std::map<Account*, Person*>& Bank::get_account_2_customer_map(std::string& bank_fingerprint) const {
  if(Hash(bank_fingerprint) != hashed_bank_fingerprint)
    throw std::invalid_argument("bank fingerprint don't match.");

  return account_2_customer;
}

const std::map<Person*, std::vector<Account*>>& Bank::get_customer_2_accounts_map(std::string& bank_fingerprint) const {
  if(Hash(bank_fingerprint) != hashed_bank_fingerprint)
    throw std::invalid_argument("bank fingerprint don't match.");

  return customer_2_accounts;
}

const std::map<Person*, double>& Bank::get_customer_2_paid_loan_map(std::string& bank_fingerprint) const {
  if(Hash(bank_fingerprint) != hashed_bank_fingerprint)
    throw std::invalid_argument("bank fingerprint don't match.");

  return customer_2_paid_loan;
}

const std::map<Person*, double>& Bank::get_customer_2_unpaid_loan_map(std::string& bank_fingerprint) const {
  if(Hash(bank_fingerprint) != hashed_bank_fingerprint)
    throw std::invalid_argument("bank fingerprint don't match.");

  return customer_2_unpaid_loan;
}

double Bank::get_bank_total_balance(std::string& bank_fingerprint) const {
  if(Hash(bank_fingerprint) != hashed_bank_fingerprint)
    throw std::invalid_argument("bank fingerprint don't match.");

  return bank_total_balance;
}

double Bank::get_bank_total_loan(std::string& bank_fingerprint) const {
  if(Hash(bank_fingerprint) != hashed_bank_fingerprint)
    throw std::invalid_argument("bank fingerprint don't match.");

  return bank_total_loan;
}

// 为什么账号的所有人可以修改呢？
// 而且给定参数中new_owner是const Person*类型，并不和原来的Account中的Person*兼容
bool Bank::set_owner(Account& account, Person* new_owner, std::string& owner_fingerprint, std::string& bank_fingerprint) {
  auto owner = account.owner;
  if (hashed_bank_fingerprint != Hash(bank_fingerprint) || owner->get_hashed_fingerprint() != Hash(owner_fingerprint)) {
    throw std::invalid_argument("Input fingerprint don't match.");
  }

  account.owner = new_owner;
  std::erase(customer_2_accounts[owner], &account);

  if (!customer_2_accounts.contains(owner)) {
    bank_customers.push_back(owner);
    customer_2_paid_loan[owner] = 0.0;
    customer_2_unpaid_loan[owner] = 0.0;
  }
  account_2_customer[&account] = new_owner;
  customer_2_accounts[new_owner].push_back(&account);

  return true;
}

bool Bank::set_account_status(Account& account, bool status, std::string& bank_fingerprint) const {
  if (hashed_bank_fingerprint != Hash(bank_fingerprint))
    throw std::invalid_argument("Input fingerprint don't match.");

  account.account_status = status;
  return true;
}

bool Bank::set_exp_date(Account& account, std::string& exp_date, std::string& bank_fingerprint) const {
  if (hashed_bank_fingerprint != Hash(bank_fingerprint))
    throw std::invalid_argument("Input fingerprint don't match.");

  account.exp_date = exp_date;
  return true;
}

void Bank::get_info(std::optional<std::string> file_name) const {
  if (file_name.has_value()) {
    std::ofstream os(file_name.value());

    os << std::format("bank name: {} | ", bank_name)
       << std::format("bank total balance: {} | ", bank_total_balance)
       << std::format("bank total loan: {} | ", bank_total_loan) << std::endl;

    for (const auto&[fst, snd] : customer_2_accounts) {
      fst->get_info(file_name);
      for (const auto& account : snd) {
        account->get_info(file_name);
      }
      os << std::format("paid loan: {} |", customer_2_paid_loan.at(fst))
         << std::format("unpaid loan : {} ", customer_2_unpaid_loan.at(fst)) << std::endl;
    }
  }
  else {
    std::cout << std::format("bank name: {} | ", bank_name)
       << std::format("bank total balance: {} | ", bank_total_balance)
       << std::format("bank total loan: {} | ", bank_total_loan) << std::endl;

    for (const auto&[fst, snd] : customer_2_accounts) {
      fst->get_info(file_name);
      for (const auto& account : snd) {
        account->get_info(file_name);
      }
      std::cout << std::format("paid loan: {} |", customer_2_paid_loan.at(fst))
         << std::format("unpaid loan : {} ", customer_2_unpaid_loan.at(fst)) << std::endl;
    }
  }
}