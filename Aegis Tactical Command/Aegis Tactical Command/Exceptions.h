#pragma once
#include <exception>
#include <string>
using namespace std;

class BatteryException : public exception {
private:
    string message;
public:
    BatteryException(const string& droneName, int battery) {
        message = "[BatteryException] " + droneName +
            " cannot act! Battery at " + to_string(battery) + "%.";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InsufficientFundsException : public exception {
private:
    string message;
public:
    InsufficientFundsException(int balance, int cost) {
        message = "[InsufficientFundsException] Not enough coins! "
            "Balance: " + to_string(balance) +
            ", Cost: " + to_string(cost) + ".";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};