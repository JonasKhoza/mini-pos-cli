#pragma once

#include "domain/Product.h"

#include <filesystem>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

using FilePath_T = std::filesystem::path;
using json = nlohmann::json;

class JsonStore
{
public:
    JsonStore(const FilePath_T& data_dir);

    std::unordered_map<std::string, Product> load_products();
    void save_products(const std::unordered_map<std::string, Product>& products);

    // append an event to sales NDJSON file
   // void append_sale_event(const json& event_json);

   // std::vector<json> read_all_sale_events(); // parse NDJSON lines to json

   // void backup_products(); // create timestamped backup
private:
    FilePath_T m_data_dir;
    FilePath_T m_products_path; //json file of products
    FilePath_T m_sales_dir;

    //helps write to a file in an atomic way
    void atomic_write(const FilePath_T& dest, const std::string& data);

    //ensure parent exists
    void ensure_parent_exists();
};
