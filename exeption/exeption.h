#include <exception>
#include <string>
#include <sstream>
#include <iostream>

class TMyExeption : public std::exception {
public:
    TMyExeption(const std::string& initialMessage = "") : message(initialMessage) {}

    template <typename T>
    TMyExeption& operator<<(const T& value) {
        std::ostringstream oss;
        oss << value;
        message += oss.str();
        return *this;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

class ExeptionA : public TMyExeption {
public:
    ExeptionA() : TMyExeption("ExeptionA: ") {}
};

class ExeptionB : public TMyExeption{
public:
    ExeptionB() : TMyExeption("ExeptionB: ") {}
};
