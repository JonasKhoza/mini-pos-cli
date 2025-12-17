#include "domain/Inventory.h"

bool Inventory::add_product(const Product& p, bool overwrite)
{
    auto it {m_products.find(p.isbn)};

    if (it == m_products.end())
    {
        //new product
        m_products[p.isbn] = p;
        return true;
    }
    
    if (overwrite) 
    {
        //It's an overwrite, update an existing product
        m_products.at(p.isbn) = p;
        return true;
    }

    return false;
};

void Inventory::load_from_map(const std::unordered_map<std::string, Product>& map)
{
    if (map.empty()) return;
    m_products = map;
}

std::unordered_map<std::string, Product>  Inventory::dump_to_map() const
{
    return m_products;
}


std::vector<Product> Inventory::list_all() const
{
    std::vector<Product> products{};

    if (m_products.empty()) return products;

    for (auto pair : m_products) products.push_back(pair.second);

    return products;
    
}