

/*
A class that manages a number of products, with a unique key for each sale
When we perfom a sale, we push products into a sale internal container,
and then flush those product with the sale_id to the sales json file, with each product containing the id of the sale

An std::unordered_map would be a perfect fit for it's O(1): insertion, search, and deletion
*/

#ifndef SaleTransaction_H
#define SaleTransaction_H



// #endif
#include "SaleLine.h"

#include <vector>
#include <chrono>

struct SaleTransaction
{
    std::string sale_id;
    std::vector<SaleLine> items;
    double total_amount{};
    SaleStatus status {SaleStatus::pending};
    std::chrono::system_clock::time_point date_sold{std::chrono::system_clock::now()};
    std::chrono::system_clock::time_point created_at{std::chrono::system_clock::now()};
    std::chrono::system_clock::time_point updated_at{std::chrono::system_clock::now()};
};

#endif