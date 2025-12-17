#ifndef Error_H
#define Error_H

#include <exception>
#include <string>
#include <string_view>

class Error : public std::exception
{
    public:
        Error(std::string_view err_msg):m_error_message(err_msg) {}
        const char* what() const noexcept override { return m_error_message.c_str();}
    private:
    std::string m_error_message{};
};

#endif