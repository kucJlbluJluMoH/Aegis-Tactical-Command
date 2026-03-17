#pragma once
#include <exception>
#include <string>

class BatteryException : public std::exception {
private:
    std::string message;
public:
    BatteryException(const std::string& droneName, int battery) {
        message = "[BatteryException] " + droneName +
            " cannot act! Battery at " + std::to_string(battery) + "%.";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InsufficientFundsException : public std::exception {
private:
    std::string message;
public:
    InsufficientFundsException(int balance, int cost) {
        message = "[InsufficientFundsException] Not enough coins! "
            "Balance: " + std::to_string(balance) +
            ", Cost: " + std::to_string(cost) + ".";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidDroneIdException : public std::exception {
private:
    std::string message;
public:
    explicit InvalidDroneIdException(int id) {
        message = "[InvalidDroneIdException] No drone found with ID " +
            std::to_string(id) + ".";
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class FileIOException : public std::exception {
private:
    std::string message;
public:
    explicit FileIOException(const std::string& detail) {
        message = "[FileIOException] " + detail;
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};
