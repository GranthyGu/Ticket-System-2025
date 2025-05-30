// Created by GranthyGu on 2025/5/18

#ifndef TICKET_MANAGEMENT_HPP
#define TICKET_MANAGEMENT_HPP

#include <iostream>
#include <string>
#include "STL/vector.hpp"
#include "token_scanner.hpp"
#include "account_management.hpp"
#include "train_management.hpp"

class key_for_ticket {
public:
    int time_ = 0;
    train_id id;
    date date_;
    key_for_ticket();
    key_for_ticket(const key_for_ticket&);
    key_for_ticket(const int&, const std::string&, const std::string&);
    key_for_ticket& operator=(const key_for_ticket&);
    bool operator<(const key_for_ticket&) const;
    bool operator>(const key_for_ticket&) const;
    bool operator==(const key_for_ticket&) const;
};

class info_for_ticket {
public:
    date date_;
    char user_name[21] = {0};
    char from[31] = {0};
    char to[31] = {0};
    int num = 0;
    info_for_ticket();
    info_for_ticket(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
    info_for_ticket(const info_for_ticket&);
    info_for_ticket& operator=(const info_for_ticket&);
};

class key_for_ticket_user {
public:
    int time_ = 0;
    char user_name[21] = {0};
    key_for_ticket_user();
    key_for_ticket_user(const int&, const std::string&);
    key_for_ticket_user(const key_for_ticket_user&);
    key_for_ticket_user& operator=(const key_for_ticket_user&);
    bool operator<(const key_for_ticket_user&) const;
    bool operator>(const key_for_ticket_user&) const;
    bool operator==(const key_for_ticket_user&) const;
};

class info_for_ticket_user {
public:
    train_id id;
    date date_;
    char from[31] = {0};
    char to[31] = {0};
    int num = 0;
    int statu = 1;      // 1->success, 2->pending, 3->refunded
    info_for_ticket_user();
    info_for_ticket_user(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);
    info_for_ticket_user(const info_for_ticket_user&);
    info_for_ticket_user& operator=(const info_for_ticket_user&);
};

class ticket_management {
private:
    train_management train_manage;
    account_management account_manage;
    B_plus_tree<key_for_ticket, info_for_ticket, 70, 40> standby_by_train_date;
    B_plus_tree<key_for_ticket_user, info_for_ticket_user, 80, 30> ticket_list_by_user;
    std::pair<date, int> buy(const key_for_ticket_user&, const info_for_ticket_user&);
    std::pair<date, int> query_pending(const key_for_ticket&, const info_for_ticket&);
public:
    ticket_management();
    void buy_ticket(const token_scanner&);
    void query_order(const token_scanner&);
    void refund_ticket(const token_scanner&);
    void exit();
    void clear();
    void add_train_(const token_scanner&);
    void delete_train_(const token_scanner&);
    void release_train_(const token_scanner&);
    void query_train_(const token_scanner&);
    void query_ticket_(const token_scanner&);
    void query_transfer_(const token_scanner&);
    void add_user_(const token_scanner&);
    bool log_in_(const token_scanner&);
    bool log_out_(const token_scanner&);
    void query_profile_(const token_scanner&);
    void modify_profile_(const token_scanner&);
};

#endif      // TICKET_MANAGEMENT_HPP