#ifndef SaleLine_H
#define SaleLine_H

#include <string>

enum class SaleStatus 
{
    pending,
    committed,
    cancelled,
    refunded
};



struct SaleLine 
{
    std::string isbn;
    int quantity{};
    float price{};
};

#endif