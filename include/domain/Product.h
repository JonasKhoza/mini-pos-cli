#ifndef Product_H
#define Product_H

#include <string>
#include <chrono>

struct Product
{
        std::string isbn;
        std::string title;
        std::string author;
        int quantity{};
        float price{};
        std::chrono::system_clock::time_point created_at{std::chrono::system_clock::now()};
        std::chrono::system_clock::time_point updated_at{std::chrono::system_clock::now()};
};

#endif