#ifndef Inventory_H
#define  Inventory_H

#include "Product.h"

#include <unordered_map>
#include <string>
#include <optional>
#include <vector>


/*
std::unordered_map => for storing and managing in-memory products gives us benefit of O(1) in:
    1.Insertion
    2.Search
    3.Deletion 
So it is an almost perfect data structure for the situation
*/

class Inventory
{
private:
    std::unordered_map<std::string, Product> m_products{};
public:
    Inventory() = default;
    ~Inventory() = default;
    bool add_product(const Product& p, bool overwrite=false); //false if isbn exists (unless overwrite param)
    //std::optional<Product> get(const std::string& isbn) const;
    //bool update_quantity(std::string& isbn, int delta); // +/- quantity
    std::vector<Product> list_all() const;
    

    //Reservation API:
    //Try to reserve qty for isbn. Returns true if reserved, false if insufficient
    // bool reserve(const std::string& isbn, int qty);

    //commit reserved qty -> reduce real quanity and clear reserved
   // bool commit_reservation(const std::string& isbn, int qty);

    //direct adjust (used by import / reconcilliation) - prefer commit/reserve for sales
    //bool adjust_quantity(const std::string& isbn, int delta);

    //persistence helpers to populate inventory from store
    void load_from_map(const std::unordered_map<std::string, Product>& map);
    std::unordered_map<std::string, Product> dump_to_map() const;
};

#endif